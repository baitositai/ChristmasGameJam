#include "../Manager/Common/ScoreManager.h"
#include "../Manager/Common/FontManager.h"
#include "UiBase.h"

UiBase::UiBase() :
	 fontMng_(FontManager::GetInstance()),
	 scoreMng_(ScoreManager::GetInstance())
{
}

UiBase::~UiBase()
{
}

void UiBase::Init()
{
}

void UiBase::Update()
{
}

void UiBase::Draw()
{
	image_.DrawRota();
}
