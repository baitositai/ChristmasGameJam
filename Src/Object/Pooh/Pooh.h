#pragma once
#include "../ActorBase.h"
#include<vector>

class Pooh :
    public ActorBase 
{
public:
	enum class STATE {
		SIT			//座る
		,STAND_UP	//立つ
		,WALK		//歩く
		,BACK		//戻る
		,HIT		//衝突	
	};

	static constexpr int GOAL_POS_NUM = 6; //目的地の数

    Pooh();
    ~Pooh()override;

	void Init()override;
	void Update()override;
	void Draw()override;
	void DrawDebug();

	void StartWalk();	//散歩開始
	void HitObject();	//物体に衝突した

private:
	STATE state_;	//プーの状態

	using Update_f = void(Pooh::*)(); //状態更新関数の型定義
	Update_f updateFunc_; //状態更新関数ポインタ

	//各種更新で必要な変数
	int stayTimer_;							//滞在タイマー
	VECTOR moveDir_;						//移動方向
	VECTOR shuffleGoalPositions_[GOAL_POS_NUM]; //目的地シャッフル用配列
	std::vector<VECTOR> useGoalPositions_;	//使用するゴール位置リスト
	int useGoalNum_;						//使用するゴール位置数
	int currentGoalIndex_;					//現在のゴール位置インデックス
	bool isFinishMove_;						//移動完了フラグ


	void UpdateSit();		//座る状態の更新
	void UpdateStandUp();	//立つ状態の更新
	void UpdateWalk();		//歩く状態の更新
	void UpdateBack();		//戻る状態の更新
	void UpdateHit();		//衝突状態の更新

	//状態遷移
	void ChangeState(const STATE _next);
	//移動処理
	void Move();
	//移動方向設定
	void SetMoveDir();
	//目的地の設定(散歩)
	void SetGoalPositionsForWalk();
	//目的地の設定(初期位置)
	void SetGoalPositionsForSit();

	//疑似乱数
	int GetRandMinMax(int _min, int _max);
	//目的地位置のシャッフル
	void ShuffleGoalPositions();
};

