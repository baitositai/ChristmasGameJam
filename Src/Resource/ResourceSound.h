#pragma once
#include <vector>
#include "ResourceBase.h"
#include "../Manager/Common/SoundType.h"

class ResourceSound : public ResourceBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="path">パス名</param>
	/// <param name="soundType">サウンドの種類</param>
	/// <param name="sceneId">シーンID</param>
	ResourceSound(const RESOURCE_TYPE type, const std::wstring& path, const std::string& soundType, const int sceneId);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceSound()override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release()override;

	/// <summary>
	/// ハンドルを返す
	/// </summary>
	/// <returns>ハンドルを返す</returns>
	const int GetHandle() override;

	/// <summary>
	/// サウンドの種類を返す
	/// </summary>
	/// <returns>サウンドの種類</returns>
	const SoundType::TYPE GetSoundType() const { return soundType_; }

private:

	// タグ比較用
	const std::string KEY_BGM = "bgm";
	const std::string KEY_SE = "se";

	//複製した音声を管理
	std::vector<int> duplicateSounds_;	
	
	// サウンドの種類
	SoundType::TYPE soundType_;

	// サウンドの種類の列挙型を取得
	SoundType::TYPE GetSoundTypeEnum(const std::string& type);
};