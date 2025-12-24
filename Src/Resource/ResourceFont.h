#pragma once
#include "ResourceBase.h"

class ResourceFont : public ResourceBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="path">パス名</param>
	/// <param name="sceneId">シーンID</param>
	/// <param name="fontName">フォント名前</param>
	ResourceFont(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId, const std::wstring& fontName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ResourceFont()override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override;

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release()override;

	/// <summary>
	/// フォントの名前を返す
	/// </summary>
	/// <returns></returns>
	const std::wstring GetFontName() const { return fontName_; }

private:

	//フォントの名前
	std::wstring fontName_;

};

