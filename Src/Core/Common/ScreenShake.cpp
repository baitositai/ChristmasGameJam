#include <DxLib.h>
#include <random>
#include "../Manager/Common/SceneManager.h"
#include "ScreenShake.h"

namespace
{
	// ランダム生成器
	std::mt19937 shakeRandom{ std::random_device{}() };
}

ScreenShake::ScreenShake()
{
	isEnd_ = false;
	shakeTime_ = 0.0f;
	strength_ = 0.0f;
	step_ = 0.0f;
}

ScreenShake::~ScreenShake()
{
}

void ScreenShake::Init()
{
	isEnd_ = false;
	shakeTime_ = 0.0f;
	strength_ = 0.0f;
	step_ = 0.0f;
}

void ScreenShake::Update()
{
	// 座標
	Vector2 pos = { 0,0 };

	// ステップ更新
	step_ += scnMng_.GetDeltaTime();

	// 時間を満たしてない場合
	if (step_ < shakeTime_)
	{
		// 指定範囲内のランダムな値を生成して揺らす
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

		// ランダムで座標をずらす
		pos = {
			static_cast<int>(dist(shakeRandom)* strength_),
			static_cast<int>(dist(shakeRandom)* strength_)
		};
	}
	else
	{
		// 終了判定
		isEnd_ = true;
	}
	// 設定
	scnMng_.SetScreenPos(pos);

}

void ScreenShake::Set(const float time, const int strength)
{
	shakeTime_ = time;
	strength_ = strength;
	step_ = 0.0f;
	isEnd_ = false;
}
