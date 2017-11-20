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

//��\�k�����O �ܧ�v = (Fl/EA)/( ( cos(a)2/Scale(a) + ... + cos(z)2/Scale(z) ))
//			   ���O Fi = ( cos(i)/Scale(i) ) * F / �ܧ�v

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
	std::vector<PolynomialConnectionData> connections[3];//�T�����Ҧ���󰵥\���//�令�u���@��
	
	std::vector<VirtualWorkConnectionData> virtualWorkConnections;//����cosAlpha
	std::vector<int> CosAlphaOrder;//����������l��cosAlpha�j�p���ǡA�Ѥj��p�A�g���OVirtualWork���s�����ǡA�p3->2->1

	float displacement;//�T���������M(���~�O�X����V)
	float OuterForce[3];//�T�쪺�~�O
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
	//bool AddDimensionData( int dimension , PolynomialList* tempPolynomial );//���dimension�Ӻ��׿�i�h�A0��X 1��y 2��z
	
	bool MakeVirtualWorkConnectionData();//��VirtualWorkConnectionData��J�i�h
	int GetVirtualWorkConnectionIndexFromConnIndex( int connIndex);
	VirtualWorkConnectionData* GetVirtualWorkConnection( int index );
	bool ComputeCosAlpha();//�O�ΦX�O�Ӻ�cosAlpha

	bool Analyze();//��X�O

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

	std::vector<BallVirtualForceData> ballVirtualForceData;//�y���X�ӡA�N���X�ӵ�\�k
	
	VirtualForceAnalyzer( PolynomialList* fPolynomial , int bAmount , ZomeConnection* oConnection , ZomeStructure* oBall );
	
	bool Analyze();

private:
	bool Renew();
	//�t���\�E
	bool hasBallUnsolved();
	int GetLeastUnsolvedBallIndex();

	bool MakeConnectionRelation( int ballIndex );//�إߦ���l��������t
		//MakeConnectionRelation�y�{
		bool RedistributeDublicatedRelation( int ballIndex );
		bool CombineChainRelation( int ballIndex );
	bool VirtualWorkAnalyze( int leastUnsolvedBallIndex );
	bool SubstituteSolution( int leastUnsolvedBallIndex );

	
};




#endif