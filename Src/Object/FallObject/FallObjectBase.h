#pragma once
#include <functional>
#include <map>
#include "../ActorBase.h"

class FallObjectBase:public ActorBase
{
public:
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
	/// 状態遷移
	/// </summary>
	/// <param name="_state">状態</param>
	void ChangeState(const STATE& _state);

	/// <summary>
	/// オブジェクトタイプの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>右オブジェクトか左オブジェクトか</returns>
	inline const FALL_OBJ_TYPE GetObjectType(void) { return type_; }
	
protected:

	//球の半径
	static constexpr float RADIUS = 25.0f;
	//ジャンプ時の重力
	static constexpr float GRAVITY = 1.3f;

	//ジャンプ力
	static constexpr float JUMP_POW = 20.0f;
	static constexpr float JUMP_SIDE_SPD = 20.0f;

	//落ちるオブジェクトの種類
	FALL_OBJ_TYPE type_;

	//状態遷移
	STATE state_;
	std::map<STATE, std::function<void(void)>>changeState_;
	std::function<void(void)>updateState_;

	//初期座標
	const VECTOR startPos_;
	//終点座標
	const VECTOR goalPos_;
	//ジャンプ力
	float jumpPow_;
	//ジャンプ加速度
	float velocity_;

	//更新系
	void UpdateNone();		//None状態
	void UpdateMove();		//流れてくる状態
	void UpdateJump();		//吹っ飛び処理

	//遷移系
	void ChangeNone();		//None状態
	void ChangeMove();		//流れてくる状態
	void ChangeJump();		//吹っ飛び処理
private:
	
	/// <summary>
	/// 状態の登録
	/// </summary>
	void AddState();
};

