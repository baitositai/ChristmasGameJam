#pragma once
#include "../../Common/CharacterString.h"
#include "../../Common/Image.h"
#include "../CoreBase.h"

class ScoreManager;
class FontManager;

class UiBase : public CoreBase
{
public:

	UiBase();
	~UiBase() override;

	void Init() override;
	void Update() override;
	void Draw() override;

protected:

	static constexpr int FONT_SIZE = 38;
	static constexpr int FONT_THICK = 3;

	ScoreManager& scoreMng_;

	FontManager& fontMng_;

	Image image_;

	CharacterString text_;
};

