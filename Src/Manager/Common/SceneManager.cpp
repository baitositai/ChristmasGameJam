#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Scene/SceneTitle.h"
#include "../../Scene/SceneGame.h"
#include "../../Scene/SceneResult.h"
#include "../../Common/Loading.h"
#include "../Common/ResourceManager.h"
#include "../Common/SoundManager.h"
#include "../Common/FontManager.h"
#include "../Common/ScoreManager.h"
#include "../Common/EffectManager.h"
#include "Camera.h"
#include "SceneManager.h"

void SceneManager::Init()
{
	//シーンID初期化
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	//フェード
	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Init();

	// サウンド管理生成
	SoundManager::CreateInstance();

	// エフェクト管理生成
	EffectManager::CreateInstance();

	// フォント管理クラス生成
	FontManager::CreateInstance();

	// スコア管理生成
	ScoreManager::CreateInstance();

	// 読み込み中処理管理クラス生成
	Loading::CreateInstance();
	Loading::GetInstance().Init();

	// シーン遷移中
	isSceneChanging_ = true;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// スクリーン
	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

	// ウィンドウがアクティブ状態でなくとも処理を行う
	SetAlwaysRunFlag(true);

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	CreateScene(std::make_shared<SceneTitle>());
}

void SceneManager::Init3D()
{
	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);
	
	//標準ライトの計算を行わない
	SetLightEnable(true);

	// ライトの設定
	ChangeLightTypeDir({ 0.2f, -1.0f, 0.2f });

	// フォグ設定
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(FOG_START, FOG_END);
}

void SceneManager::Update()
{
	//if (scene_ == nullptr) { return; }

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;
	totalTime_ += deltaTime_;

	// フェーダー更新
	fader_->Update();

	// シーンチェンジ中の処理
	if (isSceneChanging_)
	{
		// フェード処理
		Fade();
	}
	
	// シーンごとの更新
	scenes_.back()->Update();

	// カメラ更新
	camera_->Update();

	// サウンドの更新
	SoundManager::GetInstance().Update();
}

void SceneManager::Draw()
{
	// メインスクリーンを指定
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// 描画
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}

	// 主にポストエフェクト用
	camera_->Draw();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	// 暗転・明転
	fader_->Draw();

	// スクリーンバックへ変更
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラ設定
	camera_->CameraSetting();

	// メインスクリーンを描画
	DrawGraph(screenPos_.x, screenPos_.y, mainScreen_, true);
}

void SceneManager::CreateScene(const std::shared_ptr<SceneBase>& scene)
{
	if (scenes_.empty())
	{
		scenes_.push_back(scene);
	}
	else
	{
		scenes_.front() = scene;
	}

	//データのロード
	scenes_.front()->Load();
}

void SceneManager::PushScene(const std::shared_ptr<SceneBase>& scene)
{
	scene->Init();
	scenes_.push_back(scene);
}

void SceneManager::PopScene()
{
	if (scenes_.size() >= 1)
	{
		scenes_.pop_back();
	}
}

void SceneManager::Release()
{
	DeleteGraph(mainScreen_);

	//全てのシーンで使うシングルトンクラスやリソースはここで解放する
	FontManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	ScoreManager::GetInstance().Destroy();
	Loading::GetInstance().Destroy();
}

void SceneManager::ChangeScene(const SCENE_ID nextId, const Fader::STATE fadeState)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(fadeState);
	isSceneChanging_ = true;

}

void SceneManager::StartFadeIn(const Fader::STATE fadeState)
{
	//フェードを明ける
	fader_->SetFade(fadeState);

	//シーンチェンジ
	isSceneChanging_ = false;
}

SceneManager::SceneManager()
{
	mainScreen_ = -1;
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;
	scenes_.clear();
	fader_ = nullptr;
	camera_ = nullptr;
	isSceneChanging_ = false;
	deltaTime_ = 1.0f / 60.0f;	// デルタタイム
	totalTime_ = -1.0f;
	screenPos_ = {};
}

void SceneManager::ResetDeltaTime()
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放（空チェックあり）
	if (!scenes_.empty() && scenes_.back() != nullptr)
	{
		scenes_.back().reset();
		scenes_.pop_back(); // シーンを使い終わったのでリストからも削除
	}

	// シーン生成
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		CreateScene(std::make_shared<SceneTitle>());
		break;
	case SCENE_ID::GAME:
		CreateScene(std::make_shared<SceneGame>());
		break;
	case SCENE_ID::RESULT:
		CreateScene(std::make_shared<SceneResult>());
		break;
	}

	// デルタタイムの初期化
	ResetDeltaTime();

	// 待機シーンの初期化
	waitSceneId_ = SCENE_ID::NONE;
}

void SceneManager::Fade()
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
	case Fader::STATE::IMMEDIATE:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			//isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
	case Fader::STATE::FINISH:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			//fader_->SetFade(Fader::STATE::FADE_IN);
			fader_->SetFade(Fader::STATE::NONE);
		}
		break;
	}
}