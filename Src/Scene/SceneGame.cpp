#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Object/Player/Player.h"
#include"../Object/Pooh/Pooh.h"
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
}

void SceneGame::Init()
{
	// カメラ設定
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);

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

	// プレイヤー
	player_->Update();

	pooh_->Update();

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

void SceneGame::DebugUpdate()
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
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