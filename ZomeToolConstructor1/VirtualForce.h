#ifndef VIRTUAL_FORCE
#define VIRTUAL_FORCE

#include "stdafx.h"
#include "ZomeDirection.h"
#include "StaticZomeDirection.h"
#include "Model.h"
#include "ZomeStructure.h"
#include "PolarCoordinates.h"
#include "PolynomialList.h"
#include <map>
#include <set>
#include <iomanip>
#include <vector>

//=====================================Relation//=====================================
class Relation
{
public:
	int relatedConnIndex;  //F2 = coefficient * F1 + constant
	float coefficient;
	float constant;
	Relation( int rConnIndex , float coef , float cons );
};
//=====================================Relation//=====================================

//虛功法公式是 變形v = (Fl/EA)/( ( cos(a)2/Scale(a) + ... + cos(z)2/Scale(z) ))
//			   受力 Fi = ( cos(i)/Scale(i) ) * F / 變形v

//=====================================ConnectionData//=====================================
class ConnectionData
{
public:
	int index;
	int color;
	int size;
	int ballFace;
	float scale;

	bool isSolved;
	float solution;

	bool isBall;
	ZomeStructure* sourceBall;
	VirtualBall* sourceVirtualBall;
	ZomeConnection* connection;
	ConnectionData();
	ConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection );
	ConnectionData( int i , VirtualBall* sVirtualBall , ZomeConnection* oConnection );
};
//=====================================ConnectionData//=====================================


//=====================================PolynomialConnectionData//=====================================
class PolynomialConnectionData: public ConnectionData
{
public:
	PolynomialConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection , float c );
	PolynomialConnectionData( int i , VirtualBall* sVirtualBall , ZomeConnection* oConnection , float c );
	float coefficient;

};
//=====================================PolynomialConnectionData//=====================================


//=====================================VirtualWorkConnectionData//=====================================
class VirtualWorkConnectionData: public ConnectionData
{
public:
	VirtualWorkConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection );
	VirtualWorkConnectionData( PolynomialConnectionData pConnectionData );
	float cosAlpha;

	std::vector< Relation > relation;

	
	bool ComputeCosAlpha( GLfloat* outerForceDirection );
};
//=====================================VirtualWorkConnectionData//=====================================


//=====================================BallVirtualForceData//=====================================
class BallVirtualForceData//
{
public:
	int ballIndex;
	bool isBall;
	ZomeStructure* ball;
	VirtualBall* virtualBall;
	ZomeConnection* originalConn;

	std::set<int> connectionNum;
	std::vector<PolynomialConnectionData> connections[3];//三維的所有桿件做功資料//改成只有一維
	
	std::vector<VirtualWorkConnectionData> virtualWorkConnections;//紀錄cosAlpha
	std::vector<int> CosAlphaOrder;//紀錄未知竿子的cosAlpha大小順序，由大到小，寫的是VirtualWork的編號順序，如3->2->1

	float displacement;//三維的偏移和(往外力合的方向)
	float OuterForce[3];//三位的外力
	std::map< int , float > solutions;
	bool solved;
	BallVirtualForceData( PolynomialList* pDimension1 , 
						  PolynomialList* pDimension2 , 
						  PolynomialList* pDimension3 , 
						  ZomeStructure* ball ,
						  ZomeConnection* oConnection );
	BallVirtualForceData( PolynomialList* pDimension1 , 
						  PolynomialList* pDimension2 , 
						  PolynomialList* pDimension3 , 
						  VirtualBall* vBall ,
						  ZomeConnection* oConnection );
	//bool AddDimensionData( int dimension , PolynomialList* tempPolynomial );//把第dimension個維度輸進去，0式X 1式y 2式z
	
	bool MakeVirtualWorkConnectionData();//把VirtualWorkConnectionData輸入進去
	int GetVirtualWorkConnectionIndexFromConnIndex( int connIndex);
	VirtualWorkConnectionData* GetVirtualWorkConnection( int index );
	bool ComputeCosAlpha();//力用合力來算cosAlpha

	bool Analyze();//算出力

	bool hasConnection( int connIndex );
	bool Substitute( int connIndex , float solution );
};

class VirtualForceAnalyzer
{
public:
	PolynomialList* firstPolynomial;
	int ballAmount;
	ZomeConnection* originalConn;
	ZomeStructure* originalBall;
	std::vector<int> solvedConnections;

	std::vector<BallVirtualForceData> ballVirtualForceData;//球有幾個，就有幾個虛功法
	
	VirtualForceAnalyzer( PolynomialList* fPolynomial , int bAmount , ZomeConnection* oConnection , ZomeStructure* oBall );
	
	bool Analyze();

private:
	bool Renew();
	//系結功聚
	bool hasBallUnsolved();
	int GetLeastUnsolvedBallIndex();

	bool MakeConnectionRelation( int ballIndex );//建立位支竿子之間的灌系
		//MakeConnectionRelation流程
		bool RedistributeDublicatedRelation( int ballIndex );
		bool CombineChainRelation( int ballIndex );
	bool VirtualWorkAnalyze( int leastUnsolvedBallIndex );
	bool SubstituteSolution( int leastUnsolvedBallIndex );

	
};




#endif