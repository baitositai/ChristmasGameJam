#include "../Utility/UtilityCommon.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Manager/Common/SoundManager.h"
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
	type_ = FALL_OBJ_TYPE::LEFT_OBJ;
	transform_.SetModel(resMng_.GetHandle("watermelon"));
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, 0.0f, UtilityCommon::Deg2RadF(90.0f) });

	throwVec_ = transform_.GetLeft();
}

void LeftObject::Draw()
{
	if (state_ == STATE::NONE)return;
	MV1DrawModel(transform_.modelId);
	//DrawSphere3D(transform_.pos, RADIUS, 10, UtilityCommon::BLUE, UtilityCommon::BLUE, false);
}

void LeftObject::AddScore()
{
	if (playerActState_ == Player::ACTION_STATE::LEFT||playerActState_==Player::ACTION_STATE::THROW)
	{
		sndMng_.PlaySe(SoundType::SE::HIT_SUCCESS);
		scoreMng_.AddScore();
	}
	else
	{
		sndMng_.PlaySe(SoundType::SE::HIT_MISS);
		scoreMng_.Miss();
	}
}

