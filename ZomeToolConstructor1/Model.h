#ifndef MODEL
#define MODEL

#ifndef BUFFER_OFFSET(i)
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#endif


#include "ObjUser.h"
#include "stdafx.h"
//#include "MathHelper.h"
//#include <GL/freeglut.h>
//#include <fstream>
//#include <stdio.h>

class Model
{
public:
	Model();
	Model( int vSize , int nSize , int fSize );
	Model( char* path );

	GLfloat** vertices;
	GLfloat** normals;
	GLuint** indices;

	GLuint verticeSize, normalSize, indiceSize;
};


class ModelObject
{
public:
	ModelObject(){};
	ModelObject(Model* m, GLfloat* sca , GLfloat* tra, GLfloat* rot , GLuint verOffs , GLuint indOffs );

	GLuint GetNextVertexOffset();
	GLuint GetNextIndexOffset();

	Model* model;
	GLfloat* scale;
	GLfloat* translate;
	GLfloat* rotate;

	//socket用，拉伸socket 還有分割用
	GLfloat elongX;
	GLfloat elongY;
	GLfloat elongZ;

	GLfloat* scaleMatrix;
	GLfloat* translateMatrix;
	GLfloat* rotateThetaMatrix;
	GLfloat* rotatePhiMatrix;
	GLfloat* rotateYawMatrix;
	GLfloat** vertices;
	GLfloat** normals;
	GLuint** indices;

	

	GLuint verticeSize, normalSize, indiceSize;

	GLuint vertexOffset;
	GLuint indexOffset;

	ModelObject* link;
	ModelObject* prev;
};

#endif