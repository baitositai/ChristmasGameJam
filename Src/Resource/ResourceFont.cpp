#include <windows.h>
#include "ResourceFont.h"

ResourceFont::ResourceFont(const RESOURCE_TYPE type, const std::wstring& path, const int sceneId, const std::wstring& fontName) :
	ResourceBase(type, path, sceneId),
	fontName_(fontName)
{
}

ResourceFont::~ResourceFont()
{
}

void ResourceFont::Load()
{
	if (AddFontResourceEx(
		path_.c_str(),
		FR_PRIVATE,
		NULL)
		== -1)
	{
		return;
	}
}

void ResourceFont::Release()
{
	if(RemoveFontResourceEx(
		path_.c_str(),
		FR_PRIVATE,
		NULL)
		== -1)
		{
			return;
		}
}