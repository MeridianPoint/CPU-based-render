#include "stdafx.h"
#include "GTranformation.h"


GTranformation::GTranformation()
{
}

//void GTranformation::AddModel(std::string name,PolyModel &new_Model){
//	ModelList[name]=new_Model;
//}

void GTranformation::setIdentity(){
	float Mat[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	TransMatrix = Mat4f(Mat);
}

void GTranformation::upcast(PolyModel& Model){
	dataTemp.resize(Model.m_verts.size());
	for (int i = 0; i < Model.m_verts.size(); i++){
		float x = Model.m_verts[i].x();
		float y = Model.m_verts[i].y();
		float z = Model.m_verts[i].z();
		float w = 1.0f;
		Vec4f up_point = Vec4f(x, y, z, w);
		dataTemp[i] = up_point;
	}
}

//void GTranformation::upcast(fbx_loader& fbxModel){
//	dataTemp.resize(fbxModel.m_vertex_list.size());
//	for (int i = 0; i < fbxModel.m_vertex_list.size(); i++){
//		float x = fbxModel.m_vertex_list[i].x();
//		float y = fbxModel.m_vertex_list[i].y();
//		float z = fbxModel.m_vertex_list[i].z();
//		float w = 1.0f;
//		Vec4f up_point = Vec4f(x, y, z, w);
//		dataTemp[i] = up_point;
//	}
//}

void GTranformation::downcast3dnorm(PolyModel& Model){  
	for (int i = 0; i < Model.m_verts.size(); i++){
		float x = Model.m_verts[i].x();
		float y = Model.m_verts[i].y();
		float z = Model.m_verts[i].z();
		float w = 1.0f;
		Vec3f point = Vec3f(dataTemp[i].x() / dataTemp[i].w(), dataTemp[i].y() / dataTemp[i].w(), dataTemp[i].z() / dataTemp[i].w());
		//load transformed vertex
		Model.m_verts[i] = point;
	}
}

//void GTranformation::downcast3dnorm(fbx_loader& fbxModel){
//	for (int i = 0; i < fbxModel.m_vertex_list.size(); i++){
//		float x = fbxModel.m_vertex_list[i].x();
//		float y = fbxModel.m_vertex_list[i].y();
//		float z = fbxModel.m_vertex_list[i].z();
//		float w = 1.0f;
//		Vec3f point = Vec3f(dataTemp[i].x() / dataTemp[i].w(), dataTemp[i].y() / dataTemp[i].w(), dataTemp[i].z() / dataTemp[i].w());
//		//load transformed vertex
//		fbxModel.m_vertex_list[i] = point;
//	}
//}

void GTranformation::downcast2d(PolyModel& Model){
	Model.m_2dverts.resize(Model.m_verts.size());
	for (int i = 0; i < Model.m_verts.size(); i++){
		float x = Model.m_verts[i].x();
		float y = Model.m_verts[i].y();
		float z = Model.m_verts[i].z();
		float w = 1.0f;
		Vec3f point = Vec3f(dataTemp[i].x() / dataTemp[i].w(), dataTemp[i].y() / dataTemp[i].w(), dataTemp[i].z() / dataTemp[i].w());
		//load transformed vertex
		//Model.m_verts[i] = point;
		//load 2d vertex
		Model.m_2dverts[i] =Vec2f(Model.m_verts[i].x(), Model.m_verts[i].y());
	}
	//MatrixStack.push(dataTemp);
	dataTemp.resize(0);
}

//void GTranformation::downcast2d(fbx_loader& fbxModel){
//	//Model.m_2dverts.resize(Model.m_verts.size());
//	//for (int i = 0; i < Model.m_verts.size(); i++){
//	//	float x = Model.m_verts[i].x();
//	//	float y = Model.m_verts[i].y();
//	//	float z = Model.m_verts[i].z();
//	//	float w = 1.0f;
//	//	Vec3f point = Vec3f(dataTemp[i].x() / dataTemp[i].w(), dataTemp[i].y() / dataTemp[i].w(), dataTemp[i].z() / dataTemp[i].w());
//	//	//load transformed vertex
//	//	//Model.m_verts[i] = point;
//	//	//load 2d vertex
//	//	Model.m_2dverts[i] = Vec2f(Model.m_verts[i].x(), Model.m_verts[i].y());
//	//}
//	//MatrixStack.push(dataTemp);
//	dataTemp.resize(0);
//}

void GTranformation::ScreenSapceTranslateMat(const Camera& cur_camera){
	//build Matix
	float Mat[4][4] = {
		1.0f, 0.0f, 0.0f, -cur_camera.positionx(),
		0.0f, 1.0f, 0.0f, -cur_camera.positiony(),
		0.0f, 0.0f, 1.0f, -cur_camera.positionz(),
		0.0f, 0.0f, 0.0f, 1.0f
	};
	TransMatrix = Mat4f(Mat)*TransMatrix;
}



void GTranformation::ScreenSpaceRotationTransformation(const Camera& cur_camera){
	//build matrix
	float Mat[4][4] = {
		cur_camera.Ux(), cur_camera.Uy(), cur_camera.Uz(), 0.0f,
		cur_camera.Vx(), cur_camera.Vy(), cur_camera.Vz(), 0.0f,
		cur_camera.directionx(), cur_camera.directiony(), cur_camera.directionz(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	TransMatrix = Mat4f(Mat)*TransMatrix;
}


void GTranformation::ProjectionMat(const Camera& cur_camera){
	//build matrix
	float Mat1[4][4] = {
		(cur_camera.d() / cur_camera.H()), 0.0f, 0.0f, 0.0f,
		0.0f, (cur_camera.d() / cur_camera.H()), 0.0f, 0.0f,
		0.0f, 0.0f, (cur_camera.f() / (cur_camera.f() - cur_camera.d())), -((cur_camera.d()* cur_camera.f()) / (cur_camera.f() - cur_camera.d())),
		0.0f, 0.0f, -1.0f, 0.0f
	};

	TransMatrix = Mat4f(Mat1)*TransMatrix;
	BasicMatrix = TransMatrix;
}

void GTranformation::ObliqueProjection(const Camera& cur_camera){
	float Mat2[4][4] = {
		(2*cur_camera.d() / (cur_camera.R()-cur_camera.L())), 0.0f, -((cur_camera.R()+cur_camera.L()) / (cur_camera.R()-cur_camera.L())), 0.0f,
		0.0f, (2 * cur_camera.d() / (cur_camera.T() - cur_camera.B())), -((cur_camera.T() + cur_camera.B()) / (cur_camera.T() - cur_camera.B())), 0.0f,
		0.0f, 0.0f, (cur_camera.f()/ (cur_camera.f() - cur_camera.d())), -((cur_camera.d()* cur_camera.f()) / (cur_camera.f() - cur_camera.d())),
		0.0f, 0.0f, -1.0f, 0.0f
	};
	TransMatrix = Mat4f(Mat2)*TransMatrix;
}

void GTranformation::Projection(const Camera& cur_camera, PolyModel& Model){
	//build matrix
	if (false){
		//float Mat2[4][4] = {
		//	(2*cur_camera.d() / (cur_camera.R()-cur_camera.L())), 0.0f, ((cur_camera.R()+cur_camera.L()) / (cur_camera.R()-cur_camera.L())), 0.0f,
		//	0.0f, (2*cur_camera.d() / (cur_camera.T()-cur_camera.B())), ((cur_camera.T()+cur_camera.B()) / (cur_camera.T()-cur_camera.B())), 0.0f,
		//	0.0f, 0.0f, (cur_camera.f() / (cur_camera.f() - cur_camera.d())), -((cur_camera.d()* cur_camera.f()) / (cur_camera.f() - cur_camera.d())),
		//	0.0f, 0.0f, -1.0f, 0.0f
		//};
	}
	//float Mat1[4][4] = {
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1 / cur_camera.d(), 0.0f
	//};
	float Mat2[4][4] = {
		(cur_camera.d() / cur_camera.H()), 0.0f, 0.0f, 0.0f,
		0.0f, (cur_camera.d() / cur_camera.H()), 0.0f, 0.0f,
		0.0f, 0.0f, (cur_camera.f() / (cur_camera.f() - cur_camera.d())), -((cur_camera.d()* cur_camera.f()) / (cur_camera.f() - cur_camera.d())),
		0.0f, 0.0f, 1.0f, 0.0f
	};
	Mat4f Projectionmatrix = Mat4f(Mat2);
	//Mat4f Prespectivematrix = Mat4f(Mat1);
	for (int i = 0; i < Model.m_verts.size(); i++){
		float x = Model.m_verts[i].x();
		float y = Model.m_verts[i].y();
		float z = Model.m_verts[i].z();
		float w = 1.0f;
		Vec4f up_point = Vec4f(x, y, z, w);
		
		//up_point = Prespectivematrix*up_point;
		up_point = Projectionmatrix*up_point;
		Model.m_verts[i] = Vec3f(up_point.x() / up_point.w(), up_point.y() / up_point.w(), up_point.z() / up_point.w());
	}
}


//void GTranformation::Clipping(const Camera& cur_camera){
//	vector<Vec4f> Clipping_vertex;
//	vector<Vec4f> clipping_planes_norm;  //face normal of clipping plane
//	for (unsigned int i = 0; i < ModelList[0].m_polys.size();i++){
//		for (unsigned int j = 0; j < ModelList[0].m_polys[i].size(); j++){
//			vector<int> newPolygon;
//			if (j = ModelList[0].m_polys[i].size()-1){
//				//if(ModelList[0].m_verts[m_polys[i][j]-1].isoutside&&ModelList[0].m_verts[m_polys[i][0]-1].isoutside){
//				//remove edge
//			//}
//				//do 
//			}
//			else{
//			}
//		}
//	}
//	//Clipping_vertex.push_back(Vec4f(cur_camera));
//}


void GTranformation::BackfaceCulling(const Camera& cur_camera, PolyModel& Model){
	Model.m_face_dir.resize(Model.m_polys.size());
	//traverse polygons
	for (int i = 0; i < Model.m_polys.size(); i++){
		Vec3f vector1 = Model.m_face_norms[i];
		Vec3f facemiddle = Vec3f(0.0f,0.0f,0.0f);
		for (int j = 0; j < Model.m_polys[i].size(); j++){
			facemiddle += Model.m_verts[Model.m_polys[i][j] - 1];
		}
		//get the position of face center
		facemiddle = facemiddle / (float)Model.m_polys[i].size();
		//get the viewing vector
		Vec3f vector2 =  Vec3f(cur_camera.positionx(), cur_camera.positiony(), cur_camera.positionz())-facemiddle;
		float result = vector2.dot(vector1);
		//evaluate the visibility of the face
		if (result >= 0){
			Model.m_face_dir[i] = true;
		}
		else{
			Model.m_face_dir[i] = false;
		}
	}
}

void GTranformation::PreformTransformation(){
	for (int i = 0; i < dataTemp.size(); i++){
		dataTemp[i] = TransMatrix*dataTemp[i];
	}
}

void GTranformation::screen_2dPrejection(const Camera& cur_camera){
	float Mat[4][4] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1 / cur_camera.d(), 0.0f
	};
	TransMatrix = Mat4f(Mat)*TransMatrix;
}

void GTranformation::scaleVertex(int width, int height){
	//create scaling matrix
	Mat4f ScaleMatrix = Mat4f(
		(float)width,0.0f,0.0f,0.0f,
		0.0f, (float)height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
	TransMatrix = ScaleMatrix*TransMatrix;
}

void GTranformation::PrespectiveView(PolyModel &model, const Camera& cur_camera){
	setIdentity();

	//backface culling
	BackfaceCulling(cur_camera, model);

	//preform transformation approach
	//load the vertex matrix and upcast it to 4d
	upcast(model);
	//create screen space translate matrix
	ScreenSapceTranslateMat(cur_camera);
	//create screen space rotation matrix and multiply it by space translation matrix
	ScreenSpaceRotationTransformation(cur_camera);
	//create screen space projection matrix and multiply it by the result of previous transfomration matrices
	ProjectionMat(cur_camera);
	//create screen space rotation matrix and multiply it by the result of previous transfomration matrices
	PreformTransformation();
	//downcast the 4d model matrix to 3d
	downcast3dnorm(model);
	//2d projection
	//screen_2dPrejection(cur_camera);
	//Model.draw();
	//
	//PreformTransformation();
	//downcast the 4d model matrix to 2d and restore back in the model's object
	downcast2d(model);
}

GTranformation::~GTranformation()
{
}