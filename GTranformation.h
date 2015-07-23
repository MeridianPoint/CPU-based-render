/* ---------------------------------------------------------------------------
**
** GTranformation.h
** Apply matrix transformation to model matrices
**
** Author: Qiushuo Wang
** -------------------------------------------------------------------------*/


#ifndef _G_TRANS_H_
#define _G_TRANS_H_

#include "stdafx.h"
//#include "Vec3.h"
#include "Mat4.h"
#include "Vec2.h"
#include "Camera.h"
#include <vector>
using std::vector;
using std::stack;

class GTranformation
{
private:
	//std::map<std::string,PolyModel> ModelList;
	//restore the 4d transformation matrix
	Mat4f TransMatrix;
	Mat4f BasicMatrix;
	//restore the 4d model matrix
	vector<Vec4f> dataTemp;
	//matrix stack
	stack<Mat4f> MatrixStack;
public:
	GTranformation();
	//multiply by identity matrix
	/* Name: setIdentity
	*
	* Description:	set the dataTemp to identity matrix
	*/

	//void AddModel(std::string name,PolyModel &new_Model);

	void setIdentity();

	/* Name: upcast
	*
	* Description:	transform model matrix to 4d matrix and restore it in this class
	*	including any I/O it performs.
	* Arguments:	PolyModel& Model: the 3d model class the data came from
	*	repeat their types from the argument list.
	* Post:	The 4d vertex matrix restored in this.dataTemp
	* Exceptions:	
	*/
	void upcast(PolyModel& Model);


	//load 4d matrix data back to model
	/* Name: downcast
	*
	* Description:	load the 4d vertex matrix in dataTemp to a object of model
	* restored as 2d and 3d vertex matrices
	* Arguments:	PolyModel& Model: the 3d model class the data came to
	* Modifies:	dataTemp is modified to the 4d vertex matrix of the model
	* Returns:	nothing
	* Pre:		use upcast to restore the vertex matirx in the instance of this object
	*	preform necessary trnasformation upon this object
	*/
	void downcast2d(PolyModel& Model);

	void downcast3dnorm(PolyModel& Model);


	//preform screen space translate
	/* Name: setIdentity
	*
	* Description:	set the
	*	including any I/O it performs.
	* Arguments:	describe what each argument is; do not simply
	*	repeat their types from the argument list.
	* Modifies:	description of any changes the function makes
	*	to the externally visible state of the object.
	* Returns:	description of what the function returns.
	* Pre:		dataTemp matrix is set to Indentity matrix
	* Post:	describe any postconditions
	*	that the function guarantees.
	*	otherwise the client won’t know about them.
	* Exceptions:	describe any exceptions that may be thrown.
	*/
	void ScreenSapceTranslateMat(const Camera& cur_camera);

	//preform screen space rotation transformation
	void ScreenSpaceRotationTransformation(const Camera& cur_camera);

	//preform prespective projection
	void ProjectionMat(const Camera& cur_camera);

	//preform oblique projection
	void ObliqueProjection(const Camera& cur_camera);

	void Projection(const Camera& cur_camera, PolyModel& Model);

	//2d Projection
	void screen_2dPrejection(const Camera& cur_camera);

	//clipping (not implement yet)
	//void Clipping(const Camera& cur_camera);


	//Back-face culling
	/* Name: BackfaceCulling
	*
	* Description:	 prepare backface culling by building a vector of the visibility vector
	* Arguments:	const Camera& cur_camera: The camera, provide the parameters of viewing direction
	*				PolyModel& Model: model provide geometry infomation and restore the result
	* Modifies:	a vector of visibility is created
	* Returns:	None
	* Pre:		the camera must be created
	* Post:	the vector for polygon visibilty is created
	*/
	void BackfaceCulling(const Camera& cur_camera, PolyModel& Model);

	//multiply matrix by 4d data
	/* Name: PreformTransformation
	*
	* Description:	 multiply the loaded model matrix with the transformation matrix
	* Arguments:	None
	* Modifies:	dataTemp is modified
	* Returns:	None
	* Pre:		model vertex matrix must be loaded
	*	transfomation matrix must be created successfully
	* Post:	The 4d vertex is transformed accordingly
	*/
	void PreformTransformation();

	//preform all necessary prespective transformations
	void PrespectiveView(PolyModel &model, const Camera& cur_camera);

	//scale the vertices
	void scaleVertex(int width,int height);

	///////////////         transformation matrix functions //////////////////////
	void PushTransformationMatrix(){
		Mat4f TempMatrix = MatrixStack.top();
		MatrixStack.push(TempMatrix);
	}

	void PopTransformationMatrix(){
		MatrixStack.pop();
	}

	void LoadMatrix(Mat4f iMatrix){
		MatrixStack.pop();
		MatrixStack.push(iMatrix);
	}
	Mat4f GetMatrix(){
		return MatrixStack.top();
	}

	Mat4f testGetMatrix(Camera cur_camera){
		return BasicMatrix;
	}

	////////////////////////////////////

	~GTranformation();
};

#endif
