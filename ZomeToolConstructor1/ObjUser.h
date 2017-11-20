#ifndef OBJ_USER
#define OBJ_USER


#include "stdafx.h"
#include <map>
#include "ZomeDirection.h"
//#include "MathHelper.h"
//#include <GL/freeglut.h>
//#include <fstream>
//#include <stdio.h>



class ObjImporter
{
public:
	void GetSize(char* path, GLuint* VerticeSize, GLuint* NormalSize, GLuint* FaceSize);
	void Import(char* path, GLfloat* Vertices, GLuint* VerticeSize, GLfloat* Normals, GLuint* NormalSize, GLuint* Faces, GLuint* FaceSize);
};

class ObjExporter
{
public:
	void Export(GLfloat Vertices[], GLuint VerticeSize, GLfloat Normals[], GLuint NormalsSize, GLuint Faces[], GLuint FacesSize , char* filePath );
	void Export(GLfloat Vertices[], 
				GLuint VerticeSize, 
				GLfloat Normals[], 
				GLuint NormalsSize, 
				GLuint Faces[], 
				GLuint FacesSize , 
				char* filePath ,
				std::map<int,int>* material );//�i�H�ѥX�C��
	void Export(GLfloat Vertices[], 
				GLuint VerticeSize, 
				GLfloat Normals[], 
				GLuint NormalsSize, 
				GLuint Faces[], 
				GLuint FacesSize , 
				char* filePath ,
				std::map<int,float>* matForce );//�i�H�ѥX�C�ڱ��Ҩ����O
	void Export_Old(GLfloat Vertices[], GLuint VerticeSize, GLfloat Normals[], GLuint NormalsSize, GLuint Faces[], GLuint FacesSize);
};




#endif