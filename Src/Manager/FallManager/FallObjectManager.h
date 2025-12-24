#pragma once
#include<memory>
#include <list>

class FallObjectBase;

class FallObjectManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FallObjectManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FallObjectManager();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

#ifdef _DEBUG
	void DrawDebug();
#endif // _DEBUG

private:

	//道幅
	static constexpr float FAR_WIDTH = 100.0f;
	static constexpr float NEAR_WIDTH = 300.0f;

	/// <summary>
	/// オブジェクトを決める
	/// </summary>
	void DesideObject();

	//落ちてくるオブジェクト
	std::list<std::unique_ptr<FallObjectBase>>fallObjs_;
};

