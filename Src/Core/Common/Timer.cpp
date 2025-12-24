#include "../../Manager/Common/SceneManager.h"
#include "Timer.h"

Timer::Timer(const float goalTime) :
	goalTime_(goalTime),
	scnMng_(SceneManager::GetInstance())
{
	step_ = 0.0f;
}

Timer::~Timer()
{
}

void Timer::InitCountDown()
{	
	//ステップ初期化
	step_ = goalTime_;
}

void Timer::InitCountUp()
{
	step_ = 0.0f;
}

bool Timer::CountDown()
{
	// ステップの更新
	step_ -= scnMng_.GetDeltaTime();

	//目標時間に達しているか
	if (0.0f > step_)
	{
		// 初期化しておく
		step_ = goalTime_;
		return true;
	}

	return false;
}

bool Timer::CountUp()
{
	// ステップの更新
	step_ += scnMng_.GetDeltaTime();

	//目標時間に達しているか
	if (goalTime_ < step_)
	{
		// 初期化しておく
		step_ = 0.0f;
		return true;
	}

	return false;
}
