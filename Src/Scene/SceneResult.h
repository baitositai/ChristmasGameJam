#pragma once
#include "SceneBase.h"

class SceneResult : public SceneBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneResult();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneResult();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	Image logo_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;
};