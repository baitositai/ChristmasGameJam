#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Manager/Common/ScoreManager.h"
#include "../../Utility/UtilityCommon.h"
#include "UiScore.h"

UiScore::UiScore()
{
}

UiScore::~UiScore()
{
}

void UiScore::Init()
{
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontHanazome"), FONT_SIZE, FONT_THICK);
	text_.color = UtilityCommon::WHITE;
	text_.string = L"SCORE %d/ %d";
	text_.fontHandle = font;
	text_.data1 = scoreMng_.GetScore();
	text_.data2 = ScoreManager::SCORE_MAX;
	text_.pos = { 0, 0 };
}

void UiScore::Draw()
{
	text_.data1 = scoreMng_.GetScore();
	text_.Draw();
}
