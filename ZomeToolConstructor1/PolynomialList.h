#ifndef POLYNOMIAL_LIST
#define POLYNOMIAL_LIST

#include "stdafx.h"
#include "ZomeDirection.h"
#include "Model.h"
#include "ZomeStructure.h"
#include "PolarCoordinates.h"
#include <map>
#include <iomanip>
#include <vector>
#include <Eigen/Sparse>

#define CONSTANT -1

class PolynomialList
{
public:
	PolynomialList();
	PolynomialList( Eigen::SparseMatrix<float,Eigen::RowMajor>* m , Eigen::VectorXf* v );
	PolynomialList( Eigen::SparseMatrix<float,Eigen::RowMajor>* m , Eigen::VectorXf* v , int row );
	PolynomialList( ZomeStructure* b );//紀錄來源的球
	PolynomialList( VirtualBall* vBall );//紀錄來源的虛擬球
	PolynomialList( int connIndex , float solution );//solution式在等式的另一邊，所以要加入時要變成負的
	int index;
	bool isBall;
	ZomeStructure* sourceBall;
	bool isVirtualBall;
	VirtualBall* sourceVirtualBall;
	std::map< int , float > polynomial;//多項式，第幾根桿件，係數多少，前項-1代表常數
	PolynomialList* firstPolynomial;
	PolynomialList* link;

	std::vector<int>* solvedConnections;

	ZomeConnection* firstConn;
	ZomeStructure* firstBall;
	void Compute();

	bool Include( PolynomialList* included );
	int Overlap( PolynomialList* overlapped );//兩個是子重疊在哪一項
	bool DivideScalar();//把第一項變成1
	bool DivideScalar( int term );//把第term項變成1
	bool Subtract( PolynomialList* subtracted );//本式拿來剪另一個式子，並把變成0的項刪掉
	bool Eliminate( PolynomialList* eliminated );//將兩個多項式相消，如果不能相消，就回傳false，
												 //消完的結果寫在目前這個式子，如果消完只剩一個項，
												 //就直接把該項待到所有式子終，使用的函數是Substitute();
	bool Eliminate( PolynomialList* eliminated , int term );//對某一項進行消除
	bool Substitute();//當這個是子只剩一個項時，就把這個項代入所有式子中。
	bool Replace( PolynomialList* replacer );//把貝代入的式子代入本式中，如本式為AB，貝代入式為BC，本式就會變BC
	//聯所代入
	bool OneSameUnknownAmongTwo( PolynomialList* p );
	bool FindChainPolynomial( std::vector< PolynomialList* >* chainPolynomial , PolynomialList* p );
	bool ChainSubstitute( std::vector< PolynomialList* >* chainPolynomial , PolynomialList* p );
	
	bool HasUnknown( int term );
	bool IsOneUnknown();//這個式子只有一個項，就是true。
	bool IsEmpty();//這個式子沒有任何項
	int GetOneUnknown();
	float GetTermParameter( int term );

	bool PrintPolynomialList();//列出整個式子
	bool PrintAllPolynomialList();
	bool SolvePolynomialList();

	PolynomialList* AddToLink( PolynomialList* fPolynomial );
	PolynomialList* AddToTail( PolynomialList* fPolynomial );
};



#endif