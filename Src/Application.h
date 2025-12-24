#pragma once
#include <string>
#include "Template/Singleton.h"

class FpsControl;
class FontManager;

class Application : public Singleton<Application>
{
	friend class Singleton<Application>; 

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1280;
	static constexpr int SCREEN_SIZE_Y = 720;

	// スクリーンサイズハーフ
	static constexpr int SCREEN_HALF_X = SCREEN_SIZE_X / 2;
	static constexpr int SCREEN_HALF_Y = SCREEN_SIZE_Y / 2;

	// FPSレート
	static constexpr int FPS_RATE = 60;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_EFFECT;
	static const std::string PATH_SOUND;
	static const std::string PATH_FONT;
	static const std::string PATH_TEXT;
	static const std::string PATH_JSON;
	static const std::string PATH_CSV;
	static const std::string PATH_SHADER;
	//-------------------------------------------

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	/// <returns>初期化成功の場合true,そうじゃない場合false</returns>
	bool Init();

	/// <summary>
	/// ゲームの実行ループ
	/// </summary>
	/// <param name=""></param>
	void Run();

	/// <summary>
	/// リソースの破棄
	/// </summary>
	/// <param name=""></param>
	/// <returns>解放成功の場合true,そうじゃない場合false</returns>
	bool Release();

	/// <summary>
	/// ゲーム終了処理
	/// </summary>
	void GameEnd();

private:

	// カラービット数
	static constexpr int COLOR_DEPTH = 32;

	// パーティクル最大数
	static constexpr int EFFECT_MAX_NUM = 8000;

	// ゲームの終了判定
	bool isGameEnd_;

	// FPS
	std::unique_ptr<FpsControl> fps_;

	// コンストラクタ
	Application();

	// デストラクタ
	~Application() = default;

	// Effekseerの初期化
	void InitEffekseer();

};