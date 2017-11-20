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
	bool AddToLink( SegmentFace* segmentFace );//�٨S�g

	SegmentFace* GetSegmentFace( int segmentFaceIndex );

	bool IsConnectedWith( Line* line );
	bool IsConnectedWith( Plane* plane );
	bool ListContainPoint( int i );//�٨S�g�C�O�_�s�b�@���I�Oindex�Oi
	bool ContainPoint( int i );//�٨S�g�C�O�_�s�b�@���I�Oindex�Oi
	bool ListContainFace( int i );//�٨S�g�C�O�_�s�b�@�ӭ��Oindex�Oi
	bool ContainFace( int i );//�٨S�g�C�O�_�s�b�@�ӭ��Oindex�Oi
	bool Contain( Line* line );
	SegmentFace* GetNeighborSegmentFace();
	bool FindEndPointOnPlane( Plane* plane , GLfloat* endPoint0 , GLfloat* endPoint1 );//�٨S�g�A�����έ��ɨϥ�
	//���o�H���P�����۳s�����u�q
	bool GetConnectingLine( Plane* plane , Line*& connectingLine );
	GLfloat GetArea();
	GLfloat GetTotalArea();

	bool BelongToSegment( SegmentFace* segmentFace );//�٨S�g

	int index;
	bool isPolygon;
	PolyhedronPoint* polyhedronFace;
	Line* sourceLine;//���q�Ӥ����{�A�s��polygon�����A�o�����@��polygon�O�ڭ̭n��
	MyPolygon* polygon;

	bool isFirst;//�Ƥ��O�Ĥ@�ӫإߪ����A�ƪ���delete��N���n�Rsource line
	bool intersected;
	bool isTooBig;//�p�G���Ӥj �i��O��� �¤s��

	//GLfloat area;//���n


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
	GLfloat area;//���n




	Segment* firstSegment;
	Segment* link;
};


#endif