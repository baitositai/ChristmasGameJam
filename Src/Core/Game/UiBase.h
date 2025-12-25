#pragma once
#include "../../Common/CharacterString.h"
#include "../../Common/Image.h"
#include "../CoreBase.h"

class UiBase : public CoreBase
{
public:

	UiBase();
	~UiBase() override;

	void Init() override;
	void Update() override;
	void Draw() override;

protected:

	Image image_;

};

