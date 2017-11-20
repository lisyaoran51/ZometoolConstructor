#ifndef BRANCH
#define BRANCH

#include "stdafx.h"
#include "Model.h"
#include "Grid.h"
#include "ZomeDirection.h"
#include "OuterCone.h"

#define SEGMENT_AREA_LIMIT_TO_BRANCH 8000
#define BRANCH_LENGTH_LIMIT 7 //GrowBranch��
#define TOWARD_TAIL true
#define TOWARD_HEAD false
#define BRANCH_HEAD 0
#define BRANCH_TAIL 1
#define BRANCH_END_TYPE_AMOUNT 9
#define BRANCH_END_DIRECTION_AMOUNT 5


/*��K���骺���A ��0��1
1.�u�ŵu��
2.�u���u��
3.�����u��
4.�u������
5.���ŵu��
6.�u������
7.��������
8.��������
*/

//5�ӼƤ������V


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

	Branch* Grow( Branch* tempBranch , int face );//�q���ݪ��X�s���Ƥ�
	
	int GetFaceBranchIndex( int face );
	int GetBranchIndexFace( int branchIndex );
	int GetOppositeBranchIndex( int branchIndex );

	bool CheckIntersectWithConeFace();//�qouterCone��
	bool CheckIntersectWithConnector();//�qouterCone��
	bool CheckIntersectWithBranch( Branch* fBranch );
	bool CheckDuplicated( Branch* tempFirstBranch );
	bool CheckConnectToOtherHead();//�o�Ӿ�䪺���ڦ��S���s��ثe�o�����O����䪺�Y

	bool ConnectWithNeighborBranch( Branch* tempBranch );
	bool SetPathDistance( int lastDistance , bool direction );//�]�U�@�ӶZ��
	bool SetShortestPath( int lastDistance );//

	int index;
	//int type;//�ũM��
	int distance;
	bool onPath;//�p��̵u���|��
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
	bool SelectConstructedHole();//�ھ�cone��direction�A�إߥL����l ok
	
	bool CheckIntersectWithConeFace();                        //���S����L�T���@ ok
	bool CheckIntersectWithConnector();						  //���S���M�T���@�Υ~���T���@����l��� ok
	//bool CheckIntersectWithBranch();
	bool CheckIntersectWithBranchEnd();//���S���M��K���ݥ�� ok

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

	bool CheckIntersectWithBranch( Branch* fBranch );		  //���S���M��K���
	bool TryDifferentType( int t , int bEndDorection );//ok

	bool Check( );

	
	
};


#endif