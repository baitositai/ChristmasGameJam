
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
    float3 world        : WORLD_POSITION;
	float3 normal       : TEXCOORD2;	// 法線( ビュー座標 )
	float4 diffuse      : COLOR0;		// ディフューズカラー
    float3 lightAtPos   : TEXCOORD3; // ライト座標
    float3 tan          : TANGENT;
    float3 bin          : BINORMAL;
    float fogFactor     : FOG;
    float lightPower    : LIGHT_POW;
};

struct VertexToPixelSpecular
{
    float4 svPos        : SV_POSITION;  // 座標( プロジェクション空間 )
    float2 uv           : TEXCOORD0;    // テクスチャ座標
    float3 vwPos        : TEXCOORD1;    // 座標( ビュー座標 )
    float3 normal       : TEXCOORD2;    // 法線( ビュー座標 )
    float4 diffuse      : COLOR0;       // ディフューズカラー
    float3 specular     : COLOR1;       // スペキュラカラー
    float3 tan          : TANGENT;
    float3 bin          : BINORMAL;
    float fogFactor     : FOG;
    float lightPower    : LIGHT_POW;
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
    float4 svPosRead    : TEXCOORD0;    // 座標( プロジェクション空間 )
    float2 uv           : TEXCOORD1;    // テクスチャ座標
    float4 vwPos        : TEXCOORD2;    // 座標( ビュー座標 )
    float3 normal       : TEXCOORD3;    // 法線
};
