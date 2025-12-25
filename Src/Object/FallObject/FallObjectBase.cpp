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
	jumpPow_(0.0f),
	moveDeg_(0.0f),
	throwVec_({})
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
	//transform_.pos.y -= RADIUS;

	AddState();
	state_ = STATE::NONE;
	ChangeState(STATE::MOVE);
}

void FallObjectBase::Update()
{
	updateState_();
	transform_.Update();
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
	moveDeg_ += MOVE_ROT_SPD;
	transform_.quaRot=Quaternion::AngleAxis(UtilityCommon::Deg2RadF(moveDeg_), Utility3D::AXIS_X);

	if (transform_.pos.z < -RADIUS)
	{
		//画面外の座標に居たら
		scoreMng_.Miss();		//見逃しでミス
		ChangeState(STATE::DEATH);
	}
}

void FallObjectBase::UpdateJump()
{
	velocity_ -= GRAVITY;
	jumpPow_ = velocity_;

	//ジャンプ方向を決める
	VECTOR vec = DesideJumpVec();

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

const VECTOR FallObjectBase::DesideJumpVec()
{
	switch (playerActState_)
	{
	case Player::ACTION_STATE::NONE:
		return Utility3D::VECTOR_ZERO;		//ここは通らない
		break;
	case Player::ACTION_STATE::RIGHT:
		return transform_.GetRight();
		break;
	case Player::ACTION_STATE::LEFT:
		return transform_.GetLeft();
		break;
	case Player::ACTION_STATE::THROW:
		return throwVec_;
		break;
	default:
		break;
	}
}
