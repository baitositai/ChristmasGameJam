#pragma once
#include <unordered_map>
#include "../../Common/Image.h"
#include "SceneBase.h"

class SceneTitle : public SceneBase
{

public:

	// コンストラクタ
	SceneTitle();

	// デストラクタ
	~SceneTitle();

	// 初期化処理
	void Init() override;

private:

	// タイトルロゴ
	Image titleLogo_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;
};
