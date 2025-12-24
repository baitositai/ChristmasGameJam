#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

// JSON名前空間
using Json = nlohmann::json;

class UtilityLoad
{
public:

	/// <summary>
	/// エクスプローラーからファイルを開く
	/// </summary>
	/// <returns></returns>
	static std::string OpenFileDialog();

	/// <summary>
	/// JSONデータを読み込んでMap型で要素を配列で返す
	/// </summary>
	/// <param name="fileName">読み込むファイル名前</param>
	/// <returns>読み込んだファイルデータ</returns>
	static const std::unordered_map<std::string, std::vector<Json>> GetJsonMapArrayData(const std::string& fileName);

	/// <summary>
	/// JSONデータを読み込んでMap型で返す
	/// </summary>
	/// <param name="fileName">読み込むファイル名前</param>
	/// <returns>読み込んだファイルデータ</returns>
	static const std::unordered_map<std::string, Json> GetJsonMapData(const std::string& fileName);

	/// <summary>
	/// JSONデータを読み込んでvector型で返す
	/// </summary>
	/// <param name="fileName">読み込むファイル名前</param>
	/// <returns>読み込んだファイルデータ</returns>
	static const std::vector<Json> GetJsonArrayData(const std::string& fileName);
};

