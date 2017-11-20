#ifndef OUTER_CONE
#define OUTER_CONE

#include "Cone.h"

#define TYPE_AMOUNT 10

/*
這邊儲存最外層與模型連接的追 美種錐都有八個方向
應該是24個方向


type:
1:
短黃短黃短欄

2.短紅短紅短紅 不好
短紅中黃中黃 

3.
中鴻中欄中藍

4.
面01時 中黃短紅短紅 漢 面23時 中紅短藍短欄
//櫻該是面23的?改掉
5.
中紅短藍短欄

6.
面01時 中紅中藍短欄 漢 面23時 中欄中藍短欄 面23時0泰長
//應該是面23的?改掉
7.
//改成短黃中藍短欄 只有一邊 改掉中欄中藍短欄

8.
面01時 中紅短欄中藍 漢 面23時 中欄短欄中藍 面23時0泰長
改掉

9.
改成短黃短欄中藍 只有一邊 改掉中欄短欄中藍
*/


class OuterCone
{
private:
	bool CheckConeDirection(); //把cone放到他該放的位置
	bool SelectConstructedHole();//根據cone的direction，建立他的竿子

	bool CheckNeighborOuterCone();//查看相鄰的outercone
	bool CheckDublicatedCone(); //看看這個cone有沒有和其他的cone重疊
	bool CheckDublicatedConeEdge( Cone* cone );//看看這個cone的edge有沒有重疊
	bool CheckDublicatedOuterCone(); //看看這個cone有沒有和其他的外錐重疊
	bool CheckDublicatedOuterConeEdge( OuterCone* outerCone );
	bool CheckIntersectWithConnector();//查看有沒有和其他乾子交錯
	bool CheckIntersectWithConeFace();
	GLfloat CheckTwoLineDistance( Point* vec1Initial , Point* vec1End , Point* vec2Initial , Point* vec2End );

	bool CheckEngaged(); //看看有沒有與外面相交
	

public:
	OuterCone( Cone* cone , int coneFace , int t );
	OuterCone( Cone* cone , int coneFace , int t , OuterCone* firstOuterCone );
	~OuterCone();


	bool AddToLink();
	bool CheckIntersectOuterConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckIntersectOuterConeOnConeFace( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckDistanceToSurface(); //看看他的末端距離多遠
	bool TryDifferentType( int differentType );//重建一次方向，然後重是一次engaged漢距離漢dublicated
	bool RefreshDistanceToSurface();//0116，當外部錐rearrange時，舊的distance to surface還未刪掉，所以要重新改為-1

	int index;
	int type;
	int coneDirection;//24個方向

	Cone* sourceCone;
	int sourceConeFace;

	Point** sourceCorner;//3個點 每個點各由一個號碼的洞插入竿子 竿子的另一端視end 分別是上 左 右
	int* constructedHole;
	int* constructedSize;
	Point** cornerEnd;
	int singleConnector;

	OuterCone** neighborOuterCone;

	bool engaged;
	bool* connectorEngaged;
	bool dublicatedWithCone;//和某個cone
	int dublicatedConeIndex;
	
	bool dublicatedWithOuterCone;//和某個cone
	int dublicatedOuterConeIndex;
	bool* cornerDublicated;

	bool intersected;//有沒有交錯

	GLfloat distanceToSurface[ TYPE_AMOUNT ];
	GLfloat weightedDistance[ TYPE_AMOUNT ];//加權距離，與表面距離-與其他表面點距離+100
	Point* nearestPointOnSurface;

	OuterCone* firstOuterCone;
	OuterCone* link;

	ZomeDirection* zomeDirection;
	/*
	Cone( Point* centerPoint );
	Cone( Cone* cone , int facePropagateNumber );
	~Cone();

	void SetCorner( Point* centerPoint );
	void SetCorner( Cone* cone , int facePropagateNumber ); 
	
	bool CheckIntersectConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckDuplicated();
	bool CheckNeighbor();
	bool CheckDistance( int face );
	void AddToLink();

	bool Compare( Cone* cone );
	bool CloselyCompare( Cone* cone );
	int type;//正常:0 期他
	int index;
	int scale;
	GLfloat* position;
	GLfloat* b0Direction;
	GLfloat* b5Direction;
	bool engaged;
	bool dublicated;
	bool* engagedFace;
	bool* facePropagated;
	bool* faceApproached;//??
	int engagedDistance;//4面 ??? 距離edgaged的三角錐有多遠
	Cone** neighborCone;//4面
	Point** corner;//4個點
	Point** localCorner;//4個點 本身做標系 以點0為原點
	ZomeDirection* zomeDirection;

	Cone* firstCone;
	Cone* link;
	*/


};



#endif