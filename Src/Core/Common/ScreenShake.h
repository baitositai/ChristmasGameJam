#pragma once
#include "../CoreBase.h"
#include "../../Common/Vector2.h"

class ScreenShake : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ScreenShake();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ScreenShake();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 設定
	/// </summary>
	/// <param name="time">時間</param>
	/// <param name="strength">強さ</param>
	void Set(const float time, const int strength);

	/// <summary>
	/// 終了判定
	/// </summary>
	/// <returns></returns>
	const bool IsEnd() { return isEnd_; }

private:

	// 位置
	Vector2 currentPos_;

	// シェイクステップ
	float step_;

	// 強さ
	int strength_;

	// 時間
	float shakeTime_;

	// 終了判定
	bool isEnd_;
};