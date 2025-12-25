#pragma once
#include "UiBase.h"

class UiDir : public UiBase
{
public:

    UiDir();
    ~UiDir() override;

    void Init() override;
    void Update() override;

private:

    //—h‚ç‚·ü”g”
    static constexpr float AMPLITUDE = 3.0f;

    //—h‚ç‚·U•
    static constexpr float FREQUENCY = 5.0f;
};

