#include <DxLib.h>
#include "Image.h"

Image::Image() :
	handleId(-1),
	pos(Vector2(0, 0)),
	size(Vector2(0, 0)),
	center(Vector2(0, 0)),
	angle(0.0f),
	scale(1.0f),
	isTrans(true),
	isFlipX(false),
	isFlipY(false)
{
}

Image::~Image()
{
}

const Vector2 Image::GetCenterPos() const
{
	// íÜêSç¿ïWÇåvéZÇµÇƒï‘Ç∑
	return Vector2(
		pos.x + size.x / 2,
		pos.y + size.y / 2
	);
}

const void Image::Draw() const
{
	DrawGraph(
		pos.x,
		pos.y,
		handleId,
		isTrans
	);
}

const void Image::DrawRota() const
{
	DrawRotaGraph(
		pos.x,
		pos.y,
		scale,
		angle,
		handleId,
		isTrans,
		isFlipX,
		isFlipY
	);
}

const void Image::DrawExtend() const
{
	DrawExtendGraph(
		pos.x,
		pos.y,
		size.x + pos.x,
		size.y + pos.y,
		handleId,
		isTrans
	);
}
