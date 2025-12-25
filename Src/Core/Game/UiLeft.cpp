#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "UiLeft.h"

UiLeft::UiLeft()
{
}

UiLeft::~UiLeft()
{
}

void UiLeft::Init()
{
	image_.handleId = resMng_.GetHandle("uiLeft");
	image_.pos = { 180, Application::SCREEN_SIZE_Y - 35 };
}