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
	std::vector< int > errorBalls;//�Ψ��\�X�����y

//SetMatrix
	int bCount;
	int realBCount;
	int cCount;
	SM stiffMat;			// �l�ׯx�}
	V f;					// �~�O
	V disp;					// �첾
	int knFAmount;			// �w���~�O�`��
	bool* knF;				// �w���~�O
	bool* knDisp;			// �w���첾
	V innerF;				// ��l���O

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
	std::map<int,float> matForce; // ���襼��ĴX��
	StructureAnalyzer( Constructor* constructor );

	bool Analyze();
	bool SDAnalyze();
};


#endif