#include "../../Application.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Common/ScoreManager.h"
#include "../../Utility/UtilityCommon.h"
#include "UiLife.h"

UiLife::UiLife()
{
}

UiLife::~UiLife()
{
}

void UiLife::Init()
{
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontHanazome"), FONT_SIZE, FONT_THICK);
	text_.color = UtilityCommon::WHITE;
	text_.string = L"REST:%d";
	text_.fontHandle = font;
	text_.data1 = scoreMng_.GetLife();
	text_.pos = { Application::SCREEN_SIZE_X  - 200 , 0 };
}

void UiLife::Draw()
{
	text_.data1 = scoreMng_.GetLife();
	text_.Draw();
}
