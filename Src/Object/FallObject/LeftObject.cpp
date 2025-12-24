#include "../Utility/UtilityCommon.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "LeftObject.h"

LeftObject::LeftObject(const VECTOR _startPos, const VECTOR _goalPos):
	FallObjectBase(_startPos, _goalPos)
{
}

LeftObject::~LeftObject()
{
}

void LeftObject::Init()
{
	FallObjectBase::Init();
	type_ = FALL_OBJ_TYPE::RIGHT_OBJ;
	transform_.SetModel(resMng_.GetHandle("watermelon"));
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, 0.0f, UtilityCommon::Deg2RadF(90.0f) });
}

void LeftObject::Draw()
{
	MV1DrawModel(transform_.modelId);
	//DrawSphere3D(transform_.pos, RADIUS, 10, UtilityCommon::BLUE, UtilityCommon::BLUE, true);
}

void LeftObject::AddScore()
{
	if (playerActState_ == Player::ACTION_STATE::LEFT)
	{
		scoreMng_.AddScore();
	}
	else
	{
		scoreMng_.Miss();
	}
}

