#ifndef MATH_HELPER
#define MATH_HELPER

#include "stdafx.h"
//#include <GL/freeglut.h>
//#include <math.h>
//#include <stdio.h>

class MathHelper
{
public:

	static void matrixMult4x4(GLfloat* result, GLfloat* ml, GLfloat* m2);
	static void matrixMult4x4Column(GLfloat* result, GLfloat* ml, GLfloat* m2);

	static void makeIdentity(GLfloat* result);

	static void makeTranslate(GLfloat* result, GLfloat x, GLfloat y, GLfloat z);

	static void translateMatrixBy(GLfloat* result, GLfloat x, GLfloat y, GLfloat z);

	static void makeRotateX(GLfloat* result, GLfloat rotation);
	static void makeRotateY(GLfloat* result, GLfloat rotation);
	static void makeRotateZ(GLfloat* result, GLfloat rotation);

	static void makeQuaternion( GLfloat* result , GLfloat* axis , GLfloat theta );

	static void makeScale(GLfloat* result, GLfloat x, GLfloat y, GLfloat z);
	static void makePerspectiveMatrix(GLfloat* result, GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);
	static void makePlane( GLfloat* result , GLfloat* vec1 , GLfloat* vec2 , GLfloat* vec3 );

	static void copyMatrix(GLfloat* src, GLfloat* dest);
	static void CleanMatrixSmall( GLfloat* result );
	static void print4x4Matrix(GLfloat* mat);

	static void vectorMultMatrix(GLfloat* result, GLfloat* vec, GLfloat* mat);
	static void vectorMultMatrix(GLfloat* result , GLfloat* mat);
	static void vectorNormalize(GLfloat* v);

	static void SetVector( GLfloat* vec , GLfloat x , GLfloat y , GLfloat z );
	static void SetVector( GLfloat* result , GLfloat* initPosition , GLfloat* endPosition );
	static void VectorPlus( GLfloat* result , GLfloat* plused , GLfloat* pluser );
	static void VectorPlus( GLfloat* plused , GLfloat* pluser );
	static void VectorPlus( GLfloat* plused , GLfloat x , GLfloat y , GLfloat z );
	static void VectorMinus( GLfloat* result , GLfloat* minused , GLfloat* minuser );
	static void VectorMinus( GLfloat* minused , GLfloat* minuser );
	static void VectorMutiply( GLfloat* result , GLfloat* multiplied , GLfloat multiplier );
	static void VectorMutiply( GLfloat* multiplied , GLfloat multiplier );
	static void VectorDot( GLfloat* result , GLfloat* vec1 , GLfloat* vec2 );
	static GLfloat VectorDot( GLfloat* vec1 , GLfloat* vec2 );
	static void VectorCorss( GLfloat* result , GLfloat* vec1 , GLfloat* vec2 );

	static void VectorReverse( GLfloat* result );
	static void VectorNormalize( GLfloat* result );

	static GLfloat VectorDistance( GLfloat* vec1 , GLfloat* vec2 );
	static GLfloat VectorAbsolution( GLfloat* vec1 );
	static bool CompareVector( GLfloat* vec1 , GLfloat* vec2 );
	static bool CompareVector( GLfloat* vec1 , GLfloat x , GLfloat y , GLfloat z );
	static bool VectorApproximation( GLfloat* vec1 , GLfloat* vec2 );
	static bool CompareDirection( GLfloat* vec1 , GLfloat* vec2 );
	static bool CompareDirection( GLfloat* vec1Initial , GLfloat* vec1End , GLfloat* vec2Initial , GLfloat* vec2End );
	static bool CompareDirection( GLfloat* vec1 , GLfloat x , GLfloat y , GLfloat z );

	static bool CheckIntersectOnTriangle( GLfloat* vecInitial , 
										  GLfloat* vecEnd , 
										  GLfloat* point0 , 
										  GLfloat* point1 , 
										  GLfloat* point2 );
	static GLfloat CheckTwoLineDistance( GLfloat* vec1Initial , 
										 GLfloat* vec1End , 
										 GLfloat* vec2Initial , 
										 GLfloat* vec2End );
	static GLfloat CheckDistanceToTriangle( GLfloat* position , 
										    GLfloat* point0 , 
										    GLfloat* point1 , 
										    GLfloat* point2 ,
											GLfloat* normal0 , 
											GLfloat* normal1 , 
											GLfloat* normal2 ,
											GLfloat* nearestPointOnSurface );

	static void CopyVector( GLfloat* result , GLfloat* vec );


	static void MirrorVector( GLfloat* result , GLfloat* vec , GLfloat* plane );

	static GLfloat GetDirectionTheta( GLfloat* direction );
	static GLfloat GetDirectionPhi( GLfloat* direction );
	static GLfloat GetDirectionYaw( GLfloat* direction );

	
private:
};


#endif