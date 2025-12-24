#pragma once

class SceneManager;

class Timer 
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="goalTime">初期目標時間</param>
	Timer(const float goalTime = 0.0f);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Timer();

	/// <summary>
	/// カウントダウン初期化
	/// </summary>
	void InitCountDown();

	/// <summary>
	/// カウントアップ初期化
	/// </summary>
	void InitCountUp();

	/// <summary>
	/// カウントダウン
	/// </summary>
	/// <returns>trueの場合0,falseの場合まだ残り時間あり</returns>
	bool CountDown();

	/// <summary>
	/// カウントアップ
	/// </summary>
	/// <returns>trueの場合設定時間経過,falseの場合まだ達していない</returns>
	bool CountUp();

	/// <summary>
	/// 目標時間の設定
	/// </summary>
	/// <param name="goalTime">目標時間</param>
	void SetGoalTime(const float goalTime) { goalTime_ = goalTime; }

	/// <summary>
	/// 現在の時間を返す
	/// </summary>
	/// <returns>時間</returns>
	const int GetCount() const { return static_cast<int>(step_); }

private:

	// シーン管理クラスの参照
	SceneManager& scnMng_;

	// 更新ステップ
	float step_;

	// 目標タイム
	float goalTime_;
};