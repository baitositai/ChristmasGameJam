#include "../../Manager/Common/ResourceManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "KeyBlade.h"

KeyBlade::KeyBlade()
{
	followFrameNo_ = -1;
	isDamage_ = false;
	localPos_ = Utility3D::VECTOR_ZERO;
	localRot_ = Utility3D::VECTOR_ZERO;
	targetTransform_ = nullptr;
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
}

void KeyBlade::Update()
{
	// 対象フレームの位置にtargetを配置し、
	// 対象フレームの回転に加え、指定した相対座標・回転を加える
	Utility3D::SetFrameWorldMatrix(
		*targetTransform_, followFrameNo_,
		transform_,
		localPos_, localRot_
	);

	transform_.quaRot = Quaternion::GetRotation(transform_.matRot);
	transform_.Update();
}

void KeyBlade::Draw()
{
	MV1DrawModel(transform_.modelId);
}

void KeyBlade::SetTargetAndFrameNo(Transform* targetTransform, const int frameNo)
{
	targetTransform_ = targetTransform;
	followFrameNo_ = frameNo;
}