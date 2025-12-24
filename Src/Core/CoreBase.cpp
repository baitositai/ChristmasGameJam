#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "CoreBase.h"

CoreBase::CoreBase() :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	fontMng_(FontManager::GetInstance())
{
}

CoreBase::~CoreBase()
{
}

void CoreBase::Load()
{
}

void CoreBase::Init()
{
}

void CoreBase::Update()
{
#ifdef _DEBUG
	UpdateDebug();
#endif
}

void CoreBase::Draw()
{
#ifdef _DEBUG
	DrawDebug();
#endif
}

void CoreBase::UpdateDebug()
{
}

void CoreBase::DrawDebug()
{
}
