#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Player/Player.h"
#include "FallObjectBase.h"

FallObjectBase::FallObjectBase(const VECTOR _startPos, const VECTOR _goalPos):
	startPos_(_startPos),
	goalPos_(_goalPos),
	type_(FALL_OBJ_TYPE::LEFT_OBJ),
	state_(STATE::NONE),
	playerActState_(Player::ACTION_STATE::NONE),
	scoreMng_(ScoreManager::GetInstance()),
	jumpCnt_(0.0f),
	jumpPow_(0.0f)
{
}

FallObjectBase::~FallObjectBase()
{
}

void FallObjectBase::Init()
{
	transform_.pos = startPos_;

	//デバッグの球の半径分上に動かす
	//transform_.pos.y += RADIUS;
	transform_.pos.y -= RADIUS;

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

void FallObjectBase::HitPlayerAttack(const Player::ACTION_STATE _actState)
{
	playerActState_ = _actState;
	AddScore();
	ChangeState(STATE::JUMP);
}

void FallObjectBase::HitPooh(void)
{
	ChangeState(STATE::DEATH);
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
	transform_.pos = VAdd(transform_.pos, VScale(vec, MOVE_SPD));
	if (transform_.pos.z < -RADIUS)
	{
		//画面外の座標に居たら
		ChangeState(STATE::DEATH);
	}
}

void FallObjectBase::UpdateJump()
{
	velocity_ -= GRAVITY;
	jumpPow_ = velocity_;

	VECTOR vec = {};
	//プレイヤーから受け取った攻撃情報で飛ばす方向を決める「
	if (playerActState_ == Player::ACTION_STATE::LEFT)
	{
		vec = transform_.GetLeft();
	}
	else if (playerActState_ == Player::ACTION_STATE::RIGHT)
	{
		vec = transform_.GetRight();
	}

	//モデルをプレイヤーの攻撃方向に飛ばす
	transform_.pos = VAdd(transform_.pos, VScale(vec, JUMP_SIDE_SPD));
	transform_.pos.y += jumpPow_;

	//一定時間経ったら削除
	if (jumpCnt_ > JUMP_ERASE_TIME)
	{
		ChangeState(STATE::DEATH);
	}
	jumpCnt_ += scnMng_.GetDeltaTime();

}

void FallObjectBase::UpdateDeath()
{
	//状態としてあるのみ
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
	velocity_ = JUMP_POW;
	jumpPow_ = velocity_;
	updateState_ = [this]() {UpdateJump(); };
}

void FallObjectBase::ChangeDeath()
{
	updateState_ = [this]() {UpdateDeath(); };
}

void FallObjectBase::AddState()
{
	changeState_ = {
		{STATE::NONE,[this]() {ChangeNone(); }},
		{STATE::MOVE,[this]() {ChangeMove(); }},
		{STATE::JUMP,[this]() {ChangeJump(); }},
		{STATE::DEATH,[this]() {ChangeDeath(); }},
	};
}
