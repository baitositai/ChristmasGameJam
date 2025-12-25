#include "../../Manager/Common/InputManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/Camera.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Scene/SceneGame.h"
#include "../Common/ControllerAnimation.h"
#include "../Common/Capsule.h"
#include "../Weapons/KeyBlade.h"
#include "Player.h"

Player::Player()
{
	state_ = STATE::NONE;
	actionState_ = ACTION_STATE::NONE;
	attackPos_ = Utility3D::VECTOR_ZERO;
	moveDir_ = Utility3D::VECTOR_ZERO;
	movePow_ = Utility3D::VECTOR_ZERO;
	movedPos_ = Utility3D::VECTOR_ZERO;
	playerRotY_ = Quaternion();
	stanTime_ = 0.0f;
	stepRotTime_ = 0.0f;
	moveSpeed_ = 0.0f;
	attackTime_ = 0.0f;

	stateChangesMap_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChangesMap_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChangesMap_.emplace(STATE::ACTION, std::bind(&Player::ChangeStateAction, this));
	stateChangesMap_.emplace(STATE::STAN, std::bind(&Player::ChangeStateStan, this));
	stateChangesMap_.emplace(STATE::THROW, std::bind(&Player::ChangeStateThrow, this));
}

Player::~Player()
{
}

void Player::Init()
{
	// モデル設定
	transform_.SetModel(resMng_.GetHandle("player"));

	// トランスフォーム初期化
	InitTransform();

	// アニメーション初期化
	InitAnimation();

	// カプセルコライダ
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop(PL_CAP_UP_LOCAL_POS);
	capsule_->SetLocalPosDown(PL_CAP_DOWN_LOCAL_POS);
	capsule_->SetRadius(RADIUS);

	// 初期状態
	ChangeState(STATE::PLAY);

	// 武器
	keyBlade_ = std::make_unique<KeyBlade>();
	keyBlade_->SetTargetAndFrameNo(&transform_, HAND_FRAME_NO);
	keyBlade_->Init();
}

void Player::Update()
{
	update_();

	// 移動方向に応じた回転
	Rotate();

	//移動処理
	transform_.pos = movedPos_;

	// 重力方向に沿って回転させる
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);

	// トランスフォーム更新
	transform_.Update();

	// アニメーション更新
	animation_->Update();

	// 武器の更新
	keyBlade_->Update();
}

void Player::Draw()
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// 武器の描画
	keyBlade_->Draw();

	// カプセルの描画
	//capsule_->Draw();

	//// 当たり判定の描画
	//if (state_ == STATE::ACTION)
	//{
	//	// 色
	//	int color = actionState_ == ACTION_STATE::RIGHT ? UtilityCommon::RED : UtilityCommon::BLUE;

	//	// 判定の描画
	//	DrawSphere3D(attackPos_, ATTACK_RADIUS, 30, color, color, true);
	//}
}

void Player::Stan()
{
	// 状態変更
	ChangeState(STATE::STAN);

	// スタン時間
	stanTime_ = STAN_TIME;

	animation_->Play("FallingDown", false);

	sndMng_.PlaySe(SoundType::SE::STAN);
}

void Player::Throw()
{
	// 状態変更
	ChangeState(STATE::THROW);

	// 目標角度の設定
	SetGoalRotate(ROT_DEG_F);

	keyBlade_->Throw();

	animation_->Play("Throw", false, 53.0f);

	sndMng_.PlaySe(SoundType::SE::BLADE_ROTA);
}

const Player::STATE Player::GetState() const
{
	return state_;
}

const Player::ACTION_STATE Player::GetAtctionState() const
{
	return actionState_;
}

const VECTOR& Player::GetAttackPos() const
{
	return attackPos_;
}

const Capsule& Player::GetCapsule() const
{
	return *capsule_;
}

KeyBlade& Player::GetWeapon() const
{
	return *keyBlade_;
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

	// 攻撃位置を設定
	attackPos_ = VAdd(transform_.pos, VScale(transform_.GetForward(), ATTACK_DISTANCE));

	// 目標角度の設定
	SetGoalRotate(ROT_DEG_F);

	// 攻撃時間の初期化
	attackTime_ = 0.0f;

	sndMng_.PlaySe(SoundType::SE::ATTACK);
}

void Player::ChangeStateStan()
{
	update_ = std::bind(&Player::UpdateStan, this);
}

void Player::ChangeStateThrow()
{
	update_ = std::bind(&Player::UpdateThrow, this);
}

void Player::UpdatePlay()
{
	// 移動操作処理
	ProcessMove();

	// 攻撃操作処理
	ProcessAction();

	// 移動制限
	MoveLimit();
}

void Player::UpdateAction()
{
	// 時間更新
	attackTime_ += scnMng_.GetDeltaTime();

	// 攻撃位置を設定
	attackPos_ = VAdd(transform_.pos, VScale(transform_.GetForward(), ATTACK_DISTANCE));

	if (attackTime_ > 1.0f)
	{
		animation_->Play("Idle", false, 0.0f, -1.0f, 0.1f, false, true);
		actionState_ = ACTION_STATE::NONE;
		ChangeState(STATE::PLAY);
		attackTime_ = 0.0f;
	}
}

void Player::UpdateStan()
{
	stanTime_ -= scnMng_.GetDeltaTime();

	if (0.0f > stanTime_)
	{
		ChangeState(STATE::PLAY);
	}
}

void Player::UpdateThrow()
{
	if (keyBlade_->GetState() == KeyBlade::STATE::FOLLOW)
	{
		ChangeState(STATE::PLAY);

		actionState_ = ACTION_STATE::NONE;

		sndMng_.StopSe(SoundType::SE::BLADE_ROTA);
		sndMng_.PlaySe(SoundType::SE::CATCH);
	}
}

void Player::InitTransform()
{
	transform_.scl = INIT_SCALE;
	transform_.pos = Utility3D::VECTOR_ZERO;
	transform_.rot = Utility3D::VECTOR_ZERO;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();
}

void Player::InitAnimation()
{
	// アニメーションクラスの生成
	animation_ = std::make_unique<ControllerAnimation>(transform_.modelId);

	// アニメーションの登録
	animation_->Add("Idle", resMng_.GetHandle("playerAnimIdle"), 40.0f);
	animation_->Add("Run", resMng_.GetHandle("playerAnimRun"), 40.0f);
	animation_->Add("RightAttack", resMng_.GetHandle("playerAnimRightAttack"), 60.0f);
	animation_->Add("LeftAttack", resMng_.GetHandle("playerAnimLeftAttack"), 60.0f);
	animation_->Add("Throw", resMng_.GetHandle("playerAnimThrow"), 40.0f);
	animation_->Add("FallingDown", resMng_.GetHandle("playerAnimFallingDown"), 40.0f);

	// 初期アニメーション設定
	animation_->Play("Idle");

	// 初期更新
	animation_->Update();
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
		animation_->Play("Run");
	}
	else
	{
		// アニメーション処理
		animation_->Play("Idle");
	}

	// 移動後座標を設定
	movedPos_ = VAdd(transform_.pos, movePow_);
}

void Player::ProcessAction()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::PLAYER_ACTION_RIGHT))
	{
		// 状態遷移
		ChangeState(STATE::ACTION);

		// 攻撃状態の設定
		actionState_ = ACTION_STATE::RIGHT;

		// アニメーション開始
		animation_->Play("RightAttack", false, 0.0f, -1.0f, 0.5f, false, true);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::PLAYER_ACTION_LEFT))
	{	
		// 状態遷移
		ChangeState(STATE::ACTION);

		// 攻撃状態の設定
		actionState_ = ACTION_STATE::LEFT;

		// アニメーション開始
		animation_->Play("LeftAttack", false, 0.0f, -1.0f, 0.5f, false, true);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::PLAYER_ACTION_THROW))
	{
		// 攻撃状態の設定
		actionState_ = ACTION_STATE::THROW;
	}
	else
	{
		actionState_ == ACTION_STATE::NONE;
	}
}

void Player::MoveLimit()
{
	// X方向の移動制限
	if (movedPos_.x > SceneGame::MOVE_LIMIT_X)
	{
		movedPos_.x = SceneGame::MOVE_LIMIT_X;
	}
	else if (movedPos_.x < -SceneGame::MOVE_LIMIT_X)
	{
		movedPos_.x = -SceneGame::MOVE_LIMIT_X;
	}
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