#ifndef PYRAMID
#define PYRAMID

#include "stdafx.h"
#include "Model.h"
#include "Grid.h"
#include "ZomeDirection.h"

#define PYRAMID_TYPE_BOTTOM 0
#define PYRAMID_TYPE_TOP 1

//   
//   45       
//   2       
// 1   3 67   
//   0          
//            
//            
//           
//        

class Pyramid
{
public:
	Pyramid( int t , PyramidGroup* pyramidGroup );
	Pyramid( Pyramid* pyramid , int facePropagatedFrom , PyramidGroup* pyramidGroup );
	~Pyramid();

	void SetCorner();
	bool CheckIntersectPyramidOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckDistance( int face );
	bool CheckDuplicated();

	bool Compare( Pyramid* pyramid );

	int type;//���`:0123 ���L
	int index;
	int groupIndex;
	int scale;
	GLfloat* position;
	bool engaged;
	bool* engagedFace;
	bool* facePropagated;
	bool* faceApproached;
	Pyramid** neighborPyramid;//4��
	Point** corner;//4���I
	Point** distance;//4�� ???
	ZomeDirection* zomeDirection;

	PyramidGroup* parent;
	Pyramid* firstPyramid;
	Pyramid* link;
};


class PyramidGroup
{
public:
	PyramidGroup( Point* centerPoint );
	PyramidGroup( PyramidGroup* pyramidGroup , int facePropagatedFrom );

	void SetPyramid( int childrenPyramidIndex );
	void CheckInnerFacePropagate();
	bool CheckIntersect( int pyramidIndex );
	bool CheckDuplicated();
	bool Compare( PyramidGroup* pyramidGroup );


	int index;

	bool* pyramidInserted;//4��
	bool* pyramidIntersected;//4��
	Pyramid** childrenPyramid;//4��
	
	bool* facePropagated;//8�ӭ�
	GLfloat* position;
	ZomeDirection* zomeDirection;

	PyramidGroup* firstPyramidGroup;
	PyramidGroup* link;
};

#endif