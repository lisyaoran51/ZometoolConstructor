#ifndef CONE
#define CONE

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

class Cone
{
public:
	Cone( Point* centerPoint );
	Cone( Cone* cone , int facePropagateNumber );
	~Cone();

	void SetCorner( Point* centerPoint );
	void SetCorner( Cone* cone , int facePropagateNumber ); 
	
	bool CheckIntersectConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckDuplicated();
	bool CheckNeighbor();
	bool CheckDistance( int face );
	void AddToLink();

	bool Compare( Cone* cone );
	bool CloselyCompare( Cone* cone );
	int type;//正常:0 期他
	int index;
	int scale;
	GLfloat* position;
	GLfloat* b0Direction;
	GLfloat* b5Direction;
	bool activated;
	bool engaged;
	bool dublicated;
	bool* engagedFace;
	bool* facePropagated;
	bool* faceApproached;//和最外層outercone是否相連
	int* faceApproachedIndex;
	int engagedDistance;//4面 ??? 距離edgaged的三角錐有多遠
	Cone** neighborCone;//4面
	Point** corner;//4個點
	Point** localCorner;//4個點 本身做標系 以點0為原點
	ZomeDirection* zomeDirection;

	Cone* firstCone;
	Cone* link;
};

/*
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

	bool* pyramidInserted;//4個
	bool* pyramidIntersected;//4個
	Pyramid** childrenPyramid;//4個
	
	bool* facePropagated;//8個面
	GLfloat* position;
	ZomeDirection* zomeDirection;

	PyramidGroup* firstPyramidGroup;
	PyramidGroup* link;
};
*/
#endif