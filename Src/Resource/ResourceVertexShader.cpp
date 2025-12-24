#include <DxLib.h>
#include "ResourceVertexShader.h"

ResourceVertexShader::ResourceVertexShader(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId):
	ResourceBase(type, path, sceneId)
{
}

ResourceVertexShader::~ResourceVertexShader()
{
}

void ResourceVertexShader::Load()
{
	handleId_ = LoadVertexShader(path_.c_str());
}

void ResourceVertexShader::Release()
{
	DeleteShader(handleId_);
}
