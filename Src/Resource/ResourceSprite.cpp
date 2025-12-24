#include <DxLib.h>
#include "ResourceSprite.h"

ResourceSprite::ResourceSprite(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId, const int divX, const int divY, const int sizeX, const int sizeY):
	ResourceBase(type, path, sceneId),
	divX_(divX),
	divY_(divY),
	sizeX_(sizeX),
	sizeY_(sizeY)
{
	handleIds_ = nullptr;
}

ResourceSprite::~ResourceSprite()
{
}

void ResourceSprite::Load()
{
	// •ªŠ„”‚É‰‚¶‚Äƒƒ‚ƒŠŠm•Û
	handleIds_ = new int[divX_ * divY_];

	LoadDivGraph(
		path_.c_str(),
		divX_ * divY_,
		divX_,
		divY_,
		sizeX_,
		sizeY_,
		&handleIds_[0]);
}

void ResourceSprite::Release()
{
	int num = divX_ * divY_;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_[i]);
	}
	delete[] handleIds_;
}
