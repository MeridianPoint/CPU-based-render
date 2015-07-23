#include "stdafx.h"
#include "Scan_conversion.h"
#include <algorithm>

//Scan_conversion::Scan_conversion()
//{
//	model = NULL;
//}

Scan_conversion::Scan_conversion(int width, int height, PolyModel &target_model, Z_Buffer &Zbuffer, Mat4f matrix){
	window_width = width;
	window_height = height;
	model = target_model;
	zbuffer = Zbuffer;
	viewingMatrix = matrix;
}

Vec3f Scan_conversion::HomeGenousTransformation(Vec3f vector, Mat4f TransformMatrix){
	Vec4f Homo_vector(vector.x(), vector.y(), vector.z(), 1);
	Homo_vector = TransformMatrix*Homo_vector;
	Vec3f result_vector = Vec3f(Homo_vector.x(), Homo_vector.y(), Homo_vector.z());
	return result_vector;
}

std::vector<Vec3f> Scan_conversion::HomeGenousTransformation(std::vector<Vec3f> Postion_Vector, Mat4f Transformation_Matrix){
	std::vector<Vec3f> resultVector;
	resultVector.resize(Postion_Vector.size());
	for (unsigned int i = 0; i < Postion_Vector.size();i++)
	{
		resultVector[i]=HomeGenousTransformation(Postion_Vector[i], Transformation_Matrix);
	}
	return resultVector;
}


void Scan_conversion::createShader(int ShaderType, Vec3f color){
	//calculate color
	Vec3f base_color = color;

	//traverse all lights
	/*for (unsigned int ln = 0; ln < Lightlist.size(); ln++){
	}*/
	
	//inverse trnasform vertex coordinate
	vector<Vec3f> World_cord_verts;
	//World_cord_verts.resize(model.m_verts.size());
	Mat4f TransformationMat = viewingMatrix.Inverse();
	World_cord_verts = HomeGenousTransformation( model.m_verts,TransformationMat);
	//for (unsigned int u = 0; u < World_cord_verts.size(); u++){
	//	World_cord_verts[i]=TransFormationMat*model.m_verts[i];
	//}

	//use gouroud shader
	if (ShaderType ==CONSTANT_COLOR){
		//traverse each polygon visible

		

		for (unsigned int i = 0; i < model.m_polys.size(); i++){
			//backface culling
			//if (!model.m_face_dir[i]){
			//	continue;
			//}
			////////////calcalate color for each polygon//////////////////////////////////////////////
			float Intensity;
			/*Vec3f facePosition(0.0f,0.0f,0.0f);
			for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
				facePosition += World_cord_verts[model.m_polys[i][j]-1];
			}*/
			//Vec3f facePosition = facePosition / (float)model.m_polys[i].size();
			Vec3f facePosition = World_cord_verts[model.m_polys[i][0] - 1];
			//float faceDistance;
			//calculate face Position
			//faceDistance = (Lightlist[0].getPosition() - facePosition).length();

			//calculate intensity
			Vec3f cur_color;
			cur_color = CalculateIllumination(Lightlist[0], model.m_face_norms[i], facePosition, model);
			//cur_color = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka(), model.getAmbientColor())/* + CalculateDiffuseIntensity(Lightlist[0], model.m_face_norms[i], facePosition, model.getkd(), model.getDiffuseColor()) + CalculateSpecularIntensity(Lightlist[0], model.m_face_norms[i], facePosition, cur_camera.getPosition(), model.getSpecular_level(), model.getks(), model.getSpecularColor())*/;
			//cur_color = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka(), model.getAmbientColor()) + CalculateDiffuseIntensity(Lightlist[0], model.m_face_norms[i], facePosition, model.getkd(), model.getDiffuseColor()) + CalculateSpecularIntensity(Lightlist[0], model.m_face_norms[i], facePosition, cur_camera.getPosition(), model.getGlossy(), model.getks(),model.getSpecularColor());
			//Vec3f cur_color = color*Intensity;

			/////////////////////draw polygon /////////////////////////////////////////////

			//fill polygon
			fillPolygonFlat(i, cur_color);
		}
	}
	if (ShaderType == GOUROUD_SHADER){
		//////////light intensity of each vertex;////////////////
		vector<float> VertexIntensity;
		vector<Vec3f> VertexColor;
		//test
		//VertexIntensity.resize(model.m_vert_norms.size());
		//for (unsigned int n = 0; n < model.m_vert_norms.size(); n++){
		//	VertexIntensity[n] = Lightlist[0].getPosition().dot(World_cord_verts[n]);
		//}
		//get world coordinate 
		/*vector<Vec3f> word_cord_verts;
		word_cord_verts.resize(model.m_verts.size());*/
		//end of testing
		VertexIntensity.resize(model.m_vert_norms.size());
		VertexColor.resize(model.m_vert_norms.size());
		for (unsigned int n = 0; n < model.m_vert_norms.size(); n++){
			VertexColor[n] = CalculateIllumination(Lightlist[0], model.m_vert_norms[n], World_cord_verts[n], model);
			//VertexIntensity[n] = CalculateDiffuseIntensity(Lightlist[0], model.m_vert_norms[n], World_cord_verts[n], model.getkd()) + CalculateSpecularIntensity(Lightlist[0], model.m_vert_norms[n], World_cord_verts[n], cur_camera.getPosition(), model.getGlossy(), model.getks());
			//if (VertexIntensity[n] < 0){
			//	VertexIntensity[n] = 0;
			//}
		}
		
		///////////////////////draw polygon////////////////////////////
		//
		for (unsigned int i = 0; i < model.m_polys.size(); i++){
			//if (model.m_face_dir[i]){
			//	continue;
			//}
			//fillPolygon(GOUROUD_SHADER, i, color, VertexIntensity);
			fillPolygonGauroud(i, color, VertexColor);
		}
		//
	}

	//use phong shader
	if (ShaderType == PHONG_SHADER){
		////////// get vertex world coordinate ////////////////

		//get world coordinate 
		//vector<Vec3f> word_cord_verts;
		//word_cord_verts.resize(model.m_verts.size());
		//end of testing
		//fillPolygonPhong(20, color, World_cord_verts);

		///////////////////////draw polygon////////////////////////////
		for (unsigned int i = 0; i < model.m_polys.size(); i++){
			//backface culling
			if (!model.m_face_dir[i]){
				continue;
			}
			//fill polygon
			fillPolygonPhong(i, color, World_cord_verts);
		}
	}
}

//flat shading @para Polyindex polygon index
void Scan_conversion::fillPolygonFlat(int Poly_index, Vec3f color){
	//initate edgetables
	vector<vector<lineList>> GET;
	//initate Active edge table
	vector<lineList> AET;
	//index of polygon
	int i = Poly_index;
	//output z-buffer?
	bool PrintZBuffer = false;

  //constant color shading
		//define z bound
		float zmin, zmax;
		ZbufferBound(zmin,zmax);

			//create a vector to restore vertices
			vector<Vec2i> vertexlist;
			vector<float> vertexZValue;

			vertexlist.resize(model.m_polys[i].size());
			vertexZValue.resize(model.m_polys[i].size());
			
			for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
				//convert point data to integer
				Vec2i point = Vec2i((model.m_2dverts[model.m_polys[i][j]-1].x()+1)*window_width/2, (model.m_2dverts[model.m_polys[i][j]-1].y()+1)*window_height/2);
				vertexZValue[j] = model.m_verts[model.m_polys[i][j] - 1].z();
				vertexlist[j] = point;
			}


			/*	create edge table	*/

			//create edges holder
			vector<lineList> listcollection;

			listcollection.resize(vertexlist.size());

			//fill edge holder
			for (int edgecounter = 0; edgecounter < listcollection.size(); edgecounter++){
				//if first elem
				if (edgecounter == (listcollection.size()-1)){
					if ((vertexlist[edgecounter].y() - vertexlist[0].y())!=0){
						listcollection[edgecounter] = createLinelist(vertexlist[edgecounter],vertexlist[0],vertexZValue[edgecounter],vertexZValue[0]);
					}
				}
				else {
					if ((vertexlist[edgecounter].y() - vertexlist[edgecounter + 1].y())!=0){
						listcollection[edgecounter] = createLinelist(vertexlist[edgecounter],vertexlist[edgecounter + 1],vertexZValue[edgecounter],vertexZValue[edgecounter + 1]);
					}
					
				}
				
			}



			/* finish edge table creation*/

			//define color
			//color *= (1-model.m_face_norms[i].z());

			

			/* begin drawing*/
			glPointSize(1.0f);

			/////*	drawing   polygon*///////
			glBegin(GL_POINTS);
			//define color
			glColor3f(color.r(), color.g(), color.b());
			//iniate and defien the drawing bounding
			int ymin,ymax;
			auto ymin_point = std::min_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
			ymin = ymin_point->y();
			auto ymax_point = std::max_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
			ymax = ymax_point->y();

			/*	edge table related operations*/
			buildEdgeTable(i, GET, listcollection, vertexlist,ymax);


			//traverse fill space
			//traverse y coordinates
			float incrementIndicator = 0;
			for (int ycoord = ymin; ycoord <= ymax; ycoord++){
				//update AET
				if (GET[ycoord].size()!=0){
					//add new list
					AET.insert(AET.end(), GET[ycoord].begin(), GET[ycoord].end());

					//sort according to x --ascending
					sort(AET.begin(), AET.end(), [](const lineList& edge1, const lineList& edge2){return edge1.Xmin<edge2.Xmin; });
				}
				else{
					sort(AET.begin(), AET.end(), [](const lineList& edge1, const lineList& edge2){return edge1.Xmin<edge2.Xmin; });
				}
				
				/*	scanline drawing		*/
				//count traversed line
				int traversedLine=0;
				//traverse x each scanline
				int xcoord = 0;
				//record z index by pixel
				float z_index = 0;
				//normal by pixel
				Vec3f Cur_Normal;

				//traverse all line in active edge table
				while (traversedLine<AET.size()){
					//initate xcoord and z index
					if (traversedLine == 0){
						xcoord = AET[0].Xmin;
						//assign z buffer value
						z_index = AET[0].bottomZ;
						//update z
						if (zbuffer.getpixel(xcoord, ycoord)>z_index)
						{
							zbuffer.setpixel(xcoord, ycoord, z_index);
							//draw z for debugging
							if (PrintZBuffer){
								glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
							}
							glVertex2i(xcoord, ycoord);
						}
						traversedLine++;
					}
					//draw
					if (traversedLine % 2 == 1/*&&traversedLine!=1*/){
						//z-buffer condition
						//increment z
						//z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
						if (zbuffer.getpixel(xcoord, ycoord)>z_index)
						{
							//update zbuffer
							zbuffer.setpixel(xcoord, ycoord, z_index);
							//draw z for debugging
							if (PrintZBuffer){
								glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
							}
							glVertex2i(xcoord, ycoord);
						}
						
						//draw z for debugging
						//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));

						//glVertex2i(xcoord, ycoord);
					}
					//don't draw
					else{
					}

					/* check cross edge*/
					if (xcoord == floor(AET[traversedLine].Xmin)){
						//incerment traversed line counter
						traversedLine++;
						z_index = AET[traversedLine-1].bottomZ;
						//ZBuffer checking
						//if (zbuffer.getpixel(xcoord, ycoord)>z_index)
						//{
						//	//update zbuffer
						//	zbuffer.setpixel(xcoord, ycoord, z_index);
						//	//draw
						//	glVertex2i(xcoord, ycoord);
						//}

						//if on lowest/highest vertex
						if (traversedLine <= (AET.size() - 1) && floorf(AET[traversedLine].Xmin) == floorf(AET[traversedLine - 1].Xmin)){
							if (zbuffer.getpixel(xcoord, ycoord)>z_index)
							{
								//update zbuffer
								zbuffer.setpixel(xcoord, ycoord, z_index);
								//draw z for debugging
								if (PrintZBuffer){
									glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
								}
								//draw
								//draw pixel
								glVertex2i(xcoord, ycoord);
							}
							//glVertex2i(xcoord, ycoord);
							traversedLine++;
						}
					}
					//increment x
					xcoord++;
					//incrementz
					if (traversedLine < AET.size()){
						z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (floor(AET[traversedLine - 1].Xmin) - floor(AET[traversedLine].Xmin));
						/*z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);*/
					}
					
					
				}

				/*	upate AET*/
				//update AET elements
				for (unsigned int increment = 0; increment < AET.size();increment++){
						AET[increment].Xmin += AET[increment].slope;
					AET[increment].bottomZ += AET[increment].Zslope;
				}

				//delete items from AET
				//delete invalid list
				AET.erase(remove_if(AET.begin(), AET.end(), [ycoord](const lineList& edge){return (edge.Ymax <= ycoord); }),AET.end());
				/* end of update AET*/

			}
			//end of traverse y coordinate
			glEnd();

	
	//end of //constant color shading
}

//gauroud shading
void Scan_conversion::fillPolygon(int ShaderType, int Poly_index, Vec3f color, std::vector<float> VertexIntensity){
	//initate edgetables
	vector<vector<lineList>> GET;
	//initate Active edge table
	vector<lineList> AET;
	//index of polygon
	int i = Poly_index;
	//output z-buffer?
	bool PrintZBuffer = false;
	if (ShaderType == GOUROUD_SHADER){
		//define z bound
		float zmin, zmax;
		ZbufferBound(zmin, zmax);

		//intensity on pixel
		float PixelIntesity;
		//PixelIntensity

		//create a vector to restore vertices
		vector<Vec2i> vertexlist;
		vector<float> vertexZValue;

		vertexlist.resize(model.m_polys[i].size());
		vertexZValue.resize(model.m_polys[i].size());

		for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
			//convert point data to integer
			Vec2i point = Vec2i((model.m_2dverts[model.m_polys[i][j] - 1].x() + 1)*window_width / 2, (model.m_2dverts[model.m_polys[i][j] - 1].y() + 1)*window_height / 2);
			vertexZValue[j] = model.m_verts[model.m_polys[i][j] - 1].z();
			vertexlist[j] = point;
		}


		/*	create edge table	*/

		//create edges holder
		vector<lineList> listcollection;

		listcollection.resize(vertexlist.size());

		//fill edge holder
		//for (int edgecounter = 0; edgecounter < listcollection.size(); edgecounter++){
		//	//if first elem
		//	if (edgecounter == (listcollection.size() - 1)){
		//		if ((vertexlist[edgecounter].y() - vertexlist[0].y()) != 0){
		//			//listcollection[edgecounter] = createLinelist(vertexlist[edgecounter], vertexlist[0], vertexZValue[edgecounter], vertexZValue[0]);
		//			listcollection[edgecounter] = createLinelist(edgecounter,0,VertexIntensity);
		//		}
		//	}
		//	else {
		//		if ((vertexlist[edgecounter].y() - vertexlist[edgecounter + 1].y()) != 0){
		//			//listcollection[edgecounter] = createLinelist(vertexlist[edgecounter], vertexlist[edgecounter + 1], vertexZValue[edgecounter], vertexZValue[edgecounter + 1]);
		//			listcollection[edgecounter] = createLinelist(edgecounter, edgecounter + 1, VertexIntensity);
		//		}

		//	}

		//}

		for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
			if (j == model.m_polys[i].size() - 1){
				listcollection[j] = createLinelist(model.m_polys[i][j] - 1, model.m_polys[i][0] - 1, VertexIntensity);
			}
			else{
				listcollection[j] = createLinelist(model.m_polys[i][j] - 1, model.m_polys[i][j+1] - 1, VertexIntensity);
			}
		}



		/* finish edge table creation*/

		//define color
		//color *= (1 - model.m_face_norms[i].z());



		/* begin drawing*/
		glPointSize(1.0f);

		/////*	drawing   polygon*///////
		glBegin(GL_POINTS);
		//define color
		glColor3f(color.r(), color.g(), color.b());
		//iniate and defien the drawing bounding
		int ymin, ymax;
		auto ymin_point = std::min_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
		ymin = ymin_point->y();
		auto ymax_point = std::max_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
		ymax = ymax_point->y();

		/*	edge table related operations*/
		buildEdgeTable(i, GET, listcollection, vertexlist, ymax);


		//traverse fill space
		//traverse y coordinates
		for (int ycoord = ymin; ycoord <= ymax; ycoord++){
			//update AET
			if (GET[ycoord].size() != 0){
				//add new list
				AET.insert(AET.end(), GET[ycoord].begin(), GET[ycoord].end());
				//delete invalid list
				//remove_if(AET.begin(), AET.end(), [ycoord](const lineList& edge){return (edge.Ymax == ycoord); });
				//sort according to x --ascending
				sort(AET.begin(), AET.end(), [](const lineList& edge1, const lineList& edge2){return edge1.Xmin<edge2.Xmin; });
			}
			else{
				sort(AET.begin(), AET.end(), [](const lineList& edge1, const lineList& edge2){return edge1.Xmin<edge2.Xmin; });
			}

			/*	scanline drawing		*/
			//count traversed line
			int traversedLine = 0;
			int xcoord = 0;
			//record z index by pixel
			float z_index = 0;
			//normal by pixel
			Vec3f Cur_Normal;

			//traverse all line in active edge table
			while (traversedLine<AET.size()){
				//initate xcoord and z index
				if (traversedLine == 0){
					xcoord = AET[0].Xmin;
					//assign z buffer value
					z_index = AET[0].bottomZ;
					//pixel
					PixelIntesity = AET[0].bottomIntensity;

					//update z
					if (zbuffer.getpixel(xcoord, ycoord)>z_index)
					{
						zbuffer.setpixel(xcoord, ycoord, z_index);
						//draw z for debugging
						//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
						Vec3f cur_color = color*PixelIntesity;
						glColor3fv(cur_color.getPtr());
						glVertex2i(xcoord, ycoord);
					}


					traversedLine++;
					//if on vertex
					//if (AET[0].Xmin == AET[1].Xmin){
					//	traversedLine++;
					//	//if vertex is 
					//	if (traversedLine == AET.size())
					//		break;
					//}
				}
				//draw
				if (traversedLine % 2 == 1){
					//z-buffer condition
					//increment z
					//z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
					if (zbuffer.getpixel(xcoord, ycoord)>z_index)
					{
						//update zbuffer
						zbuffer.setpixel(xcoord, ycoord, z_index);
						//draw z for debugging
						//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
						
						Vec3f pixelcolor = color*PixelIntesity;
						//gouroud color
						glColor3fv(pixelcolor.getPtr());
						glVertex2i(xcoord, ycoord);
					}

					//draw z for debugging
					//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));

					//glVertex2i(xcoord, ycoord);
				}
				//don't draw
				else{
				}

				/* check cross edge*/
				if (xcoord == floor(AET[traversedLine].Xmin)){
					//incerment traversed line counter
					traversedLine++;
					z_index = AET[traversedLine - 1].bottomZ;
					PixelIntesity = AET[traversedLine - 1].bottomIntensity;
					//ZBuffer checking
					//if (zbuffer.getpixel(xcoord, ycoord)>z_index)
					//{
					//	//update zbuffer
					//	zbuffer.setpixel(xcoord, ycoord, z_index);
					//	//draw
					//	glVertex2i(xcoord, ycoord);
					//}

					//if on lowest/highest vertex
					if (traversedLine <= (AET.size() - 1) && floorf(AET[traversedLine].Xmin) == floorf(AET[traversedLine - 1].Xmin)){
						if (zbuffer.getpixel(xcoord, ycoord)>z_index)
						{
							//update zbuffer
							zbuffer.setpixel(xcoord, ycoord, z_index);
							//draw z for debugging
							//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
							//draw
							Vec3f pixelcolor = color*PixelIntesity;
							//gouroud color
							glColor3fv(pixelcolor.getPtr());
							//draw pixel
							glVertex2i(xcoord, ycoord);
						}
						//glVertex2i(xcoord, ycoord);
						traversedLine++;
					}
				}
				//increment x
				xcoord++;
				//incrementz
				if (traversedLine < AET.size()){
					z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
					PixelIntesity += (AET[traversedLine - 1].bottomIntensity - AET[traversedLine].bottomIntensity) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
				}


			}

			/*	upate AET*/
			//update AET elements
			for (unsigned int increment = 0; increment < AET.size(); increment++){
				AET[increment].Xmin += AET[increment].slope;
				AET[increment].bottomZ += AET[increment].Zslope;
				AET[increment].bottomIntensity += AET[increment].IntensitySlop;
			}

			//delete items from AET
			//delete invalid list
			AET.erase(remove_if(AET.begin(), AET.end(), [ycoord](const lineList& edge){return (edge.Ymax <= ycoord); }), AET.end());
			/* end of update AET*/

		}
		//end of traverse y coordinate
		glEnd();
	}
}

//alternative gouroud shading
void Scan_conversion::fillPolygonGauroud(int Poly_index, Vec3f color, std::vector<Vec3f> VertexColor){
	//initate edgetables
	vector<vector<lineList>> GET;
	//initate Active edge table
	vector<lineList> AET;
	//index of polygon
	int i = Poly_index;
	//output z-buffer?
	bool PrintZBuffer = false;
		//define z bound
		float zmin, zmax;
		ZbufferBound(zmin, zmax);

		//cache intensity on pixel
		Vec3f PixelColor;
		//PixelIntensity

		//create a vector to restore vertices
		vector<Vec2i> vertexlist;
		vector<float> vertexZValue;

		vertexlist.resize(model.m_polys[i].size());
		vertexZValue.resize(model.m_polys[i].size());

		for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
			//convert point data to integer
			Vec2i point = Vec2i((model.m_2dverts[model.m_polys[i][j] - 1].x() + 1)*window_width / 2, (model.m_2dverts[model.m_polys[i][j] - 1].y() + 1)*window_height / 2);
			vertexZValue[j] = model.m_verts[model.m_polys[i][j] - 1].z();
			vertexlist[j] = point;
		}


		/*	create edge table	*/

		//create edges holder
		vector<lineList> listcollection;

		listcollection.resize(vertexlist.size());

		//fill edge holder
		//for (int edgecounter = 0; edgecounter < listcollection.size(); edgecounter++){
		//	//if first elem
		//	if (edgecounter == (listcollection.size() - 1)){
		//		if ((vertexlist[edgecounter].y() - vertexlist[0].y()) != 0){
		//			//listcollection[edgecounter] = createLinelist(vertexlist[edgecounter], vertexlist[0], vertexZValue[edgecounter], vertexZValue[0]);
		//			listcollection[edgecounter] = createLinelist(edgecounter,0,VertexIntensity);
		//		}
		//	}
		//	else {
		//		if ((vertexlist[edgecounter].y() - vertexlist[edgecounter + 1].y()) != 0){
		//			//listcollection[edgecounter] = createLinelist(vertexlist[edgecounter], vertexlist[edgecounter + 1], vertexZValue[edgecounter], vertexZValue[edgecounter + 1]);
		//			listcollection[edgecounter] = createLinelist(edgecounter, edgecounter + 1, VertexIntensity);
		//		}

		//	}

		//}

		for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
			if (j == model.m_polys[i].size() - 1){
				listcollection[j] = createLinelist(model.m_polys[i][j] - 1, model.m_polys[i][0] - 1, VertexColor);
			}
			else{
				listcollection[j] = createLinelist(model.m_polys[i][j] - 1, model.m_polys[i][j + 1] - 1, VertexColor);
			}
		}



		/* finish edge table creation*/

		//define color
		//color *= (1 - model.m_face_norms[i].z());



		/* begin drawing*/
		glPointSize(1.0f);

		/////*	drawing   polygon*///////
		glBegin(GL_POINTS);
		//define color
		glColor3f(color.r(), color.g(), color.b());
		//iniate and defien the drawing bounding
		int ymin, ymax;
		auto ymin_point = std::min_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
		ymin = ymin_point->y();
		auto ymax_point = std::max_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
		ymax = ymax_point->y();

		/*	edge table related operations*/
		buildEdgeTable(i, GET, listcollection, vertexlist, ymax);


		//traverse fill space
		//traverse y coordinates
		for (int ycoord = ymin; ycoord <= ymax; ycoord++){
			//update AET
			if (GET[ycoord].size() != 0){
				//add new list
				AET.insert(AET.end(), GET[ycoord].begin(), GET[ycoord].end());
				//delete invalid list
				//remove_if(AET.begin(), AET.end(), [ycoord](const lineList& edge){return (edge.Ymax == ycoord); });
				//sort according to x --ascending
				sort(AET.begin(), AET.end(), [](const lineList& edge1, const lineList& edge2){return edge1.Xmin<edge2.Xmin; });
			}
			else{
				sort(AET.begin(), AET.end(), [](const lineList& edge1, const lineList& edge2){return edge1.Xmin<edge2.Xmin; });
			}

			/*	scanline drawing		*/
			//count traversed line
			int traversedLine = 0;
			int xcoord = 0;
			//record z index by pixel
			float z_index = 0;
			//normal by pixel
			Vec3f Cur_Normal;

			//traverse all line in active edge table
			while (traversedLine<AET.size()){
				//initate xcoord and z index
				if (traversedLine == 0){
					xcoord = AET[0].Xmin;
					//assign z buffer value
					z_index = AET[0].bottomZ;
					//pixel
					PixelColor = AET[0].BottomColor;

					//update z
					if (zbuffer.getpixel(xcoord, ycoord)>z_index)
					{
						zbuffer.setpixel(xcoord, ycoord, z_index);
						//draw z for debugging
						//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
						glColor3fv(PixelColor.getPtr());
						glVertex2i(xcoord, ycoord);
					}


					traversedLine++;
					//if on vertex
					//if (AET[0].Xmin == AET[1].Xmin){
					//	traversedLine++;
					//	//if vertex is 
					//	if (traversedLine == AET.size())
					//		break;
					//}
				}
				//draw
				if (traversedLine % 2 == 1){
					//z-buffer condition
					//increment z
					//z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
					if (zbuffer.getpixel(xcoord, ycoord)>z_index)
					{
						//update zbuffer
						zbuffer.setpixel(xcoord, ycoord, z_index);
						//draw z for debugging
						//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));

						//gouroud color
						glColor3fv(PixelColor.getPtr());
						glVertex2i(xcoord, ycoord);
					}

					//draw z for debugging
					//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));

					//glVertex2i(xcoord, ycoord);
				}
				//don't draw
				else{
				}

				/* check cross edge*/
				if (xcoord == floor(AET[traversedLine].Xmin)){
					//incerment traversed line counter
					traversedLine++;
					z_index = AET[traversedLine - 1].bottomZ;
					PixelColor = AET[traversedLine - 1].BottomColor;
					//ZBuffer checking
					//if (zbuffer.getpixel(xcoord, ycoord)>z_index)
					//{
					//	//update zbuffer
					//	zbuffer.setpixel(xcoord, ycoord, z_index);
					//	//draw
					//	glVertex2i(xcoord, ycoord);
					//}

					//if on lowest/highest vertex
					if (traversedLine <= (AET.size() - 1) && floorf(AET[traversedLine].Xmin) == floorf(AET[traversedLine - 1].Xmin)){
						if (zbuffer.getpixel(xcoord, ycoord)>z_index)
						{
							//update zbuffer
							zbuffer.setpixel(xcoord, ycoord, z_index);
							//draw z for debugging
							//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));
							//draw
							//gouroud color
							glColor3fv(PixelColor.getPtr());
							//draw pixel
							glVertex2i(xcoord, ycoord);
						}
						//glVertex2i(xcoord, ycoord);
						traversedLine++;
					}
				}
				//increment x
				xcoord++;
				//incrementz
				if (traversedLine < AET.size()){
					z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (floor(AET[traversedLine - 1].Xmin) - floor(AET[traversedLine].Xmin));
					PixelColor += (AET[traversedLine - 1].BottomColor - AET[traversedLine].BottomColor) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
				}


			}

			/*	upate AET*/
			//update AET elements
			for (unsigned int increment = 0; increment < AET.size(); increment++){
				AET[increment].Xmin += AET[increment].slope;
				AET[increment].bottomZ += AET[increment].Zslope;
				AET[increment].BottomColor += AET[increment].ColorSlope;
			}

			//delete items from AET
			//delete invalid list
			AET.erase(remove_if(AET.begin(), AET.end(), [ycoord](const lineList& edge){return (edge.Ymax <= ycoord); }), AET.end());
			/* end of update AET*/

		}
		//end of traverse y coordinate
		glEnd();
}

//phong shading
void Scan_conversion::fillPolygonPhong(int Poly_index, Vec3f color, vector<Vec3f> word_vert){
	//////////////////////////////
	//initate edgetables
	vector<vector<lineList_Phong>> GET;
	//initate Active edge table
	vector<lineList_Phong> AET;
	//index of polygon
	int i = Poly_index;
	//output z-buffer?
	bool PrintZBuffer = false;
	///////////////////////////////

	//define z bound
	float zmin, zmax;
	ZbufferBound(zmin, zmax);

	//create a vector to restore vertices
	vector<Vec2i> vertexlist;
	vector<float> vertexZValue;

	vertexlist.resize(model.m_polys[i].size());
	vertexZValue.resize(model.m_polys[i].size());

	for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
		//convert point data to integer
		Vec2i point = Vec2i((model.m_2dverts[model.m_polys[i][j] - 1].x() + 1)*window_width / 2, (model.m_2dverts[model.m_polys[i][j] - 1].y() + 1)*window_height / 2);
		vertexZValue[j] = model.m_verts[model.m_polys[i][j] - 1].z();
		vertexlist[j] = point;
	}


	/*	create edge table	*/

	//create edges holder
	vector<lineList_Phong> listcollection;

	listcollection.resize(vertexlist.size());

	//fill edge holder
	for (unsigned int j = 0; j < model.m_polys[i].size(); j++){
		if (j == model.m_polys[i].size() - 1){
			listcollection[j] = createPhongLinelist(model.m_polys[i][j] - 1, model.m_polys[i][0] - 1, word_vert);
		}
		else{
			listcollection[j] = createPhongLinelist(model.m_polys[i][j] - 1, model.m_polys[i][j + 1] - 1, word_vert);
		}
	}



	/* finish edge table creation*/

	//define color
	//color *= (1-model.m_face_norms[i].z());



	/* begin drawing*/
	glPointSize(1.0f);

	/////*	drawing   polygon*///////
	glBegin(GL_POINTS);
	//define color
	//glColor3f(color.r(), color.g(), color.b());
	//iniate and defien the drawing bounding
	int ymin, ymax;
	auto ymin_point = std::min_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
	ymin = ymin_point->y();
	auto ymax_point = std::max_element(vertexlist.begin(), vertexlist.end(), [](const Vec2i& point1, const Vec2i& point2){return (point1.y() < point2.y()); });
	ymax = ymax_point->y();

	/*	edge table related operations*/
	buildEdgeTable(i, GET, listcollection, vertexlist, ymax);

	//cache pixel color
	Vec3f Pixelcolor;

	//traverse fill space
	//traverse y coordinates
	for (int ycoord = ymin; ycoord <= ymax; ycoord++){
		//update AET
		if (GET[ycoord].size() != 0){
			//add new list
			AET.insert(AET.end(), GET[ycoord].begin(), GET[ycoord].end());
			//delete invalid list
			//remove_if(AET.begin(), AET.end(), [ycoord](const lineList& edge){return (edge.Ymax == ycoord); });
			//sort according to x --ascending
			sort(AET.begin(), AET.end(), [](const lineList_Phong& edge1, const lineList_Phong& edge2){return edge1.Xmin < edge2.Xmin; });
		}
		else{
			sort(AET.begin(), AET.end(), [](const lineList_Phong& edge1, const lineList_Phong& edge2){return edge1.Xmin < edge2.Xmin; });
		}

		/*	scanline drawing		*/
		//count traversed line
		int traversedLine = 0;
		int xcoord = 0;
		//record z index by pixel
		float z_index = 0;
		//normal by pixel
		Vec3f Cur_Normal;
		//position by pixel
		Vec3f Cur_Pos;
		//pixel Intensity
		float pixelIntensity;

		//traverse all line in active edge table
		while (traversedLine < AET.size()){
			//initate xcoord and z index
			if (traversedLine == 0){
				xcoord = AET[0].Xmin;
				//assign z buffer value
				z_index = AET[0].bottomZ;
				//phong 
				Cur_Normal = AET[traversedLine].BottomNormal;
				Cur_Pos = AET[traversedLine].BottomPos;

				//update z
				if (zbuffer.getpixel(xcoord, ycoord) > z_index)
				{
					zbuffer.setpixel(xcoord, ycoord, z_index);
					//
					//define color
					if (usemap)
					{
						//Vec3f interpolate = AET[traversedLine].positionSlope*AET[traversedLine].slope + (AET[traversedLine - 1].BottomPos - AET[traversedLine].BottomPos) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
						Vec3f map_color = uvmap.getColor(Cur_Pos);
						//Pixelcolor = map_color;
						Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()*map_color) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, map_color) + CalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getGlossy(), model.getks());
					}
					else if (model.getMaterialIllumination() == PHONG){
						Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, model.getkd()) + CalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getGlossy(), model.getks());
					}
					else if (model.getMaterialIllumination() == WARD){
						Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, model.getkd()) + WardCalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getSpecular_level(), model.getks(), model.getRoughness());
					}
					
					//Pixelcolor = CalculateIllumination(Lightlist[0], Cur_Normal, Cur_Pos, model);
					glColor3fv(Pixelcolor.getPtr());
					glVertex2i(xcoord, ycoord);
				}
				traversedLine++;
				//if on vertex
				if (AET[0].Xmin == AET[1].Xmin){
					traversedLine++;
					//if vertex is 
					if (traversedLine == AET.size())
						break;
				}
			}
			//draw
			if (traversedLine % 2 == 1){
				//z-buffer condition
				//increment z
				//z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
				if (zbuffer.getpixel(xcoord, ycoord) > z_index)
				{
					zbuffer.setpixel(xcoord, ycoord, z_index);
					//
					if (usemap)
					{
						//Vec3f interpolate = AET[traversedLine].positionSlope*AET[traversedLine].slope + (AET[traversedLine - 1].BottomPos - AET[traversedLine].BottomPos) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
						Vec3f map_color = uvmap.getColor(Cur_Pos);
						//Pixelcolor = map_color;
						Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()*map_color) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, map_color) + CalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getGlossy(), model.getks());
					}
					else if (model.getMaterialIllumination() == PHONG){
						Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, model.getkd()) + CalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getGlossy(), model.getks());
					}
					else if (model.getMaterialIllumination() == WARD){
						Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, model.getkd()) + WardCalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getSpecular_level(), model.getks(), model.getRoughness());
					}
					//Pixelcolor = CalculateIllumination(Lightlist[0], Cur_Normal, Cur_Pos, model);
					glColor3fv(Pixelcolor.getPtr());
					glVertex2i(xcoord, ycoord);
				}

				//draw z for debugging
				//glColor3f(scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax), scaleZValue(zbuffer.getpixel(xcoord, ycoord), zmin, zmax));

				//glVertex2i(xcoord, ycoord);
			}
			//don't draw
			/*else{
			}*/

			/* check cross edge*/
			if (xcoord == floor(AET[traversedLine].Xmin)){
				//incerment traversed line counter
				traversedLine++;
				z_index = AET[traversedLine - 1].bottomZ;
				Cur_Normal = AET[traversedLine - 1].BottomNormal;
				Cur_Pos = AET[traversedLine-1].BottomPos;
				//ZBuffer checking
				//if (zbuffer.getpixel(xcoord, ycoord)>z_index)
				//{
				//	//update zbuffer
				//	zbuffer.setpixel(xcoord, ycoord, z_index);
				//	//draw
				//	glVertex2i(xcoord, ycoord);
				//}

				//if on lowest/highest vertex
				if (traversedLine <= (AET.size() - 1) && floorf(AET[traversedLine].Xmin) == floorf(AET[traversedLine - 1].Xmin)){
					if (zbuffer.getpixel(xcoord, ycoord) > z_index)
					{
						//update zbuffer
						zbuffer.setpixel(xcoord, ycoord, z_index);
						//draw
						//phong color
						if (usemap)
						{
							//Vec3f interpolate = AET[traversedLine].positionSlope*AET[traversedLine].slope + (AET[traversedLine - 1].BottomPos - AET[traversedLine].BottomPos) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
							Vec3f map_color = uvmap.getColor(Cur_Pos);
							//Pixelcolor = map_color;
							Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()*map_color) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, map_color) + CalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getGlossy(), model.getks());
						}
						else if (model.getMaterialIllumination() == PHONG){
							Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, model.getkd()) + CalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getGlossy(), model.getks());
						}
						else if (model.getMaterialIllumination() == WARD){
							Pixelcolor = CalculateAmbientIntensity(Lightlist[0].getColor(), model.getka()) + CalculateDiffuseIntensity(Lightlist[0], Cur_Normal, Cur_Pos, model.getkd()) + WardCalculateSpecularIntensity(Lightlist[0], Cur_Normal, Cur_Pos, cur_camera.getPosition(), model.getSpecular_level(), model.getks(), model.getRoughness());
						}
						//Pixelcolor = CalculateIllumination(Lightlist[0], Cur_Normal, Cur_Pos, model);
						glColor3fv(Pixelcolor.getPtr());
						//draw pixel
						glVertex2i(xcoord, ycoord);
					}
					//glVertex2i(xcoord, ycoord);
					traversedLine++;
				}
			}
			//increment x
			xcoord++;
			//incrementz
			if (traversedLine < AET.size()){
				z_index += (AET[traversedLine - 1].bottomZ - AET[traversedLine].bottomZ) / (floor(AET[traversedLine - 1].Xmin) - floor(AET[traversedLine].Xmin));
				Cur_Normal += (AET[traversedLine - 1].BottomNormal - AET[traversedLine].BottomNormal) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
				Cur_Pos += (AET[traversedLine - 1].BottomPos - AET[traversedLine].BottomPos) / (AET[traversedLine - 1].Xmin - AET[traversedLine].Xmin);
			}


		}

		/*	upate AET*/
		//update AET elements
		for (unsigned int increment = 0; increment < AET.size(); increment++){
			AET[increment].Xmin += AET[increment].slope;
			AET[increment].bottomZ += AET[increment].Zslope;
			AET[increment].BottomNormal += AET[increment].NormalSlope;
			AET[increment].BottomPos += AET[increment].positionSlope;
		}

		//delete items from AET
		//delete invalid list
		AET.erase(remove_if(AET.begin(), AET.end(), [ycoord](const lineList_Phong& edge){return (edge.Ymax <= ycoord); }), AET.end());
		/* end of update AET*/

	}
	//end of traverse y coordinate
	glEnd();
//end of phong shading
}

//create linelists
Scan_conversion::lineList Scan_conversion::createLinelist(Vec2i point1, Vec2i point2, float point1z, float point2z){
	lineList list;
	list.Ymax = std::max(point1.y(), point2.y());

	//assign xmin
	if (point1.y() == list.Ymax){
		list.Xmin = point2.x();
		list.bottomZ = point2z;
		//phong
	}
	else{
		list.Xmin = point1.x();
		list.bottomZ = point1z;
	}
	list.slope = (float)(point1.x() - point2.x())/(point1.y() - point2.y());
	list.Zslope = (float)(point1z - point2z) / (point1.y() - point2.y());
	
	//reassuring the z slope
	return list;
}

Scan_conversion::lineList Scan_conversion::createLinelist(int point1Index, int point2Index){
	//create new list instance
	lineList list;
	int y1 = (model.m_2dverts[point1Index].y() + 1)*window_height / 2;
	int y2 = (model.m_2dverts[point2Index].y() + 1)*window_height / 2;
	int x1 = (model.m_2dverts[point1Index].x() + 1)*window_width / 2;
	int x2 = (model.m_2dverts[point2Index].x() + 1)*window_width / 2;
	//list.Ymax = std::max((model.m_2dverts[point1Index].y() + 1)*window_height / 2, (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	list.Ymax = std::max(y1, y2);

	//assign xmin
	if (/*(int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2) == list.Ymax*/y1==list.Ymax){    //point 1 is higher(has greater y value)
		list.Xmin = x2;
		list.bottomZ = model.m_verts[point2Index].z();
		//guoraud
		//list.bottomIntensity = Intensity[point2Index];
		
		//phong

	}
	else{   //point 2 is higher
		list.Xmin = x1;
		list.bottomZ = model.m_verts[point1Index].z();
		//guoraud
		//list.bottomIntensity = Intensity[point1Index];
		//phong

	}
	list.slope = ((float)(x1 - x2) / (float)(y1 - y2));
	list.Zslope = (float)(model.m_verts[point1Index].z() - model.m_verts[point2Index].z()) / (y1 - y2);
	//gouruad related
	//list.IntensitySlop = (Intensity[point2Index] - Intensity[point1Index]) / (y2 - y1);
	//phong related
	//list.NormalSlope = (model.m_face_norms[point1Index] - model.m_face_norms[point2Index]) / (int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2 - (model.m_2dverts[point2Index].y() + 1)*window_height / 2);

	//reassuring the z slope
	return list;
}


//color gauroud shading
Scan_conversion::lineList Scan_conversion::createLinelist(int point1Index, int point2Index, vector<Vec3f> VertexColor){
	//create new list instance
	lineList list;
	int y1 = (model.m_2dverts[point1Index].y() + 1)*window_height / 2;
	int y2 = (model.m_2dverts[point2Index].y() + 1)*window_height / 2;
	int x1 = (model.m_2dverts[point1Index].x() + 1)*window_width / 2;
	int x2 = (model.m_2dverts[point2Index].x() + 1)*window_width / 2;
	//list.Ymax = std::max((model.m_2dverts[point1Index].y() + 1)*window_height / 2, (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	list.Ymax = std::max(y1, y2);

	//assign xmin
	if (/*(int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2) == list.Ymax*/y1 == list.Ymax){    //point 1 is higher(has greater y value)
		list.Xmin = x2;
		list.bottomZ = model.m_verts[point2Index].z();
		//guoraud
		//list.bottomIntensity = Intensity[point2Index];

		//phong
		list.BottomColor = VertexColor[point2Index];
	}
	else{   //point 2 is higher
		list.Xmin = x1;
		list.bottomZ = model.m_verts[point1Index].z();
		//guoraud
		//list.bottomIntensity = Intensity[point1Index];
		//phong
		list.BottomColor = VertexColor[point1Index];
	}
	list.slope = ((float)(x1 - x2) / (float)(y1 - y2));
	list.Zslope = (float)(model.m_verts[point1Index].z() - model.m_verts[point2Index].z()) / (y1 - y2);
	//gouruad related
	//list.IntensitySlop = (Intensity[point2Index] - Intensity[point1Index]) / (y2 - y1);
	//phong related
	//list.NormalSlope = (model.m_face_norms[point1Index] - model.m_face_norms[point2Index]) / (int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2 - (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	list.ColorSlope = (VertexColor[point1Index] - VertexColor[point2Index]) / (y1 - y2);
	//reassuring the z slope
	return list;
}


Scan_conversion::lineList Scan_conversion::createLinelist(int point1Index, int point2Index, vector<float> VertexIntensity){
	//create new list instance
	lineList list;
	int y1 = (model.m_2dverts[point1Index].y() + 1)*window_height / 2;
	int y2 = (model.m_2dverts[point2Index].y() + 1)*window_height / 2;
	int x1 = (model.m_2dverts[point1Index].x() + 1)*window_width / 2;
	int x2 = (model.m_2dverts[point2Index].x() + 1)*window_width / 2;
	//list.Ymax = std::max((model.m_2dverts[point1Index].y() + 1)*window_height / 2, (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	list.Ymax = std::max(y1, y2);

	//assign xmin
	if (/*(int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2) == list.Ymax*/y1 == list.Ymax){    //point 1 is higher(has greater y value)
		list.Xmin = x2;
		list.bottomZ = model.m_verts[point2Index].z();
		//guoraud
		list.bottomIntensity = VertexIntensity[point2Index];

		//phong
		//list.BottomNormal = model.m_face_norms[point2Index];
	}
	else{   //point 2 is higher
		list.Xmin = x1;
		list.bottomZ = model.m_verts[point1Index].z();
		//guoraud
		list.bottomIntensity = VertexIntensity[point1Index];
		//phong
		//list.BottomNormal = model.m_face_norms[point1Index];
	}
	list.slope = ((float)(x1 - x2) / (float)(y1 - y2));
	list.Zslope = (float)(model.m_verts[point1Index].z() - model.m_verts[point2Index].z()) / (y1 - y2);
	//gouruad related
	list.IntensitySlop = (VertexIntensity[point2Index] - VertexIntensity[point1Index]) / (y2 - y1);
	//phong related
	//list.NormalSlope = (model.m_face_norms[point1Index] - model.m_face_norms[point2Index]) / (int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2 - (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	//list.NormalSlope = (model.m_face_norms[point1Index] - model.m_face_norms[point2Index]) / (y1 - y2);
	//reassuring the z slope
	return list;
}

Scan_conversion::lineList_Phong Scan_conversion::createPhongLinelist(int point1Index, int point2Index, vector<Vec3f>vert_position){
	//create new list instance
	lineList_Phong list;
	int y1 = (model.m_2dverts[point1Index].y() + 1)*window_height / 2;
	int y2 = (model.m_2dverts[point2Index].y() + 1)*window_height / 2;
	int x1 = (model.m_2dverts[point1Index].x() + 1)*window_width / 2;
	int x2 = (model.m_2dverts[point2Index].x() + 1)*window_width / 2;
	//list.Ymax = std::max((model.m_2dverts[point1Index].y() + 1)*window_height / 2, (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	list.Ymax = std::max(y1, y2);

	//assign xmin
	if (/*(int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2) == list.Ymax*/y1 == list.Ymax){    //point 1 is higher(has greater y value)
		list.Xmin = x2;
		list.bottomZ = model.m_verts[point2Index].z();

		//phong
		list.BottomNormal = model.m_vert_norms[point2Index];
		list.BottomPos = vert_position[point2Index];
	}
	else{   //point 2 is higher
		list.Xmin = x1;
		list.bottomZ = model.m_verts[point1Index].z();
		//phong
		list.BottomNormal = model.m_vert_norms[point1Index];
		list.BottomPos = vert_position[point1Index];
	}
	list.slope = ((float)(x1 - x2) / (float)(y1 - y2));
	list.Zslope = (float)(model.m_verts[point1Index].z() - model.m_verts[point2Index].z()) / (y1 - y2);
	//gouruad related
	//list.IntensitySlop = (Intensity[point2Index] - Intensity[point1Index]) / (y2 - y1);
	//phong related
	//list.NormalSlope = (model.m_face_norms[point1Index] - model.m_face_norms[point2Index]) / (int)((model.m_2dverts[point1Index].y() + 1)*window_height / 2 - (model.m_2dverts[point2Index].y() + 1)*window_height / 2);
	list.NormalSlope = (model.m_vert_norms[point1Index] - model.m_vert_norms[point2Index]) / (y1 - y2);
	//reassuring the z slope


	//postion slope
	list.positionSlope = (vert_position[point1Index] - vert_position[point2Index])/ (y1 - y2);

	return list;
}


//build the global edge table
void Scan_conversion::buildEdgeTable(int Poly_index, vector<vector<lineList>> &GET, vector<lineList> linecollection, vector<Vec2i> VertexList, int ymax){
    //resize global edge list
	GET.resize(window_height);

	//record y coordinate 
	int ycoord;
	//traverse vertex list
	for (unsigned int j=0; j < VertexList.size(); j++){
		//if vextex in y coording
		ycoord = VertexList[j].y();
			//temporay vector
			vector<lineList> temp;

			//check if point is at the head of list
			if (j==0){
				if (linecollection[0].Ymax != ycoord && linecollection[0].Ymax>=0){   //check if line is added
					lineList newlist = shortenedge(linecollection[0], ymax);
					temp.push_back(newlist);
				}
				if (linecollection[VertexList.size() - 1].Ymax != ycoord && linecollection[VertexList.size() - 1].Ymax >= 0){ //check if line is added
					lineList newlist = shortenedge(linecollection[VertexList.size() - 1], ymax);
					temp.push_back(newlist);
				}	
			}
			//j is not at the head of list
			else{
				// point is not at head
				if (linecollection[j].Ymax != ycoord && linecollection[j].Ymax >= 0){   //check if line is added
					lineList newlist=shortenedge(linecollection[j], ymax);
					temp.push_back(newlist);
				}

				if (linecollection[j - 1].Ymax != ycoord && linecollection[j - 1].Ymax >= 0){   //check if line is added
					lineList newlist=shortenedge(linecollection[j - 1], ymax);
					temp.push_back(newlist);
				}
			}


			//add to certain point
			if (GET[ycoord].size() == 0){//if not list attached
				//set i pos to temp
				GET[ycoord] = temp;
			}
			else{//if not empty
				//insert at the back
				GET[ycoord].insert(GET[ycoord].end(), temp.begin(), temp.end());
			}
		}//end point traverse

}
void Scan_conversion::buildEdgeTable(int Poly_index, vector<vector<lineList_Phong>> &GET, vector<lineList_Phong> linecollection, vector<Vec2i> VertexList, int ymax){
	//resize global edge list
	GET.resize(window_height);

	//record y coordinate 
	int ycoord;
	//traverse vertex list
	for (unsigned int j = 0; j < VertexList.size(); j++){
		//if vextex in y coording
		ycoord = VertexList[j].y();
		//temporay vector
		vector<lineList_Phong> temp;

		//check if point is at the head of list
		if (j == 0){
			if (linecollection[0].Ymax != ycoord && linecollection[0].Ymax >= 0){   //check if line is added
				lineList_Phong newlist = shortenedge(linecollection[0], ymax);
				temp.push_back(newlist);
			}
			if (linecollection[VertexList.size() - 1].Ymax != ycoord && linecollection[VertexList.size() - 1].Ymax >= 0){ //check if line is added
				lineList_Phong newlist = shortenedge(linecollection[VertexList.size() - 1], ymax);
				temp.push_back(newlist);
			}
		}
		//j is not at the head of list
		else{
			// point is not at head
			if (linecollection[j].Ymax != ycoord && linecollection[j].Ymax >= 0){   //check if line is added
				lineList_Phong newlist = shortenedge(linecollection[j], ymax);
				temp.push_back(newlist);
			}

			if (linecollection[j - 1].Ymax != ycoord && linecollection[j - 1].Ymax >= 0){   //check if line is added
				lineList_Phong newlist = shortenedge(linecollection[j - 1], ymax);
				temp.push_back(newlist);
			}
		}


		//add to certain point
		if (GET[ycoord].size() == 0){//if not list attached
			//set i pos to temp
			GET[ycoord] = temp;
		}
		else{//if not empty
			//insert at the back
			GET[ycoord].insert(GET[ycoord].end(), temp.begin(), temp.end());
		}
	}//end point traverse
}

//shorten edge
Scan_conversion::lineList Scan_conversion::shortenedge(lineList &line, int &ymax){
	lineList newlist = line;
	if (line.Ymax != ymax){
		newlist.Ymax--;
	}
	return newlist;
}

Scan_conversion::lineList_Phong Scan_conversion::shortenedge(lineList_Phong &line, int &ymax){
	lineList_Phong newlist = line;
	if (line.Ymax != ymax){
		newlist.Ymax--;
	}
	return newlist;
}

//get boundery of z value
void Scan_conversion::ZbufferBound(float &zmin, float &zmax){
	auto result = std::minmax_element(model.m_verts.begin(), model.m_verts.end(), [](const Vec3f &point1, const Vec3f &point2){return (point1.y() < point2.y()); });
	zmin = result.first->z();
	zmax = result.second->z();
}

//get relative z value ---for 8bit z-buffer dispaly
float Scan_conversion::scaleZValue(const float &cur_Z, const float &zmin, const float &zmax){
	if (cur_Z == std::numeric_limits<float>::max())
		return 0;
	else{
		return 1-((cur_Z - zmin) / (zmax - zmin));
	}
}

void Scan_conversion::updateZbuffer(Z_Buffer &z_buffer){
	z_buffer = zbuffer;
}

Scan_conversion::~Scan_conversion()
{
}


//////////////illumination functions  /////////////////////////////

//float Scan_conversion::CalculateAmbientIntensity(Vec3f light_color, float ka){
//	return light_color*ka;
//}

Vec3f Scan_conversion::CalculateAmbientIntensity(Vec3f light_color, Vec3f ka){
	return light_color*ka;
}


float Scan_conversion::CalculateDiffuseIntensity(Light light, Vec3f normal, Vec3f Position,float kd){
	float intensity;
	//vector to light
	Vec3f lightVec = light.getPosition()-Position;

	//get light intensity
	float lightIntensity=light.getIntensity();
	if (light.getFallOffType() == NONE){  //no falloff
		lightIntensity = light.getIntensity();
	}
	else if (light.getFallOffType() == INVERSE){  //linear falloff
		lightIntensity = light.getIntensity() / lightVec.length();
	}
	else {   //inverse square falloff
		lightIntensity = light.getIntensity() / lightVec.lengthsq();
	}
	lightVec.normalize();

	intensity = lightIntensity*normal.dot(lightVec)*kd;

	return std::max(intensity,0.0f);
}

//calculate specular
float Scan_conversion::CalculateSpecularIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f cameraPosition, int Glossy, float ks){
	float intensity;
	//
	Vec3f lightVec = light.getPosition() - Position;
	//reflected ray
	Vec3f ReflecVec = normal*2 * normal.dot(lightVec) - lightVec;
	//vector to camera
	Vec3f ViewingVec = cameraPosition - Position;

	//get light intensity
	float lightIntensity = light.getIntensity();
	if (light.getFallOffType() == NONE){  //no falloff
		//lightIntensity = light.getIntensity();
	}
	else if (light.getFallOffType() == INVERSE){  //linear falloff
		lightIntensity /=lightVec.length();
	}
	else {   //inverse square falloff
		lightIntensity /= lightVec.lengthsq();
	}

	//ideal direction
	ReflecVec.normalize();
	ViewingVec.normalize();

	
	intensity = lightIntensity*pow(ViewingVec.dot(ReflecVec), Glossy)*ks;
	//check light direction
	if (normal.dot(lightVec) <= 0){
		intensity = 0.0f;
	}

	return std::max(intensity, 0.0f);
}

Vec3f Scan_conversion::CalculateDiffuseIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f kd){
	float intensity;
	//vector to light
	Vec3f lightVec = light.getPosition() - Position;

	//get light intensity
	float lightIntensity = light.getIntensity();
	if (light.getFallOffType() == NONE){  //no falloff
		lightIntensity = light.getIntensity();
	}
	else if (light.getFallOffType() == INVERSE){  //linear falloff
		lightIntensity = light.getIntensity() / lightVec.length();
	}
	else {   //inverse square falloff
		lightIntensity = light.getIntensity() / lightVec.lengthsq();
	}
	lightVec.normalize();

	intensity = lightIntensity*normal.dot(lightVec);

	intensity = std::max(intensity, 0.0f);
	return light.getColor()*kd*intensity;
}
Vec3f Scan_conversion::CalculateSpecularIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f cameraPosition, int Glossy, Vec3f ks){
	float intensity;
	//
	Vec3f lightVec = light.getPosition() - Position;
	//reflected ray
	Vec3f ReflecVec = normal * 2 * normal.dot(lightVec) - lightVec;
	//vector to camera
	Vec3f ViewingVec = cameraPosition - Position;

	///////// Blinn//////////////

	/*Vec3f HalfVec = lightVec + ViewingVec;
	HalfVec.normalize();*/
	///// end of Blinn /////////////////

	//get light intensity
	float lightIntensity = light.getIntensity();
	if (light.getFallOffType() == NONE){  //no falloff
		//lightIntensity = light.getIntensity();
	}
	else if (light.getFallOffType() == INVERSE){  //linear falloff
		lightIntensity /= lightVec.length();
	}
	else {   //inverse square falloff
		lightIntensity /= lightVec.lengthsq();
	}

	//ideal direction
	ReflecVec.normalize();
	ViewingVec.normalize();
	lightVec.normalize();

	Vec3f Spec_color = light.getColor()*lightIntensity*std::max(pow(ViewingVec.dot(ReflecVec), Glossy), 0.0f)*ks /*/ (normal.dot(lightVec))*/;
	if (normal.dot(lightVec) <= 0){
		Spec_color = Vec3f(0.0f,0.0f,0.0f);
	}


	return Spec_color;
}

//Ward BRDF specular
Vec3f Scan_conversion::WardCalculateSpecularIntensity(Light light, Vec3f normal, Vec3f Position, Vec3f cameraPosition, int Glossy, Vec3f ks, Vec2f Roughness){
	//float intensity;
	//
	Vec3f lightVec = light.getPosition() - Position;
	//reflected ray
	Vec3f ReflecVec = normal * 2 * normal.dot(lightVec) - lightVec;
	//vector to camera
	Vec3f ViewingVec = cameraPosition - Position;

	normal.normalize();
	ReflecVec.normalize();
	ViewingVec.normalize();
	lightVec.normalize();

	//half way
	Vec3f HalfWayVec = lightVec + ViewingVec;
	HalfWayVec.normalize();

	Roughness += Vec2f(1.5e-6,1.5e-6);

	// Define the coordinate frame
	Vec3f epsilon = Vec3f(0.0f, 0.8f, 0.6f);
	Vec3f tangent = normal.cross(epsilon);
	tangent.normalize();
	Vec3f bitangent = normal.cross(tangent);
	bitangent.normalize();

	// Evaluate the specular exponent
	float beta_a = HalfWayVec.dot(tangent) / Roughness.x();
	beta_a *= beta_a;

	float beta_b = HalfWayVec.dot(bitangent) / Roughness.y();
	beta_b *= beta_b;

	float beta = -2.0f * ((beta_a + beta_b) / (1.0f + HalfWayVec.dot(normal)));
	// Evaluate the specular denominator
	float s_den = 3.14159f;
	s_den *= Roughness.x();
		s_den *= Roughness.y();
		s_den *= sqrt(std::abs((lightVec.dot(normal) * ViewingVec.dot(normal))));
		if ((lightVec.dot(normal) * ViewingVec.dot(normal))<0 /*&& lightVec.dot(normal)<0*/ /*&& ReflecVec.dot(normal)<0*/){
			//s_den = 0 - s_den;
		}
	/*s_den = std::max(s_den, 0.0f);*/
	float SI = (exp(beta) / s_den);

	Vec3f Specular_color = light.getColor()*SI*ks*std::max(lightVec.dot(normal),0.0f);
	/*if (normal.dot(lightVec) <= 0){
		Specular_color = Vec3f(0.0f, 0.0f, 0.0f);
	}*/
	return Specular_color;
}

Vec3f Scan_conversion::CalculateIllumination(Light light, Vec3f normal, Vec3f Position, PolyModel model){
	Vec3f color(0.0, 0.0, 0.0);
	if (model.getMaterialIllumination() == PHONG){
		color = CalculateAmbientIntensity(light.getColor(), model.getka()) + CalculateDiffuseIntensity(light, normal, Position, model.getkd()) + CalculateSpecularIntensity(light, normal, Position, cur_camera.getPosition(), model.getSpecular_level(), model.getks());
	}
	else if (model.getMaterialIllumination() == WARD){
		color = CalculateAmbientIntensity(light.getColor(), model.getka()) + CalculateDiffuseIntensity(light, normal, Position, model.getkd()) + WardCalculateSpecularIntensity(light, normal, Position, cur_camera.getPosition(), model.getSpecular_level(), model.getks(), model.getRoughness());
	}

	//color = CalculateAmbientIntensity(light.getColor(), model.getka(), model.getAmbientColor()) + CalculateDiffuseIntensity(light, normal, Position, model.getkd(), model.getDiffuseColor()) + CalculateSpecularIntensity(light, normal, Position, cur_camera.getPosition(), model.getSpecular_level(), model.getks(), model.getSpecularColor());

	return color;

}

//////////////////////////////////////////////////////////////////////////
//void Scan_conversion::InitateUVMap(void &texture){
//	uvmap.LoadMap(texture);
//	uvmap.createSummedMap();
//}

void Scan_conversion::setMappingMethod(float *attributeList, int MappingType){}
