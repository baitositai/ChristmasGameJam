#pragma once
#include "Common/Transform.h"

class SceneManager;
class ResourceManager;
class SoundManager;
class InputManager;

class ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ActorBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ActorBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

protected:

	// 管理クラスの参照
	SceneManager& scnMng_;
	ResourceManager& resMng_;
	SoundManager& sndMng_;
	InputManager& inputMng_;

	// トランスフォーム
	Transform transform_;

};