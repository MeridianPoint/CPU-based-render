#include "stdafx.h"
#include "Z_Buffer.h"


Z_Buffer::Z_Buffer()
{
}


float Z_Buffer::getpixel(int x, int y){
	return Depth_Buffer[y][x];
}
void Z_Buffer::setpixel(int x, int y, float value){
	Depth_Buffer[y][x] = value;
}

Z_Buffer::~Z_Buffer()
{
	/*if (Depth_Buffer.size() > 0){
		Depth_Buffer.clear();
		width = 0;
		height = 0;
	}*/
		
	//Refresh_Buffer.clear();
}

void Z_Buffer::Reset(){
	for (unsigned int i = 0; i < Depth_Buffer.size();i++){
		for (unsigned int j = 0; j < Depth_Buffer[0].size(); j++){
			Depth_Buffer[i][j] = std::numeric_limits<float>::max();
		}
	}
}


void Z_Buffer::ReserSize(int window_width, int window_height){
	width = window_width;
	height = window_height;
	//initate buffers
	//Refresh_Buffer = vector<vector<float>>(window_height, vector<float>(window_width, std::numeric_limits<float>::max()));
	Depth_Buffer = vector<vector<float>>(window_height, vector<float>(window_width, std::numeric_limits<float>::max()));
}