#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <algorithm>
#include "../../Application.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility//UtilityCommon.h"
#include "../../Object/Common/Transform.h"
#include "InputManager.h"
#include "Camera.h"

Camera::Camera(void) : 
	input_(InputManager::GetInstance())
{
	angles_ = VECTOR();
	cameraUpVector_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = Utility3D::VECTOR_ZERO;
	cameraLightPos_ = Utility3D::VECTOR_ZERO;
	targetPos_ = Utility3D::VECTOR_ZERO;
	followTransform_ = nullptr;

	//状態遷移処理の登録
	changeModeMap_.emplace(MODE::NONE, std::bind(&Camera::ChangeModeNone, this));
	changeModeMap_.emplace(MODE::FIXED_POINT, std::bind(&Camera::ChangeModeFixedPoint, this));
	changeModeMap_.emplace(MODE::FOLLOW, std::bind(&Camera::ChangeModeFollow, this));
	changeModeMap_.emplace(MODE::FPS, std::bind(&Camera::ChangeModeFps, this));
	changeModeMap_.emplace(MODE::FREE, std::bind(&Camera::ChangeModeFree, this));	
	changeModeMap_.emplace(MODE::SHADOW, std::bind(&Camera::ChangeModeShadow, this));	
	
	// カメラの初期設定
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Init(void)
{
	// パラメータ初期化
	SetDefault();
}

void Camera::SetBeforeDraw(void)
{
	// カメラモードごとの描画前処理
	beforeDrawFunc_();

	// カメラの設定
	CameraSetting();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void Camera::CameraSetting()
{
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUpVector_
	);
}

void Camera::CameraSettingShadow()
{
	constexpr float SIZE = 13250.0f;
	constexpr float S_NEAR = 10.0f;
	constexpr float S_FAR = 13050.0f;

	// カメラのタイプを正射影タイプにセット、描画範囲も指定
	SetupCamera_Ortho(SIZE);

	// 描画する奥行き範囲をセット
	SetCameraNearFar(S_NEAR, S_FAR);

	//SetCameraPositionAndTarget_UpVecY(LIGHT_POS, LIGHT_TARGET);
	SetCameraPositionAndTarget_UpVecY(pos_, targetPos_);
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetTargetPos(const VECTOR& targetPos)
{
	targetPos_ = targetPos;
}

void Camera::SetAngles(const VECTOR& angles)
{
	angles_ = angles;
}

void Camera::SetCameraUpVector(const VECTOR& cameraUpVector)
{
	cameraUpVector_ = cameraUpVector;
}

const VECTOR& Camera::GetPos(void) const
{
	return pos_;
}

const VECTOR& Camera::GetAngles(void) const
{
	return angles_;
}

const VECTOR& Camera::GetTargetPos(void) const
{
	return targetPos_;
}

const VECTOR& Camera::GetCameraUpVector() const
{
	return cameraUpVector_;
}

const Quaternion& Camera::GetQuaRot(void) const
{
	return rot_;
}

const Quaternion& Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

const Camera::MODE Camera::GetMode() const
{
	return mode_;
}

const VECTOR& Camera::GetCameraLightPos() const
{
	return cameraLightPos_;
}

void Camera::SetPos(const VECTOR& pos)
{
	pos_ = pos;
}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;

	// 状態遷移処理の実行
	changeModeMap_[mode_]();
}

void Camera::SetDefault(void)
{
	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility3D::VECTOR_ZERO;

	// カメラの上方向
	cameraUpVector_ = Utility3D::DIR_U;

	angles_.x = UtilityCommon::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

	// マウスを表示する
	SetMouseDispFlag(true);
}

void Camera::SyncFollow(void)
{
	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility3D::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility3D::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS_FOLLOW);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS_FOLLOW);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUpVector_ = Utility3D::DIR_U;
}

void Camera::SyncFollowFps()
{
	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility3D::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility3D::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rot_.PosAxis(LOCAL_F2T_POS_FPS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS_FPS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUpVector_ = Utility3D::DIR_U;
}

void Camera::ProcessRotFollow(void)
{
	auto& ins = InputManager::GetInstance();
	float rotPow = UtilityCommon::Deg2RadF(SPEED);
	if (CheckHitKey(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
	if (CheckHitKey(KEY_INPUT_UP)) { angles_.x += rotPow; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { angles_.x -= rotPow; }

	if (angles_.x >= LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
	else if (angles_.x <= LIMIT_X_DW_RAD)
	{
		angles_.x = LIMIT_X_DW_RAD;
	}
}

void Camera::ProcessRotFps(void)
{
	// マウス感度
	constexpr float MOUSE_SENSITIVITY = 0.1f;
	constexpr float PAD_SENSITIVITY = 0.015f;

	float rotPow = UtilityCommon::Deg2RadF(SPEED);
	if (input_.IsNew(InputManager::TYPE::CAMERA_MOVE_RIGHT)) { angles_.y += rotPow; }
	if (input_.IsNew(InputManager::TYPE::CAMERA_MOVE_LEFT)) { angles_.y -= rotPow; }
	if (input_.IsNew(InputManager::TYPE::CAMERA_MOVE_UP)) { angles_.x -= rotPow; }
	if (input_.IsNew(InputManager::TYPE::CAMERA_MOVE_DOWN)) { angles_.x += rotPow; }


	auto rStick = input_.GetKnockRStickSize();
	if (Vector2::IsSameVector2({ 0,0 }, rStick))
	{
		rotPow = PAD_SENSITIVITY;
		angles_.x += UtilityCommon::Deg2RadF(rStick.y * rotPow);
		angles_.y += UtilityCommon::Deg2RadF(rStick.x * rotPow);
	}

	auto mouseMove = input_.GetMouseMove();
	if (mouseMove.x != 0.0f || mouseMove.y != 0.0f) // より明示的にチェック
	{
		rotPow = MOUSE_SENSITIVITY;
		angles_.x += UtilityCommon::Deg2RadF(mouseMove.y * rotPow);
		angles_.y += UtilityCommon::Deg2RadF(mouseMove.x * rotPow);
	}

	// マウスの位置を画面中央に戻す
	input_.SetMousePos({ Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y });

	// 角度制限（ピッチ）
	if (angles_.x <= LIMIT_X_UP_RAD_FPS)
	{
		angles_.x = LIMIT_X_UP_RAD_FPS;
	}
	if (angles_.x >= LIMIT_X_DW_RAD_FPS)
	{
		angles_.x = LIMIT_X_DW_RAD_FPS;
	}
}

void Camera::ProcessRotFree()
{
	// 任意の定数
	const float CAMERA_MOVE_SPEED = 4.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;
	const float CAMERA_DISTANCE = 50.0f; // 注視点からカメラまでの距離

	// キーボード入力による回転
	if (CheckHitKey(KEY_INPUT_RIGHT)) angles_.y += CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))  angles_.y -= CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_UP))    angles_.x -= CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_DOWN))  angles_.x += CAMERA_ROTATE_SPEED;

	// 角度の制限
	if (angles_.x <= LIMIT_X_UP_RAD_FPS) { angles_.x = LIMIT_X_UP_RAD_FPS; }
	else if (angles_.x >= LIMIT_X_DW_RAD_FPS) { angles_.x = LIMIT_X_DW_RAD_FPS; }

	// キーボード入力による注視点の移動
	if (CheckHitKey(KEY_INPUT_W)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetForward(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_S)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetBack(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_A)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetLeft(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_D)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetRight(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_Q)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetDown(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_E)) pos_ = VAdd(pos_, VScale(Quaternion::Quaternion(angles_).GetUp(), CAMERA_MOVE_SPEED));

	//角度を計算
	rot_ = (Quaternion::Quaternion(angles_));

	// 注視点からカメラ位置までの相対座標を計算
	VECTOR localPos = rot_.PosAxis(LOCAL_F2T_POS_FOLLOW);

	// 注視点を更新
	targetPos_ = VAdd(pos_, localPos);
}

void Camera::ProcessRotShadow()
{
	// 任意の定数
	const float CAMERA_MOVE_SPEED = 4.0f;
	const float CAMERA_ROTATE_SPEED = 0.01f;
	const float CAMERA_DISTANCE = 50.0f; // 注視点からカメラまでの距離

	// キーボード入力による回転
	if (CheckHitKey(KEY_INPUT_RIGHT)) angles_.y += CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_LEFT))  angles_.y -= CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_UP))    angles_.x -= CAMERA_ROTATE_SPEED;
	if (CheckHitKey(KEY_INPUT_DOWN))  angles_.x += CAMERA_ROTATE_SPEED;

	// 角度の制限
	if (angles_.x <= LIMIT_X_UP_RAD_FPS) { angles_.x = LIMIT_X_UP_RAD_FPS; }
	else if (angles_.x >= LIMIT_X_DW_RAD_FPS) { angles_.x = LIMIT_X_DW_RAD_FPS; }

	// キーボード入力による注視点の移動
	if (CheckHitKey(KEY_INPUT_W)) shadowLightPos_ = VAdd(shadowLightPos_, VScale(Quaternion::Quaternion(angles_).GetForward(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_S)) shadowLightPos_ = VAdd(shadowLightPos_, VScale(Quaternion::Quaternion(angles_).GetBack(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_A)) shadowLightPos_ = VAdd(shadowLightPos_, VScale(Quaternion::Quaternion(angles_).GetLeft(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_D)) shadowLightPos_ = VAdd(shadowLightPos_, VScale(Quaternion::Quaternion(angles_).GetRight(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_Q)) shadowLightPos_ = VAdd(shadowLightPos_, VScale(Quaternion::Quaternion(angles_).GetDown(), CAMERA_MOVE_SPEED));
	if (CheckHitKey(KEY_INPUT_E)) shadowLightPos_ = VAdd(shadowLightPos_, VScale(Quaternion::Quaternion(angles_).GetUp(), CAMERA_MOVE_SPEED));

	//角度を計算
	rot_ = (Quaternion::Quaternion(angles_));

	// 注視点からカメラ位置までの相対座標を計算
	VECTOR localPos = rot_.PosAxis(LOCAL_F2T_POS_FOLLOW);

	// 注視点を更新
	shadowLighTarget_ = VAdd(shadowLightPos_, localPos);
}

void Camera::ChangeModeNone()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawNone, this);
}

void Camera::ChangeModeFixedPoint()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFixedPoint, this);
}

void Camera::ChangeModeFollow()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFollow, this);
}

void Camera::ChangeModeFps()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFps, this);

	// マウスを非表示にする
	SetMouseDispFlag(false);
}

void Camera::ChangeModeFree()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawFree, this);
}

void Camera::ChangeModeShadow()
{
	beforeDrawFunc_ = std::bind(&Camera::SetBeforeDrawShadow, this);
}

void Camera::SetBeforeDrawFixedPoint()
{
	//角度を計算
	rot_ = (Quaternion::Quaternion(angles_));
}

void Camera::SetBeforeDrawFollow(void)
{
	// カメラ操作
	ProcessRotFollow();

	// 追従対象との相対位置を同期
	SyncFollow();
}

void Camera::SetBeforeDrawFps()
{
	//マウスでのカメラ操作
	ProcessRotFps();

	// 追従対象との相対位置を同期
	SyncFollowFps();
}

void Camera::SetBeforeDrawFree()
{
	//カメラ操作
	ProcessRotFree();
}

void Camera::SetBeforeDrawShadow()
{
	ProcessRotShadow();
}
