#include "ScoreManager.h"

void ScoreManager::Init()
{
	// 通常のスコアを設定
	score_ = DEFAULT_SCORE;
}

void ScoreManager::SetEndState(const END_STATE state)
{
	state_ = state;
}

void ScoreManager::AddScore(const int score)
{
	// スコアを追加
	score_ += score;

	// スコアが0未満の場合は0にする
	if (score_ < 0)
	{
		score_ = 0;
	}
}

ScoreManager::ScoreManager()
{
	state_ = END_STATE::MAX;
	score_ = 0;
}
