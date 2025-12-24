#pragma once
#include <functional>
#include <unordered_map>

class Fader
{
public:

	// フェードが進む速さ
	static constexpr float SPEED_ALPHA = 5.0f;

	// 状態
	enum class STATE
	{
		NONE, 
		IMMEDIATE,  // 即時フェード
		FINISH,		// フェード完了
		FADE_OUT,	// 徐々に暗転
		FADE_IN		// 徐々に明転
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Fader();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fader();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns>状態</returns>
	const STATE GetState() const { return state_; }

	/// <summary>
	/// フェード処理の終了判定を返す
	/// </summary>
	/// <returns>フェード処理の終了判定</returns>
	const bool IsEnd() const { return isEnd_; };

	/// <summary>
	/// 設定したフェードを開始する
	/// </summary>
	/// <param name="state">状態</param>
	void SetFade(const STATE state);

private:

	// フェード最大値
	static constexpr float MAX_ALPHA = 255.0f;

	// 透明度
	float alpha_;

	// 状態(STATE)を保ったまま終了判定を行うため、
	// Update->Draw->Updateの1フレーム判定用
	bool isPreEnd_;

	// フェード処理の終了判定
	bool isEnd_;

	// 状態
	STATE state_;

	// 更新処理の管理
	std::unordered_map<STATE, std::function<void()>> setMap_;

	// 更新処理の管理
	std::function<void()> update_;

	// 描画状態管理
	std::function<void()> draw_;

	// 処理の登録
	void RegisterStateSetFunction(const STATE state, const std::function<void()> function);

	// 各種フェード設定
	void SetNone();
	void SetFadeOut();
	void SetFadeIn();
	void SetImmediate();
	void SetFinish();

	// 各種フェード処理
	void UpdateNone() {};
	void UpdateFadeOut();
	void UpdateFadeIn();

	// 各種描画処理
	void DrawNone() {};
	void DrawFade();
};