

#ifndef Basic_Wave_h__
#define Basic_Wave_h__

#include "stdafx.h"
class Basic_Wave
{
private:
	float height, width, depth;  //dimension
	Vec3f color1;
	Vec3f color2;
public:
	Basic_Wave();
	Vec3f clamp(Vec3f color);
	Vec3f smoothClamp(Vec3f color, float limit, float limit2);
	Vec3f getColor(Vec3f position);
	~Basic_Wave();
};

#endif // Basic_Wave_h__

