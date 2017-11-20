#ifndef SEGMENT
#define SEGMENT

#include "stdafx.h"
#include "Polyhedron.h"
#include "Point.h"
#include "Model.h"
#include "Plane.h"
#include "MyPolygon.h"
#include "SplitSurface.h"

#define SPLIT_SURFACE_LIMIT 1000
#define SEGMENT_AREA_LIMIT 1000000


class SegmentFace
{
	bool Delete( Point* pointList );

public:
	SegmentFace( PolyhedronPoint* pFace );
	SegmentFace( PolyhedronPoint* pFace , Line* sLine );
	~SegmentFace();
	bool AddToLink( SegmentFace* segmentFace );//還沒寫

	SegmentFace* GetSegmentFace( int segmentFaceIndex );

	bool IsConnectedWith( Line* line );
	bool IsConnectedWith( Plane* plane );
	bool ListContainPoint( int i );//還沒寫。是否存在一個點是index是i
	bool ContainPoint( int i );//還沒寫。是否存在一個點是index是i
	bool ListContainFace( int i );//還沒寫。是否存在一個面是index是i
	bool ContainFace( int i );//還沒寫。是否存在一個面是index是i
	bool Contain( Line* line );
	SegmentFace* GetNeighborSegmentFace();
	bool FindEndPointOnPlane( Plane* plane , GLfloat* endPoint0 , GLfloat* endPoint1 );//還沒寫，見分割面時使用
	//取得碎片與平面相連成的線段
	bool GetConnectingLine( Plane* plane , Line*& connectingLine );
	GLfloat GetArea();
	GLfloat GetTotalArea();

	bool BelongToSegment( SegmentFace* segmentFace );//還沒寫

	int index;
	bool isPolygon;
	PolyhedronPoint* polyhedronFace;
	Line* sourceLine;//先從來元的現，連到polygon的面，得知哪一個polygon是我們要的
	MyPolygon* polygon;

	bool isFirst;//事不是第一個建立的面，事的話delete實就不要刪source line
	bool intersected;
	bool isTooBig;//如果面太大 可能是抓錯 舊山調

	//GLfloat area;//面積


	SegmentFace* firstSegmentFace;
	SegmentFace* link;
};

class Segment
{
public:
	Segment( SegmentFace* s , Polyhedron* p );
	~Segment();
	bool AddToLink( Segment* segment );

	Point* GetFarPoint( Point* sPoint );
	GLfloat GetArea();

	int index;
	Point* surfacePoint;
	Point* farPoint;
	Polyhedron* polyhedron;
	SegmentFace* segmentFace;
	SplitSurface* splitSurface;
	GLfloat area;//面積




	Segment* firstSegment;
	Segment* link;
};


#endif