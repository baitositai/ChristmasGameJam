#include <DxLib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include "../../Application.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Resource/ResourceBase.h"
#include "../../Resource/ResourceEffect.h"
#include "../../Resource/ResourceFont.h"
#include "../../Resource/ResourceModel.h"
#include "../../Resource/ResourcePixelShader.h"
#include "../../Resource/ResourceSound.h"
#include "../../Resource/ResourceSprite.h"
#include "../../Resource/ResourceTexture.h"
#include "../../Resource/ResourceVertexShader.h"
#include "ResourceManager.h"

// JSON名前空間
using json = nlohmann::json;

// 文字列からenum の対応表
static const std::unordered_map<std::string, ResourceBase::RESOURCE_TYPE> RESOURCE_TYPE_MAP =
{
	{"model", ResourceBase::RESOURCE_TYPE::MODEL},
	{"sprite", ResourceBase::RESOURCE_TYPE::SPRITE},
	{"texture", ResourceBase::RESOURCE_TYPE::TEXTUR},
	{"sound", ResourceBase::RESOURCE_TYPE::SOUND},
	{"font", ResourceBase::RESOURCE_TYPE::FONT},
	{"effect", ResourceBase::RESOURCE_TYPE::EFFECT},
	{"pixelShader", ResourceBase::RESOURCE_TYPE::PIXEL_SHADER},
	{"vertexShader", ResourceBase::RESOURCE_TYPE::VERTEX_SHADER}
};

void ResourceManager::Init(void)
{
	//ローカル変数を定義
	int divX = -1;
	int divY = -1;
	int sizeX = -1;
	int sizeY = -1;
	int sceneId = -1;
	std::string key = "";
	std::string soundType = "";
	std::string stringType = "";
	std::wstring path = L"";
	std::string fontName = "";

	//JSONファイルからリソース情報を読み込む
	//JSONファイル読み込み
	std::ifstream ifs((Application::PATH_JSON + "Resources.json").c_str());
	
	//読み込めない場合アサート
	assert(ifs.is_open() && "ファイルが開けません");
	
	json j;
	ifs >> j;

	//配列を走査
	for (auto& res : j["resources"])
	{
		//共通項目の情報受け取り
		key = res["key"].get<std::string>();
		stringType = res["type"].get<std::string>();
		path = UtilityCommon::GetWStringFromString(res["path"].get<std::string>());
		sceneId = res["sceneId"].get<int>();

		//列挙型へ変換
		auto it = RESOURCE_TYPE_MAP.find(stringType);
		assert(it != RESOURCE_TYPE_MAP.end() && "登録されてない種類です");
		ResourceBase::RESOURCE_TYPE type = it->second;
		
		//情報の格納
		std::unique_ptr<ResourceBase> resource;
		switch (type)
		{
		case ResourceBase::RESOURCE_TYPE::MODEL:
			resource = make_unique<ResourceModel>(type, path, sceneId);
			break;

		case ResourceBase::RESOURCE_TYPE::TEXTUR:
			resource = make_unique<ResourceTexture>(type, path, sceneId);
			break;

		case ResourceBase::RESOURCE_TYPE::SPRITE:
			divX = res["divX"].get<int>();
			divY = res["divY"].get<int>();
			sizeX = res["sizeX"].get<int>();
			sizeY= res["sizeY"].get<int>();
			resource = make_unique<ResourceSprite>(type, path, sceneId, divX, divY, sizeX, sizeY);
			break;

		case ResourceBase::RESOURCE_TYPE::EFFECT:
			resource = make_unique<ResourceEffect>(type, path, sceneId);
			break;

		case ResourceBase::RESOURCE_TYPE::SOUND:
			soundType = res["soundType"].get<std::string>();
			resource = make_unique<ResourceSound>(type, path, soundType, sceneId);
			break;

		case ResourceBase::RESOURCE_TYPE::FONT:
			fontName = UtilityCommon::ConvertUtf8ToSjis((res["fontName"].get<std::string>().c_str()));
			resource = make_unique<ResourceFont>(type, path, sceneId, UtilityCommon::GetWStringFromString(fontName));
			break;

		case ResourceBase::RESOURCE_TYPE::PIXEL_SHADER:
			resource = make_unique<ResourcePixelShader>(type, path, sceneId);
			break;

		case ResourceBase::RESOURCE_TYPE::VERTEX_SHADER:
			resource = make_unique<ResourceVertexShader>(type, path, sceneId);
			break;

		default:
			break;
		}

		//マップに格納
		resourcesMap_.emplace(key, std::move(resource));
	}

	//共通項目のリソースを読み込む
	for (auto& p : resourcesMap_)
	{
		//共通項目のリソースを読み込む
		if (p.second->GetSceneId() == 0)
		{
			//読み込み処理
			p.second->Load();		
			
			//コピーコンストラクタ
			loadedMap_.emplace(p.first, p.second.get());
		}
	}
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::SceneChangeResource(const int nextSceneId)
{
	// 現在読み込んだリソースを解放
	for (auto it = loadedMap_.begin(); it != loadedMap_.end(); )
	{
		// 共通リソース以外を破棄する
		if (it->second->GetSceneId() != 0)
		{
			it->second->Release();
			it = loadedMap_.erase(it); // eraseして次へ
		}
		else
		{
			++it; // 共通リソースは残す
		}
	}

	// 次のシーンのリソースを読み込む
	for (auto& p : resourcesMap_)
	{
		// 指定したシーンのリソースだけロード
		if (p.second->GetSceneId() == nextSceneId)
		{
			p.second->Load();
			loadedMap_.emplace(p.first, p.second.get());
		}
	}
}

std::unordered_map<std::string, ResourceSound*>& ResourceManager::GetSceneSounds() const
{
	// シーンリソースを格納するマップ
	static std::unordered_map<std::string, ResourceSound*> sceneSounds;

	// 空じゃないか確認
	if (loadedMap_.empty())
	{
		assert(false && "読み込んだリソースがありません");
		return sceneSounds;
	}

	// 指定した種類のリソースを探す
	for (auto& p : resourcesMap_)
	{
		// 指定した種類のリソースだけを抽出
		if (p.second->GetType() == ResourceBase::RESOURCE_TYPE::SOUND)
		{
			// 読み込んだリソースか確認
			auto it = loadedMap_.find(p.first);
			if (it != loadedMap_.end())
			{
				// 実行時型チェック
				ResourceSound* sound = dynamic_cast<ResourceSound*>(p.second.get());
				
				// 型が正しい場合
				if (sound != nullptr)
				{
					// 格納	
					sceneSounds.emplace(p.first, sound);
				}
				// 型が不正な場合
				else 
				{
					assert(false && "型情報がSOUNDなのにキャストに失敗しました");
				}
			}
		}
	}

	// シーンリソースを返す
	return sceneSounds;
}

std::unordered_map<std::string, ResourceEffect*>& ResourceManager::GetSceneEffects() const
{
	// シーンリソースを格納するマップ
	static std::unordered_map<std::string, ResourceEffect*> sceneEffects;

	// 空じゃないか確認
	if (loadedMap_.empty())
	{
		assert(false && "読み込んだリソースがありません");
		return sceneEffects;
	}

	// 指定した種類のリソースを探す
	for (auto& p : resourcesMap_)
	{
		// 指定した種類のリソースだけを抽出
		if (p.second->GetType() == ResourceBase::RESOURCE_TYPE::EFFECT)
		{
			// 読み込んだリソースか確認
			auto it = loadedMap_.find(p.first);
			if (it != loadedMap_.end())
			{
				// 実行時型チェック
				ResourceEffect* sound = dynamic_cast<ResourceEffect*>(p.second.get());

				// 型が正しい場合
				if (sound != nullptr)
				{
					// 格納	
					sceneEffects.emplace(p.first, sound);
				}
				// 型が不正な場合
				else
				{
					assert(false && "型情報がEFFECTなのにキャストに失敗しました");
				}
			}
		}
	}

	// シーンリソースを返す
	return sceneEffects;
}

const int ResourceManager::GetHandle(const std::string& key) const
{
	//リソースを探す
	const auto& res = loadedMap_.find(key);

	//取得できないときアサート
	assert(res != loadedMap_.end() && "指定したキーのリソースは取得できないです");

	//リソースを返す
	return res->second->GetHandle();
}

const int* ResourceManager::GetHandles(const std::string& key) const
{
	// リソースを探す
	auto it = loadedMap_.find(key);
	assert(it != loadedMap_.end() && "指定したキーのリソースは取得できないです");

	// dynamic_cast で派生型にキャスト
	auto sprite = dynamic_cast<ResourceSprite*>(it->second);
	assert(sprite && "リソースの型が ResourceSprite ではありません");

	// ハンドルIDを返す
	return sprite->GetHandleIds();
}

const std::wstring ResourceManager::GetFontName(const std::string& key) const
{
	// リソースを探す
	auto it = loadedMap_.find(key);
	assert(it != loadedMap_.end() && "指定したキーのリソースは取得できないです");

	// dynamic_cast で派生型にキャスト
	auto font = dynamic_cast<ResourceFont*>(it->second);
	assert(font && "リソースの型が ResourceFont ではありません");

	// ハンドルIDを返す
	return font->GetFontName();
}

ResourceManager::ResourceManager(void)
{

}