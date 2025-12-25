#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Manager/FallManager/FallObjectManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/Utility3D.h"
#include "../Object/Player/Player.h"
#include "../Object/FallObject/FallObjectBase.h"
#include "../Object/Pooh/Pooh.h"
#include "../Object/Weapons/KeyBlade.h"
#include "../Object/Common/Capsule.h"
#include "ScenePause.h"
#include "SceneGame.h"

namespace {
	const int TIMER_MAX = 3;
}

SceneGame::SceneGame()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneGame::LoadingUpdate, this);

	// 描画関数のセット
	drawFunc_ = std::bind(&SceneGame::LoadingDraw, this);
}

SceneGame::~SceneGame()
{
	FallObjectManager::GetInstance().Destroy();
}

void SceneGame::Init()
{
	// カメラ設定
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);

	FallObjectManager::CreateInstance();
	//オブジェクトを出現させる
	FallObjectManager::GetInstance().Init();
	ScoreManager::GetInstance().Init();
	
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	player_->Init();
	
	pooh_ = std::make_unique<Pooh>();
	pooh_->Init();

	stageImg_.handleId = resMng_.GetHandle("stageImg");
	stageImg_.size= { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };

	isFinishStartDirec_ = false;
	startTimer_ = TIMER_MAX;
	counter_ = 0;

	isStartEndDirec_ = false;

	// BGMの再生
	//sndMng_.PlayBgm(SoundType::BGM::GAME);
}

void SceneGame::NormalUpdate()
{
	// ポーズ画面
	//if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE))
	//{
	//	scnMng_.PushScene(ScenePause_);
	//	return;
	//}

	//スタート演出
	if (!isFinishStartDirec_) {
		UpdateStartDirec();
		return;
	}

	//終了演出
	if (isStartEndDirec_) {
		return;
	}

	ScoreManager& scoreMng = ScoreManager::GetInstance();

	//シーン遷移
	if (scoreMng.IsClear()|| scoreMng.IsFailed()) 
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	//落ちてくるオブジェクト
	FallObjectManager::GetInstance().Update();

	// プレイヤー
	player_->Update();

	pooh_->Update();

	Collision();

#ifdef _DEBUG	

	DebugUpdate();

#endif 
}


void SceneGame::NormalDraw()
{	
#ifdef _DEBUG

	DebugDraw();

#endif
	stageImg_.DrawExtend();
	player_->Draw();

	//スタートカウント中はオブジェクトを描画しない
	if (isFinishStartDirec_)
	{
		FallObjectManager::GetInstance().Draw();
	}


	ScoreManager& scoreMng = ScoreManager::GetInstance();

	const int BLACK = 0x000000;
	DrawFormatString(10, 10, BLACK, L"SCORE　%d　/　%d", scoreMng.GetScore(), ScoreManager::SCORE_MAX);
	DrawFormatString(1000, 10, BLACK, L"LIFE　%d", scoreMng.GetLife());

	pooh_->Draw();

	if (!isFinishStartDirec_) {
		DrawStartDirec();
	}
}

void SceneGame::ChangeNormal()
{
	// 処理変更
	updataFunc_ = std::bind(&SceneGame::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneGame::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn();
}

void SceneGame::Collision()
{
	// 判定用情報
	FallObjectManager& objMng = FallObjectManager::GetInstance();
	constexpr float OFFSET_X = 30.0f;
	const VECTOR poohPos = pooh_->GetTransform().pos;
	const VECTOR playerCapTopPos = player_->GetCapsule().GetPosTop();
	const VECTOR playerCapEndPos = player_->GetCapsule().GetPosDown();
	const float playerCapRadius = player_->GetCapsule().GetRadius();
	isThrowUi_ = false;

	// オブジェクトリストを回す
	for (auto& obj : objMng.GetFallObjectLists())
	{
		// オブジェクト情報
		const VECTOR objPos = obj->GetTransform().pos;

		// プレイヤー状態がPLAYの場合
		if (player_->GetState() == Player::STATE::PLAY)
		{
			// プレイヤーとの衝突判定
			if (Utility3D::CheckHitSphereToCapsule(FallObjectBase::RADIUS, objPos, playerCapTopPos, playerCapEndPos, playerCapRadius))
			{
				// スタン処理
				player_->Stan();
				continue;
			}
		}
		else if (player_->GetState() == Player::STATE::ACTION)
		{
			// 攻撃との衝突判定
			if (Utility3D::CheckHitSphereToSphere(FallObjectBase::RADIUS, objPos, Player::ATTACK_RADIUS, player_->GetAttackPos()))
			{
				sndMng_.PlaySe(SoundType::SE::HIT_OBJECT);
				obj->HitPlayerAttack(player_->GetAtctionState());
				continue;
			}
		}

		// プーとの衝突判定
		if (Utility3D::CheckHitSphereToSphere(FallObjectBase::RADIUS, objPos, Pooh::RADIUS, pooh_->GetTransform().pos))
		{
			// 衝突後処理
			pooh_->HitObject();
			obj->HitPooh();
			ScoreManager::GetInstance().Miss();
			continue;
		}

		// 武器情報の取得
		auto& weapon = player_->GetWeapon();

		//投げた武器とオブジェクトの衝突判定
		if (weapon.GetState() == KeyBlade::STATE::THROW)
		{
			if (Utility3D::CheckHitSphereToSphere(FallObjectBase::RADIUS, objPos, KeyBlade::RADIUS, player_->GetWeapon().GetTransform().pos))
			{
				// 吹っ飛び方向を決定
				Player::ACTION_STATE state = obj->GetObjectType() == FallObjectBase::FALL_OBJ_TYPE::LEFT_OBJ ? Player::ACTION_STATE::LEFT : Player::ACTION_STATE::RIGHT;

				// オブジェクトの吹っ飛び処理
				obj->HitPlayerAttack(state);
			}
		}
		else if (weapon.GetState() == KeyBlade::STATE::FOLLOW)
		{
			// プーのいる範囲かつプーより奥にオブジェクトがある場合
			if (objPos.x + OFFSET_X > poohPos.x && objPos.x - OFFSET_X < poohPos.x && poohPos.z < objPos.z)
			{
				isThrowUi_ = true;
				if (player_->GetAtctionState() == Player::ACTION_STATE::THROW)
				{
					// 武器目的地の設定
					weapon.SetGoalTargetPos(obj->GetTransform().pos);

					// 投げ処理
					player_->Throw();
				}
			}
		}
	}
}

void SceneGame::UpdateStartDirec()
{
	counter_++;

	if (counter_ > Application::FPS_RATE) {
		startTimer_--;
		counter_ = 0;
	}

	//終わったら
	if (startTimer_ < 0) {
		isFinishStartDirec_ = true;
	}
}

void SceneGame::DrawStartDirec()
{
	if (startTimer_ > 0) {
		DrawFormatString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, UtilityCommon::GREEN, L"%d", startTimer_);
	}
	else {
		DrawString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y, L"START!!", UtilityCommon::GREEN);
	}
}

void SceneGame::DebugUpdate()
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_CAMERA_CHANGE))
	{
		switch (mainCamera.GetMode())
		{
		case Camera::MODE::FIXED_POINT:
			mainCamera.ChangeMode(Camera::MODE::FREE);
			break;

		case Camera::MODE::FREE:
			mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);
			break;

		default:
			break;
		}
	}


}

void SceneGame::DebugDraw()
{
	constexpr int INIT_POS_Y = 60;
	constexpr int OFFSET_Y = 20;
	int posY = INIT_POS_Y;

	// 背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::CYAN, true);
	
	// カメラ位置
	VECTOR cPos = mainCamera.GetPos();
	VECTOR cTarget = mainCamera.GetTargetPos();
	VECTOR cAngles = mainCamera.GetAngles();

	// プレイヤー位置
	VECTOR pPos = player_->GetTransform().pos;

	// キーブレード位置
	VECTOR kPos = player_->GetWeapon().GetTransform().pos;

	// 描画
	DrawFormatString(0, posY, UtilityCommon::RED, L"カメラ位置：%2f,%2f,%2f", cPos.x, cPos.y, cPos.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"注視点位置：%2f,%2f,%2f", cTarget.x, cTarget.y, cTarget.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"カメラ角度：%2f,%2f,%2f", cAngles.x, cAngles.y, cAngles.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"プレイヤー位置：%2f,%2f,%2f", pPos.x, pPos.y, pPos.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"キーブレード位置：%2f,%2f,%2f", kPos.x, kPos.y, kPos.z);
	posY += OFFSET_Y;
	if(isThrowUi_)
	{
		DrawFormatString(0, posY, UtilityCommon::RED, L"投げ攻撃");
		posY += OFFSET_Y;
	}

	pooh_->DrawDebug();
}