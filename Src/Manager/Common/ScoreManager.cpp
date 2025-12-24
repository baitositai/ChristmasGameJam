#include<DxLib.h>
#include "ScoreManager.h"

namespace {
	const int LIFE_DEFAULT = 5;
	const int BLACK = 0x000000;
}

void ScoreManager::Init()
{
	// 通常のスコアを設定
	score_ = 0;
	life_ = LIFE_DEFAULT;
}

void ScoreManager::Draw()
{
	DrawFormatString(10, 10, BLACK, L"SCORE　%d　/　%d", score_, SCORE_MAX);
	DrawFormatString(1000, 10, BLACK, L"LIFE　%d", life_);
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
