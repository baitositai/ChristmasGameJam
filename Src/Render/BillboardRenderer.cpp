#include "ModelMaterial.h"
#include "BillboardRenderer.h"

BillboardRenderer::BillboardRenderer(ModelMaterial& material) :
	modelMaterial_(material)
{
	for (WORD& index : indexes_) { index = 0; }
	for (auto& vertex : vertexs_) { vertex = {}; }
	pos_ = {};
	size_ = {};
}

BillboardRenderer::~BillboardRenderer()
{

}

void BillboardRenderer::MakeSquereVertex(const VECTOR& pos, const Vector2& size)
{

	pos_ = pos;
	size_ = size;

	// 画像のハーフサイズ
	VECTOR hSize = {
		static_cast<float>(size_.x) / 2.0f,
		static_cast<float>(size_.y) / 2.0f
	};

	// 画像の足元原点とする
	int cnt = 0;
	VECTOR leftTop = {
		-hSize.x,
		static_cast<float>(size.y),
		0.0f
	};
	VECTOR rightDown = {
		+hSize.x,
		0.0f,
		0.0f
	};

	// ４頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		//vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
		vertexs_[i].norm = VGet(0.0f, 0.0f, -1.0f);
		vertexs_[i].tan = VGet(1.0f, 0.0f, 0.0f);
		vertexs_[i].binorm = VGet(1.0f, 0.0f, 0.0f);
		vertexs_[cnt].spos = FLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// 左上
	vertexs_[cnt].pos = VGet(leftTop.x, leftTop.y, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右上
	vertexs_[cnt].pos = VGet(rightDown.x, leftTop.y, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 0.0f;
	cnt++;

	// 右下
	vertexs_[cnt].pos = VGet(rightDown.x, rightDown.y, 0.0f);
	vertexs_[cnt].u = 1.0f;
	vertexs_[cnt].v = 1.0f;
	cnt++;

	// 左下
	vertexs_[cnt].pos = VGet(leftTop.x, rightDown.y, 0.0f);
	vertexs_[cnt].u = 0.0f;
	vertexs_[cnt].v = 1.0f;

	/*
	　～～～～～～
		0-----1
		|     |
		|     |
		3-----2
	　～～～～～～
		0-----1
		|  ／
		|／
		3
	　～～～～～～
			  1
		   ／ |
		 ／   |
		3-----2
	　～～～～～～
	*/


	// ビルボード行列
	auto mat = GetCameraBillboardMatrix();

	// 回転成分を抽出
	auto rot = MGetRotElem(mat);

	const int NUM_VERTEX = 4;
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		vertexs_[i].pos = VAdd(pos_, VTransform(vertexs_[i].pos, rot));
	}

	// 頂点インデックス
	cnt = 0;
	indexes_[cnt++] = 0;
	indexes_[cnt++] = 1;
	indexes_[cnt++] = 3;

	indexes_[cnt++] = 1;
	indexes_[cnt++] = 2;
	indexes_[cnt++] = 3;

}

void BillboardRenderer::MakeSquereVertex()
{
	MakeSquereVertex(pos_, size_);
}

void BillboardRenderer::Draw()
{
	Draw(pos_, size_);
}

void BillboardRenderer::Draw(const VECTOR& pos, const Vector2& size)
{

	// オリジナルシェーダ設定(ON)
	MV1SetUseOrigShader(true);

	// 座標とサイズの更新
	pos_ = pos;
	size_ = size;

	// ポリゴンの再作成
	// 座標、サイズ、カメラが変わっていなければ更新は不要
	MakeSquereVertex(pos, size);

	SetReserveVS();
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	// 描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawPolygonIndexed3DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// テクスチャアドレスタイプを元に戻す
	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// テクスチャ解除
	const auto& textures = modelMaterial_.GetTextures();
	size_t tSize = textures.size();
	if (tSize == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, -1);
		}
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	// オリジナルシェーダ設定(OFF)
	MV1SetUseOrigShader(false);

}

void BillboardRenderer::SetReserveVS()
{

	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBufVS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsVS();

	size_t size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 頂点シェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 頂点シェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// 頂点シェーダー設定
	SetUseVertexShader(modelMaterial_.GetShaderVS());

}

void BillboardRenderer::SetReservePS()
{

	// ピクセルシェーダーにテクスチャを転送
	const auto& textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		// 前回使用分のテクスチャを引き継がないように
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto& pair : textures)
		{
			SetUseTextureToShader(pair.first, pair.second);
		}
	}

	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsPS();

	size = constBufs.size();
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// ピクセルシェーダー用の定数バッファを定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// ピクセルシェーダー設定
	SetUsePixelShader(modelMaterial_.GetShaderPS());

}
