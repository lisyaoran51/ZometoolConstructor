#ifndef STRUCTURE_ANALYZER
#define STRUCTURE_ANALYZER

#include <Eigen/Sparse>
#include <vector>
#include "stdafx.h"
#include "Constructor.h"
#include "MatrixComputer.h"




typedef Eigen::SparseMatrix<double,Eigen::RowMajor> SM;
typedef Eigen::Triplet<double> T;
typedef std::vector<T> VT;
typedef Eigen::VectorXd V;

class StructureAnalyzer
{
//FindLowestHeight
	float lowestEndHeight;
	std::vector< int > Bottom;

//FindConnectors
	std::vector< VirtualBall* > virtualBalls;
	std::vector< int > errorBalls;//用來擺出錯的球

//SetMatrix
	int bCount;
	int realBCount;
	int cCount;
	SM stiffMat;			// 勁度矩陣
	V f;					// 外力
	V disp;					// 位移
	int knFAmount;			// 已知外力總數
	bool* knF;				// 已知外力
	bool* knDisp;			// 已知位移
	V innerF;				// 竿子內力

	float totalWeight;

	Constructor* cons;
	ZomeDirection* zD;


	bool FindLowestHeight();
	bool SetVirtualBalls();
	bool SDSetVirtualBalls();
	bool SetMatrix();
	;	int GetTowardBallIndex( ZomeConnection* c );
	;	float GetStiffness( ZomeConnection* c );
	bool SDSetMatrix();
	bool SolveMatrix();
	;	bool DebugPrintStiffness( Eigen::SparseMatrix<double>* tempM , Eigen::VectorXd* v );
	bool SolveInnerForce();
	bool SDSolveInnerForce();
	bool PrintAnalysis();
	bool MapToMaterial();

public:
	std::map<int,float> matForce; // 材質未於第幾面
	StructureAnalyzer( Constructor* constructor );

	bool Analyze();
	bool SDAnalyze();
};


#endif