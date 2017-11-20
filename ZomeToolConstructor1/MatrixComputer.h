#ifndef MATRIX_COMPUTER
#define MATRIX_COMPUTER

#include <Eigen/Sparse>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#ifndef SMALL_NUMBER
#define SMALL_NUMBER 0.0001
#endif

//template<typename _Scalar>
//class SparseLU<Eigen::SparseMatrix<_Scalar> >;


template<typename _MatrixType>
class SparseLU
{
	typedef _MatrixType MatrixType;
    typedef typename MatrixType::Scalar Scalar;

public:
	SparseLU<MatrixType>();
	SparseLU<MatrixType>( Eigen::SparseMatrix<Scalar> mat );
	Eigen::Matrix<Scalar,Eigen::Dynamic,1> Solve( Eigen::Matrix<Scalar,Eigen::Dynamic,1> b );
	
	void PrintPLU();

protected:
	Eigen::SparseMatrix<Scalar> _mat;
	Eigen::SparseMatrix<Scalar> _P;
	Eigen::SparseMatrix<Scalar> _L;
	Eigen::SparseMatrix<Scalar> _U;

	std::vector<int> rPElement;
	Eigen::SparseMatrix<Scalar> rP;

// SparseLU<Eigen::SparseMatrix<Scalar> >( Eigen::SparseMatrix<Scalar> mat );
	
	bool ComputeP();
	bool ComputeLU();
	bool CheckPivotNonZero();
	bool RandomP();

// Eigen::Matrix<Scalar> Solve( Eigen::Matrix<Scalar> b );
	Eigen::Matrix<Scalar,Eigen::Dynamic,1> BackwardSubstitution_Lyb( Eigen::Matrix<Scalar,Eigen::Dynamic,1> b );
	Eigen::Matrix<Scalar,Eigen::Dynamic,1> BackwardSubstitution_Uxy( Eigen::Matrix<Scalar,Eigen::Dynamic,1> y );


private:

	//~SparseLU();


};

#endif