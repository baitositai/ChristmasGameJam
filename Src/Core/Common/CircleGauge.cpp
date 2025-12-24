#include <DxLib.h>
#include "CircleGauge.h"

CircleGauge::CircleGauge()
{
	//‰Šú‰»
	pos = { 0,0 };
	percent = 0.0f;
	handle = -1;
	startPercent = 0.0f;
	scale = 1.0f;
	reverseX = false;
	reverseY = false;
}

CircleGauge::~CircleGauge()
{
}

void CircleGauge::Draw()
{
	DrawCircleGauge(
		pos.x,
		pos.y,
		percent,
		handle,
		startPercent,
		scale,
		reverseX,
		reverseY
	);
}