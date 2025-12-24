#include<DxLib.h>
#include "../Manager/Common/SceneManager.h"
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
}

void FallObjectManager::Init()
{
	DesideObject();
	for (auto& obj : fallObjs_)
	{
		obj->Init();
	}
}

void FallObjectManager::Update()
{
	//spawnCnt_ += SceneManager::GetInstance().GetDeltaTime();
	//if (spawnCnt_ > SPAWN_DIS)
	//{
	//	DesideObject();
	//}

	for (auto& obj : fallObjs_)
	{
		obj->Update();
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
	//ÉåÅ[ÉìÇÃï`âÊ
	for (int i = 0; i < LANE_NUM+1; i++)
	{
		const float WIDTH = 500.0f / LANE_NUM;

		VECTOR start = START_POS;

		VECTOR end = { -250.0f + WIDTH * i,0.0f,0.0f };

		DrawLine3D(end, start, UtilityCommon::RED);
	}
}

#endif // _DEBUG

void FallObjectManager::DesideObject()
{


	FALL_OBJ_TYPE type
		= static_cast<FALL_OBJ_TYPE>(GetRand(static_cast<int>(FALL_OBJ_TYPE::LEFT_OBJ)));
	VECTOR startPos = START_POS;
	VECTOR goalPos = {};

	if (type == FALL_OBJ_TYPE::LEFT_OBJ)
	{
		std::unique_ptr<FallObjectBase>fallobj = std::make_unique<LeftObject>(startPos, goalPos);
		fallObjs_.emplace_back(std::move(fallobj));
	}
	else
	{
		std::unique_ptr<FallObjectBase>fallobj = std::make_unique<RightObject>(startPos, goalPos);
		fallObjs_.emplace_back(std::move(fallobj));
	}

}

void FallObjectManager::DesideLane()
{
	
	
}
