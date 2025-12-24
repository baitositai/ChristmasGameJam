#include <DxLib.h>
#include "ResourceModel.h"


ResourceModel::ResourceModel(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId) :
	ResourceBase(type, path, sceneId)
{
	duplicateModels_.clear();
}

ResourceModel::~ResourceModel()
{
}

void ResourceModel::Load()
{
	handleId_ = MV1LoadModel(path_.c_str());
}

void ResourceModel::Release()
{
	if (!duplicateModels_.empty())
	{
		for (auto model : duplicateModels_)
		{
			MV1DeleteModel(model);
		}
	}

	MV1DeleteModel(handleId_);
}

const int ResourceModel::GetHandle()
{
	int handle = MV1DuplicateModel(handleId_);
	duplicateModels_.push_back(handle);
	return handle;
}
