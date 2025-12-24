#pragma once

class SceneManager;

class GlitchScreen
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GlitchScreen();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GlitchScreen();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// グリッチエフェクトのパラメータ
	static constexpr int GLITCH_LINE_HEIGHT_MIN = 5;	// グリッチのブロックの最小高さ
	static constexpr int GLITCH_LINE_HEIGHT_MAX = 30;	// グリッチのブロックの最大高さ
	static constexpr int GLITCH_OFFSET_MAX = 100;		// 水平方向の最大ズレ量
	static constexpr int GLITCH_DENSITY = 3;			// グリッチ発生の頻度 (小さいほど頻繁)
	static constexpr int GLITCH_COLOR_MAGNITUDE = 100;	// グリッチで発生する色の変化の大きさ

	// ノイズのパラメータ
	static constexpr int NOISE_PIXEL_COUNT = 500;		// ノイズのピクセル数
	static constexpr int NOISE_BLEND_ALPHA = 128; 		// ノイズのブレンドアルファ値
	static constexpr int NOISE_COLOR_BASE = 100;		// ノイズの基本色
	static constexpr int NOISE_COLOR_RANGE = 155;		// ノイズの色の範囲
	static constexpr int NOISE_SIZE_MAX = 5;			// ノイズの最大サイズ
	static constexpr int NOISE_COLOR_MAGNITUDE = 200;	// ノイズの色の変化の大きさ

	// カラーノイズの透明度
	static constexpr int GLITCH_COLOR_NOISE_ALPHA = 64;

	// シーン管理クラスの参照
	SceneManager& scnMng_;

	// エフェクトスクリーン
	int effectScreen_;
};

