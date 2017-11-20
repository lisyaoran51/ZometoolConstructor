#ifndef POLYGON
#define POLYGON

#include "stdafx.h"
#include "Point.h"
#include "Plane.h"

#define IS_INSIDE true//�O�_�b�����v�q�V��V
#define IS_OUTSIDE false//�O�_�b�����k�V�q�Ϥ�V


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
	bool Contain( Line* line );//�u�n���]�t�A���פ��̩w�n�ۦP
	MyPolygon* GetPolygon( int i ); 
	GLfloat DistanceToPoint( Point* point );
	bool CheckIntersectionPolygonOnPlane( Plane* plane );
	


	int index;
	int cornerAmount;
	Point** corner;

	bool cornerTooClose;//������I�Ӿa�񪺮ɭԡA�γo�Ӭ����o�̤Ӿa��A�o��polygon�¤���ϥ�

	MyPolygon* firstPolygon;
	MyPolygon* link;
};



#endif