#pragma once
#include <unordered_map>
#include <functional>
#include "../ActorBase.h"

class KeyBlade : public ActorBase
{
public:

	enum class STATE
	{
		NONE,	// なし
		FOLLOW,	// 追従
		THROW,	// 投げ
		BACK,	// 戻る
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeyBlade();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyBlade() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 投げ処理
	/// </summary>
	void Throw();

	/// <summary>
	/// 追従対象とフレーム位置の指定
	/// </summary>
	/// <param name="targetTransform">追従対象のトランスフォーム</param>
	/// <param name="frameNo">フレーム番号</param>
	void SetTargetAndFrameNo(Transform* targetTransform, const int frameNo);

	/// <summary>
	/// 攻撃対象のトランスフォームを取得
	/// </summary>
	/// <param name="targetAttackObject">攻撃対象のトランスフォーム</param>
	void SetTargetAttackObject(Transform* targetAttackObject);

	/// <summary>
	/// 状態を返す
	/// </summary>
	/// <returns>状態</returns>
	const STATE GetState() const;

private:

	//ローカル角度(デグリー)
	static constexpr VECTOR LOCAL_ANGLE_DEG = { 0.0f,190.0f,-45.0f };

	//武器の大きさ
	static constexpr float WEAPON_SCL = 0.15f;

	//武器の最大頂点座標
	static constexpr VECTOR MAX_VERTEX_POS = { 91.0f,533.0f,19.7f };
	
	//武器の最小頂点座標
	static constexpr VECTOR MIN_VERTEX_POS = { 91.0f,-117.0f,19.7f };

	//カプセルコライダーの半径
	static constexpr float CAPSULE_COL_RADIUS = MAX_VERTEX_POS.x * WEAPON_SCL;
	
	//カプセルコライダーの高さ
	static constexpr float CAPSULE_COL_HEIGHT = (MAX_VERTEX_POS.y - MIN_VERTEX_POS.y) * WEAPON_SCL;

	// 投げ時間
	static constexpr float THROW_TIME = 1.0f;

	// 戻り時間
	static constexpr float BACK_TIME = 1.0f;

	//武器の所有者対象
	Transform* ownerTransform_;

	// 武器の攻撃オブジェクト対象
	Transform* attackObjectTransform_;

	//追従するフレームの番号
	int followFrameNo_;

	//武器の相対位置
	VECTOR localPos_;

	//武器の相対回転
	VECTOR localRot_;

	//ダメージフラグ
	bool isDamage_;

	// 投げステップ
	float throwStep_;

	// 開始位置
	VECTOR backStartPos_;

	// 状態
	STATE state_;

	// 状態遷移管理マップ
	std::unordered_map<STATE, std::function<void()>> stateChangesMap_;

	// 更新処理
	std::function<void()> update_;

	// 状態遷移処理
	void ChangeState(const STATE state);
	void ChangeStateNone();
	void ChangeStateFollow();
	void ChangeStateThrow();
	void ChangeStateBack();

	// 状態別更新処理
	void UpdateNone() {};
	void UpdateFollow();
	void UpdateThrow();
	void UpdateBack();
};