#ifndef ZOME_GENERATOR
#define ZOME_GENERATOR

#include "stdafx.h"
#include "Model.h"
#include "Point.h"
#include "ZomeDirection.h"
//#include "Pyramid.h"
#include "Cone.h"
#include "FailurePointFixer.h"
#include "OuterCone.h"
#include "ObjFunction.h"
#include "Constructor.h"
#include "Socket.h"
#include "Branch.h"
#include "Segmenter.h"
#include <map>
#include "StructureAnalyzer.h"

#define DISTANCE_PARAM 2.0
#define EFFICIENCY_PARAM 5.0


class ZomeGenerator
{
private:
	//================General//================
	bool ConstructNewBall( Point* point , int size );
	bool ConstructNeighborBall( Point* point , int size );

	//================Cone//================
	int CheckEngagedDistance( Cone* cone , int maxDistance );
	int GetSampleNumber( Model* model );
	void RandomInnerPoint( Model* model , Point* randomPoint );

	bool IfConeCollide( Cone* cone );
	bool CheckEngaged( Cone* cone );
	bool CheckBallEngaged( Cone* cone );

	void ComputeEngagedDistance();
	void FixFailurePoint();

	bool ConstructConeNeighborBall( Cone* cone , Point* point , int size );
	bool ConstructConeConn( Cone* cone , Point* point , int size );
	
	void GenerateConeStructure();
	void DeleteCone( Cone* cone );

	void ZomeStructureAnalyze_Old();
	void ZomeStructureAnalyze();
	//================OuterCone//================
	bool TryDifferentType( OuterCone* outerCone );
	bool GenerateOuterCone();
	void GenerateOuterConeStructure();

	bool CheckDistanceToSurface( OuterCone* outerCone );
	//================Socket//================
	bool GenerateSocket();
	bool GenerateBranchSocket();
	bool GenerateSocketModel();

	bool GenerateSurfacePoint( Point* sPoint );
	//================Branch//================
	bool GenerateBranch( Polyhedron* polyhedron , Segmenter* segmenter , OuterCone* oCone );
	bool CheckSegmentTooBig( Segment* s );
	bool GrowBranch( Branch* branch , Point* farPoint );
	Branch* GrowBranch( Point* point , OuterCone* outerCone , Point* farPoint );
	bool GrowNextBranch( Branch* lastBranch , int iterateTimes );

	OuterCone* GetOuterCone( Segment* segment , OuterCone* outerCone );
	int GetBranchIndexFace( int branchIndex );

	bool CheckEngaged( Branch* branch );
	bool CheckIntersectWithBranch( Branch* branch );
	bool CheckDistanceToSurface( Branch* branch );
	int CheckEfficientPath( Branch*& branch , Point* farPoint , OuterCone* outerCone , Polyhedron* p , GLfloat* endDistance );
	
	bool ConstructBranch();

	bool AddToLink( Branch* branch );
	bool Delete( Branch* branch );
	bool DeleteFromList( Branch* branch );
	//================BranchEnd//================
	bool GenerateBranchEnd();//ok
	bool TryDifferentType( BranchEnd* branchEnd );//ok

	bool CheckEngaged( BranchEnd* branchEnd ); //ok
	bool CheckIntersectWithBranch( BranchEnd* branchEnd );	//ok
	bool CheckSocketConflict( BranchEnd* branchEnd );
	bool CheckDistanceToSurface( BranchEnd* branchEnd ); 

	bool ConstructBranchEnd();
	
	bool AddToLink( BranchEnd* branchEnd );//ok

public:
	//================General//================
	ZomeGenerator( Model* m );
	ZomeGenerator(); // Self-Define Structure
	~ZomeGenerator();
	void Delete( Point* point );
	void Delete( PointList* pointList );

	//================Cone//================
	//================OuterCone//================
	bool CheckEngaged( OuterCone* outerCone );
	
	void DeleteOuterCone( OuterCone* outerCone );
	//================Socket//================
	bool GetNearestPointOnSurface( Point* pointList );
	//bool GetMoreNearestPointOnSurface( Point* pointList );
	bool ExportNearestPointOnSurface( Point* pointList );
	//================Branch//================
	bool MakeBranch( Polyhedron* polyhedron , Segmenter* segmenter );

	// ================ Debug //================
	bool DebugSocket();
	Cone* GrowCone( Cone* c , int f );
	bool GrowOuterCone( Cone* c , int f , int t );
	bool ConstructCusStruc();
	bool ActivateAllCone();
	bool ConstructStructure();
	bool CustomizedAnalysis();


	Model* model;

	int count;
	Cone* firstCone;
	Cone* tempCone;

	OuterCone* firstOuterCone;
	OuterCone* tempOuterCone;

	ZomeDirection* zomeDirection;
	Constructor* constructor;
	PointList* pointList;
	SocketConstructor* socketConstructor;
	SocketConstructor* AnalyzeDisplay;
	StructureAnalyzer* structureAnalyzer;


	Point* nearestPointOnSurface;

	//Branch
	Polyhedron* polyhedron;
	Segmenter* segmenter;

	std::map< Segment* , Branch* > rootList;
	Branch* firstBranch;
	Branch* tempBranch;

	BranchEnd* firstBranchEnd;
	BranchEnd* tempBranchEnd;
};


#endif