// フォグの色
static float3 FOG_COLOR = { 0.02, 0.02, 0.05 };

// ライトの色
static float3 LIGHT_COLOR = { 0.4f, 0.4f, 0.3f };

// ライトの強さ
static float LIGHT_POW = 4.0f;

// 法線の計算
float3 CalculateNormal(const float3 tan, const float3 bin, const float3 normal, const float3 tanNormal)
{
    const float3x3 tangentViewMat = transpose(float3x3(normalize(tan), normalize(bin), normalize(normal)));
    return normalize(mul(tangentViewMat, tanNormal));
}

// スペキュラの計算
float3 CalculateSpecular(const float3 cameraPos, const float3 worldPos, const float3 lightDir, const float3 normal, const float specularIntensity, const float shininess)
{
    // 視線ベクトル
    float viewVec = normalize(cameraPos - worldPos);
    
    // ハーフベクトル
    float3 vecHalf = normalize(lightDir + viewVec);

    // スペキュラ項の計算
    float specPower = pow(max(0, dot(normal, vecHalf)), shininess);

    // 最終スペキュラカラー（ライトの色と強度を乗算）
    return specPower * specularIntensity;
}

// 明部減衰
float3 CalculateAmbientAttenuation(const float3 material, const float3 ambientColor)
{
    // 環境光（暗いマテリアルを持ち上げる）
    float3 ambientBase = material * ambientColor.rgb;
    
    // 明部減衰（明るい色ほど環境光を弱める）
    float brightness = dot(material, float3(0.299, 0.587, 0.114)); // 輝度 (0.0 ～ 1.0)
    float lightAttenuation = saturate(brightness); // 明るいほど1.0に近い
    float attenuationFactor = 1.0 - lightAttenuation * 0.8f; // 最大40%減衰
    float3 ambientAttenuated = ambientBase * attenuationFactor;
    return ambientAttenuated;
}

// フォグの適用
float3 ApplyFog(const float3 color, const float fogFactor)
{
    float fogFactorClamped = saturate(1.0f - fogFactor); // 0=カメラ近, 1=遠
    float3 foggedColor = lerp(color, FOG_COLOR, fogFactorClamped);
    return foggedColor;
}

// スポットライトの計算
float3 CalculateSpotLite(const float3 worldPos, const float3 spotLitePos, const float3 spotLiteDir, const float3 normal)
{
   // ピクセル位置と光源の位置からベクトルを計算
    float3 spotPixel = worldPos - spotLitePos;
    float distSq = dot(spotPixel, spotPixel); // 距離の2乗
    
    // 距離による減衰
    const float RANGE = 2000.0f;
    float attenuation = 1.0f - min(distSq / (RANGE * RANGE), 1.0f);
    
    // 近いところを明るくして、遠いところを暗くする
    float dist = pow(attenuation, 8.0f);
    
    // スポットライトの向きとピクセルのベクトルの角度計算
    float3 L = normalize(spotPixel);
    float cosAngle = dot(L, normalize(spotLiteDir));
    
    // スポットライトの円の中心は明るく,外は暗くする
    // 減衰用のPower関数
    float k = 80.0f;
    
    // 近いと少し円を広くする
    k *= 1.0f - min((attenuation * 0.8f), 1.0f);
    
    // 円錐に近づくほど減衰させる為の係数
    float spotIntensity = pow(cosAngle, k);
        
    // スポットライトのディフューズ光の計算
    float NdotLSpot = max(0.0f, dot(normal, -L));
        
    // スポットライトの最終色計算
    float3 spotLight = LIGHT_COLOR * LIGHT_POW * NdotLSpot * attenuation * spotIntensity;
    
    return spotLight;
}

// ランダム生成
float Rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453) * 0.5 + 0.5;
}

float ShadowCalculation(float3 lightAtPos, Texture2D tex, SamplerState texSampler, float3 normal, float3 lightDir)
{
    float blurScale = 2.0f;
    float2 depthUV;
    depthUV.x = (lightAtPos.x + 1.0f) / 2.0f;
    depthUV.y = 1.0f - (lightAtPos.y + 1.0f) / 2.0f;

    // シャドウマップの解像度に合わせてテクセルサイズを計算
    float size = 16000.0f;
    float2 texelSize = float2(1.0f / size, 1.0f / size);

    float shadowAccum = 0.0f;
    float bias = 0.001f; // ストライプ（アクネ）を防ぐためのオフセット

    // 3x3の範囲でサンプリング
    [unroll]
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            // 周辺ピクセルの座標を計算
            float2 offset = float2(x, y) * texelSize * blurScale;
            float pcfDepth = tex.Sample(texSampler, depthUV + offset).r;
            
            // 現在の深度と比較
            // 影なら 0.5f、光が当たっていれば 1.0f を加算
            shadowAccum += (lightAtPos.z > pcfDepth + bias) ? 0.5f : 1.0f;
        }
    }

    // 9点の平均を返す
    return shadowAccum / 9.0f;
}