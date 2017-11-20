#ifndef BRANCH
#define BRANCH

#include "stdafx.h"
#include "Model.h"
#include "Grid.h"
#include "ZomeDirection.h"
#include "OuterCone.h"

#define SEGMENT_AREA_LIMIT_TO_BRANCH 8000
#define BRANCH_LENGTH_LIMIT 7 //GrowBranch用
#define TOWARD_TAIL true
#define TOWARD_HEAD false
#define BRANCH_HEAD 0
#define BRANCH_TAIL 1
#define BRANCH_END_TYPE_AMOUNT 9
#define BRANCH_END_DIRECTION_AMOUNT 5


/*樹枝末梢的型態 先0後1
1.短藍短紅
2.短紅短藍
3.中黃短黃
4.短黃中黃
5.中藍短紅
6.短紅中藍
7.中紅中黃
8.中黃中紅
*/

//5個數之末梢方向


class Branch
{

	bool CheckNeighborBranch();
	bool CheckNearestNode();

public:
	Branch();
	Branch( GLfloat position[3] , int branchIndex , OuterCone* outerCone , GLfloat* fPoint );
	Branch( int branchIndex , Branch* lastBranch );
	~Branch();
	bool AddToLink( Branch* branch );

	Branch* Grow( Branch* tempBranch , int face );//從尾端長出新的數支
	
	int GetFaceBranchIndex( int face );
	int GetBranchIndexFace( int branchIndex );
	int GetOppositeBranchIndex( int branchIndex );

	bool CheckIntersectWithConeFace();//從outerCone拿
	bool CheckIntersectWithConnector();//從outerCone拿
	bool CheckIntersectWithBranch( Branch* fBranch );
	bool CheckDuplicated( Branch* tempFirstBranch );
	bool CheckConnectToOtherHead();//這個樹支的尾巴有沒有連到目前這條的別的樹支的頭

	bool ConnectWithNeighborBranch( Branch* tempBranch );
	bool SetPathDistance( int lastDistance , bool direction );//設下一個距離
	bool SetShortestPath( int lastDistance );//

	int index;
	//int type;//藍和紅
	int distance;
	bool onPath;//計算最短路徑用
	int sourceConeIndex;
	bool pathDirection;
	GLfloat endDistanceToFarPoint;
	GLfloat farPoint[3];
	
	bool isEnd;

	Point* root;
	OuterCone* sourceOuterCone;
	Point* sourceCorner[2];
	int connectFace[2];
	GLfloat distanceToSurface[2];
	Point* nearestPointOnSurface;

	Point* neighborCorner[12][2];
	Branch* neighborBranch[12][2];

	Branch* firstBranch;
	Branch* link;

	ZomeDirection* zomeDirection;
};

class BranchEnd
{
	int index;
	bool SelectConstructedHole();//根據cone的direction，建立他的竿子 ok
	
	bool CheckIntersectWithConeFace();                        //有沒有穿過三角錐 ok
	bool CheckIntersectWithConnector();						  //有沒有和三角錐或外部三角錐的桿子交錯 ok
	//bool CheckIntersectWithBranch();
	bool CheckIntersectWithBranchEnd();//有沒有和樹枝末端交錯 ok

public:
	int type;

	int branchDirection;
	int branchEndDirection;

	Branch* sourceBranch;
	Point* sourceCorner[2];
	Point* cornerEnd[2];
	int constructedHole[2];
	int constructedSize[2];

	GLfloat endPoint[3];
	GLfloat surfacePoint[3];
	
	Point* nearestPointOnSurface;

	GLfloat distance[ BRANCH_END_TYPE_AMOUNT ][ BRANCH_END_DIRECTION_AMOUNT ];
	GLfloat weightedDistance[ BRANCH_END_TYPE_AMOUNT ][ BRANCH_END_DIRECTION_AMOUNT ];

	BranchEnd* firstBranchEnd;
	BranchEnd* link;

	ZomeDirection* zomeDirection;

	BranchEnd( Branch* branch , BranchEnd* fBranchEnd );//ok

	bool CheckIntersectWithBranch( Branch* fBranch );		  //有沒有和樹枝交錯
	bool TryDifferentType( int t , int bEndDorection );//ok

	bool Check( );

	
	
};


#endif