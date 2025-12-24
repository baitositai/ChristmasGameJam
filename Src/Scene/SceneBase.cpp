#include <DxLib.h>
#include <string>
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SoundManager.h"
#include "../Common/Loading.h"
#include "../Utility/UtilityCommon.h"
#include "SceneBase.h"

SceneBase::SceneBase() :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	inputMng_(InputManager::GetInstance()),
	fontMng_(FontManager::GetInstance()),
	sndMng_(SoundManager::GetInstance()),
	loading_(Loading::GetInstance())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Load()
{
	// 非同期読み込み開始
	loading_.StartASyncLoad();
	
	// シーン内のリソースを読み込む
	resMng_.SceneChangeResource(static_cast<int>(scnMng_.GetSceneID()));
}

void SceneBase::Init()
{
}

void SceneBase::Update()
{
	updataFunc_();
}

void SceneBase::Draw()
{
	drawFunc_();
}

void SceneBase::LoadingUpdate()
{
	// ローディング中の場合
	if (loading_.IsLoading())
	{
		// ローディング中の更新処理
		loading_.Update();
		return;
	}

	// サウンドのリソースの切り替え
	sndMng_.SceneChangeResources();

	// 初期化
	Init();

	// 処理の変更
	ChangeNormal();
}

void SceneBase::NormalUpdate()
{
}

void SceneBase::LoadingDraw()
{
	// ローディング画面の描画
	loading_.Draw();
}

void SceneBase::NormalDraw()
{
}

void SceneBase::ChangeNormal()
{
}
