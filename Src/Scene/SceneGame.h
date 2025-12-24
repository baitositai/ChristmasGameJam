#pragma once
#include <memory>
#include "SceneBase.h"

class ScenePause;
class Pooh;

class SceneGame : public SceneBase
{
public:

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

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	//プーさん枠
	std::unique_ptr<Pooh> pooh_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;

	//デバッグ処理
	void DebugUpdate();
	void DebugDraw();
};