#ifndef POLYHEDRON
#define POLYHEDRON


#include "stdafx.h"
#include "Point.h"
#include "Plane.h"
#include "Model.h" 
#include <vector>
#include <algorithm>

#define POLYHEDRON_POINT 0
#define POLYHEDRON_EDGE 1
#define POLYHEDRON_FACE 2


class PolyhedronPoint
{

public:
	PolyhedronPoint( int i , GLfloat* pos , GLfloat* nor , int t );
	PolyhedronPoint( int i , PolyhedronPoint* cPoint0 , PolyhedronPoint* cPoint1 , PolyhedronPoint* cPoint2 , int t);
	~PolyhedronPoint();
	//bool Set( int i , GLfloat* pos , GLfloat* nor );
	//bool ContainConnectedPoint();
	//face用
	bool Contain( int pointNumber );
	bool Contain( PolyhedronPoint* point );
	bool Contain( Line* line );//是否有一條線未在該邊上，
	
	int GetPolyhedronFace( PolyhedronPoint* connectedPoint0 , PolyhedronPoint* connectedPoint1 );
	int GetPolyhedronFace( int connectedPoint0 , int connectedPoint1 );
	PolyhedronPoint* GetPolyhedronFace( Line* propagatingLine );
	bool GetPropagatingLine( PolyhedronPoint* propagatingPolyhedronFace , PolyhedronPoint*& connectedPoint0 , PolyhedronPoint*& connectedPoint1 );
	GLfloat GetArea();

	int index;
	int type;
	GLfloat position[3];
	GLfloat normal[3];
	int connectedPointsAmount;
	PolyhedronPoint** connectedPoint;//
	int connectedEdgesAmount;
	PolyhedronPoint** connectedEdge;
	int connectedFacesAmount;
	PolyhedronPoint** connectedFace;//
	

};

/*
class PolyhedronEdge
{
public:
	int index;
	//GLfloat position[2][3];
	GLfloat vector[3];
	PolyhedronPoint** containedPoint;
	//int connectedEdgesAmount;
	//PolyhedronEdge** connectedEdge;
	PolyhedronFace** connectedFace;
};

class PolyhedronFace
{
public:
	PolyhedronFace( int i , PolyhedronPoint* cPoint0 , PolyhedronPoint* cPoint1 , PolyhedronPoint* cPoint2 );
	//bool Set( int i , PolyhedronPoint* cPoint0 , PolyhedronPoint* cPoint1 , PolyhedronPoint* cPoint2 );
	bool Contain( int pointNumber );
	bool Contain( PolyhedronPoint* point );
	bool Contain( Line* line );//是否有一條線未在該邊上，

	int GetPolyhedronFace( PolyhedronPoint* connectedPoint0 , PolyhedronPoint* connectedPoint1 );
	int GetPolyhedronFace( int connectedPoint0 , int connectedPoint1 );
	PolyhedronFace* GetPolyhedronFace( Line* propagatingLine );
	bool GetPropagatingLine( PolyhedronFace* propagatingPolyhedronFace , PolyhedronPoint* connectedPoint0 , PolyhedronPoint* connectedPoint1 );

	int index;
	GLfloat normal[3];
	PolyhedronPoint* containedPoint[3];//
	PolyhedronFace* connectedFace[3];
};
*/
class Polyhedron
{
	bool Connect();


public:
	Polyhedron();
	Polyhedron( Model* m );
	~Polyhedron();

	int GetClosestFace( GLfloat* pos );
	PolyhedronPoint* GetPolyhedronPoint( int i );
	PolyhedronPoint* GetPolyhedronFace( int i );

	Model* model;

	int pointAmount;
	int faceAmount;

	PolyhedronPoint** point;
	PolyhedronPoint** face;


};








#endif