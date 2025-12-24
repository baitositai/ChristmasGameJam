#pragma once
#include <functional>
#include <map>
#include "../Object/Player/Player.h"
#include "../ActorBase.h"

class ScoreManager;

class FallObjectBase:public ActorBase
{
public:

	//球の半径
	static constexpr float RADIUS = 45.0f;

	enum class FALL_OBJ_TYPE
	{	
		RIGHT_OBJ,
		LEFT_OBJ
	};

	enum class STATE
	{
		NONE,
		MOVE,
		JUMP,
		DEATH
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FallObjectBase(const VECTOR _startPos, const VECTOR _goalPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~FallObjectBase();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();
	
	/// <summary>
	/// プレイヤーの攻撃がヒットしたら
	/// </summary>
	/// <param name="_actState">プレイヤーの攻撃方向</param>
	void HitPlayerAttack(const Player::ACTION_STATE _actState);

	/// <summary>
	/// プーとヒットしたら
	/// </summary>
	/// <param name=""></param>
	void HitPooh(void);

	/// <summary>
	/// オブジェクトタイプの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>右オブジェクトか左オブジェクトか</returns>
	inline const FALL_OBJ_TYPE GetObjectType(void) { return type_; }

	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>状態</returns>
	const STATE GetState() { return state_; }

	/// <summary>
	/// スコア加算
	/// </summary>
	/// <param name=""></param>
	virtual void AddScore() = 0;
	
protected:


	//ジャンプ時の重力
	static constexpr float GRAVITY = 1.3f;

	//ジャンプ力
	static constexpr float JUMP_POW = 20.0f;
	static constexpr float JUMP_SIDE_SPD = 20.0f;

	//レーンに流れる速度
	static constexpr float MOVE_SPD = 2.0f;

	//移動中の画面外Z座標
	static constexpr float MOVE_LIMIT_Z = -200.0f;

	//ジャンプ中配列削除するまでの時間
	static constexpr float JUMP_ERASE_TIME = 2.0f;

	//吹っ飛ばしによるスコアポイント
	static constexpr int SCORE_POINT = 1;

	//落ちるオブジェクトの種類
	FALL_OBJ_TYPE type_;

	//状態遷移
	STATE state_;
	std::map<STATE, std::function<void()>>changeState_;
	std::function<void()>updateState_;

	//プレイヤーの攻撃方向
	Player::ACTION_STATE playerActState_;

	//初期座標
	const VECTOR startPos_;
	//終点座標
	const VECTOR goalPos_;

	//スコアマネージャ
	ScoreManager& scoreMng_;

	//ジャンプ力
	float jumpPow_;
	//ジャンプ加速度
	float velocity_;
	//ジャンプカウンタ(一定時間経ったら配列から削除)
	float jumpCnt_;

	/// <summary>
	/// 状態遷移
	/// </summary>
	/// <param name="_state">状態</param>
	void ChangeState(const STATE& _state);

	//更新系
	void UpdateNone();		//None状態
	void UpdateMove();		//流れてくる状態
	void UpdateJump();		//吹っ飛び処理
	void UpdateDeath();		//死亡処理

	//遷移系
	void ChangeNone();		//None状態
	void ChangeMove();		//流れてくる状態
	void ChangeJump();		//吹っ飛び処理
	void ChangeDeath();		//死亡
private:
	
	/// <summary>
	/// 状態の登録
	/// </summary>
	void AddState();
};

