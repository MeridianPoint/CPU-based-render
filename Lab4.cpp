// Lab4.cpp : Defines the entry point for the console application.
//

//load pre-compile header
#include "stdafx.h"

//define global parameters
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//static parameters
static int shading = PHONG_SHADER;
static int WinWidth = WINDOW_WIDTH;
static int WinHeight = WINDOW_HEIGHT;

#define FILE_DIRECTORY "..//Lab4//Resources//"
//model & textures
PolyModel Model;   //define model
PolyModel Model1;   //define model

image_t texture;

Camera main_camera;  //define camera
Camera Oblique_camera; //Oblique camera

Omni_Light Default_Light; //set up light
vector<Light*> lightlist;

//set filename
std::string fileName = FILE_DIRECTORY + std::string("better-ball.d");
std::string fileName1 = FILE_DIRECTORY + std::string("column.d");
std::string mapName = FILE_DIRECTORY + std::string("LandscapeVarious0096_M.tga");
std::ifstream ifs(fileName.c_str());
std::ifstream ifs1(fileName1.c_str());


///////static scene parameters

Z_Buffer *default_ZBuffer;
static Mat4f ViewingMatrix;

void Init(){
	//set camera position
	Vec3f Camera_Pos = Vec3f(1.2f, 1.2f, -1.2f);
	main_camera = Camera(Camera_Pos);
	//set camera target
	Vec3f Target = Vec3f(-0.0f, -0.0f, -0.2f);
	main_camera.LookAt(Target);
	//set camera Up vector
	Vec3f Up_v = Vec3f(0.0f, 0.0f, -1.2f);
	main_camera.SetCamera_Up(Up_v);
	//set clipping
	float clipping[2] = { 0.3f, 7.0f };
	main_camera.SetClipping(clipping[0], clipping[1]);
	main_camera.setViewheight(0.2f);

	char* cstr = new char[mapName.size() + 1];
	strcpy(cstr, mapName.c_str());

	tgaLoad(cstr, &texture, TGA_DEFAULT);

	////////////////// set up light ////////////////////////////
	//Default_Light = Omni_Light(Vec3f(140.0f, 140.0f, 60.0f));
	Default_Light = Omni_Light();
	Default_Light.setPostion(Vec3f(-2.0f, -3.0f, -4.0f));
	Default_Light.setColor(Vec3f(1.0f, 1.0f, 1.0f));
	Default_Light.setIntensity(4.0f);
	Default_Light.setFalloutType(NONE);
	Light* Spot_light1 = new Omni_Light();
	lightlist.push_back(Spot_light1);
}

void InitModels(){
	Model.loadModel(ifs);

	UVW_Map uv;


	Model1.loadModel(ifs1);

	//set material
	Model.setSpecularLevel(60);
	Model.setGlossy(16);
	Model.setColor(Vec3f(0.3f, 0.4f, 0.8f), Vec3f(0.2f, 0.3f, 0.9f), Vec3f(1.0f, 1.0f, 1.0f));
	//using ward's model   ---comment this to use phong illumination model
	Model.shiftTOWard(Vec3f(0.0, 0.3, 0.9), Vec2f(0.3, 0.6));

	//
	GTranformation Trans;
	Trans.PrespectiveView(Model, main_camera);

	GTranformation Trans1;
	Trans1.PrespectiveView(Model1, main_camera);

	Mat4f viewingMatrix = Trans.testGetMatrix(main_camera);
	//Mat4f viewingMatrix1 = Trans1.testGetMatrix(main_camera);
	ViewingMatrix = Trans.testGetMatrix(main_camera);
}

void DrawScene(){
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//load models
	/*if (!default_ZBuffer){
	default_ZBuffer = new Z_Buffer(WinWidth, WinHeight);
	}
	else{
	default_ZBuffer->ReserSize(WinWidth, WinHeight);
	}*/
	default_ZBuffer = new Z_Buffer(WinWidth, WinHeight);

	//use transformation
	/*GTranformation Trans;
	Trans.PrespectiveView(Model, main_camera);

	GTranformation Trans1;
	Trans1.PrespectiveView(Model1, main_camera);

	Mat4f viewingMatrix = Trans.testGetMatrix(main_camera);
	Mat4f viewingMatrix1 = Trans1.testGetMatrix(main_camera);
	ViewingMatrix = Trans.testGetMatrix(main_camera);*/

	////Draw wireframe (back- culling)
	//Model.draw();
	//drawing the graph according to 2d vertex matrix
	//Model.draw2d();
	//Model1.draw2d();

	/*apply shader and color*/
	gluOrtho2D(0, WinWidth, 0, WinHeight);

	////*   draw shader*////

	//draw model1
	Scan_conversion conver(WinWidth, WinHeight, Model, *default_ZBuffer, ViewingMatrix);
	conver.AddLight(Default_Light);
	conver.AlignCamera(main_camera);

	//uv mapping functions
	conver.LoadMap(texture);
	//conver.setPlanarMapping(4.0, 2.0, Vec3f(-1.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0), Vec3f(1.0, 0.0, 0.0));         //planar mapping
	//conver.setCylinderMapping(3.0, 5.0, Vec3f(0.0, 0.0, -0.3), Vec3f(0.0, 0.0, 1.0), Vec3f(1.0, 0.0, 0.0));    //cylinder mapping
	conver.setSphericalMapping(6.0, Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 0.0, 1.0), Vec3f(0.0,1.0,0.0));           //spherical mapping
	//conver.createShader(CONSTANT_COLOR, Vec3f(0.3f, 0.4f, 0.6f));
	//conver.createShader(GOUROUD_SHADER, Vec3f(0.3f, 0.4f, 0.6f));
	//conver.createShader(PHONG_SHADER, Vec3f(0.3f, 0.4f, 0.6f));
	conver.createShader(shading, Vec3f(0.5f, 0.2f, 0.2f));
	conver.updateZbuffer(*default_ZBuffer);

	//draw model2
	/*Scan_conversion conver1(WinWidth, WinHeight, Model1, *default_ZBuffer, ViewingMatrix);
	conver1.AddLight(Default_Light);
	conver1.AlignCamera(main_camera);
	conver1.createShader(CONSTANT_COLOR, Vec3f(0.1f, 0.2f, 0.6f));
	conver1.updateZbuffer(*default_ZBuffer);*/

	//draw model1
	//Scan_conversion conver2(WINDOW_WIDTH, WINDOW_HEIGHT, Model, default_ZBuffer);
	//conver2.createShader(CONSTANT_COLOR, Vec3f(0.3f, 0.4f, 0.6f));
	//conver2.updateZbuffer(default_ZBuffer);

	//delete default_ZBuffer;

	glutSwapBuffers();
}

void debugdraw(){
	Model.loadModel(ifs);
	//use transformation
	GTranformation Trans;
	//Trans.BackfaceCulling(main_camera, Model);
	Trans.setIdentity();

	//backface culling
	Trans.BackfaceCulling(main_camera, Model);

	//preform transformation approach
	//load the vertex matrix and upcast it to 4d
	Trans.upcast(Model);
	//create screen space translate matrix
	Trans.ScreenSapceTranslateMat(main_camera);
	//create screen space rotation matrix and multiply it by space translation matrix
	Trans.ScreenSpaceRotationTransformation(main_camera);
	//create screen space projection matrix and multiply it by the result of previous transfomration matrices
	Trans.ProjectionMat(main_camera);
	//create screen space rotation matrix and multiply it by the result of previous transfomration matrices
	//Trans.screen_2dPrejection(main_camera);
	//multiply the transformation matrix with vertex matrix
	Trans.PreformTransformation();
	//downcast the 4d model matrix to 3d and 2d and restore back in the model's object
	Trans.downcast3dnorm(Model);
	//2d projection
	Trans.screen_2dPrejection(main_camera);
	//downcast the 4d model matrix to 3d and 2d and restore back in the model's object
	Trans.downcast2d(Model);

	//Model.debugdraw();
	//drawing the graph according to 2d vertex matrix
	//Model.draw2d();

	glutSwapBuffers();
}

void resize(GLsizei width, GLsizei height){
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	delete default_ZBuffer;
	default_ZBuffer = new Z_Buffer(width, height);
	//glViewport(0,0,width,height);
	WinWidth = width;
	WinHeight = height;
	//gluOrtho2D(0, WinWidth, 0, WinHeight);
	//Model.draw2d();
	Scan_conversion conver(width, height, Model, *default_ZBuffer, ViewingMatrix);
	conver.AddLight(Default_Light);
	conver.AlignCamera(main_camera);
	conver.createShader(shading, Vec3f(0.5f, 0.2f, 0.2f));
	conver.updateZbuffer(*default_ZBuffer);

	glutSwapBuffers();
}

int _tmain(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(30, 30);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Lab 4: TextureMapping");

	Init();
	InitModels();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(resize);

	//main loop
	glutMainLoop();

	return 0;
}

