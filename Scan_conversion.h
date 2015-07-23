/*realize scan conversion
 *
 *@author Qiushuo Wang
 *
*/

#ifndef _SCAN_CONVERSION_H_
#define _SCAN_CONVERSION_H_

//#include "PolyModel.h"
#include "stdafx.h"
#include <vector>
#include "Z_Buffer.h"
//#include "Ultility.h"
#include <algorithm>
using std::vector;

enum shader_type { CONSTANT_COLOR, GOUROUD_SHADER, PHONG_SHADER };

//enum illumination_model{PHONG,WARD};

class Scan_conversion
{
	//list node for edge list
	struct lineList{
		int Ymax;
		float Xmin;
		float slope;
		//  Z-Buffer related //
		//zbuffer
		float bottomZ;  //value of Z at Ymin
		//float topZ;
		float Zslope;    // dz/dy

		//gauround related////

		float bottomIntensity;
		//
		float IntensitySlop;

		//gauroud related//
		Vec3f BottomColor;
		Vec3f ColorSlope;
	};
	struct lineList_Phong{
		int Ymax;
		float Xmin;
		float slope;
		//  Z-Buffer related //
		//zbuffer
		float bottomZ;  //value of Z at Ymin
		//float topZ;
		float Zslope;    // dz/dy

		//phong related
		Vec3f BottomNormal;
		Vec3f NormalSlope;
		
		Vec3f BottomPos;
		Vec3f positionSlope;
	};


private:
	//the model produced
	//PolyModel &model;
	PolyModel model;

	//vector<Vec2i> VertexData;

	//window size
	int window_height;
	int window_width;

	//zbuffer
	Z_Buffer zbuffer;

	Mat4f viewingMatrix;

	//Ultility ultile;

	int Illumination_Model;

	//the base color
	//Vec3f base_color;
	//shader types
	
public:

	//constructors
	//Scan_conversion();
	Scan_conversion(int width,int height,PolyModel &target_model, Z_Buffer &Zbuffer,Mat4f matrix);


	//create filling model polygons
	void createShader(int ShaderType, Vec3f color);

	/////   fill polygon  //////////

	//flat shading
	void fillPolygonFlat(int Poly_index, Vec3f color);

	//gauroud shading
	void fillPolygon(int ShaderType, int Poly_index, Vec3f color, std::vector<float> VertexIntensity);
	void fillPolygonGauroud(int Poly_index, Vec3f color, std::vector<Vec3f> VertexColor);

	//phong shading
	void fillPolygonPhong(int Poly_index, Vec3f color, vector<Vec3f> word_vert);

	////*     edge table related functions*       //////

	//create line
	lineList createLinelist(Vec2i point1, Vec2i point2, float point1z, float point2z);
	//alternative
	lineList createLinelist(int point1Index, int point2Index);

	//gouroud 
	lineList createLinelist(int point1Index, int point2Index, vector<float> VertexIntensity);
	lineList createLinelist(int point1Index, int point2Index, vector<Vec3f> VertexColor);

	//phong
	lineList_Phong createPhongLinelist(int point1Index, int point2Index, vector<Vec3f>vert_position);

	//build global edge table
	void buildEdgeTable(int Poly_index, vector<vector<lineList>> &GET, vector<lineList> linecollection, vector<Vec2i> VertexList, int ymax);
	//phong global edge table
	void buildEdgeTable(int Poly_index, vector<vector<lineList_Phong>> &GET, vector<lineList_Phong> linecollection, vector<Vec2i> VertexList, int ymax);

	//shorten edge
	lineList shortenedge(lineList &line, int &ymax);

	lineList_Phong shortenedge(lineList_Phong &line, int &ymax);

	////	Z buffer related functions	////
	//get boundery of z value
	void ZbufferBound(float &zmin,float &zmax);

	//get relative z value ---for 8bit z-buffer dispaly
	float scaleZValue(const float &cur_Z,const float &zmin, const float &zmax);
	void updateZbuffer(Z_Buffer &z_buffer);

	////  end of Z Buffer realted functions ////

	//interpolation for gauroud shading & phong shading
	template <class valueType, class indexType>
	valueType Interpolation(valueType value1, valueType value2, indexType value1Index, indexType value2Index, indexType cur_index);

	~Scan_conversion();
	
public:
	Vec3f HomeGenousTransformation(Vec3f Postion, Mat4f Transformation_Matrix);
	std::vector<Vec3f> HomeGenousTransformation(std::vector<Vec3f> Postion_Vector, Mat4f Transformation_Matrix);
	//void InitateUVMap(void &texture);
	void setMappingMethod(float *attributeList,int MappingType);
	Vec3f getColor(){}
public:
	//
	void AddLight(const Light &light){
		Lightlist.push_back(light);
	}
	void AlignCamera(Camera &camera){
		cur_camera = camera;
	}

	////illumination model functions///////////////////

	//general encapsulated function
	Vec3f CalculateIllumination(Light light, Vec3f normal, Vec3f Position, PolyModel model);

	//float CalculateAmbientIntensity(float AmbientIntensity, float ka);
	float CalculateDiffuseIntensity(Light light, Vec3f normal, Vec3f Position,float kd);
	float CalculateSpecularIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f cameraPosition,int Glossy,float ks);
	//improved return color
	Vec3f CalculateAmbientIntensity(Vec3f light_color, Vec3f ka);
	Vec3f CalculateDiffuseIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f kd);
	Vec3f CalculateSpecularIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f cameraPosition, int Glossy, Vec3f ks);

	////Wards's Anisotrophy ///////////////////////////////

	Vec3f WardCalculateSpecularIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f cameraPosition, int Glossy, Vec3f ks, Vec2f Roughness);

private://illumination related parameters
	vector<Light> Lightlist;
	Camera cur_camera;
	//
	UVW_Map uvmap;  //uv mapping 
	int mapping;  //map id
	bool usemap = false;  //if map is used
public:  //mapping functions
	void LoadMap(image_t &TextureMap){
		uvmap.LoadMap(&TextureMap);
	}
	void setPlanarMapping(float width, float height, Vec3f Pos, Vec3f width_direc, Vec3f height_direc){
		uvmap.GeneratePlanarMapping(width, height, Pos, width_direc, height_direc);
		usemap = true;
	}
	void setCylinderMapping(float radius, float height, Vec3f Pos, Vec3f axis_direc, Vec3f radius_direct){
		uvmap.GenerateCylinderMapping(radius, height, Pos, axis_direc, radius_direct);
		usemap = true;
	}
	void setSphericalMapping(float radius, Vec3f Position, Vec3f axis_direc, Vec3f radius_direct){
		uvmap.GenerateSphereMapping(radius, Position, axis_direc, radius_direct);
		usemap = true;
	}

};

#endif
