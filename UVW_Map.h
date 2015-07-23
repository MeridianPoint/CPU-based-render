/*!
 * \class UVW_Map
 *
 * \brief 
 * Implement UVWMaping
 *
 * \author Qiushuo
 * \date April 2015
 */

#ifndef UVW_Map_h__
#define UVW_Map_h__
#include "stdafx.h"

enum Warp_Type{None,Plane,Cylinder,Box,Spherical};  //define type

class UVW_Map
{
private:
	///////// global parameters /////////////////////////////////////////////////////////////////
	std::vector<std::vector<Vec3f>> Map;  //texture map
	std::vector<std::vector<Vec3f>> SAT; //Summed SummedAreaTable
	unsigned int map_height, map_width;  //map size
	int uv_type; //uv mapping type
	Mat4f ProjectionMatrix;
	Vec2i ScreenScale;


	Mat4f texture_coordinate; //coordinate space
	//for planar
	Mat4f Plane_Space;
	Vec3f Plane_direction;
	Vec3f Plane_Bidirection;
	Vec3f Plane_Position;
	float plane_width, plane_height;
	//for cylinder
	Vec3f Cylinder_Position;
	Vec3f Cylinder_direction;
	Vec3f Cylinder_Bidirection;
	float c_radius;
	float c_height;
	//box

	//Spherical
	Vec3f Sphere_Position;
	float S_radius;

private:  //private functions
	Vec2i getPointColorAtPosition(Vec3f WorldPos);
	Vec3f HomeGenousTransformation(Vec3f Postion, Mat4f Transformation_Matrix);
	Vec2i CastTo2D(Vec3f relative_position);  //for planar mapping
	//coordinate transformation
	Vec3f Cartesian2Cylinder(Vec3f relative_pos);
	Vec3f Cartesian2Sphereic(Vec3f relative_pos);

	Vec3f uchar2Vec3(unsigned char* colorinfo);
public:
	UVW_Map();
	void LoadMap(void * texture_map);
	void LoadMap(image_t * pImage);
	void createSummedMap();
	void GeneratePlanarMapping(float width, float height, Vec3f Pos, Vec3f direc1, Vec3f direc2);
	void GenerateCylinderMapping(float radius, float height, Vec3f Pos, Vec3f direc1, Vec3f direc2);
	void GenerateSphereMapping(float radius, Vec3f Pos, Vec3f direc1, Vec3f direc2);
	Vec3f GetColor(Vec3f WorldPosMin,Vec3f WorldPosMax,Vec2i screen_pos);
	Vec3f getColor(Vec3f worldPos);  //get color by world position
	~UVW_Map();
};
#endif // UVW_Map_h__

