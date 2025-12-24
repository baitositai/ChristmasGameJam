#pragma once
#include <unordered_map>
#include <string>
#include "../../Template/Singleton.h"
#include "../../Resource/ResourceBase.h"

class ResourceBase;
class ResourceSound;
class ResourceEffect;

class ResourceManager : public Singleton<ResourceManager>
{

	friend class Singleton<ResourceManager>;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// リソースの完全解放
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// シーン変更の処理
	/// </summary>
	/// <param name="nextSceneId">次シーンのID</param>
	void SceneChangeResource(const int nextSceneId);

	/// <summary>
	/// シーンで用いるサウンドを返す
	/// </summary>
	/// <returns>シーンで用いるサウンド</returns>
	std::unordered_map<std::string, ResourceSound*>& GetSceneSounds() const;

	/// <summary>
	/// シーンで用いるエフェクトを返す
	/// </summary>
	/// <returns>シーンで用いるエフェクト</returns>
	std::unordered_map<std::string, ResourceEffect*>& GetSceneEffects() const;

	/// <summary>
	/// 指定したキーのリソースを返す
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <returns>リソースのハンドル</returns>
	const int GetHandle(const std::string& key)const;

	/// <summary>
	/// 指定したキーのリソースを返す
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <returns>リソースのハンドル(主にスプライト)</returns>
	const int* GetHandles(const std::string& key)const;

	/// <summary>
	/// 指定したキーのフォントの名前を返す
	/// </summary>
	/// <param name="key">リソースキー</param>
	/// <returns>フォントの名前</returns>
	const std::wstring GetFontName(const std::string& key)const;

private:

	// リソース管理の対象
	std::unordered_map<std::string, std::unique_ptr<ResourceBase>> resourcesMap_;

	// 読み込み済みリソース
	std::unordered_map<std::string, ResourceBase*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager();
	~ResourceManager() = default;
};