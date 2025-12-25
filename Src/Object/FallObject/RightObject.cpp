#include "../Utility/UtilityCommon.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "RightObject.h"

RightObject::RightObject(const VECTOR _startPos, const VECTOR _goalPos):
	FallObjectBase(_startPos,_goalPos)
{
}

RightObject::~RightObject()
{
}

void RightObject::Init()
{
	FallObjectBase::Init();
	type_ = FALL_OBJ_TYPE::RIGHT_OBJ;

	transform_.SetModel(resMng_.GetHandle("pumpkin"));
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, 0.0f, UtilityCommon::Deg2RadF(90.0f) });

	transform_.localPos = { LOCAL_POS_X,0.0f,0.0f };


	throwVec_ = transform_.GetRight();

	transform_.Update();
}

void RightObject::Draw()
{
	if (state_ == STATE::NONE)return;
	MV1DrawModel(transform_.modelId);
	DrawSphere3D(transform_.pos, RADIUS, 10, UtilityCommon::RED, UtilityCommon::RED, false);
}

void RightObject::AddScore(void)
{
	if (playerActState_ == Player::ACTION_STATE::RIGHT || playerActState_ == Player::ACTION_STATE::THROW)
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
