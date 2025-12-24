#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/ScoreManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "../Utility/UtilityCommon.h"
#include "SceneResult.h"

SceneResult::SceneResult()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneResult::LoadingUpdate, this);
	// 描画関数のセット
	drawFunc_ = std::bind(&SceneResult::LoadingDraw, this);
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
	sndMng_.PlayBgm(SoundType::BGM::RESULT);
}

void SceneResult::NormalUpdate()
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::DEBUG_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		sndMng_.StopBgm(SoundType::BGM::RESULT);
		return;
	}
}

void SceneResult::NormalDraw()
{
	// 背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::CYAN, true);
	DrawFormatString(0, 0, UtilityCommon::RED, L"リザルト");

	ScoreManager& scoreMng = ScoreManager::GetInstance();
	DrawFormatString(100, 100, UtilityCommon::BLACK, L"スコア　:　%d", scoreMng.GetScore());
	DrawFormatString(100, 140, UtilityCommon::BLACK, L"ライフ　:　%d", scoreMng.GetLife());
}

void SceneResult::ChangeNormal()
{
	// 処理変更
	updataFunc_ = std::bind(&SceneResult::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneResult::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn(Fader::STATE::IMMEDIATE);
}