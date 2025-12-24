#include <DxLib.h>
#include "ResourceAnimation.h"

ResourceAnimation::ResourceAnimation(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId) : 
	ResourceBase(type, path, sceneId)
{
}

ResourceAnimation::~ResourceAnimation()
{
}

void ResourceAnimation::Load()
{
	handleId_ = MV1LoadModel(path_.c_str());
}

void ResourceAnimation::Release()
{
	MV1DeleteModel(handleId_);
}