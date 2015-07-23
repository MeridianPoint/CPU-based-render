#include "stdafx.h"
#include "Direction_Light.h"


Direction_Light::Direction_Light()
{
}

float Direction_Light::getIntensity(Vec3f lightVec){
	Vec3f norm_dir = direct;
	norm_dir.normalize();
	Vec3f norm_light= lightVec;
	norm_light.normalize();
	return intensity*norm_dir.dot(norm_light);
}

//float Direction_Light::getIntensity(){
//	//return direct*intensity;
//}

Direction_Light::~Direction_Light()
{
}
