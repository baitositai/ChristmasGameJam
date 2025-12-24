#pragma once
#include<memory>
#include <list>
#include "../../Template/Singleton.h"
class FallObjectBase;

class FallObjectManager : public Singleton<FallObjectManager>
{

	// シングルトンにだけ共有する
	friend class Singleton<FallObjectManager>;
public:

	enum class LANE_TYPE
	{
		RIGHT_LANE,
		CENTER_LANE,
		LEFT_LANE,
		MAX
	};

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
	//出現カウント
	static constexpr float SPAWN_DIS = 5.0f;

	//道幅
	static constexpr float FAR_WIDTH = 100.0f;
	static constexpr float NEAR_WIDTH = 300.0f;

	//オブジェクトが流れるレーン数
	static constexpr int LANE_NUM = 3;

	//スタート座標
	static constexpr VECTOR START_POS= { 0.0f ,300.0f,300.0f };

	/// <summary>
	/// オブジェクトを決める
	/// </summary>
	void DesideObject();

	//流れるレーンを決める
	void DesideLane();

	//落ちてくるオブジェクト
	std::list<std::unique_ptr<FallObjectBase>>fallObjs_;

	//出現カウント
	float spawnCnt_;

	//出現間隔
	float spawnDisCnt_;
};

