#include"../../Utility/Utility3D.h"
#include"../../Manager/Common/Camera.h"
#include"../../Manager/Common/SceneManager.h"
#include"../../Manager/Common/ResourceManager.h"
#include"AnimationController.h"
#include "Pooh.h"

namespace {
	const int ANIM_IDLE = 0;		//待機
	const int ANIM_WALK = 1;		//歩く
	const int ANIM_STAND_UP = 12;	//立つ
	const int ANIM_SIT_DOWN = 14;	//座る
	const int ANIM_HIT_FLY = 8;		//吹っ飛び

	const float ANIM_SPEED = 1.0f;		//アニメーション速度

	const int YELLOW = 0xffff00;				//黄色
	const VECTOR INIT_SCL = { 1.0f,1.0f,1.0f };	//初期スケール
	const VECTOR INIT_POS = { 200.0f,220.0f,180.0f };//初期位置
	const VECTOR INIT_ROT = { 0.0f,0.0f,0.0f };	//初期回転
	const VECTOR INIT_DEG = { 0.0f,45.0f,0.0f };	//初期回転(度)

	const int STAY_TIME = 80;				//立っている時間
	const float MOVE_SPEED = 1.0f;			//移動速度(散歩)
	const float HIT_SPEED = 5.0f;			//移動速度(吹っ飛び)

	//散歩ウェイポイント
	const VECTOR GOAL_POSITIONS[Pooh::GOAL_POS_NUM] = {
		{ -110.0f,140.0f,140.0f },	//一番左
		{ -50.0f,180.0f,180.0f },	//左上
		{ 50.0f,180.0f,180.0f },	//右上

		{ 110.0f,140.0f,140.0f },	//一番右
		{ -50.0f,110.0f,110.0f },	//左下
		{ 50.0f,110.0f,110.0f }		//右下
	};

	const float GOAL_TOLERANCE = 10.0f; //目的地到達許容範囲
	const VECTOR AFTER_HIT_POS = { 200.0f,50.0f,-100.0f };	//吹っ飛び後の位置
	const float TIME_ROT = 1.0f;					//回転完了までの時間
	const float HIT_GOAL_POS_DIFF_Y = 30.0f;		//衝突後の目標位置の調整
}

Pooh::Pooh()
	:ActorBase()
	,state_(STATE::SIT)
	, stayTimer_(0)
	, updateFunc_(&Pooh::UpdateSit)
	, moveDir_{ 0.0f,0.0f,0.0f }
	, useGoalNum_(-1)
	, currentGoalIndex_(0)
	, isFinishMove_(false)
	, speed_(0.0f)
	, charaRotY_(Quaternion())
	, goalQuaRot_(Quaternion())
{
}

Pooh::~Pooh()
{
}

void Pooh::Init()
{
	transform_.SetModel(resMng_.GetHandle("pooh"));
	transform_.pos = INIT_POS;
	transform_.scl = INIT_SCL;
	transform_.rot = INIT_ROT;
	transform_.quaRot = Quaternion::Euler(VECTOR{ INIT_DEG.x * DX_PI_F / 180.0f, INIT_DEG.y * DX_PI_F / 180.0f, INIT_DEG.z * DX_PI_F / 180.0f });

	transform_.Update();

	InitAnimation();

	ChangeState(STATE::STAND_UP);
}

void Pooh::Update()
{
	(this->*updateFunc_)();
	// 重力方向に沿って回転させる
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(charaRotY_);

	Rotate();

	animCtrl_->Update();
	transform_.Update();
}

void Pooh::Draw()
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);
	DrawSphere3D(transform_.pos, RADIUS, 16, YELLOW, YELLOW, false);
}

void Pooh::DrawDebug()
{
	//if (useGoalNum_ > 0)
	//{
	//	DrawSphere3D(useGoalPositions_[currentGoalIndex_], RADIUS, 8, 0xff0000, 0xff0000, false);
	//}
}

void Pooh::StartWalk()
{
	//座り状態以外ならば必要ない
	if (state_ != STATE::SIT)return;
	//起立から
	ChangeState(STATE::STAND_UP);
}

void Pooh::HitObject()
{
	ChangeState(STATE::HIT);
}

void Pooh::InitAnimation()
{
	//アニメーションコントローラー生成
	animCtrl_ = std::make_unique<AnimationController>(transform_.modelId);

	//アニメーション登録
	using ANIM = AnimationController::ANIM_NAME;
	using PLAY_TYPE = AnimationController::PLAY_TYPE;

	animCtrl_->Add(ANIM::SIT_DOWN, ANIM_SIT_DOWN, PLAY_TYPE::LOOP);
	animCtrl_->Add(ANIM::STAND_UP, ANIM_STAND_UP, PLAY_TYPE::NOMAL);
	animCtrl_->Add(ANIM::WALK, ANIM_WALK, PLAY_TYPE::LOOP);
	animCtrl_->Add(ANIM::IDLE, ANIM_IDLE, PLAY_TYPE::LOOP);
	animCtrl_->Add(ANIM::HIT_FLY, ANIM_HIT_FLY, PLAY_TYPE::NOMAL);
}

void Pooh::UpdateSit()
{
	//何もしない
}

void Pooh::UpdateStandUp()
{
	//カウンター更新
	stayTimer_++;
	//一定時間立ったら歩きへ
	if (stayTimer_ > STAY_TIME)
	{
		ChangeState(STATE::WALK);
	}
}

void Pooh::UpdateWalk()
{
	//歩く
	Move();
	//一定時間歩いたら
	if (isFinishMove_) {
		ChangeState(STATE::BACK);
	}
}

void Pooh::UpdateBack()
{
	//初期位置に戻る
	Move();
	//戻り終えたら座る
	if (isFinishMove_) {
		ChangeState(STATE::SIT);
	}
}

void Pooh::UpdateHit()
{
	//カメラ側に吹っ飛ぶ
	Move();
	//吹っ飛び終えたら
	// 画面右側へ強制移動→戻る
	if (isFinishMove_) {
		transform_.pos = AFTER_HIT_POS;
		ChangeState(STATE::BACK);
	}
}

void Pooh::ChangeState(const STATE _next)
{
	state_ = _next;
	isFinishMove_ = false;
	useGoalNum_ = -1;
	goalQuaRot_ = Quaternion();
	charaRotY_ = Quaternion();

	//更新状態の切り替え
	switch (_next)
	{
	case STATE::SIT:
		updateFunc_ = &Pooh::UpdateSit;
		animCtrl_->Play(AnimationController::ANIM_NAME::SIT_DOWN, ANIM_SPEED);

		VECTOR toCamera = VSub(transform_.pos, mainCamera.GetPos());
		SetGoalRot(atan2f(toCamera.x, toCamera.z));

		break;
	case STATE::STAND_UP:
		updateFunc_ = &Pooh::UpdateStandUp;
		//変数初期化
		stayTimer_ = 0;
		//モデル立たせる
		animCtrl_->Play(AnimationController::ANIM_NAME::STAND_UP, ANIM_SPEED);
		animCtrl_->AddNextAnim({ AnimationController::ANIM_NAME::IDLE });
		break;
	case STATE::WALK:
		updateFunc_ = &Pooh::UpdateWalk;
		SetGoalPositionsForWalk();
		SetMoveDir();
		animCtrl_->Play(AnimationController::ANIM_NAME::WALK, ANIM_SPEED);
		break;
	case STATE::BACK:
		updateFunc_ = &Pooh::UpdateBack;
		SetGoalPositionsForSit();
		SetMoveDir();
		animCtrl_->Play(AnimationController::ANIM_NAME::WALK, ANIM_SPEED);
		break;
	case STATE::HIT:
		updateFunc_ = &Pooh::UpdateHit;
		SetGoalPositionsForHit();
		SetMoveDir();
		animCtrl_->Play(AnimationController::ANIM_NAME::HIT_FLY, ANIM_SPEED);
		break;
	default:
		break;
	}
}

void Pooh::Move()
{
	//移動
	transform_.pos = VAdd(transform_.pos, VScale(moveDir_, speed_));
	const float diff = Utility3D::Length(transform_.pos, useGoalPositions_[currentGoalIndex_]);
	//ゴールに一定距離近づいたら
	if (diff < GOAL_TOLERANCE) {
		currentGoalIndex_++;	//次の目的地

		//次の目的地があるかを検索
		//なかった場合
		if (currentGoalIndex_ >= useGoalNum_) {
			isFinishMove_ = true;
		}
		else {
			SetMoveDir();
		}
	}
}

void Pooh::SetMoveDir()
{
	VECTOR  diffVec = VSub(useGoalPositions_[currentGoalIndex_], transform_.pos);
	moveDir_ = Utility3D::VNormalize(diffVec);

	VECTOR radVec= VSub(transform_.pos, useGoalPositions_[currentGoalIndex_]);
	float goalRad = atan2f(radVec.x, radVec.z);

	SetGoalRot(goalRad);
}

void Pooh::SetGoalPositionsForWalk()
{
	speed_ = MOVE_SPEED;

	//初期化
	currentGoalIndex_ = 0;
	
	//何個使用するか決める
	useGoalNum_ = GetRandMinMax(2, GOAL_POS_NUM);
	//シャッフル
	ShuffleGoalPositions();

	for (int i = 0; i < useGoalNum_; i++)
	{
		useGoalPositions_.push_back(shuffleGoalPositions_[i]);
	}
}

void Pooh::SetGoalPositionsForSit()
{
	speed_ = MOVE_SPEED;

	currentGoalIndex_ = 0;
	useGoalPositions_.clear();

	useGoalNum_ = 1;
	useGoalPositions_.push_back(INIT_POS);
}

void Pooh::SetGoalPositionsForHit()
{
	speed_ = HIT_SPEED;

	currentGoalIndex_ = 0;
	useGoalPositions_.clear();

	useGoalNum_ = 1;
	VECTOR hitGoalPos = mainCamera.GetPos();
	hitGoalPos.y += HIT_GOAL_POS_DIFF_Y; //高さは変えない
	useGoalPositions_.push_back(hitGoalPos);
}

int Pooh::GetRandMinMax(int _min, int _max)
{
	return _min + rand() % (_max - _min + 1);
}

void Pooh::ShuffleGoalPositions()
{
	//元データコピー
	for (int i = 0; i < GOAL_POS_NUM; i++)
	{
		shuffleGoalPositions_[i] = GOAL_POSITIONS[i];
	}

	// 配列をランダムにシャッフル
	for (int i = GOAL_POS_NUM - 1; i > 0; i--)
	{
		int swapIndex = GetRand(i);

		VECTOR temp = shuffleGoalPositions_[i];
		shuffleGoalPositions_[i] = shuffleGoalPositions_[swapIndex];
		shuffleGoalPositions_[swapIndex] = temp;
	}
}

void Pooh::SetGoalRot(const float _rad)
{
	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + _rad, Utility3D::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値より大きかったら
	if (angleDiff > 0.1f) {
		//回転し始めに設定
		stepRotTime_ = 0.0f;
	}
	goalQuaRot_ = axis;
}

void Pooh::Rotate()
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	charaRotY_ = Quaternion::Slerp(
		charaRotY_, goalQuaRot_,
		(TIME_ROT - stepRotTime_) / TIME_ROT);
}
