#include <string>
#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Utility/UtilityCommon.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneTitle::LoadingUpdate, this);

	// 描画関数のセット
	drawFunc_ = std::bind(&SceneTitle::LoadingDraw, this);
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	titleLogo_.handleId = resMng_.GetHandle("titleLogo");
	titleLogo_.size = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };

	titleFont_ = fontMng_.CreateMyFont(resMng_.GetFontName("fontApril"), 50, 0);

	sndMng_.PlayBgm(SoundType::BGM::TITLE);

}

void SceneTitle::NormalUpdate()
{
	// シーン遷移
	if (inputMng_.IsTrgDown(InputManager::TYPE::TITLE_SCENE_CHANGE))
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
		sndMng_.PlaySe(SoundType::SE::GAME_START);
		sndMng_.StopBgm(SoundType::BGM::TITLE);
		return;
	}
}

void SceneTitle::NormalDraw()
{
	//タイトルpush座標のオフセット
	const Vector2 PUSH_SPACE_POS_OFFSET = { -200,100 };
	Vector2 pushSpacePos = { Application::SCREEN_HALF_X + PUSH_SPACE_POS_OFFSET.x, Application::SCREEN_HALF_Y + PUSH_SPACE_POS_OFFSET.y };

	titleLogo_.DrawExtend();
	DrawString(0, 0, L"RSHIFTでシーン遷移", 0xffffff);
	//座標位置を設定
	//int posX = static_cast<int>(Application::SCREEN_HALF_X -  * FONT_SIZE / 2);
	//int posY = Application::SCREEN_HALF_Y - OFFSET_Y + TEXT_MARGINT * i;

	//文字列を描画
	DrawFormatStringToHandle(pushSpacePos.x, pushSpacePos.y, UtilityCommon::WHITE, titleFont_, L"PUSH SPACE");
}

void SceneTitle::ChangeNormal()
{
	//処理変更
	updataFunc_ = std::bind(&SceneTitle::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneTitle::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn();
}