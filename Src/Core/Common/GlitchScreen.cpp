#include <DxLib.h>
#include <random>
#include <chrono>
#include "../../Manager/Common/SceneManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Application.h"
#include "GlitchScreen.h"

// 乱数生成器
std::mt19937 mt(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

GlitchScreen::GlitchScreen() :
	scnMng_(SceneManager::GetInstance())
{
	effectScreen_ = -1;
}

GlitchScreen::~GlitchScreen()
{
	DeleteGraph(effectScreen_);
}

void GlitchScreen::Init()
{
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

void GlitchScreen::Draw()
{
	// メインスクリーンの取得
	int mainScreen = scnMng_.GetMainScreen();

	// 現在の描画先を保存
	int originalTargetScreen = GetDrawScreen();

	SetDrawScreen(effectScreen_);
	DrawGraph(0, 0, mainScreen, FALSE); // 元の画面の内容をコピー

	// 描画先を元の画面に戻す
	SetDrawScreen(mainScreen);

	// 描画モードを通常に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, (int)UtilityCommon::ALPHA_MAX);

	// 画面を真っ赤に描画(グリッチでずれた箇所が黒くならないよう、ベースカラーを指定)
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, UtilityCommon::RED, TRUE);

	int y = 0;
	while (y < Application::SCREEN_SIZE_Y)
	{
		int lineHeight = GLITCH_LINE_HEIGHT_MIN + (mt() % (GLITCH_LINE_HEIGHT_MAX - GLITCH_LINE_HEIGHT_MIN + 1));
		if (y + lineHeight > Application::SCREEN_SIZE_Y)
		{
			lineHeight = Application::SCREEN_SIZE_Y - y;
		}
		
		// 一定の確率でグリッチを発生
		if (mt() % GLITCH_DENSITY == 0) 
		{
			int offsetX = (mt() % (GLITCH_OFFSET_MAX * 2 + 1)) - GLITCH_OFFSET_MAX;

			// 描画元の矩形
			int srcX = 0;
			int srcY = y;
			int srcW = Application::SCREEN_SIZE_X;
			int srcH = lineHeight;

			// 描画先の矩形
			int destX = offsetX;
			int destY = y;

			// コピーした一時画面の該当部分を、ずらして描画
			DrawRectGraph(destX, destY, srcX, srcY, srcW, srcH, effectScreen_, FALSE);

			// ズレた部分に、わずかに異なる色やノイズを重ねる (オプション)
			// 例: 青みがかったノイズを重ねて色の分離感を出す
			if (mt() % 2 == 0) 
			{ 
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, GLITCH_COLOR_NOISE_ALPHA);
				const int noiseColorR = (int)UtilityCommon::ALPHA_MAX - (mt() % GLITCH_COLOR_MAGNITUDE);
				const int noiseColorG = mt() % GLITCH_COLOR_MAGNITUDE;
				const int noiseColorB = mt() % GLITCH_COLOR_MAGNITUDE;
				DrawBox(destX, destY, destX + srcW, destY + srcH, GetColor(noiseColorR, noiseColorG, noiseColorB), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, (int)UtilityCommon::ALPHA_MAX); // 元に戻す
			}
		}
		else
		{
			// グリッチしない場合は、元の位置に描画
			DrawRectGraph(0, y, 0, y, Application::SCREEN_SIZE_X, lineHeight, effectScreen_, FALSE);
		}

		y += lineHeight;
	}

	// ランダムなピクセルノイズや小さなブロックノイズを重ねる
	SetDrawBlendMode(DX_BLENDMODE_ADD, NOISE_BLEND_ALPHA); // 加算ブレンドでノイズを強調
	for (int i = 0; i < NOISE_PIXEL_COUNT; ++i)
	{
		const int px = mt() % Application::SCREEN_SIZE_X;
		const int py = mt() % Application::SCREEN_SIZE_Y;
		const int noiseSize = 1 + (mt() % NOISE_SIZE_MAX); // ノイズのサイズをランダムに
		const int noiseColorR = NOISE_COLOR_BASE + (mt() % NOISE_COLOR_RANGE);
		const int noiseColorG = mt() % NOISE_COLOR_MAGNITUDE;
		const int noiseColorB = mt() % NOISE_COLOR_MAGNITUDE;
		DrawBox(px, py, px + noiseSize, py + noiseSize, GetColor(noiseColorR, noiseColorG, noiseColorB), TRUE);
	}
	
	// 元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, (int)UtilityCommon::ALPHA_MAX);

	// 描画先を元の画面に戻す
	SetDrawScreen(originalTargetScreen);
}