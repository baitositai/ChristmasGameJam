#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Utility/UtilityCommon.h"
#include "UiDir.h"

UiDir::UiDir()
{
}

UiDir::~UiDir()
{
}

void UiDir::Init()
{
	image_.handleId = resMng_.GetHandle("uiDir");
	image_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 250 };
}

void UiDir::Update()
{
	//è„â∫óhÇÁÇµÇÃèàóù
	image_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y + 250 };
	image_.pos = UtilityCommon::SinShake(image_.pos, AMPLITUDE, FREQUENCY);
}
