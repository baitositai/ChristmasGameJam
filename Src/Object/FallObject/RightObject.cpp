#include "../Utility/UtilityCommon.h"
#include "RightObject.h"

RightObject::RightObject(const VECTOR _startPos, const VECTOR _goalPos):
	FallObjectBase(_startPos,_goalPos)
{
}

RightObject::~RightObject()
{
}

void RightObject::Init()
{
	FallObjectBase::Init();
	type_ = FALL_OBJ_TYPE::RIGHT_OBJ;
}

void RightObject::Draw()
{
	DrawSphere3D(transform_.pos, RADIUS, 10, UtilityCommon::RED, UtilityCommon::RED, true);
}
