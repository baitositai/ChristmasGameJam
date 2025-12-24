#pragma once
#include "FallObjectBase.h"
class RightObject :
    public FallObjectBase
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    RightObject(const VECTOR _startPos, const VECTOR _goalPos);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~RightObject()override;

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// スコア加算
    /// </summary>
    /// <param name=""></param>
    void AddScore()override;
private:
};

