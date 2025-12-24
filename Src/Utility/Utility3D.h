#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"

class Utility3D
{
public:

	//VECTORの初期化
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XZ = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XYZ = { 1.0f, 1.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius,
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// 正規化
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// VECTOR間の距離を計算する関数
	static float Length(const VECTOR& v1, const VECTOR& v2);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	/// <summary>
	/// カプセルとカプセルの衝突判定
	/// </summary>
	/// <param name="capTopPosA">カプセルA先端座標</param>
	/// <param name="capDownPosA">カプセルA末端座標</param>
	/// <param name="capRadiusA">カプセルA半径</param>
	/// <param name="capTopPosB">カプセルB先端座標</param>
	/// <param name="capDownPosB">カプセルB末端座標</param>
	/// <param name="capRadiusB">カプセルB半径</param>
	/// <returns></returns>
	static bool CheckHitCapsuleToCapsule(const VECTOR& capTopPosA, const VECTOR& capDownPosA, float capRadiusA, const VECTOR& capTopPosB, const VECTOR& capDownPosB, float capRadiusB);

	/// <summary>
	/// カプセルとラインの衝突判定
	/// </summary>
	/// <param name="capTopPos">カプセル先端座標</param>
	/// <param name="capDownPos">カプセル末端座標</param>
	/// <param name="capRadius">カプセルの半径</param>
	/// <param name="lineTopPos">ライン先端座標</param>
	/// <param name="lineEndPos">ラインの末端座標</param>
	/// <returns></returns>
	static bool CheckHitCapsuleToLine(const VECTOR& capTopPos, const VECTOR& capDownPos, const float capRadius, const VECTOR& lineTopPos, const VECTOR& lineEndPos);

	/// <summary>
	/// スフィアとラインの衝突判定
	/// </summary>
	/// <param name="sphrerRadius">スフィアの半径</param>
	/// <param name="spherePos">スフィアの位置</param>
	/// <param name="lineTopPos">ラインの先端座標</param>
	/// <param name="lineEndPos">ラインの末尾座標</param>
	/// <returns>判定結果</returns>
	static bool CheckHitSphereToLine(const float sphrerRadius, const VECTOR& spherePos, const VECTOR& lineTopPos, const VECTOR& lineEndPos);

	/// <summary>
	/// スフィアとカプセルの衝突判定
	/// </summary>
	/// <param name="sphrerRadius">スフィアの半径</param>
	/// <param name="spherePos">スフィアの位置</param>
	/// <param name="capTopPos">カプセル上部座標</param>
	/// <param name="capDownPos">カプセル下部座標</param>
	/// <param name="capRadius">カプセル半径</param>
	/// <returns></returns>
	static bool CheckHitSphereToCapsule(const float sphrerRadius, const VECTOR& spherePos, const VECTOR& capTopPos, const VECTOR& capDownPos, const float capRadius);

	/// <summary>
	/// スフィアとスフィアの衝突判定
	/// </summary>
	/// <param name="sphrerRadius1">スフィア半径1</param>
	/// <param name="spherePos1">スフィア位置1</param>
	/// <param name="sphrerRadius2">スフィア半径2</param>
	/// <param name="spherePos2">スフィア位置2</param>
	/// <returns>判定結果</returns>
	static bool CheckHitSphereToSphere(const float sphrerRadius1, const VECTOR& spherePos1, const float sphrerRadius2, const VECTOR& spherePos2);
};