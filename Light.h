
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include "stdafx.h"

//type of light 
enum light_type {POINT_LIGHT, PARALELL_LIGHT,CRONE_LIGHT};

enum falloff_type {NONE,INVERSE,SQUARE_INVERSE};

class Light
{
protected:
	//position
	Vec3f Light_Pos;
	//color
	Vec3f color;
	//intensity
	float intensity;

	//direction --not for point light
	Vec3f direct;
	//target
	Vec3f light_target;

	//parameters
	int falloff_type;   //use enum NONE etc
public:
	Light();
	Light(Vec3f pos) :Light_Pos(pos),intensity(1.0f),color(Vec3f(1.0f,1.0f,1.0f)),falloff_type(NONE){}
	Light(Vec3f pos, float Inten) :Light_Pos(pos), intensity(Inten), color(Vec3f(1.0f, 1.0f, 1.0f)), falloff_type(NONE){}
	//set parameters
	void setDirection(Vec3f direction){ direct = direction; }
	void setPostion(Vec3f position){ Light_Pos = position; }
	void setIntensity(float I){ intensity = I; }
	//void setTarget(Vec3f target){}
	void setColor(Vec3f Color){ color = Color; }
	void setFalloutType(int type){ falloff_type = type; }  //set fallout type :NONE,LINEAR, INVERSE_SQUARE

	//get parameters
	Vec3f getColor() { return color; }
	Vec3f getPosition(){ return Light_Pos; }
	virtual float getIntensity(){ return intensity; }
	//virtual float getIntensity(float distance);
	int getFallOffType(){ return falloff_type; };

	~Light();
};

#endif