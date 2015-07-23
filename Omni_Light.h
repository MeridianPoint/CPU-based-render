#pragma once
#include "Light.h"
class Omni_Light :
	public Light
{
public:
	Omni_Light();
	//Omni_Light(Vec3f pos) :_super.Light_Pos(pos), intensity(1.0f), color(Vec3f(1.0f, 1.0f, 1.0f)), falloff_type(NONE){}
	~Omni_Light();
	float getIntensity(){
		return intensity;
	}
	//float getIntensity(float distance);
};

