#include "CharacterString.h"
#include <DxLib.h>

CharacterString::CharacterString(void):
	fontHandle(-1),
	string(L""),
	pos({ 0, 0 }),
	color(0),
	data1(0),
	data2(0)
{
}

CharacterString::~CharacterString(void)
{
}

void CharacterString::Draw(void) const
{
	// 文字列を描画
	DrawFormatStringToHandle(pos.x, pos.y, color, fontHandle, string.c_str(), data1, data2);
}

void CharacterString::DrawCenter(void) const
{
	// フォーマット済みの文字列を作成
	wchar_t buffer[BUFFER_SIZE];

	swprintf(buffer, BUFFER_SIZE, string.c_str());

	// フォーマット済み文字列の幅を取得
	int strWidth = GetDrawStringWidthToHandle(buffer, wcslen(buffer), fontHandle);

	// 幅の直後に次の文字列を描画
	DrawStringToHandle(pos.x - strWidth / 2, pos.y, string.c_str(), color, fontHandle);
}

void CharacterString::DrawFormatCenter(void) const
{
	// フォーマット済み文字列用バッファ
	wchar_t buffer[BUFFER_SIZE];

	swprintf_s(buffer, BUFFER_SIZE, string.c_str(), data1, data2);

	// フォーマット済み文字列の幅を取得
	int strWidth = GetDrawStringWidthToHandle(buffer, wcslen(buffer), fontHandle);

	// 中央に描画
	DrawStringToHandle(pos.x - strWidth / 2, pos.y, buffer, color, fontHandle);
}