#include <DxLib.h>
#include "ResourceSound.h"

ResourceSound::ResourceSound(const RESOURCE_TYPE type, const std::wstring& path, const std::string& soundType, const int sceneId):
	ResourceBase(type,path,sceneId),
	soundType_(GetSoundTypeEnum(soundType))
{
}

ResourceSound::~ResourceSound()
{
}

void ResourceSound::Load()
{
	handleId_ = LoadSoundMem(path_.c_str());
}

void ResourceSound::Release()
{
	if (!duplicateSounds_.empty())
	{
		for (auto snd : duplicateSounds_)
		{
			DeleteSoundMem(snd);
		}
	}

	DeleteSoundMem(handleId_);
}

const int ResourceSound::GetHandle()
{
	int sound = DuplicateSoundMem(handleId_);
	duplicateSounds_.push_back(sound);
	return sound;
}

SoundType::TYPE ResourceSound::GetSoundTypeEnum(const std::string& type)
{
	if (type == KEY_BGM) { return SoundType::TYPE::BGM; }
	else if (type == KEY_SE) { return SoundType::TYPE::SE; }
	else { return SoundType::TYPE::NONE; }
}