/*!
 * \class Z_Buffer
 *
 * \brief 
 *
 * \author Qiushuo
 * \date April 2015
 */

#pragma once
#ifndef Z_Buffer_h__
#define Z_Buffer_h__

#include "stdafx.h"

using std::vector;

#undef max
#undef min

class Z_Buffer
{
private:
	//size of buffer
	int width;
	int height;
	
	//buffers
	//vector<vector<float>> Refresh_Buffer;
	vector<vector<float>> Depth_Buffer;
public:
	Z_Buffer();
	Z_Buffer(int window_width, int window_height){
		width = window_width;
		height = window_height;
		//initate buffers
		//Refresh_Buffer = vector<vector<float>>(window_height, vector<float>(window_width, std::numeric_limits<float>::max()));
		Depth_Buffer = vector<vector<float>>(window_height, vector<float>(window_width, std::numeric_limits<float>::max()));
	};
	float getpixel(int x,int y);
	void setpixel(int x, int y,float value);
	void Reset();
	void ReserSize(int window_width, int window_height);
	~Z_Buffer();
};
#endif // Z_Buffer_h__

