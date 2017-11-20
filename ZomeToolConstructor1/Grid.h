#ifndef GRID
#define GRID



#include "stdafx.h"
#include "Model.h"
#include "Point.h"



class Grid
{
public:
	Grid();
	Grid( Point* centerPoint );
	Grid( Grid* grid , int faceProgated );
	~Grid();

	void SetCorner();

	bool CheckIntersectGridOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckIntersectTriOnGrid( GLfloat* point1 , GLfloat* point2 );
	bool CheckDuplicated();
	bool CheckEngagedInDistance( int distance );

	void PostCheck();
	int GetOppositeFace( int face );
	bool Compare( Grid* grid );
	


	int index;
	int groupIndex;
	GLfloat* position;
	bool engaged;
	bool engagedInDistance;
	bool* engagedFace;
	bool* facePropagated;
	Grid** neighborGrid;//6��
	Point** corner;//�K���I

	Grid* firstGrid;
	Grid* link;
};

#endif
