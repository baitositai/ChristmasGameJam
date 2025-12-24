#include<DxLib.h>
#include "../Object/FallObject/FallObjectBase.h"
#include "../Object/FallObject/RightObject.h"
#include "../Object/FallObject/LeftObject.h"
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
}

void FallObjectManager::Update()
{
}

void FallObjectManager::Draw()
{
	//DrawLine3D()
}

#ifdef _DEBUG

void FallObjectManager::DrawDebug()
{


}

#endif // _DEBUG

void FallObjectManager::DesideObject()
{
	FALL_OBJ_TYPE type
		= static_cast<FALL_OBJ_TYPE>(GetRand(static_cast<int>(FALL_OBJ_TYPE::LEFT_OBJ)));

	if (type == FALL_OBJ_TYPE::LEFT_OBJ)
	{
		std::unique_ptr<FallObjectBase>fallobj = std::make_unique<LeftObject>();
		fallObjs_.emplace_back(std::move(fallobj));
	}
	else
	{
		std::unique_ptr<FallObjectBase>fallobj = std::make_unique<RightObject>();
		fallObjs_.emplace_back(std::move(fallobj));
	}
}
