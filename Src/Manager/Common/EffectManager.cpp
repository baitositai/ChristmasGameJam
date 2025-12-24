#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include<Effekseer.h>
#include "../../Utility/Utility3D.h"
#include "../../Resource/ResourceEffect.h"
#include "ResourceManager.h"
#include "EffectManager.h"

using TYPE = EffectType::TYPE;

void EffectManager::SceneChangeResources()
{
	ResourceManager& res = ResourceManager::GetInstance();

	// 一時マップ
	std::unordered_map<TYPE, int> tempMap;

	// シーン用リソースを取得
	std::unordered_map<std::string, ResourceEffect*> resourceMap = res.GetSceneEffects();

	// リソースが空の場合は終了
	if (resourceMap.empty())
	{
		return;
	}

	// 新規リソースを追加
	for (auto it = resourceMap.begin(); it != resourceMap.end(); )
	{
		// 音源の種類を取得
		TYPE type = EffectType::KEY_MAP.at(it->first);

		// エフェクトデータの格納
		tempMap.emplace(type, it->second->GetHandle());

		// 次へ
		++it;
	}

	// 読み込み済みマップを更新
	effectResMap_ = tempMap;
}

void EffectManager::Update()
{
	// 空の場合
	if (effectPlayMap_.empty())
	{
		// 更新を回さない
		return;
	}


	for (auto& effects : effectPlayMap_)
	{
		int size = static_cast<int>(effects.second.size()) - 1;
		for (int i = size; i >= 0; i--)
		{
			// 再生中の場合
			if (IsPlay(effects.first, i))
			{
				continue;
			}
			else
			{
				// 停止
				Stop(effects.first, i);

				// 要素の削除
				effects.second.erase(effects.second.begin() + i);
			}
		}
	}
}

void EffectManager::Play(const TYPE type, const VECTOR& pos, const VECTOR& scale, const Quaternion& quaRot, const float speed, const bool isLoop)
{	
	// 再生中のハンドルを生成
	int playId = PlayEffekseer3DEffect(effectResMap_[type]);

	// 再生データを生成
	EffectData effectData = {};
	effectData.playHandle = playId;

	// 指定種類の配列が存在しない場合
	if (effectPlayMap_.count(type) == 0)
	{
		// 配列を生成して格納
		std::vector<EffectData> effectArray;
		effectArray.push_back(effectData);
		effectPlayMap_.emplace(type, std::move(effectArray));
	}
	else
	{
		// データの格納
		effectPlayMap_.at(type).push_back(effectData);
	}

	// インデックスの取得
	int index = static_cast<int>(effectPlayMap_[type].size()) - 1;

	// パラメータ設定
	Sync(type, pos, scale, quaRot, speed, index);
}

void EffectManager::Sync(const TYPE type, const VECTOR& pos, const VECTOR& scale, const Quaternion& quaRot, const float speed, const int index)
{
	// パラメータの更新
	EffectData& effectData = effectPlayMap_[type][index];
	effectData.pos = pos;
	effectData.scl = scale;
	effectData.quaRot = quaRot;
	effectData.speed = speed;

	// パラメータの反映
	SetPosPlayingEffekseer3DEffect(effectData.playHandle, effectData.pos.x, effectData.pos.y, effectData.pos.z);
	const VECTOR rot = effectData.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(effectData.playHandle, effectData.scl.x, effectData.scl.y, effectData.scl.z);
	SetScalePlayingEffekseer3DEffect(effectData.playHandle, effectData.scl.x, effectData.scl.y, effectData.scl.z);
	SetSpeedPlayingEffekseer3DEffect(effectData.playHandle, effectData.speed);
}

void EffectManager::Stop(const TYPE type, const int index)
{
	int size = static_cast<int>(effectPlayMap_[type].size());
	
	// 配列内の場合
	if (size > index && index >= 0)
	{	
		// 指定した番号のエフェクトの停止
		StopEffekseer3DEffect(effectPlayMap_[type][index].playHandle);
	}
	// 配列外の場合
	else
	{
		// 指定した種類のエフェクトをすべて停止
		for (auto& effect : effectPlayMap_[type])
		{
			StopEffekseer3DEffect(effect.playHandle);
		}
	}
}

const bool EffectManager::IsPlay(const TYPE type, const int index) const
{
	//再生が終わっている場合
	if (IsEffekseer3DEffectPlaying(effectPlayMap_.at(type)[index].playHandle) == -1)
	{
		// 終了判定
		return false;
	}

	// 再生中
	return true;
}

EffectManager::EffectManager()
{
	effectPlayMap_.clear();
	effectResMap_.clear();
}

EffectManager::~EffectManager()
{
}
