#ifndef SEGMENTER
#define SEGMENTER


#include "Segment.h"

#define IDENTIFIER false
#define GENERATOR true


class Segmenter
{
	bool CheckIntersectionOnSplitPlane( Point* surfacePoint , Plane* splitPlane , SegmentFace*& segmentFace );//�Ĥ@���I�s�ɩI�s�o�ӡA�ΨӬd�߲Ĥ@�ӭ��A�����s�U�@��
	bool CheckIntersectionOnSplitPlane( Line* sLine , Plane* splitPlane , SegmentFace* segmentFace );
	bool GenerateSplitSurface( Plane* splitPlane , SegmentFace* segmentFace , Point* surfacePoint );
	bool GenerateSegment( bool isGenerator );
	bool ConstructSplitSurface();
	
	Segment* FindAdjacentSegment( PolyhedronPoint* polyhedronFace );//����X�۾F���H��
	Segment* FindAdjacentSegment( PolyhedronPoint* polyhedronFace , std::vector<int>* faceSearched );//��S�����ƹL�����A�ݦ��S������ӸH�����|
	Segment* FindBelongToSegment( PolyhedronPoint* polyhedronFace );//��X�����ӸH���P�o�ӤT���έ��|
	bool CombineSegment(  SegmentFace* segmentFace , Segment* segmentToCombine );//�M���o�Ǥ��ݩ����H�������[�J�o�ӸH����

	//debug��
	void PrintSegment( Segment* s );
public:
	Segmenter();
	Segmenter( Polyhedron* p , Point* sPointList );
	Segmenter( Polyhedron* p , Point* sPointList , bool isGenerator );

	bool Delete( Plane* plane );
	bool Delete( SegmentFace* segmentFace );
	bool DeleteFromLink( SegmentFace* segmentFace );

	int count;
	//bool isTooBig;

	Polyhedron* polyhedron;
	
	Point* surfacePointList;
	Segment* segment;
	int segmentAmount;

	SplitSurfaceConstructor** splitSurfaceConstructor;




};





#endif SEGMENTER