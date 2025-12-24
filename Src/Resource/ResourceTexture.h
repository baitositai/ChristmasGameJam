#pragma once
#include "ResourceBase.h"

class ResourceTexture : public ResourceBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="path">パス名</param>
	/// <param name="sceneId">シーンID</param>
	ResourceTexture(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceTexture()override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release()override;
};

