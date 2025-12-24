#include "SoundType.h"

// •¶Žš—ñ‚©‚çenum ‚Ì‘Î‰ž•\
const std::map<std::string, SoundType::BGM> SoundType::KEY_BGM_MAP =
{
	{"bgmTitle",SoundType::BGM::TITLE},
	{"bgmResult", SoundType::BGM::RESULT}
};

const std::map<std::string, SoundType::SE> SoundType::KEY_SE_MAP =
{
    {"seGameStart", SoundType::SE::GAME_START},
	{"seSelect", SoundType::SE::SELECT}
};