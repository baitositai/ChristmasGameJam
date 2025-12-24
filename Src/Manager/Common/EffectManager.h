#pragma once
#include <DxLib.h>
#include <unordered_map>
#include "../../Common/Quaternion.h"
#include "../../Template/Singleton.h"
#include "EffectType.h"

class EffectManager : public Singleton<EffectManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<EffectManager>;

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void SceneChangeResources();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="pos">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="quaRot">クォータニオン回転</param>
	/// <param name="speed">再生速度</param>
	/// <param name="isLoop">ループ判定</param>
	void Play(const EffectType::TYPE type, const VECTOR& pos, const VECTOR& scale, const Quaternion& quaRot, const float speed = 1.0f, const bool isLoop = true);
	
	/// <summary>
	/// エフェクトの追従
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="pos">座標</param>
	/// <param name="scale">大きさ</param>
	/// <param name="quaRot">クォータニオン</param>
	/// <param name="speed">再生速度</param>
	/// <param name="index">エフェクト格納番号(指定しない場合一つ目を編集)</param>
	void Sync(const EffectType::TYPE type, const VECTOR& pos, const VECTOR& scale, const Quaternion& quaRot, const float speed = 1.0f, const int index = 0);

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="index">エフェクトの格納番号(指定しない場合指定した種類のエフェクトをすべて停止)</param>
	void Stop(const EffectType::TYPE type, const int index = -1);

	/// <summary>
	/// 再生判定
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="index">エフェクト格納番号</param>
	/// <returns>trueの場合再生中, falseの場合非再生中</returns>
	const bool IsPlay(const EffectType::TYPE type, const int index = 0) const;

private:

	// エフェクトの情報
	struct EffectData
	{
		int playHandle = 0;
		VECTOR pos = {};
		VECTOR scl = {};
		Quaternion quaRot = {};
		float speed = 0.0f;
		bool isLoop = false;
	};

	// エフェクトのリソース管理マップ
	std::unordered_map<EffectType::TYPE, int> effectResMap_;

	// 再生中のエフェクト管理マップ
	std::unordered_map<EffectType::TYPE, std::vector<EffectData>> effectPlayMap_;

	// コンストラクタ
	EffectManager();

	// デストラクタ
	~EffectManager();
};

