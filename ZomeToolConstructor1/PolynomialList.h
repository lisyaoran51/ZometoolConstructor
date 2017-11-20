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
	PolynomialList( ZomeStructure* b );//�����ӷ����y
	PolynomialList( VirtualBall* vBall );//�����ӷ��������y
	PolynomialList( int connIndex , float solution );//solution���b�������t�@��A�ҥH�n�[�J�ɭn�ܦ��t��
	int index;
	bool isBall;
	ZomeStructure* sourceBall;
	bool isVirtualBall;
	VirtualBall* sourceVirtualBall;
	std::map< int , float > polynomial;//�h�����A�ĴX�ڱ��A�Y�Ʀh�֡A�e��-1�N��`��
	PolynomialList* firstPolynomial;
	PolynomialList* link;

	std::vector<int>* solvedConnections;

	ZomeConnection* firstConn;
	ZomeStructure* firstBall;
	void Compute();

	bool Include( PolynomialList* included );
	int Overlap( PolynomialList* overlapped );//��ӬO�l���|�b���@��
	bool DivideScalar();//��Ĥ@���ܦ�1
	bool DivideScalar( int term );//���term���ܦ�1
	bool Subtract( PolynomialList* subtracted );//�������Ӱťt�@�Ӧ��l�A�ç��ܦ�0�����R��
	bool Eliminate( PolynomialList* eliminated );//�N��Ӧh�����ۮ��A�p�G����ۮ��A�N�^��false�A
												 //���������G�g�b�ثe�o�Ӧ��l�A�p�G�����u�Ѥ@�Ӷ��A
												 //�N������Ӷ��ݨ�Ҧ����l�סA�ϥΪ���ƬOSubstitute();
	bool Eliminate( PolynomialList* eliminated , int term );//��Y�@���i�����
	bool Substitute();//��o�ӬO�l�u�Ѥ@�Ӷ��ɡA�N��o�Ӷ��N�J�Ҧ����l���C
	bool Replace( PolynomialList* replacer );//�⨩�N�J�����l�N�J�������A�p������AB�A���N�J����BC�A�����N�|��BC
	//�p�ҥN�J
	bool OneSameUnknownAmongTwo( PolynomialList* p );
	bool FindChainPolynomial( std::vector< PolynomialList* >* chainPolynomial , PolynomialList* p );
	bool ChainSubstitute( std::vector< PolynomialList* >* chainPolynomial , PolynomialList* p );
	
	bool HasUnknown( int term );
	bool IsOneUnknown();//�o�Ӧ��l�u���@�Ӷ��A�N�Otrue�C
	bool IsEmpty();//�o�Ӧ��l�S������
	int GetOneUnknown();
	float GetTermParameter( int term );

	bool PrintPolynomialList();//�C�X��Ӧ��l
	bool PrintAllPolynomialList();
	bool SolvePolynomialList();

	PolynomialList* AddToLink( PolynomialList* fPolynomial );
	PolynomialList* AddToTail( PolynomialList* fPolynomial );
};



#endif