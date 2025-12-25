#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "UiRight.h"

UiRight::UiRight()
{
}

UiRight::~UiRight()
{
}

void UiRight::Init()
{
	image_.handleId = resMng_.GetHandle("uiRight");
	image_.pos = { Application::SCREEN_SIZE_X - 180, Application::SCREEN_SIZE_Y - 35 };
}
