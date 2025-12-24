#pragma once
#include "../ActorBase.h"

class KeyBlade : public ActorBase
{
public:

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
	/// 追従対象とフレーム位置の指定
	/// </summary>
	/// <param name="targetTransform">追従対象のトランスフォーム</param>
	/// <param name="frameNo">フレーム番号</param>
	void SetTargetAndFrameNo(Transform* targetTransform, const int frameNo);

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

	//武器の追従対象
	Transform* targetTransform_;

	//追従するフレームの番号
	int followFrameNo_;

	//武器の相対位置
	VECTOR localPos_;

	//武器の相対回転
	VECTOR localRot_;

	//ダメージフラグ
	bool isDamage_;
};