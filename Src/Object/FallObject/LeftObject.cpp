#include "../Utility/UtilityCommon.h"
#include "LeftObject.h"

LeftObject::LeftObject(const VECTOR _startPos, const VECTOR _goalPos):
	FallObjectBase(_startPos, _goalPos)
{
}

LeftObject::~LeftObject()
{
}

void LeftObject::Init()
{
	FallObjectBase::Init();
	type_ = FALL_OBJ_TYPE::RIGHT_OBJ;
}

void LeftObject::Draw()
{
	DrawSphere3D(transform_.pos, RADIUS, 10, UtilityCommon::BLUE, UtilityCommon::BLUE, true);
}
