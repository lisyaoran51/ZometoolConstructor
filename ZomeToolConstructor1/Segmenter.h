#ifndef SEGMENTER
#define SEGMENTER


#include "Segment.h"

#define IDENTIFIER false
#define GENERATOR true


class Segmenter
{
	bool CheckIntersectionOnSplitPlane( Point* surfacePoint , Plane* splitPlane , SegmentFace*& segmentFace );//第一次呼叫時呼叫這個，用來查詢第一個面，之後改叫下一個
	bool CheckIntersectionOnSplitPlane( Line* sLine , Plane* splitPlane , SegmentFace* segmentFace );
	bool GenerateSplitSurface( Plane* splitPlane , SegmentFace* segmentFace , Point* surfacePoint );
	bool GenerateSegment( bool isGenerator );
	bool ConstructSplitSurface();
	
	Segment* FindAdjacentSegment( PolyhedronPoint* polyhedronFace );//先找出相鄰的碎片
	Segment* FindAdjacentSegment( PolyhedronPoint* polyhedronFace , std::vector<int>* faceSearched );//找沒有重複過的面，看有沒有跟哪個碎片重疊
	Segment* FindBelongToSegment( PolyhedronPoint* polyhedronFace );//找出有哪個碎片與這個三角形重疊
	bool CombineSegment(  SegmentFace* segmentFace , Segment* segmentToCombine );//然後把這些不屬於任何碎片的面加入這個碎片鐘

	//debug用
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