#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/Camera.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "../Common/Capsule.h"
#include "Player.h"

Player::Player()
{
	stateChangesMap_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChangesMap_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChangesMap_.emplace(STATE::ACTION, std::bind(&Player::ChangeStateAction, this));
	stateChangesMap_.emplace(STATE::STAN, std::bind(&Player::ChangeStateStan, this));
}

Player::~Player()
{
}

void Player::Init()
{
	InitTransform();

	InitAnimation();

	// カプセルコライダ
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop(PL_CAP_UP_LOCAL_POS);
	capsule_->SetLocalPosDown(PL_CAP_DOWN_LOCAL_POS);
	capsule_->SetRadius(RADIUS);

	// 初期状態
	ChangeState(STATE::PLAY);
}

void Player::Update()
{
	update_();

	transform_.Update();
}

void Player::Draw()
{
	// モデルの描画
	//MV1DrawModel(transform_.modelId);

	capsule_->Draw();
}

void Player::ChangeState(const STATE state)
{
	state_ = state;

	stateChangesMap_[state_]();
}

void Player::ChangeStateNone()
{
	update_ = std::bind(&Player::UpdateNone, this);
}

void Player::ChangeStatePlay()
{
	update_ = std::bind(&Player::UpdatePlay, this);
}

void Player::ChangeStateAction()
{
	update_ = std::bind(&Player::UpdateAction, this);
}

void Player::ChangeStateStan()
{
	update_ = std::bind(&Player::UpdateStan, this);
}

void Player::UpdatePlay()
{
	//操作処理
	ProcessMove();

	// 移動方向に応じた回転
	Rotate();

	//移動後の座標
	movedPos_ = VAdd(transform_.pos, movePow_);

	//移動処理
	transform_.pos = movedPos_;

	// 重力方向に沿って回転させる
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);
}

void Player::UpdateAction()
{
}

void Player::UpdateStan()
{
}

void Player::InitTransform()
{
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.pos = Utility3D::VECTOR_ZERO;
	transform_.rot = Utility3D::VECTOR_ZERO;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void Player::InitAnimation()
{
}

void Player::ProcessMove()
{
	movePow_ = Utility3D::VECTOR_ZERO;			//移動量
	VECTOR dir = Utility3D::VECTOR_ZERO;		//方向
	Quaternion cameraRot = mainCamera.GetQuaRotOutX();
	double rotRad = 0.0f;

	//操作
	if (inputMng_.IsNew(InputManager::TYPE::PLAYER_MOVE_RIGHT)) { dir = cameraRot.GetRight(); rotRad = UtilityCommon::Deg2RadD(ROT_DEG_R); }
	else if (inputMng_.IsNew(InputManager::TYPE::PLAYER_MOVE_LEFT)) { dir = cameraRot.GetLeft(); rotRad = UtilityCommon::Deg2RadD(ROT_DEG_L); }

	// 移動量がある場合
	if (!Utility3D::EqualsVZero(dir))
	{
		// 移動処理
		moveSpeed_ = SPEED_MOVE;
		moveDir_ = dir;						//移動方向
		movePow_ = VScale(dir, moveSpeed_);	//移動量

		// 回転処理
		SetGoalRotate(rotRad);

		// アニメーション処理
		//anim_->Play((int)ANIM_TYPE::WALK);
	}
	else
	{
		// アニメーション処理
		//anim_->Play((int)ANIM_TYPE::IDLE);
	}

	// 移動後座標を設定
	movedPos_ = VAdd(transform_.pos, movePow_);
}

void Player::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, Utility3D::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void Player::Rotate()
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_,
		(TIME_ROT - stepRotTime_) / TIME_ROT);
}