#pragma once
#include "../../Template/Singleton.h"

class ScoreManager : public Singleton<ScoreManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<ScoreManager>;

public:

	/// <summary>
	/// 終了状態
	/// </summary>
	enum class END_STATE
	{
		CLEAR,	// クリア
		DEAD,	// 死亡
		MAX
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// ゲームの終了状態を設定
	/// </summary>
	/// <returns></returns>
	const END_STATE GetEndState() const { return state_; }

	/// <summary>
	/// スコアを返す
	/// </summary>
	/// <returns>スコア</returns>
	const int GetScore() const { return score_; }

	/// <summary>
	/// 終了状態の設定
	/// </summary>
	/// <param name="state">終了状態</param>
	void SetEndState(const END_STATE state);

	/// <summary>
	/// スコアの追加
	/// </summary>
	/// <param name="score">スコア</param>
	void AddScore(const int score);

	/// <summary>
	/// スコアを0にする
	/// </summary>
	void DeadScore() { score_ = 0; }

private:

	// 通常スコア
	static constexpr int DEFAULT_SCORE = 10000;

	// 終了状態
	END_STATE state_;

	// ゲームスコア
	int score_;

	// コンストラクタ
	ScoreManager();

	// デストラクタ
	~ScoreManager() = default;
};