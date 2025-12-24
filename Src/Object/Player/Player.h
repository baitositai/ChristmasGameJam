#pragma once
#include <unordered_map>
#include <functional>
#include "../../Common/Quaternion.h"
#include "../ActorBase.h"

class Capsule;

class Player : public ActorBase
{
public:

	// 攻撃判定用の球体半径
	static constexpr float ATTACK_RADIUS = 30.0f;

	// 攻撃判定用の相対位置
	static constexpr float ATTACK_DISTANCE = 45.0f;

	// 状態
	enum class STATE
	{
		NONE,		
		PLAY,		// プレイ
		ACTION,		// 攻撃
		STAN,		// スタン
	};

	// 攻撃状態
	enum class ACTION_STATE
	{
		NONE,
		RIGHT,		// 右攻撃
		LEFT,		// 左攻撃
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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
	const STATE GetState() const;

private:

	// ローカル回転のデフォルト値(度)
	static constexpr float DEFAULT_LOCAL_QUAROT_Y_DEG = 180.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//移動速度
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;
	static constexpr float SPEED_WARP = 40.0f;

	//プレイヤー回転角度
	static constexpr double ROT_DEG_R = 90;
	static constexpr double ROT_DEG_L = -90;
	static constexpr double ROT_DEG_F = 0;
	static constexpr double ROT_DEG_B = 180;

	// ジャンプ力
	static constexpr float POW_JUMP = 35.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	//カプセル用半径
	static constexpr float RADIUS = 20.0f;

	//プレイヤーカプセル用相対座標
	static constexpr VECTOR PL_CAP_UP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };
	static constexpr VECTOR PL_CAP_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 攻撃時間
	float attackTime_;

	//移動速度
	float moveSpeed_;

	// 攻撃位置
	VECTOR attackPos_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// 状態
	STATE state_;

	// 攻撃状態
	ACTION_STATE actionState_;

	//カプセルの衝突判定
	std::unique_ptr<Capsule> capsule_;

	// 状態遷移管理マップ
	std::unordered_map<STATE, std::function<void()>> stateChangesMap_;

	// 更新処理
	std::function<void()> update_;

	// 状態遷移処理
	void ChangeState(const STATE state);
	void ChangeStateNone();
	void ChangeStatePlay();
	void ChangeStateAction();
	void ChangeStateStan();

	// 状態別更新処理
	void UpdateNone() {};
	void UpdatePlay();
	void UpdateAction();
	void UpdateStan();

	// トランスフォーム初期化
	void InitTransform();

	// アニメーション初期化
	void InitAnimation(); 

	// 移動操作処理
	void ProcessMove();

	// 攻撃操作処理
	void ProcessAction();

	// 移動制限
	void MoveLimit();

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate();
};