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
#include "../Object/Common/Capsule.h"
#include "ScenePause.h"
#include "SceneGame.h"

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
	FallObjectManager::GetInstance().Init();

	ScoreManager::CreateInstance();
	ScoreManager::GetInstance().Init();
	
	// プレイヤー生成
	player_ = std::make_unique<Player>();
	player_->Init();
	
	pooh_ = std::make_unique<Pooh>();
	pooh_->Init();

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
	player_->Draw();
	FallObjectManager::GetInstance().Draw();
	ScoreManager::GetInstance().Draw();

	pooh_->Draw();
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
	const VECTOR playerCapTopPos = player_->GetCapsule().GetPosTop();
	const VECTOR playerCapEndPos = player_->GetCapsule().GetPosDown();
	const float playerCapRadius = player_->GetCapsule().GetRadius();

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
			// 攻撃との衝突判定
			else if (Utility3D::CheckHitSphereToSphere(FallObjectBase::RADIUS, objPos, Player::ATTACK_RADIUS, player_->GetAttackPos()))
			{
				continue;
			}
		}

		// プーとの衝突判定
		else if (Utility3D::CheckHitSphereToSphere(FallObjectBase::RADIUS, objPos, Pooh::RADIUS, pooh_->GetTransform().pos))
		{
			// 衝突後処理
			pooh_->HitObject();
			continue;
		}
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

	// 描画
	DrawFormatString(0, posY, UtilityCommon::RED, L"カメラ位置：%2f,%2f,%2f", cPos.x, cPos.y, cPos.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"注視点位置：%2f,%2f,%2f", cTarget.x, cTarget.y, cTarget.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"カメラ角度：%2f,%2f,%2f", cAngles.x, cAngles.y, cAngles.z);
	posY += OFFSET_Y;
	DrawFormatString(0, posY, UtilityCommon::RED, L"プレイヤー位置：%2f,%2f,%2f", pPos.x, pPos.y, pPos.z);
	posY += OFFSET_Y;

	pooh_->DrawDebug();
}