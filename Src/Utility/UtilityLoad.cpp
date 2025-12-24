#include <DxLib.h>
#include <fstream>
#include <iostream>
#include "../Application.h"
#include "UtilityLoad.h"

std::string UtilityLoad::OpenFileDialog()
{
    // 現在のカレントディレクトリを保存
    DWORD bufferSize = GetCurrentDirectoryA(0, nullptr);
    std::string originalCwd;
    if (bufferSize != 0)
    {
        std::vector<char> currentDirBuffer(bufferSize);
        if (GetCurrentDirectoryA(bufferSize, currentDirBuffer.data()) != 0)
        {
            originalCwd = currentDirBuffer.data();
        }
    }

    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    
    // 選択されたパスを保持する変数
    std::string selected_path = ""; 
    
    // ファイルを開くダイアログを表示
    if (GetOpenFileNameA(&ofn))
    {
        selected_path = std::string(filename);
    }

    // 元のカレントディレクトリに戻す
    if (!originalCwd.empty())
    {
        SetCurrentDirectoryA(originalCwd.c_str());
    }

    // キャンセルされた場合やエラー時は空文字列を返す
    return selected_path; 
}

const std::unordered_map<std::string, std::vector<Json>> UtilityLoad::GetJsonMapArrayData(const std::string& fileName)
{
    std::unordered_map<std::string, std::vector<Json>> dataMap;

    try
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + fileName + ".json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
            return dataMap;
        }

        // JSONデータのパース
        Json jsonData;
        ifs >> jsonData;

        // リスト順にデータを格納
        for (auto& [key, value] : jsonData.items())
        {
            // 配列の場合
            if (value.is_array())
            {
                // 配列の場合
                for (auto& obj : value)
                {
                    // 1つずつ格納
                    dataMap[key].push_back(obj);
                }
            }
            // 単一オブジェクトの場合
            else
            {
                // そのまま格納
                dataMap[key].push_back(value);
            }
        }
        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }

    return dataMap;
}

const std::unordered_map<std::string, Json> UtilityLoad::GetJsonMapData(const std::string& fileName)
{
    std::unordered_map<std::string, Json> dataMap;

    try
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + fileName + ".json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
            return dataMap;
        }

        // JSONデータのパース
        Json jsonData;
        ifs >> jsonData;

        // リスト順にデータを格納
        for (auto& [key, value] : jsonData.items())
        {

            // 配列の場合
            if (value.is_array())
            {
                // 配列の場合
                for (auto& obj : value)
                {
                    // 1つずつ格納
                    dataMap[key].push_back(obj);
                }
            }
            // 単一オブジェクトの場合
            else
            {
                // そのまま格納
                dataMap[key].push_back(value);
            }
        }
        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }

    return dataMap;
}

const std::vector<Json> UtilityLoad::GetJsonArrayData(const std::string& fileName)
{
    std::vector<Json> dataArray;
    try
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + fileName + ".json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
            return dataArray;
        }

        // JSONデータのパース
        Json jsonData;
        ifs >> jsonData;

        // リスト順にデータを格納
        for (auto& [key, value] : jsonData.items())
        {
            for (auto& obj : value)
            {
                // 1つずつ格納
                dataArray.push_back(obj);
            }
        }
        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }

	return dataArray;
}
