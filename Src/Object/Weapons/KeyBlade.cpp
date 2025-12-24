#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "KeyBlade.h"

KeyBlade::KeyBlade()
{
	state_ = STATE::NONE;
	followFrameNo_ = -1;
	isDamage_ = false;
	localPos_ = Utility3D::VECTOR_ZERO;
	localRot_ = Utility3D::VECTOR_ZERO;
	ownerTransform_ = nullptr;
	attackObjectTransform_ = nullptr;

	stateChangesMap_.emplace(STATE::NONE, std::bind(&KeyBlade::ChangeStateNone, this));
	stateChangesMap_.emplace(STATE::FOLLOW, std::bind(&KeyBlade::ChangeStateFollow, this));
	stateChangesMap_.emplace(STATE::THROW, std::bind(&KeyBlade::ChangeStateThrow, this));
	stateChangesMap_.emplace(STATE::BACK, std::bind(&KeyBlade::ChangeStateBack, this));
}

KeyBlade::~KeyBlade()
{
}

void KeyBlade::Init()
{
	// トランスフォーム初期化
	transform_.SetModel(resMng_.GetHandle("keyBlade"));
	transform_.pos = Utility3D::VECTOR_ZERO;
	transform_.scl = { WEAPON_SCL,WEAPON_SCL, WEAPON_SCL };
	transform_.rot = Utility3D::VECTOR_ZERO;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(DEFAULT_LOCAL_QUAROT_Y_DEG), 0.0f });
	transform_.Update();

	//武器の相対位置初期化
	localPos_ = Utility3D::VECTOR_ZERO;

	//武器の相対回転初期化
	localRot_ = { 0.0f,UtilityCommon::Deg2RadF(LOCAL_ANGLE_DEG.y),UtilityCommon::Deg2RadF(LOCAL_ANGLE_DEG.z) };

	// 初期状態
	ChangeState(STATE::FOLLOW);
}

void KeyBlade::Update()
{
	update_();
	transform_.quaRot = Quaternion::GetRotation(transform_.matRot);
	transform_.Update();
}

void KeyBlade::Draw()
{
	MV1DrawModel(transform_.modelId);
}

void KeyBlade::Throw()
{
	ChangeState(STATE::THROW);
}

void KeyBlade::SetTargetAndFrameNo(Transform* targetTransform, const int frameNo)
{
	ownerTransform_ = targetTransform;
	followFrameNo_ = frameNo;
}

void KeyBlade::SetTargetAttackObject(Transform* targetAttackObject)
{
	attackObjectTransform_ = targetAttackObject;
}

const KeyBlade::STATE KeyBlade::GetState() const
{
	return state_;
}

void KeyBlade::ChangeState(const STATE state)
{
	state_ = state;

	stateChangesMap_[state_]();
}

void KeyBlade::ChangeStateNone()
{
	update_ = std::bind(&KeyBlade::UpdateNone, this);
}

void KeyBlade::ChangeStateFollow()
{
	update_ = std::bind(&KeyBlade::UpdateFollow, this);
}

void KeyBlade::ChangeStateThrow()
{
	if (attackObjectTransform_ == nullptr)
	{
		ChangeState(STATE::FOLLOW);
		return;
	}

	update_ = std::bind(&KeyBlade::UpdateThrow, this);

	throwStep_ = THROW_TIME;
}

void KeyBlade::ChangeStateBack()
{
	update_ = std::bind(&KeyBlade::UpdateBack, this);

	throwStep_ = 0.0f;
}

void KeyBlade::UpdateFollow()
{
	// 対象フレームの位置にtargetを配置し、
	// 対象フレームの回転に加え、指定した相対座標・回転を加える
	Utility3D::SetFrameWorldMatrix(
		*ownerTransform_, followFrameNo_,
		transform_,
		localPos_, localRot_
	);
}

void KeyBlade::UpdateThrow()
{
	const VECTOR targetPos = attackObjectTransform_->pos;
	const float GRAVITY = 0.05f; // 3D空間のスケールに合わせて調整

	// 放物線の軌道を描きながらキーブレードを制御する
	throwStep_ -= scnMng_.GetDeltaTime();

	if (throwStep_ <= 0.0f)
	{
		transform_.pos = targetPos;
		backStartPos_ = targetPos;
		attackObjectTransform_ = nullptr;
		ChangeState(STATE::BACK);
		return;
	}

	// 水平方向(X, Z)の速度を計算
	// 残り距離を残り時間で割る
	float vx = (targetPos.x - transform_.pos.x) / throwStep_;
	float vz = (targetPos.z - transform_.pos.z) / throwStep_;

	// 垂直方向(Y)の速度を計算
	// 公式: v_y = (dist_y - 0.5 * G * t^2) / t
	float dy = targetPos.y - transform_.pos.y;
	float vy = (dy - 0.5f * GRAVITY * throwStep_ * throwStep_) / throwStep_;

	// 座標の更新
	transform_.pos.x += vx;
	transform_.pos.y += vy;
	transform_.pos.z += vz;
}

void KeyBlade::UpdateBack()
{
	const VECTOR goalPos = MV1GetFramePosition(transform_.modelId, followFrameNo_);

	// ステップ更新
	throwStep_ += scnMng_.GetDeltaTime();

	// 進行度の計算
	float t = throwStep_ / BACK_TIME;

	// 進行度が終了値を超えている場合
	if (t >= 1.0f)
	{
		t = 1.0f;		// 値を固定
		ChangeState(STATE::FOLLOW);
		transform_.pos = goalPos;
		return;
	}

	// 元の位置まで戻す
	transform_.pos = UtilityCommon::Lerp(backStartPos_, goalPos, throwStep_);
}