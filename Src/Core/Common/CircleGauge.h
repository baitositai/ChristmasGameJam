#pragma once
#include "../../Common/Vector2.h"

struct CircleGauge
{

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CircleGauge();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CircleGauge();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画位置
	/// </summary>
	Vector2 pos;

	/// <summary>
	/// 画像ハンドル
	/// </summary>
	int handle;

	/// <summary>
	/// パーセント(0~100)
	/// </summary>
	float percent;

	/// <summary>
	/// 開始パーセント
	/// </summary>
	float startPercent;

	/// <summary>
	/// スケール
	/// </summary>
	float scale;

	/// <summary>
	/// X軸反転
	/// </summary>
	bool reverseX;

	/// <summary>
	/// Y軸反転
	/// </summary>
	bool reverseY;

};