#include <DxLib.h>
#include <cassert>
#include "../Application.h"
#include "FontManager.h"

FontManager::FontManager()
{
	fontHandleMap_.clear();
}

int FontManager::CreateMyFont(const std::wstring& fontName, const int size, const int thickness)
{
	FontKey key{ fontName, size, thickness };  // キー

	auto it = fontHandleMap_.find(key);        // 既存検索
	if (it != fontHandleMap_.end())
	{
		return it->second;  // 既存を返す
	}

	// 生成
	int handle = CreateFontToHandle(
		fontName.c_str(),   // フォント名
		size,               // サイズ
		thickness);         // 太さ

	assert(handle != -1 && L"フォント生成に失敗しました");

	fontHandleMap_[key] = handle;                    // 登録
	return handle;                                   // 生成結果
}

void FontManager::Release()
{
	SceneChangeRelease();
}

void FontManager::SceneChangeRelease()
{
	//生成したフォントを解放
	for (const auto& font : fontHandleMap_)
	{
		DeleteFontToHandle(font.second);
	}

	fontHandleMap_.clear();
}
