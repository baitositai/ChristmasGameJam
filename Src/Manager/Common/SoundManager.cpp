#include <DxLib.h>
#include <cassert>
#include "../../Resource/ResourceSound.h"
#include "ResourceManager.h"
#include "SoundManager.h"

// 音源種類の省略形
using TYPE = SoundType::TYPE;
using BGM = SoundType::BGM;
using SE = SoundType::SE;

void SoundManager::SceneChangeResources()
{
	// 一時マップ
	std::unordered_map<BGM, SoundData> tempBgmMap;
	std::unordered_map<SE, SoundData> tempSeMap;

	// シーン用リソースを取得
	std::unordered_map<std::string, ResourceSound*> resourceMap = ResourceManager::GetInstance().GetSceneSounds();

	// リソースが空の場合は終了
	if (resourceMap.empty())
	{
		return;
	}

	// 新規リソースを追加
	for (auto it = resourceMap.begin(); it != resourceMap.end(); )
	{
		// 音源の種類を取得
		TYPE type = it->second->GetSoundType();

		// サウンドデータの格納
		SoundData soundData = {};
		soundData.playType = GetPlayType(type);		// 再生方法
		soundData.handle = it->second->GetHandle();	// ハンドル
		soundData.volume = 0;						// 音量

		// 音源の種類別に格納
		if (type == TYPE::BGM)
		{
			// キーから列挙型値に変換
			const BGM bgmKey = SoundType::KEY_BGM_MAP.at(it->first);

			// 格納
			tempBgmMap.emplace(bgmKey, soundData);
		}
		else if (type == TYPE::SE)
		{
			// キーから列挙型値に変換
			const SE seKey = SoundType::KEY_SE_MAP.at(it->first);

			// 格納
			tempSeMap.emplace(seKey, soundData);
		}

		// 次へ
		++it;
	}

	// 読み込み済みマップを更新
	loadedBgmMap_ = tempBgmMap;
	loadedSeMap_ = tempSeMap;
}

void SoundManager::Update()
{
	// 空の場合は処理を行わない
	if (fadeSeMap_.empty())
	{
		return;
	}

	for (auto it = fadeSeMap_.begin(); it != fadeSeMap_.end(); )
	{
		// 各種情報の取得・定義
		const SE key = it->first;		// キー
		FadeData& data = it->second;	// フェードデータ
		bool erased = false;			// 削除判定

		// フェードイン処理
		if (data.fadeType == FADE::FADE_IN)
		{
			// 音量増加
			data.soundData.volume += MAX / defaultVolumeSe_;

			// 最大音量に達した場合
			if (data.soundData.volume >= data.goalVolume)
			{
				data.soundData.volume = data.goalVolume;	// 最大音量に設定
				it = fadeSeMap_.erase(it);					// マップ内から削除
				erased = true;								// 削除判定をtrueに
			}
		}
		// フェードアウト処理
		else if (data.fadeType == FADE::FADE_OUT)
		{
			// 音量減少
			data.soundData.volume -= MAX / defaultVolumeSe_;

			// 音量が0以下になる場合
			if (data.soundData.volume <= 0)
			{
				data.soundData.volume = 0;		// 音量を0に設定
				it = fadeSeMap_.erase(it);		// マップ内から削除
				erased = true;					// 削除判定をtrueに
			}
		}

		// 削除判定が出ていない場合
		if (!erased)
		{
			// 音量変更
			ChangeVolumeSe(data.soundData.volume, key);
			++it;
		}
	}
}

void SoundManager::PlayBgm(const SoundType::BGM key, const bool topPos, const int volume)
{
	//音源があるか確認
	auto it = loadedBgmMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedBgmMap_.end()&& "追加していない音源を再生しようとしています");

	// 音量調整
	loadedBgmMap_[key].volume = volume;
	ChangeVolumeBgm(volume, key);

	//再生
	PlaySoundMem(loadedBgmMap_[key].handle, loadedBgmMap_[key].playType, topPos);
}

void SoundManager::PlaySe(const SoundType::SE key, const bool topPos, const int volume)
{
	//音源があるか確認
	auto it = loadedSeMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end()&& "追加していない音源を再生しようとしています");

	// 音量調整
	loadedSeMap_[key].volume = volume;
	ChangeVolumeSe(volume, key);

	//再生
	PlaySoundMem(loadedSeMap_[key].handle, loadedSeMap_[key].playType, topPos);
}

void SoundManager::StopBgm(const SoundType::BGM key)
{
	//音源があるか確認
	auto it = loadedBgmMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedBgmMap_.end()&& "追加していない音源を停止しようとしています");

	//停止
	StopSoundMem(loadedBgmMap_[key].handle);
}

void SoundManager::StopSe(const SoundType::SE key)
{
	//音源があるか確認
	auto it = loadedSeMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end()&& "追加していない音源を停止しようとしています");

	//停止
	StopSoundMem(loadedSeMap_[key].handle);
}

void SoundManager::FadeInSe(const SoundType::SE key, const int speed)
{
	//音源があるか確認
	auto it = loadedSeMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end() && "追加していない音源をフェードしようとしています");
	
	// フェード情報をコピー
	FadeData data = {};
	data.soundData = loadedSeMap_[key];

	// フェードイン設定
	data.fadeType = FADE::FADE_IN;

	//フェード管理に格納
	fadeSeMap_.emplace(key, data);
}

void SoundManager::FadeOutSe(const SoundType::SE key, const int speed)
{
	//音源があるか確認
	auto it = loadedSeMap_.find(key);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end() && "追加していない音源をフェードしようとしています");

	// 再生中か確認
	if (!CheckSoundMem(loadedSeMap_[key].handle))
	{
		return;
	}

	// フェード情報をコピー
	FadeData data = {};
	data.soundData = loadedSeMap_[key];

	// フェードアウト設定
	data.fadeType = FADE::FADE_OUT;

	//フェード管理に格納
	fadeSeMap_.emplace(key, data);
}

void SoundManager::StopAllSe()
{
	for (auto& se : loadedSeMap_)
	{
		if (CheckSoundMem(se.second.handle))
		{
			StopSoundMem(se.second.handle);
		}
	}
}

void SoundManager::ChangeVolumeBgm(const int volume, const SoundType::BGM bgm)
{
	//音源の音量変更
	ChangeVolumeSoundMem(MAX * volume / defaultVolumeBgm_, loadedBgmMap_[bgm].handle);
}

void SoundManager::ChangeVolumeSe(const int volume, const SoundType::SE se)
{
	//音源の音量変更
	ChangeVolumeSoundMem(MAX * volume / defaultVolumeSe_, loadedSeMap_[se].handle);
}

bool SoundManager::IsCheckPlaySe(const SoundType::SE se) const
{
	//音源があるか確認
	auto it = loadedSeMap_.find(se);

	//音源がない場合強制停止
	assert(it != loadedSeMap_.end() && "追加していない音源の確認をしようとしています");

	// 再生中か確認
	return CheckSoundMem(loadedSeMap_.at(se).handle);
}

void SoundManager::Release()
{
	loadedBgmMap_.clear();
	loadedSeMap_.clear();
	fadeSeMap_.clear();
}

int SoundManager::GetPlayType(const TYPE soundType)
{
	switch (soundType)
	{
	case TYPE::SE:
		return DX_PLAYTYPE_BACK;
		break;

	case TYPE::BGM:
		return DX_PLAYTYPE_LOOP;
		break;

	default:
		break;
	}
	return -1;
}

SoundManager::SoundManager()
{
	defaultVolumeBgm_ = DEFAULT_VOLUME;
	defaultVolumeSe_ = DEFAULT_VOLUME;
	fadeSeMap_.clear();
}
