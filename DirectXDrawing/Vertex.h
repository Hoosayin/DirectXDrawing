#pragma once

#include "xnamath.h"

struct VERTEX
{
	VERTEX() {}
	VERTEX(float x, float y, float z, float red, float green, float blue, float alpha) :
		Position(x, y, z),
		Color(red, green, blue, alpha) {}

	XMFLOAT3 Position;
	XMFLOAT4 Color;
};
