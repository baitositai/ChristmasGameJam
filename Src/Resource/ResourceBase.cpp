#include "ResourceBase.h"

ResourceBase::ResourceBase(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId) :
	type_(type),
	path_(path),
	sceneId_(sceneId)
{
	handleId_ = -1;
}
