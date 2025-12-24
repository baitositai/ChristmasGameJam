#pragma once
#include <vector>
#include "ResourceBase.h"

class ResourceModel : public ResourceBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="path">パス名</param>
	/// <param name="sceneId">シーンID</param>
	ResourceModel(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceModel()override;

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
	/// <returns>ハンドル</returns>
	const int GetHandle() override;

private:

	//複製したモデルを返す
	std::vector<int> duplicateModels_;

};

