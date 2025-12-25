#pragma once
#include <string>
#include <map>

class SoundType
{
public:

	// サウンドの種類
	enum class TYPE
	{
		NONE,
		BGM,
		SE,
	};

	// BGMの種類
	enum class BGM
	{
		TITLE,
		RESULT,
		MAX
	};

	// SEの種類
	enum class SE
	{
		GAME_START,
		SELECT,
		HIT_OBJECT,		//キーブレードとオブジェクトのヒットSE
		HIT_SUCCESS,	//仕分け成功
		HIT_MISS,		//仕分け失敗
		STAN,			// スタン
		ATTACK,			// 攻撃
		CATCH,			// キャッチ
		BLADE_ROTA,		// 武器の回転	
		MAX
	};

	// BGMの種類を管理
	static const std::map<std::string, BGM> KEY_BGM_MAP;

	// SEの種類を管理
	static const std::map<std::string, SE> KEY_SE_MAP;
};

