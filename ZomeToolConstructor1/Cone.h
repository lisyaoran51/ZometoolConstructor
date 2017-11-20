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
	int type;//���`:0 ���L
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
	bool* faceApproached;//�M�̥~�houtercone�O�_�۳s
	int* faceApproachedIndex;
	int engagedDistance;//4�� ??? �Z��edgaged���T���@���h��
	Cone** neighborCone;//4��
	Point** corner;//4���I
	Point** localCorner;//4���I �������Шt �H�I0�����I
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

	bool* pyramidInserted;//4��
	bool* pyramidIntersected;//4��
	Pyramid** childrenPyramid;//4��
	
	bool* facePropagated;//8�ӭ�
	GLfloat* position;
	ZomeDirection* zomeDirection;

	PyramidGroup* firstPyramidGroup;
	PyramidGroup* link;
};
*/
#endif