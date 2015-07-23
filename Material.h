////Material.h  ////
////Apply the material to Model
////Author Qiushuo Wang

#ifndef MATERIAL_H_
#define MATERIAL_H_

//include pre-compile header
#include "stdafx.h"

//define parameters

enum illumintation_model {PHONG,WARD};

class Material
{
private:
	Vec3f Ambient_color;   //Ambient color
	Vec3f Diffuse_color;  //diffuse color
	Vec3f Specul_color;   //specular color
	Vec3f ka=Vec3f(0.3,0.3,0.3);      //ambient strength
	Vec3f kd;      //diffuse reflecity
	Vec3f ks;     //specular reflecity
	//Vec3f ks;
	int Spec_Level;        //specular level
	int GLossiness=1;        //glossy

	//experimental 
	int Illuminate=PHONG;
	float opacity;  //opacity

	//Ward model
	Vec3f epsilon;
	Vec2f Roughness;

public:
	Material() : Ambient_color(0.5f, 0.5f, 0.5f), Diffuse_color(0.5f, 0.5f, 0.5f), Specul_color(1.0f, 1.0f, 1.0f),kd(1.0f),ks(0.0f){};
	//set Specular level --determine kd,ks
	void setColor(Vec3f Am_col,Vec3f Dif_col,Vec3f Spe_col){
		ka*= Am_col;
		//Diffuse_color = Dif_col;
		kd *= Dif_col;
		ks *= Spe_col;
	}
	void setDiffuseColor(Vec3f color){
		Diffuse_color = color;
	}

	void setSpecularColor(Vec3f color){
		Specul_color = color;
	}
	void setSpecularLevel(int SpecularLevel){
		Spec_Level = SpecularLevel;
		ks = Vec3f(1.0, 1.0, 1.0)*(float)SpecularLevel / 100;
		kd = Vec3f(1.0,1.0,1.0) - ks;
	}
	void setGlossy(int Glos){
		GLossiness = Glos;
	}

	//change to Ward's model
	void WardModel(Vec3f eps, Vec2f roughness){
		Illuminate = WARD;
		epsilon = eps;
		Roughness = roughness;
	}


	/////get functions////////////
	Vec3f GetDiffuseColor(){ return Diffuse_color; }
	Vec3f GetAmbientColor(){ return Ambient_color; }
	Vec3f GetSpecularColor(){ return Specul_color; }
	Vec3f getKd(){ return kd; }
	Vec3f getKs(){ return ks; }
	Vec3f getKa(){ return ka; }
	float getSpecularLevel() {
		return Spec_Level;
	}
	int getGLossy(){
		return GLossiness;
	}

	int getIlluminationType(){
		return Illuminate;
	}

	//ward
	Vec2f getRoughness(){
		return Roughness;
	}

	Vec3f getEspi(){
		return epsilon;
	}

	~Material();
};

#endif