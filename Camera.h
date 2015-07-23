/*Camera.h
*Realize the camera object,passing parameters for transformation
*
*@author Qiushuo Wang
*/
#include "Vec3.h"
#pragma once
class Camera
{
private:
	//position of the camera
	Vec3f Camera_Pos;  //position of the camera(world coordinate)
	Vec3f Camera_Target;  //the target of the camera
	int pro_type;  //type of camera object
	int v_Type;   //type of view port
	
private:
	//type indicator
	enum projection_type { REGULAR, OBLIQUE };  //types of projection
	enum view_type{ SQUARE, RECTANGLE };  //view port types

	//parameters
	Vec3f Camera_dir; //camera viewing direction
	Vec3f Up_vec;  //up vector
	Vec3f u;   //view right 
	Vec3f v;   //view up
	float near_clip;
	float far_clip;  //clipping plane(position to the camera)
	float h; //clipping plane size(width&height)
	float w; //width
	//oblique camera parameters
	float r, l; //left and right bound of the near clipping plane
	float t, b; //higher and lower bound of near clipping plane
public:
	//constructors--
	//default constructor
	Camera() :Camera_Pos(Vec3f(0.0f, 0.0f, 0.0f)), Camera_Target(Vec3f(0.0f, 0.0f, 0.0f)), pro_type(REGULAR), v_Type(SQUARE){}
	Camera(int proj_type, int view_type) :Camera_Pos(Vec3f(0.0f, 0.0f, 0.0f)), Camera_Target(Vec3f(0.0f, 0.0f, 0.0f)), pro_type(proj_type), v_Type(view_type){}
	Camera(Vec3f& v) :Camera_Pos(v), Camera_Target(Vec3f(0.0f, 0.0f, 0.0f)){}
	Camera(float x, float y, float z) :Camera_Pos(Vec3f(x, y, z)), Camera_Target(Vec3f(0.0f, 0.0f, 0.0f)){}
	//set basic parameters
	//set position
	void setPostion(Vec3f& v){
		Camera_Pos = v;
	}
	//////set target//////////
	void LookAt(Vec3f& v){
		Camera_Target = v;
	}
	void LookAt(float x, float y, float z){
		Camera_Target = Vec3f(x, y, z);
	}
	//set Up Vector
	void SetCamera_Up(Vec3f& v){
		Up_vec = v;
		computeCameraView();
	}
	//set view port
	void setViewSize(float height, float width){
		h = height;
		w = width;
	}

	//set viewport height
	void setViewheight(float height){
		h = height;
	}


	//set oblique viewport
	void setObilqueTrustum(float leftbound, float rightbound,float topbound, float bottombound){
		l = leftbound;
		r = rightbound;
		t = topbound;
		b = bottombound;
	}

	//calculation
	void computeCameraView(){
		//calculate camera direction N
		Camera_dir = Camera_Target - Camera_Pos;
		Camera_dir.normalize();
		//calculate camera right direction U
		u = Camera_dir.cross(Up_vec);
		u.normalize();
		//calculate camera up direction v
		v = u.cross(Camera_dir);
	}
	//set clipping plane
	void SetClipping(float near1, float far2){

		near_clip = near1;  //set 
		far_clip = far2;   //set

	}


	///////access members/////////////////
	Vec3f getPosition()const{
		return Camera_Pos;
	}
	float positionx() const{
		return Camera_Pos.x();
	}
	float positiony() const{
		return Camera_Pos.y();
	}
	float positionz() const{
		return Camera_Pos.z();
	}
	float directionx() const{
		return Camera_dir.x();
	}
	float directiony() const{
		return Camera_dir.y();
	}
	float directionz() const{
		return Camera_dir.z();
	}
	float Ux() const{
		return u.x();
	}
	float Uy() const{
		return u.y();
	}
	float Uz() const{
		return u.z();
	}
	float Vx() const{
		return v.x();
	}
	float Vy() const{
		return v.y();
	}
	float Vz() const{
		return v.z();
	}
	float d() const{
		return near_clip;
	}
	float f() const{
		return far_clip;
	}
	float H() const{
		return h;
	}
	float W() const{
		return w;
	}
	//oblique memer
	float R() const{
		return r;
	}

	float L() const{
		return l;
	}

	float T() const{
		return t;
	}

	float B() const{
		return b;
	}

	~Camera();
};

