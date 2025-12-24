#include <Dxlib.h>
#include "ResourcePixelShader.h"

ResourcePixelShader::ResourcePixelShader(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId):
	ResourceBase(type, path, sceneId)
{
}

ResourcePixelShader::~ResourcePixelShader()
{
}

void ResourcePixelShader::Load()
{
	handleId_ = LoadPixelShader(path_.c_str());
}

void ResourcePixelShader::Release()
{
	DeleteShader(handleId_);
}
