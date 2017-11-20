#ifndef MYPLANE
#define MYPLANE



#include "stdafx.h"
#include "Point.h"
#include <iomanip>

class Line
{
	bool InsideInterval( GLfloat number0 , GLfloat number1 , GLfloat testNumber );

public:
	Line();
	Line( GLfloat* iPosition , GLfloat* ePosition ); 
	~Line();
	bool AddToLink( Line* line );

	bool ContainedBy( Line* container );
	bool Containing( Line* contained );
	bool TranslateToNearest( Line* line );
	bool TranslateToNearest_2( Line* line );

	//可當原點v0
	GLfloat initPosition[3];//可當員點
	//可當向量u
	GLfloat endPosition[3];//可當vector

	Line* firstLine;
	Line* link;
};

class Plane
{
public:
	Plane();
	Plane( int i );
	Plane( GLfloat* pos , GLfloat* vec );
	Plane( Plane* p );
	~Plane();

	bool SetSplitSurface( GLfloat* pos0 , GLfloat* pos1 );
	bool AddToLink( Plane* p );

	bool CheckIntersection( GLfloat* initPosition , GLfloat* endPosition );
	bool GetIntersectPoint( GLfloat* initPosition , GLfloat* endPosition , GLfloat* intersectPoint );
	//一條吳線長線段與平面的相交點
	bool GetIntersectPointOnRay( Line* line , GLfloat* intersectPoint );
	//取得兩個平面的相交線
	bool Get2PlaneIntersectLine( Plane* plane , Line* line );//取得兩個平面的相交線
	bool Get2PlaneIntersectLine_2( Plane* plane , Line* line );//取得兩個平面的相交線
	bool IsInside( GLfloat* pos );//如果在法向量同方向，就是outside,不然就是inside
	bool Contain( GLfloat* pos );

	int index;
	GLfloat position[3];
	GLfloat vector[3];

	Plane* firstPlane;
	Plane* link;
};





#endif
