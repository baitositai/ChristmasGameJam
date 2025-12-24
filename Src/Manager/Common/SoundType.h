#pragma once
#include <string>
#include <map>

class SoundType
{
public:

	// ƒTƒEƒ“ƒh‚Ìí—Ş
	enum class TYPE
	{
		NONE,
		BGM,
		SE,
	};

	// BGM‚Ìí—Ş
	enum class BGM
	{
		TITLE,
		RESULT,
		MAX
	};

	// SE‚Ìí—Ş
	enum class SE
	{
		GAME_START,
		SELECT,
		MAX
	};

	// BGM‚Ìí—Ş‚ğŠÇ—
	static const std::map<std::string, BGM> KEY_BGM_MAP;

	// SE‚Ìí—Ş‚ğŠÇ—
	static const std::map<std::string, SE> KEY_SE_MAP;
};

