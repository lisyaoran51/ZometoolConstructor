#ifndef POLYGON
#define POLYGON

#include "stdafx.h"
#include "Point.h"
#include "Plane.h"

#define IS_INSIDE true//是否在面的髮量向方向
#define IS_OUTSIDE false//是否在面的法向量反方向


class Triangle
{

};

class MyPolygon
{
public:
	MyPolygon();
	MyPolygon( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 , Plane* plane , bool isInside );
	MyPolygon( MyPolygon* polygon , Plane* plane , bool isInside );
	MyPolygon( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	~MyPolygon();

	bool ContainIndex( int i );
	bool Contain( Line* line );//只要有包含，長度不依定要相同
	MyPolygon* GetPolygon( int i ); 
	GLfloat DistanceToPoint( Point* point );
	bool CheckIntersectionPolygonOnPlane( Plane* plane );
	


	int index;
	int cornerAmount;
	Point** corner;

	bool cornerTooClose;//當有兩個點太靠近的時候，用這個紀錄她們太靠近，這個polygon舊不能使用

	MyPolygon* firstPolygon;
	MyPolygon* link;
};



#endif