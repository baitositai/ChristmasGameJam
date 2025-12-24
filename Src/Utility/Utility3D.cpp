#include <cmath>
#include <algorithm>
#include "Utility2D.h"
#include "Utility3D.h"


VECTOR Utility3D::RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad)
{
    float x = ((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = ((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return VGet(centerPos.x + x, radiusPos.y, centerPos.z + z);
}

double Utility3D::Magnitude(const VECTOR& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Utility3D::MagnitudeF(const VECTOR& v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float Utility3D::SqrMagnitudeF(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility3D::SqrMagnitude(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double Utility3D::SqrMagnitude(const VECTOR& v1, const VECTOR& v2)
{
    return pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2);
}

double Utility3D::Distance(const VECTOR& v1, const VECTOR& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

bool Utility3D::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
    // 球体同士の衝突判定
    bool ret = false;

    // お互いの半径の合計
    float radius = radius1 + radius2;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(pos2, pos1);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool Utility3D::IsHitSphereCapsule(
    const VECTOR& sphPos, float sphRadius,
    const VECTOR& capPos1, const VECTOR& capPos2, float capRadius)
{

    bool ret = false;

    // カプセル球体の中心を繋ぐベクトル
    VECTOR cap1to2 = VSub(capPos2, capPos1);

    // ベクトルを正規化
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // カプセル繋ぎの単位ベクトルと、
    // そのベクトル元から球体へのベクトルの内積を取る
    float dot = VDot(cap1to2ENor, VSub(sphPos, capPos1));

    // 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
    VECTOR capRidePos = VAdd(capPos1, VScale(cap1to2ENor, dot));

    // カプセル繋ぎのベクトルの長さを取る
    float len = Utility3D::MagnitudeF(cap1to2);

    // 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
    float rate = dot / len;

    VECTOR centerPos;

    // 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
    if (rate > 0.0f && rate <= 1.0f)
    {
        // ①球体がカプセル繋ぎ上にいる
        centerPos = VAdd(capPos1, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // ②球体がカプセルの終点側にいる
        centerPos = capPos2;
    }
    else if (rate < 0.0f)
    {
        // ③球体がカプセルの始点側にいる
        centerPos = capPos1;
    }
    else
    {
        // ここにきてはいけない
    }

    // 球体同士の当たり判定
    if (Utility3D::IsHitSpheres(centerPos, capRadius, sphPos, sphRadius))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;

}

bool Utility3D::Equals(const VECTOR& v1, const VECTOR& v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool Utility3D::EqualsVZero(const VECTOR& v1)
{
    const VECTOR& v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR Utility3D::Normalize(const Vector2& v)
{
    VECTOR ret = VGet(
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        0.0f
    );
    float len = static_cast<float>(Utility2D::Magnitude(v));
    ret.x /= len;
    ret.y /= len;
    ret.z /= len;
    return ret;
}

VECTOR Utility3D::VNormalize(const VECTOR& v)
{
    if (Utility3D::EqualsVZero(v))
    {
        // Quaternion計算でゼロを渡して、
        // エラー(-1, -1, -1)が返ってくると困る
        return v;
    }
    return VNorm(v);
}

float Utility3D::Length(const VECTOR& v1, const VECTOR& v2)
{
	float v2x = v2.x - v1.x;
	float v2y = v2.y - v1.y;
	float v2z = v2.z - v1.z;
	return sqrtf((v2x * v2x) + (v2y * v2y) + (v2z * v2z));
}

double Utility3D::AngleDeg(const VECTOR& from, const VECTOR& to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    auto fLen = SqrMagnitude(from);
    auto tLen = SqrMagnitude(to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(from, to) / denominator;
    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

bool Utility3D::CheckHitCapsuleToCapsule(const VECTOR& capTopPosA, const VECTOR& capDownPosA, float capRadiusA, const VECTOR& capTopPosB, const VECTOR& capDownPosB, float capRadiusB)
{
    const float EPSILON = 1e-5f; // 浮動小数点誤差対策

    VECTOR d1 = VSub(capDownPosA, capTopPosA);	// 線分1の方向ベクトル
    VECTOR d2 = VSub(capDownPosB, capTopPosB);	// 線分2の方向ベクトル
    VECTOR r = VSub(capTopPosA, capTopPosB);

    float a = VDot(d1, d1); // d1・d1
    float e = VDot(d2, d2); // d2・d2
    float f = VDot(d2, r);

    float s, t;

    // 分母の計算
    float c = VDot(d1, r);
    float b = VDot(d1, d2);
    float denom = a * e - b * b; // a * e - (d1・d2)^2

    // 線分が平行または非常に近い場合
    if (denom < EPSILON)
    {
        // sを0.0fとし、tを計算してクランプ
        s = 0.0f;
        t = f / e; // t = (b * s + f) / e の s=0 の場合
        t = std::clamp(t, 0.0f, 1.0f);
    }
    // 一般的なケース
    else
    {
        // 2直線の最短距離を求めるパラメータ s, t を計算
        s = (b * f - c * e) / denom;
        t = (a * f - c * b) / denom; // t = (b * s + f) / e; の代わりにこの式を使う方が統一的

        // パラメータ s, t を線分の範囲 [0, 1] にクランプする
        if (s < 0.0f) {
            s = 0.0f;
            t = f / e;
            t = std::clamp(t, 0.0f, 1.0f);
        }
        else if (s > 1.0f) {
            s = 1.0f;
            t = (b + f) / e; // t = (b * s + f) / e の s=1 の場合
            t = std::clamp(t, 0.0f, 1.0f);
        }
        else {
            // sが[0, 1]内にある場合、tを計算してクランプ
            t = (b * s + f) / e; 
            t = std::clamp(t, 0.0f, 1.0f);

            // 初期sをクランプ
            s = std::clamp(s, 0.0f, 1.0f);

            // クランプされたsでtを再計算
            t = (b * s + f) / e;
        }
    }

    if (t < 0.0f)
    {
        t = 0.0f;
        s = std::clamp(-c / a, 0.0f, 1.0f); 
    }
    else if (t > 1.0f)
    {
        t = 1.0f;
        s = std::clamp((b - c) / a, 0.0f, 1.0f);
    }

    // 求まった s, t を使って最短距離上の点 c1, c2 を計算
    VECTOR c1 = VAdd(capTopPosA, VScale(d1, s));
    VECTOR c2 = VAdd(capTopPosB, VScale(d2, t));
    float distance = VSize(VSub(c1, c2));

    //衝突したか
    return distance <= (capRadiusA + capRadiusB);
}

bool Utility3D::CheckHitCapsuleToLine(const VECTOR& capTopPos, const VECTOR& capDownPos, const float capRadius, const VECTOR& lineTopPos, const VECTOR& lineEndPos)
{
    VECTOR u = VSub(lineEndPos, lineTopPos);
    VECTOR v = VSub(capDownPos, capTopPos);
    VECTOR w = VSub(lineTopPos, capTopPos);

    float a = VDot(u, u);
    float b = VDot(u, v);
    float c = VDot(v, v);
    float d = VDot(u, w);
    float e = VDot(v, w);

    float denom = a * c - b * b;
    float s = 0.0f, t = 0.0f;

    if (denom != 0.0f) 
    {
        s = std::clamp((b * e - c * d) / denom, 0.0f, 1.0f);
    }

    t = (b * s + e) / c;
    t = std::clamp(t, 0.0f, 1.0f);

    s = (b * t - d) / a;
    s = std::clamp(s, 0.0f, 1.0f);

    VECTOR closest1 = VAdd(lineTopPos, VScale(u, s));
    VECTOR closest2 = VAdd(capTopPos, VScale(v, t));
    VECTOR diff = VSub(closest1, closest2);

    float distance = sqrt(VDot(diff, diff));

    return distance <= capRadius;
}

bool Utility3D::CheckHitSphereToLine(const float sphrerRadius, const VECTOR& spherePos, const VECTOR& lineTopPos, const VECTOR& lineEndPos)
{
    //線のベクトル
    VECTOR d = VSub(lineEndPos, lineTopPos);

    //線の先端から球体の中心まで
    VECTOR m = VSub(spherePos, lineTopPos);

    float t = VDot(m, d) / VSquareSize(d);
    t = std::max<float>(0.0f, std::min<float>(1.0f, t));  // 線分内に制限

    VECTOR closestPoint = VAdd(lineTopPos, VScale(d, t));
    VECTOR diff = VSub(closestPoint, spherePos);

    return VSquareSize(diff) <= std::pow(sphrerRadius, 2.0);
}

bool Utility3D::CheckHitSphereToCapsule(const float sphrerRadius, const VECTOR& spherePos, const VECTOR& capTopPos, const VECTOR& capDownPos, const float capRadius)
{
    //球体とカプセルの当たり判定
    bool ret = false;

    // カプセル球体の中心を繋ぐベクトル
    VECTOR cap1to2 = VSub(capDownPos, capTopPos);

    // ベクトルを正規化
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // カプセル繋ぎの単位ベクトルと、
    // そのベクトル元から球体へのベクトルの内積を取る
    float dot = VDot(cap1to2ENor, VSub(spherePos, capTopPos));

    // 内積で求めた射影距離を使って、カプセル繋ぎ上の座標を取る
    VECTOR capRidePos = VAdd(capTopPos, VScale(cap1to2ENor, dot));

    // カプセル繋ぎのベクトルの長さを取る
    float len = sqrt((cap1to2.x * cap1to2.x) + (cap1to2.y * cap1to2.y) + (cap1to2.z * cap1to2.z));

    // 球体がカプセル繋ぎ上にいるか判別するため、比率を取る
    float rate = dot / len;

    VECTOR centerPos = { 0.0f,0.0f,0.0f };

    // 球体の位置が３エリアに分割されたカプセル形状のどこにいるか判別
    if (rate > 0.0f && rate <= 1.0f)
    {
        // ①球体がカプセル繋ぎ上にいる
        centerPos = VAdd(capTopPos, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // ②球体がカプセルの終点側にいる
        centerPos = capDownPos;
    }
    else if (rate < 0.0f)
    {
        // ③球体がカプセルの始点側にいる
        centerPos = capTopPos;
    }
    else
    {
        // ここにきてはいけない
        return false;
    }

    // お互いの半径の合計
    float radius = sphrerRadius + capRadius;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(centerPos, spherePos);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool Utility3D::CheckHitSphereToSphere(const float sphrerRadius1, const VECTOR& spherePos1, const float sphrerRadius2, const VECTOR& spherePos2)
{
    // 球体同士の衝突判定
    bool ret = false;

    // お互いの半径の合計
    float radius = sphrerRadius1 + sphrerRadius2;

    // 座標の差からお互いの距離を取る
    VECTOR diff = VSub(spherePos1, spherePos2);

    // 三平方の定理で比較(SqrMagnitudeと同じ)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

void Utility3D::GetFrameWorldMatrix(int modelId, int frameIdx, VECTOR& scl, MATRIX& matRot, VECTOR& pos)
{
    // 対象フレームのワールド行列を取得する
    auto mat = MV1GetFrameLocalWorldMatrix(modelId, frameIdx);

    // 拡大縮小成分
    scl = MGetSize(mat);

    // 回転成分＋拡大縮小成分
    matRot = MGetRotElem(mat);
    // 回転成分のみにする
    auto revScl = VGet(1.0f / scl.x, 1.0f / scl.y, 1.0f / scl.z);
    matRot = MMult(matRot, MGetScale(revScl));

    // 移動成分
    pos = MGetTranslateElem(mat);
}

void Utility3D::SetFrameWorldMatrix(const Transform& follow, int followFrameIdx, Transform& target, VECTOR localPos, VECTOR localRot)
{
    // フレームのワールド行列を取得
    MATRIX worldMatMix = MV1GetFrameLocalWorldMatrix(follow.modelId, followFrameIdx);

    // 回転調整
    VECTOR scl = follow.scl;
    scl = VGet(1.0f / scl.x, 1.0f / scl.y, 1.0f / scl.z);

    // 回転行列を取り出す際に、大きさを1に戻す
    MATRIX rotMat = MMult(MGetRotElem(worldMatMix), MGetScale(scl));

    // X軸回転
    rotMat = MMult(rotMat,
        MGetRotAxis(
            VNorm(VTransformSR(Utility3D::DIR_R, worldMatMix)),
            localRot.x
        )
    );

    // Y軸回転
    rotMat = MMult(rotMat,
        MGetRotAxis(
            VNorm(VTransformSR(Utility3D::DIR_U, worldMatMix)),
            localRot.y
        )
    );

    // Z軸回転
    rotMat = MMult(rotMat,
        MGetRotAxis(
            VNorm(VTransformSR(Utility3D::DIR_F, worldMatMix)),
            localRot.z
        )
    );

    // 回転行列反映
    target.matRot = rotMat;

    // 位置調整
    VECTOR followFramePos = MV1GetFramePosition(follow.modelId, followFrameIdx);
    target.pos = VAdd(followFramePos, VTransform(localPos, target.matRot));

}
