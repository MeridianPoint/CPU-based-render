#include "stdafx.h"
#include "UVW_Map.h"


UVW_Map::UVW_Map()
{
}


UVW_Map::~UVW_Map()
{
}

Vec2i UVW_Map::CastTo2D(Vec3f relative_position){
	return Vec2i(relative_position.x()*map_width, relative_position.y()*map_height);
}

Vec3f UVW_Map::uchar2Vec3(unsigned char* colorinfo){
	return Vec3f((float)colorinfo[0] / 256, (float)colorinfo[1] / 256, (float)colorinfo[2] / 256);
}



Vec3f UVW_Map::HomeGenousTransformation(Vec3f Postion, Mat4f Transformation_Matrix){
	Vec4f Homo_vector(Postion.x(), Postion.y(), Postion.z(), 1);
	Homo_vector = Transformation_Matrix*Homo_vector;
	Vec3f result_vector = Vec3f(Homo_vector.x(), Homo_vector.y(), Homo_vector.z());
	return result_vector;
}

Vec3f UVW_Map::Cartesian2Cylinder(Vec3f relative_pos){
	return Vec3f(sqrt(relative_pos.x()*relative_pos.x() + relative_pos.y()*relative_pos.y()), atan2f(relative_pos.y(), relative_pos.x()), relative_pos.z());
}

Vec3f UVW_Map::Cartesian2Sphereic(Vec3f relative_pos){
	return Vec3f(relative_pos.length(),atan2f(relative_pos.y(),relative_pos.x()),acos(relative_pos.z()/relative_pos.length()));
}

void UVW_Map::LoadMap(void * texture_map){
}

void UVW_Map::LoadMap(image_t * pImage){
	map_height = pImage->info.height;
	map_width = pImage->info.width;
	
	Map = std::vector<std::vector<Vec3f>>(map_height, std::vector<Vec3f>(map_width, Vec3f(0.0, 0.0, 0.0)));
	for (unsigned int i = 0; i < map_height; i++)
	{
		for (unsigned int j = 0; j < map_width; j++)
		{
			Map[i][j] = Vec3f((float)pImage->data[3 * i*map_width + 3*j]/256, (float)pImage->data[3 * i*map_width + 3*j+1]/256, (float)pImage->data[3 * i*map_width + 3*j+2]/256);
		}
	}
	
}

void UVW_Map::createSummedMap(){
	//using method in Fast Summed-Area Table Generation and its Applications
	//tA = InputImage
	//	n = log2(width)
	//	m = log2(height)
	//	// horizontal phase
	//	f or(i = 0; i < n; i = i + 1)
	//	tB[x, y] = tA[x, y] + tA[x + 2i
	//	, y]
	//	swap(tA, tB)
	//	// vertical phase
	//	f or(i = 0; i < m; i = i + 1)
	//	tB[x, y] = tA[x, y] + tA[x, y + 2i
	//	]
	//	swap(tA, tB)
	//	// Texture tA holds the result
	int m = log2(Map.size());
	int n = log2(Map[0].size());
	for (int i = 0; i < Map.size(); i++)
	{
		for (int j = 0; j < Map[0].size(); j++)
		{
			if (i==0)
			{
				SAT[i][j] = Map[i][j];
			}
			else{

			}
		}
	}
}

Vec2i UVW_Map::getPointColorAtPosition(Vec3f WorldPos){
	Vec2i tex_position;
	if (uv_type == Plane){//plane mapping
		Vec3f uvw_pos = HomeGenousTransformation(WorldPos, ProjectionMatrix);
		tex_position = CastTo2D(uvw_pos);
	}
	else if (uv_type==Cylinder)
	{
		
		Vec3f new_Position = HomeGenousTransformation(WorldPos,ProjectionMatrix);
		
		Vec3f Cylinder_Coord = Cartesian2Cylinder(new_Position);

		tex_position = Vec2i(floor(Cylinder_Coord.ctheta() *map_width / (2 * M_PI)), (int)(Cylinder_Coord.h() *map_height / c_height) );
	}
	else if (uv_type==Spherical)
	{
		Vec3f new_Position = HomeGenousTransformation(WorldPos, ProjectionMatrix);

		Vec3f Sphereical_Coord = Cartesian2Sphereic(new_Position);

		tex_position = Vec2i(floor(Sphereical_Coord.phi() *map_width / (2 * M_PI)), floor(Sphereical_Coord.stheta() *map_height / M_PI));
	}
	return tex_position;
}
void UVW_Map::GeneratePlanarMapping(float width, float height, Vec3f Pos, Vec3f direc1,Vec3f direc2){
	uv_type = Plane; //align type;
	plane_width = width;
	plane_height = height;
	Plane_Position = Pos;
	Plane_direction = direc1;
	Plane_Bidirection = direc2;
	//normalize
	direc1.normalize();
	direc2.normalize();
	//////////////////////////////////////////////////////////////////////////
	Vec3f direc3 = direc1.cross(direc2);
	Mat4f Space_Mat = Mat4f(
		direc2.x(), direc2.y(), direc2.z(), 0.0f,
		direc1.x(), direc1.y(), direc1.z(), 0.0f,
		direc3.x(), direc3.y(), direc3.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	/*Mat4f Space_Mat = Mat4f(
		direc2.x(), direc2.y(), direc2.z(),0.0f,
		direc3.x(), direc3.y(), direc3.z(), 0.0f,
		-direc1.x(), -direc1.y(), -direc1.z(), 0.0f,
		0.0f,0.0f,0.0f,1.0f
		);*/
	/*float ratioX = width / plane_width;
	float ratioY = height / plane_height;
	Mat4f Ratio_Mat = Mat4f(
		ratioX, 0.0f, 0.0f, 0.0f,
		0.0f, ratioY, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);*/
	Mat4f Translate_Mat = Mat4f(
		1.0f,0.0f,0.0f,-Plane_Position.x(),
		 0.0f,1.0f, 0.0f, -Plane_Position.y(),
		 0.0f, 0.0f, 1.0f,-Plane_Position.z(),
		0.0f,0.0f,0.0f,1.0f
		);
	ProjectionMatrix = Space_Mat*Translate_Mat;
	
}
void UVW_Map::GenerateCylinderMapping(float radius, float height, Vec3f Pos, Vec3f direc1, Vec3f direc2){
	uv_type = Cylinder;
	c_radius = radius;
	c_height = height;
	Cylinder_direction = direc1;
	Cylinder_Bidirection = direc2;
	Cylinder_Position = Pos;
	//////////////////////////////////////////////////////////////////////////
	Mat4f Translate_Mat = Mat4f(
		1.0f, 0.0f, 0.0f, -Cylinder_Position.x(),
		0.0f, 1.0f, 0.0f, -Cylinder_Position.y(),
		0.0f, 0.0f, 1.0f, -Cylinder_Position.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	direc1.normalize();
	direc2.normalize();
	Vec3f direc3 = direc1.cross(direc2);
	Mat4f Space_Mat = Mat4f(
		direc2.x(), direc2.y(), direc2.z(), 0.0f,
		direc1.x(), direc1.y(), direc1.z(), 0.0f,
		direc3.x(), direc3.y(), direc3.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	ProjectionMatrix = Space_Mat*Translate_Mat;
	//Mat4f Space_Mat = Mat4f(

	//	);
	
}
void UVW_Map::GenerateSphereMapping(float radius, Vec3f Pos, Vec3f direc1, Vec3f direc2){
	uv_type = Spherical;
	S_radius = radius;
	Sphere_Position = Pos;
	//////////////////////////////////////////////////////////////////////////
	Mat4f Translate_Mat = Mat4f(
		1.0f, 0.0f, 0.0f, -Sphere_Position.x(),
		0.0f, 1.0f, 0.0f, -Sphere_Position.y(),
		0.0f, 0.0f, 1.0f, -Sphere_Position.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	direc1.normalize();
	direc2.normalize();
	Vec3f direc3 = direc1.cross(direc2);
	Mat4f Space_Mat = Mat4f(
		direc2.x(), direc2.y(), direc2.z(), 0.0f,
		direc1.x(), direc1.y(), direc1.z(), 0.0f,
		direc3.x(), direc3.y(), direc3.z(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	ProjectionMatrix = Space_Mat*Translate_Mat;
}
Vec3f UVW_Map::GetColor(Vec3f WorldPosMin, Vec3f WorldPosMax, Vec2i screen_pos){
	Vec2i Tex_Coord1 = getPointColorAtPosition(WorldPosMin);
	Vec2i Tex_Coord2 = getPointColorAtPosition(WorldPosMax);
	if (ClosedArea(Tex_Coord1,Tex_Coord2)<=1)
	{
		return Map[Tex_Coord1.x()][Tex_Coord1.y()];
	} 
	else
	{
		//make 2>1
		if (Tex_Coord1.lenghsqrt() < Tex_Coord1.lenghsqrt()){
			Vec2i Temp = Tex_Coord1;
			Tex_Coord1 = Tex_Coord2;
			Tex_Coord2 = Temp;
		}

		return SAT[Tex_Coord2.x()][Tex_Coord2.y()] - SAT[Tex_Coord1.x()][Tex_Coord2.y()] - SAT[Tex_Coord2.x()][Tex_Coord1.y()] + SAT[Tex_Coord1.x()][Tex_Coord1.y()];
	}
	
}

Vec3f UVW_Map::getColor(Vec3f worldPos){  //get color by world position
	Vec2i TexturePostion = getPointColorAtPosition(worldPos);
	return Map[TexturePostion.y()%map_height][TexturePostion.x()%map_width];
}