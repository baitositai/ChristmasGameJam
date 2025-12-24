#pragma once
#include <memory>
#include <DxLib.h>
#include "../Common/Vector2.h"

class ModelMaterial;

class BillboardRenderer
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="modelMaterial">マテリアル</param>
	BillboardRenderer(ModelMaterial& modelMaterial);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BillboardRenderer();

	/// <summary>
	/// 描画矩形の生成
	/// </summary>
	/// <param name="pos">生成位置</param>
	/// <param name="size">画像サイズ</param>
	void MakeSquereVertex(const VECTOR& pos, const Vector2& size);
	void MakeSquereVertex();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="pos">生成位置</param>
	/// <param name="size">画像サイズ</param>
	void Draw(const VECTOR& pos, const Vector2& size);
	void Draw();

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const VECTOR& pos) { pos_ = pos; }

	/// <summary>
	/// 画像サイズ設定
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size) { size_ = size; }

	

private:

	// 頂点シェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// 座標
	VECTOR pos_;

	// 描画サイズ
	Vector2 size_;

	// 頂点
	VERTEX3DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// マテリアル
	ModelMaterial& modelMaterial_;

	// シェーダ設定(頂点)
	void SetReserveVS();

	// シェーダ設定(ピクセル)
	void SetReservePS();
};

