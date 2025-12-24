#pragma once
#include <string>
#include "Vector2.h"

struct CharacterString
{	
	/// <summary>
	/// 文字列
	/// </summary>
	std::wstring string; 

	/// <summary>
	/// 座標
	/// </summary>
	Vector2 pos;

	/// <summary>
	/// フォントハンドル
	/// </summary>
	int fontHandle; 

	/// <summary>
	/// 色
	/// </summary>
	int color;

	/// <summary>
	/// 描画用データ1
	/// </summary>
	int data1;

	/// <summary>
	/// 描画用データ2
	/// </summary>
	int data2;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterString();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CharacterString();

	/// <summary>
	/// 通常描画
	/// </summary>
	void Draw() const;

	/// <summary>
	/// 文字列の中心で描画
	/// </summary>
	void DrawCenter() const;

	/// <summary>
	/// 数値を含む文字列の中心描画
	/// </summary>
	void DrawFormatCenter() const;

private:

	static constexpr int BUFFER_SIZE = 256;
	
};