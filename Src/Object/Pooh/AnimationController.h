#pragma once
#include<string>
#include<vector>
#include<unordered_map>

//このWilidiaでは"idle"という名前のアニメーションを必ず入れること！
//NOMALのアニメーションは終了時idleを再生するようにするため


class AnimationController
{
public:
	/// <summary>
	/// 再生種類
	/// </summary>
	enum class PLAY_TYPE {
		NOMAL,	//通常再生
		LOOP,	//ループ再生
		RETURN,	//逆再生
		MAX
	};

	/// <summary>
	/// アニメーション名
	/// <summary>
	enum class ANIM_NAME {
		IDLE,
		WALK,
		STAND_UP,
		SIT_DOWN,
		HIT_FLY,
		MAX
	};

	//アニメーション関連情報
	struct AnimationInfo {
		PLAY_TYPE type;	//再生タイプ
		int idx;		//アニメーション番号
		float total;	//総再生時間
	};

	AnimationController(int& _model);
	~AnimationController(void);

	/// <summary>
	/// アニメーション追加関数
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_attach">アニメーション番号</param>
	/// <param name="_type">再生タイプ</param>
	void Add(const ANIM_NAME _animType,const int _attach, const PLAY_TYPE _type);
	/// <summary>
	/// 再生開始処理
	/// </summary>
	/// <param name="_name">登録名</param>
	/// <param name="_speed">再生速度</param>
	/// <param name="_next">連続して再生する物たち<最後以外にLOOPのものを入れないこと！！></param>
	void Play(const ANIM_NAME _animType, const float _speed, const std::vector<ANIM_NAME> _next = {});
	/// <summary>
	/// 連続して再生するアニメーションを途中で追加する
	/// </summary>
	/// <param name="_name">追加するアニメーション名</param>
	void AddNextAnim(ANIM_NAME _animType);
	/// <summary>
	/// 連続して再生するアニメーションを途中で追加する
	/// </summary>
	/// <param name="_name">追加するアニメーション名(複数)</param>
	void AddNextAnim(const std::vector<ANIM_NAME> _animTypes);
	void Update(void);

	/// <summary>
	/// 再生速度(割合)を設定
	/// </summary>
	/// <param name="_percent">0％～100％</param>
	void ChangeSpeedRate(const float _percent);

private:
	//アニメーション更新処理
	void UpdateNomalAnim(void);
	void UpdateReturnAnim(void);

	//アニメーション終了時処理
	void FinishAnimNomal(void);
	void FinishAnimLoop(void);
	void FinishAnimReturn(void);

	int& modelId_;	//モデルID
	std::unordered_map<ANIM_NAME, AnimationInfo>animDatas_;	//アニメーションデータ総まとめ
	AnimationInfo activeAnim_;	//再生中のアニメーション情報
	int attachAnim_;//実際の再生しているもの
	float speedAnim;//再生速度
	float counter_;	//更新カウンター
	float speedRate_;

	std::vector<ANIM_NAME> nextAnim_;	//次に再生するアニメーション(LOOP以外に適用)<最終以外にLOOＰを入れないこと>

	using FinishAnimation = void(AnimationController::*)(void);
	using UpdateAnimation = void(AnimationController::*)(void);
	FinishAnimation finishAnim_;
	UpdateAnimation updateAnim_;
};

