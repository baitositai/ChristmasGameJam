#include<DxLib.h>
#include "ScoreManager.h"

namespace {
	const int LIFE_DEFAULT = 5;
}

void ScoreManager::Init()
{
	// 通常のスコアを設定
	score_ = 0;
	life_ = LIFE_DEFAULT;
}

void ScoreManager::SetEndState(const END_STATE state)
{
	state_ = state;
}

void ScoreManager::AddScore()
{
	// スコアを追加
	score_++;
}

ScoreManager::ScoreManager()
{
	state_ = END_STATE::MAX;
	score_ = 0;
}
