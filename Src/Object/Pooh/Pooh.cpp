#include"../../Utility/Utility3D.h"
#include"../../Manager/Common/Camera.h"
#include"../../Manager/Common/SceneManager.h"
#include "Pooh.h"

namespace {
	const int YELLOW = 0xffff00;				//黄色
	const VECTOR INIT_SCL = { 1.0f,1.0f,1.0f };	//初期スケール
	const VECTOR INIT_POS = { 300.0f,120.0f,0.0f };//初期位置
	const VECTOR INIT_ROT = { 0.0f,0.0f,0.0f };	//初期回転

	const int STAY_TIME = 80;				//立っている時間
	const float MOVE_SPEED = 2.0f;			//移動速度(散歩)
	const float HIT_SPEED = 5.0f;			//移動速度(吹っ飛び)

	//散歩ウェイポイント
	const VECTOR GOAL_POSITIONS[Pooh::GOAL_POS_NUM] = {
		{ -200.0f,-80.0f,0.0f },
		{ -100.0f,0.0f,0.0f },
		{ 50.0f,0.0f,0.0f },
		{ 150.0f,-80.0f,0.0f },
		{ -100.0f,-150.0f,0.0f },
		{ 50.0f,-150.0f,0.0f }
	};

	const float GOAL_TOLERANCE = 10.0f; //目的地到達許容範囲
	const VECTOR AFTER_HIT_POS = { 200.0f,-100.0f,-600.0f };	//吹っ飛び後の位置
}

Pooh::Pooh()
	:ActorBase()
	,state_(STATE::SIT)
	, stayTimer_(0)
	, updateFunc_(&Pooh::UpdateSit)
{
}

Pooh::~Pooh()
{
}

void Pooh::Init()
{
	transform_.SetModel(-1); //モデルなし
	transform_.pos = INIT_POS;
	transform_.scl = INIT_SCL;
	transform_.rot = INIT_ROT;
	transform_.quaRot = Quaternion::Euler(VECTOR{ INIT_ROT.x * DX_PI_F / 180.0f, INIT_ROT.y * DX_PI_F / 180.0f, INIT_ROT.z * DX_PI_F / 180.0f });

	ChangeState(STATE::HIT);
}

void Pooh::Update()
{
	(this->*updateFunc_)();
}

void Pooh::Draw()
{
	DrawSphere3D(transform_.pos, 10.0f, 16, YELLOW, YELLOW, false);
}

void Pooh::DrawDebug()
{
	if (useGoalNum_ > 0)
	{
		DrawSphere3D(useGoalPositions_[currentGoalIndex_], 10.0f, 8, 0xff0000, 0xff0000, false);
	}
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

	//更新状態の切り替え
	switch (_next)
	{
	case STATE::SIT:
		updateFunc_ = &Pooh::UpdateSit;
		break;
	case STATE::STAND_UP:
		updateFunc_ = &Pooh::UpdateStandUp;
		//変数初期化
		stayTimer_ = 0;
		//モデル立たせる

		break;
	case STATE::WALK:
		updateFunc_ = &Pooh::UpdateWalk;
		SetGoalPositionsForWalk();
		SetMoveDir();
		break;
	case STATE::BACK:
		updateFunc_ = &Pooh::UpdateBack;
		SetGoalPositionsForSit();
		SetMoveDir();
		break;
	case STATE::HIT:
		updateFunc_ = &Pooh::UpdateHit;
		SetGoalPositionsForHit();
		SetMoveDir();
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
	moveDir_ = Utility3D::VNormalize(VSub(useGoalPositions_[currentGoalIndex_], transform_.pos));
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
	useGoalPositions_.push_back(mainCamera.GetPos());
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
