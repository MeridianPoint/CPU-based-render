
#ifndef DIRECTION_LIGHT_H_
#define DIRECTION_LIGHT_H_

#include "stdafx.h"
#include "Light.h"
class Direction_Light :
	public Light
{
public:
	Direction_Light();
	float getIntensity(Vec3f lightVec);
	//float getIntensity();
	//float getIntensity(float Intensity)=0;
	~Direction_Light();

};

#endif