#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "KeyBlade.h"

KeyBlade::KeyBlade()
{
	state_ = STATE::NONE;
	followFrameNo_ = -1;
	throwStep_ = 0.0f;
	isDamage_ = false;
	localPos_ = Utility3D::VECTOR_ZERO;
	localRot_ = Utility3D::VECTOR_ZERO;
	backStartPos_ = Utility3D::VECTOR_ZERO;
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

void KeyBlade::SetTargetAttackObject(const Transform* targetAttackObject)
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

	throwStep_ = 0.0f;

	throwStartPos_ = transform_.pos;
	throwGoalPos_ = attackObjectTransform_->pos;
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

	transform_.quaRot = Quaternion::GetRotation(transform_.matRot);
}

void KeyBlade::UpdateThrow()
{
	// 目的値の更新
	if (attackObjectTransform_)
	{
		throwGoalPos_ = attackObjectTransform_->pos;
	}
	//const float CURVE_STRENGTH = 50.0f; // 横への膨らみの強さ

	//throwStep_ += scnMng_.GetDeltaTime();
	//float t = throwStep_ / THROW_TIME;

	//if (t >= 1.0f) {
	//	transform_.pos = targetPos;
	//	backStartPos_ = targetPos;
	//	attackObjectTransform_ = nullptr;
	//	ChangeState(STATE::BACK);
	//	throwStep_ = 0.0f;
	//	return;
	//}

	//VECTOR basePos = UtilityCommon::Lerp(throwStartPos_, targetPos, t);

	//VECTOR dir = VSub(targetPos, throwStartPos_);
	//VECTOR sideDir = VCross(dir, VGet(0, 1.0f, 0));
	//sideDir = VNorm(sideDir); // 長さを1にする
	//float offsetSize = 4.0f * CURVE_STRENGTH * t * (1.0f - t);

	//// D. ベースの位置に横方向のオフセットを加える
	//transform_.pos = VAdd(basePos, VScale(sideDir, offsetSize));

	// ステップ更新
	throwStep_ += scnMng_.GetDeltaTime();

	// 進行度の計算
	float t = throwStep_ / THROW_TIME;

	// 進行度が終了値を超えている場合
	if (t >= 1.0f)
	{
		t = 1.0f;		// 値を固定
		transform_.pos = throwGoalPos_;
		backStartPos_ = throwGoalPos_;
		attackObjectTransform_ = nullptr;
		ChangeState(STATE::BACK);
		throwStep_ = 0.0f;
		return;
	}

	// 元の位置まで戻す
	transform_.pos = UtilityCommon::Lerp(throwStartPos_, throwGoalPos_, throwStep_);

	Quaternion rotPow = Quaternion();

	rotPow = rotPow.Mult(
		Quaternion::AngleAxis(
			UtilityCommon::Deg2RadF(20.0f), Utility3D::AXIS_X
		));

	// 回転諒を加える(合成)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
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

	Quaternion rotPow = Quaternion();

	rotPow = rotPow.Mult(
		Quaternion::AngleAxis(
			UtilityCommon::Deg2RadF(20.0f), Utility3D::AXIS_X
		));

	// 回転諒を加える(合成)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}