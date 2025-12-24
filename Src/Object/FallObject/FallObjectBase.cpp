#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "FallObjectBase.h"

FallObjectBase::FallObjectBase(const VECTOR _startPos, const VECTOR _goalPos):
	startPos_(_startPos),
	goalPos_(_goalPos),
	type_(FALL_OBJ_TYPE::LEFT_OBJ),
	state_(STATE::NONE)
{
}

FallObjectBase::~FallObjectBase()
{
}

void FallObjectBase::Init()
{
	transform_.pos = startPos_;

	//デバッグの球の半径分上に動かす
	transform_.pos.y += RADIUS;

	AddState();
	state_ = STATE::NONE;
	ChangeState(STATE::MOVE);
}

void FallObjectBase::Update()
{
	updateState_();
}

void FallObjectBase::Draw()
{

}

void FallObjectBase::ChangeState(const STATE& _state)
{
	if (state_ == _state)return;
	state_ = _state;
	changeState_[state_]();
}

void FallObjectBase::UpdateNone()
{
	//何もしない
}

void FallObjectBase::UpdateMove()
{
	VECTOR vec = VNorm(VSub(goalPos_, startPos_));
	transform_.pos = VAdd(transform_.pos, VScale(vec, 5.0f));
}

void FallObjectBase::UpdateJump()
{
}

void FallObjectBase::ChangeNone()
{
	updateState_ = [this]() {UpdateNone(); };
}

void FallObjectBase::ChangeMove()
{
	updateState_ = [this]() {UpdateMove(); };
}

void FallObjectBase::ChangeJump()
{
	updateState_ = [this]() {UpdateJump(); };
}

void FallObjectBase::AddState()
{
	changeState_ = {
		{STATE::NONE,[this]() {ChangeNone(); }},
		{STATE::MOVE,[this]() {ChangeMove(); }},
		{STATE::JUMP,[this]() {ChangeJump(); }}
	};
}
