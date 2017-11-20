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

	//�i����Iv0
	GLfloat initPosition[3];//�i����I
	//�i��V�qu
	GLfloat endPosition[3];//�i��vector

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
	//�@���d�u���u�q�P�������ۥ��I
	bool GetIntersectPointOnRay( Line* line , GLfloat* intersectPoint );
	//���o��ӥ������ۥ�u
	bool Get2PlaneIntersectLine( Plane* plane , Line* line );//���o��ӥ������ۥ�u
	bool Get2PlaneIntersectLine_2( Plane* plane , Line* line );//���o��ӥ������ۥ�u
	bool IsInside( GLfloat* pos );//�p�G�b�k�V�q�P��V�A�N�Ooutside,���M�N�Oinside
	bool Contain( GLfloat* pos );

	int index;
	GLfloat position[3];
	GLfloat vector[3];

	Plane* firstPlane;
	Plane* link;
};





#endif
