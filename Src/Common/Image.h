#pragma once
#include "Vector2.h"

struct Image
{
	// テクスチャハンドルID
	int handleId;

	// テクスチャの位置
	Vector2 pos;

	// 中心座標
	Vector2 center;

	// テクスチャサイズ
	Vector2 size;

	// 回転角度
	float angle;

	// 拡大率
	float scale;

	// 透過
	bool isTrans;

	// X軸反転
	bool isFlipX;

	// Y軸反転
	bool isFlipY;

	// デフォルトコンストラクタ
	Image();

	//デストラクタ
	~Image();

	/// <summary>
	/// 中心座標を返す(DrawGraphで描画しているもの用)
	/// </summary>
	/// <returns></returns>
	const Vector2 GetCenterPos() const;

	/// <summary>
	/// 通常描画
	/// </summary>
	const void Draw() const;

	/// <summary>
	/// 回転描画
	/// </summary>
	const void DrawRota() const;

	/// <summary>
	/// 拡大縮小描画
	/// </summary>
	const void DrawExtend() const;
};

