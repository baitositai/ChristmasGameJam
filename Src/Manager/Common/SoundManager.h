#pragma once
#include <string>
#include <unordered_map>
#include "../../Template/Singleton.h"
#include "SoundType.h"

class SoundManager : public Singleton<SoundManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<SoundManager>;

public:

	//デフォルトの音量
	static constexpr int DEFAULT_VOLUME = 100;
	
	/// <summary>
	/// シーン遷移する際のリソースの変更
	/// </summary>
	void SceneChangeResources();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="key">BGMのキー</param>
	/// <param name="topPos">再生位置を戻すか</param>
	/// <param name="volume">音量</param>
	void PlayBgm(const SoundType::BGM key, const bool topPos = true, const int volume = DEFAULT_VOLUME);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="key">BGMのキー</param>
	/// <param name="topPos">再生位置を戻すか</param>
	void PlaySe(const SoundType::SE key, const bool topPos = true, const int volume = DEFAULT_VOLUME);

	/// <summary>
	/// BGMの停止
	/// </summary>
	/// <param name="sound">BGMのキー</param>
	void StopBgm(const SoundType::BGM key);

	/// <summary>
	/// SEの停止
	/// </summary>
	/// <param name="key">SEのキー</param>
	void StopSe(const SoundType::SE key);

	/// <summary>
	/// SEのフェードイン
	/// </summary>
	/// <param name="key">種類</param>
	/// <param name="speed">フェード速度</param>
	void FadeInSe(const SoundType::SE key, const int speed = 1);

	/// <summary>
	/// SEのフェードアウト
	/// </summary>
	/// <param name="key">種類</param>
	/// <param name="speed">フェード速度</param>
	void FadeOutSe(const SoundType::SE key, const int speed = 1);

	/// <summary>
	/// 全ての再生中のSEの停止
	/// </summary>
	void StopAllSe();

	/// <summary>
	/// 読み込んだ音源の解放
	/// </summary>
	void Release();

	/// <summary>
	/// BGMのデフォルト音量設定
	/// </summary>
	/// <param name="volume">BGMのデフォルト音量</param>
	void SetDefaultVolumeBgm(const int volume) { defaultVolumeBgm_ = volume; }

	/// <summary>
	/// SEのデフォルト音量設定
	/// </summary>
	/// <param name="volume">SEのデフォルト音量</param>
	void SetDefaultVolumeSe(const int volume) { defaultVolumeSe_ = volume; }
	
	/// <summary>
	/// 音量調整(BGM)
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="bgm">BGMの種類</param>
	void ChangeVolumeBgm(const int volume, const SoundType::BGM bgm);

	/// <summary>
	/// 音量調整(SE)
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="se">SEの種類</param>
	void ChangeVolumeSe(const int volume, const SoundType::SE se);

	/// <summary>
	/// 指定したSEが再生中か確認
	/// </summary>
	/// <param name="se">SEの種類</param>
	/// <returns>trueの場合再生中,falseの場合非再生中</returns>
	bool IsCheckPlaySe(const SoundType::SE se) const;

private:

	static constexpr int MAX = 255;	//最大値
	static constexpr int DIV = 100;	//分割数

	enum class FADE
	{		
		NONE,
		FADE_IN,
		FADE_OUT,
	};

	/// <summary>
	/// サウンド情報管理
	/// </summary>
	struct SoundData
	{
		int handle;					// 音源
		int playType;				// 再生方法
		int volume;					// 音量
	};

	/// <summary>
	/// フェードを行う情報管理
	/// </summary>
	struct FadeData
	{
		SoundData soundData;		// サウンドデータ
		FADE fadeType;				// フェードタイプ
		int speed;					// フェード速度
		int goalVolume;				// 目標音量
	};

	// 読み込みリソース管理
	std::unordered_map<SoundType::BGM, SoundData> loadedBgmMap_;	// BGM
	std::unordered_map<SoundType::SE, SoundData> loadedSeMap_;		// SE

	// フェードイン・アウト用リソース管理
	std::unordered_map<SoundType::SE, FadeData> fadeSeMap_;		// SE

	// 音種類別の音量
	int defaultVolumeBgm_;	// BGM
	int defaultVolumeSe_;	// SE

	//再生種類を返す
	int GetPlayType(const SoundType::TYPE soundType);

	// コンストラクタ
	SoundManager();

	// デストラクタ
	~SoundManager() = default;
};