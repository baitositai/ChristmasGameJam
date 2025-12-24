#include<DxLib.h>
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Object/FallObject/FallObjectBase.h"
#include "../Object/FallObject/RightObject.h"
#include "../Object/FallObject/LeftObject.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/Utility3D.h"
#include "FallObjectManager.h"

namespace {
	using FALL_OBJ_TYPE = FallObjectBase::FALL_OBJ_TYPE;
}

FallObjectManager::FallObjectManager()
{
}

FallObjectManager::~FallObjectManager()
{
	fallObjs_.clear();
}

void FallObjectManager::Init()
{
	DesideObject();
	//for (auto& obj : fallObjs_)
	//{
	//	obj->Init();
	//}
}

void FallObjectManager::Update()
{
	spawnCnt_ += SceneManager::GetInstance().GetDeltaTime();
	if (spawnCnt_ > SPAWN_DIS)
	{
		DesideObject();
		spawnCnt_ = 0.0f;
	}

	for (auto& obj : fallObjs_)
	{
		obj->Update();
	}

	if (InputManager::GetInstance().IsTrgDown(InputManager::TYPE::DEBUG_OBJ_JUMP))
	{
		for (auto& obj : fallObjs_)
		{
			obj->ChangeState(FallObjectBase::STATE::JUMP);
		}
	}
	
}

void FallObjectManager::Draw()
{
	for (auto& obj : fallObjs_)
	{
		obj->Draw();
	}
#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}

#ifdef _DEBUG

void FallObjectManager::DrawDebug()
{
	//レーンの描画
	for (int i = 0; i < LANE_NUM+1; i++)
	{
		const float WIDTH = LANE_WIDTH / LANE_NUM;

		VECTOR start = START_POS;

		VECTOR end = { LIMIT_MIN.x + WIDTH * i,0.0f,0.0f };

		DrawLine3D(end, start, UtilityCommon::RED);
	}
}

#endif // _DEBUG

void FallObjectManager::DesideObject()
{
	FALL_OBJ_TYPE type
		= static_cast<FALL_OBJ_TYPE>(GetRand(static_cast<int>(FALL_OBJ_TYPE::LEFT_OBJ)));	
	START_GOAL_POS startGoal = DesideLane();

	if (type == FALL_OBJ_TYPE::LEFT_OBJ)
	{
		std::unique_ptr<FallObjectBase>fallobj = std::make_unique<LeftObject>(startGoal.startPos, startGoal.goalPos);
		fallobj->Init();
		fallObjs_.emplace_back(std::move(fallobj));
	}
	else
	{
		std::unique_ptr<FallObjectBase>fallobj = std::make_unique<RightObject>(startGoal.startPos, startGoal.goalPos);
		fallobj->Init();
		fallObjs_.emplace_back(std::move(fallobj));
	}

}

const FallObjectManager::START_GOAL_POS FallObjectManager::DesideLane()
{
	START_GOAL_POS startGoal = {};
	LANE_TYPE type = static_cast<LANE_TYPE>(GetRand(static_cast<int>(LANE_TYPE::MAX) - 1));

	//いったん初期座標は固定
	startGoal.startPos = START_POS;

	//目的座標の指定
	switch (type)
	{
	case FallObjectManager::LANE_TYPE::RIGHT_LANE:
		startGoal.goalPos = { -LANE_ONE_WIDTH,0.0f,0.0f };
		break;
	case FallObjectManager::LANE_TYPE::CENTER_LANE:
		startGoal.goalPos = Utility3D::VECTOR_ZERO;
		break;
	case FallObjectManager::LANE_TYPE::LEFT_LANE:
		startGoal.goalPos = { LANE_ONE_WIDTH,0.0f,0.0f };
		break;
	case FallObjectManager::LANE_TYPE::MAX:
		break;
	}
	return startGoal;

}
