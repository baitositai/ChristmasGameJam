#pragma once
#include "FallObjectBase.h"
class RightObject :
    public FallObjectBase
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    RightObject();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~RightObject();

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

