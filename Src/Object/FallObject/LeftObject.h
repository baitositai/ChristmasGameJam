#pragma once
#include "FallObjectBase.h"

class LeftObject :
    public FallObjectBase
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    LeftObject(VECTOR _startPos, VECTOR _goalPos);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~LeftObject();

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();
private:
};

