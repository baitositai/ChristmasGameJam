#include "FallObjectBase.h"

FallObjectBase::FallObjectBase()
{
}

FallObjectBase::~FallObjectBase()
{
}

void FallObjectBase::Init()
{
	AddState();
	state_ = STATE::NONE;
	ChangeState(STATE::MOVE);
}

void FallObjectBase::Update()
{
}

void FallObjectBase::Draw()
{
}

void FallObjectBase::ChangeState(const STATE& _state)
{
	if (state_ == _state)return;
	state_ = _state;
	changeState_[state_];
}

void FallObjectBase::UpdateNone()
{
	//‰½‚à‚µ‚È‚¢
}

void FallObjectBase::UpdateMove()
{
}

void FallObjectBase::UpdateJump()
{
}

void FallObjectBase::ChangeNone()
{
}

void FallObjectBase::ChangeMove()
{
}

void FallObjectBase::ChangeJump()
{
}

void FallObjectBase::AddState()
{
	changeState_ = {
		{STATE::NONE,[this]() {UpdateNone(); }},
		{STATE::MOVE,[this]() {UpdateMove(); }},
		{STATE::JUMP,[this]() {UpdateJump(); }}
	};
}
