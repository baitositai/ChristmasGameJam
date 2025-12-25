#pragma once
#include <memory>
#include "../../Common/Image.h"
#include "SceneBase.h"

class ScenePause;
class Player;
class Pooh;

class SceneGame : public SceneBase
{
public:

	// オブジェクトのX幅制限
	static constexpr int MOVE_LIMIT_X = 225;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneGame();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneGame();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// 投げUI表示判定
	bool isThrowUi_;

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	// プレイヤー
	std::unique_ptr<Player> player_;

	//プーさん枠
	std::unique_ptr<Pooh> pooh_;

	//スタートの演出が終了したか
	bool isFinishStartDirec_;
	int startTimer_;
	int counter_;

	//ゲーム終了の演出を開始したか
	bool isStartEndDirec_;

	//ステージ画像
	Image stageImg_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	// 衝突判定
	void Collision();

	//スタート演出
	void UpdateStartDirec();
	void DrawStartDirec();

	//デバッグ処理
	void DebugUpdate();
	void DebugDraw();
};