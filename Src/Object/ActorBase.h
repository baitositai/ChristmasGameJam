#pragma once
#include "Common/Transform.h"

class SceneManager;
class ResourceManager;
class SoundManager;
class InputManager;
class EffectManager;

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

	/// <summary>
	/// トランスフォームを返す
	/// </summary>
	/// <returns>トランスフォーム</returns>
	const Transform& GetTransform() const;

protected:

	// ローカル回転のデフォルト値(度)
	static constexpr float DEFAULT_LOCAL_QUAROT_Y_DEG = 180.0f;

	// 管理クラスの参照
	SceneManager& scnMng_;
	ResourceManager& resMng_;
	SoundManager& sndMng_;
	InputManager& inputMng_;
	EffectManager& effMng_;

	// トランスフォーム
	Transform transform_;

};