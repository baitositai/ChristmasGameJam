#pragma once
#include <string>
#include <map>

class EffectType
{
public:

	// エフェクトの種類の登録
	enum class TYPE
	{
		PETAL_FALL,
		MAX
	};

	// 種類を管理
	static const std::map<std::string, TYPE> KEY_MAP;
};

