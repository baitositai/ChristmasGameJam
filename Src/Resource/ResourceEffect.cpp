#include <EffekseerForDXLib.h>
#include <Effekseer.h>
#include "ResourceEffect.h"

ResourceEffect::ResourceEffect(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId)	:
	ResourceBase(type, path, sceneId)
{
}

ResourceEffect::~ResourceEffect()
{
}

void ResourceEffect::Load()
{
	handleId_ = LoadEffekseerEffect(path_.c_str());
}

void ResourceEffect::Release()
{
	DeleteEffekseerEffect(handleId_);
}
