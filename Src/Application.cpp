#include <DxLib.h>
#include <time.h>
#include <EffekseerForDXLib.h>
#include "Manager/Common/SceneManager.h"
#include "Manager/Common/InputManager.h"
#include "Manager/Common/ResourceManager.h"
#include "Manager/Common/FontManager.h"
#include "FpsControl/FpsControl.h"
#include "Application.h"

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND = "Data/Sound/";
const std::string Application::PATH_FONT = "Data/Font/";
const std::string Application::PATH_TEXT = "Data/Text/";
const std::string Application::PATH_JSON = "Data/JSON/";
const std::string Application::PATH_CSV = "Data/CSV/";
const std::string Application::PATH_SHADER = "Data/Shader/";

bool Application::Init()
{

	// アプリケーションの初期設定
	SetWindowText(L"ERROR");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_DEPTH);
	ChangeWindowMode(true);

	// FPS初期化
	fps_ = std::make_unique<FpsControl>(FPS_RATE);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	if (DxLib_Init() == -1)
	{
		return false;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();			//生成
	InputManager::GetInstance().Init();		//初期化

	// リソース管理初期化
	ResourceManager::CreateInstance();
	ResourceManager::GetInstance().Init();	

	// シーン管理初期化
	SceneManager::CreateInstance();		
	SceneManager::GetInstance().Init();	

	// ランダム生成数字の初期化
	SRand((unsigned int)time(NULL));

	return true;
}

void Application::Run()
{
	LONGLONG time = GetNowHiPerformanceCount();

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && !isGameEnd_)
	{
		// 入力更新処理
		inputManager.Update();

		// シーン更新処理
		sceneManager.Update();

		// 描画処理
		sceneManager.Draw();

#ifdef _DEBUG
		// デバッグフレーム描画
		fps_->Draw();
#endif
		ScreenFlip();

		// フレームレートの計算
		fps_->Wait();
	}
}

bool Application::Release()
{
	//各クラスのリソースの破棄	
	SceneManager::GetInstance().Release();	
	ResourceManager::GetInstance().Release();
	InputManager::GetInstance().Release();

	//インスタンスの破棄
	SceneManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	InputManager::GetInstance().Destroy();
	
	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		return false;
	}

	return true;
}

void Application::GameEnd()
{
	isGameEnd_ = true;
}

Application::Application()
{
	isGameEnd_ = false;
	fps_ = nullptr;
}

void Application::InitEffekseer()
{
	if (Effekseer_Init(EFFECT_MAX_NUM) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
