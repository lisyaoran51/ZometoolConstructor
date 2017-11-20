#include "MatrixComputer.h"




// =================================== float ========================================== //
template class SparseLU<Eigen::SparseMatrix<float> >;

template<> SparseLU<Eigen::SparseMatrix<float> >::SparseLU( Eigen::SparseMatrix<float> mat )
{
	_mat = mat;
	_P.resize( _mat.rows() , _mat.cols() );
	_L.resize( _mat.rows() , _mat.cols() );
	_U.resize( _mat.rows() , _mat.cols() );
	ComputeP();
	ComputeLU();
	int i = 0;
	while( !CheckPivotNonZero() )
	{
		ComputeP();
		RandomP();
		ComputeLU();
		std::cout << "第" << i << "次random" << std::endl;
		i++;
	}
}

template<> Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> 
	SparseLU<Eigen::SparseMatrix<float> >::Solve( 
		Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> b )
{
	// PAx=LUx=Pb
	typedef Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> Vector;
	Vector pb = _P * b;
	Vector y = BackwardSubstitution_Lyb(pb);
	Vector x = BackwardSubstitution_Uxy(y);
	return x;
}

template<> bool SparseLU<Eigen::SparseMatrix<float> >::ComputeP()
{
	_P.setIdentity();
	MatrixType tempMat = _mat;

	for( int i = 0; i < tempMat.rows(); i++ )
	{
		// 找出協角元素同直行的元素中 最大的
		Scalar max = tempMat.coeffRef(i,i);
		int maxi = i;
		for( int j = i+1; j < tempMat.cols(); j++ )
		{
			if( abs(tempMat.coeffRef(i,j)) > abs(max) )
			{
				max = tempMat.coeffRef(i,j);
				maxi = j;
			}
		}
		if( maxi == i )continue;

		// 建立交換矩陣
		MatrixType tempP( _mat.rows() , _mat.cols() ); tempP.setIdentity();
		tempP.coeffRef(i,i) = 0;			// 將原本那列的1去掉
		tempP.coeffRef(maxi,maxi) = 0;		// 將要移過去的那列的1去掉
		tempP.coeffRef(i,maxi) = 1;			// 將要移過去的那列放入原本那列
		tempP.coeffRef(maxi,i) = 1;			// 將原本那列放入要移過去的那列
		_P = tempP * _P;					// 將矩陣加入_P矩陣中

		// 將有最大直的列與原本的列交換
		tempMat = tempP * tempMat;
	}
	return true;
}

template<> bool SparseLU<Eigen::SparseMatrix<float> >::ComputeLU()
{
	_L.setZero();
	_U.setIdentity();

	MatrixType pmat = _P * _mat;
	
	//      [,1] [,2] [,3] [,4]
	// [1,]    1   25   81  169
	// [2,]    4   36  100  196
	// [3,]    9   49  121  225
	// [4,]   16   64  144  256
	//
	//				L						U
	//	|	1				|		|	1	25	81	169	|
	//	|	4				|		|					|
	//	|	9				|		|					|
	//	|	16				|		|					|
	//	

	for( int i = 0; i < pmat.rows(); i++ )
	{
		_L.coeffRef(i,0) = pmat.coeffRef(i,0);
	}
	for( int j = 0; j < pmat.cols(); j++ )
	{
		_U.coeffRef(0,j) = pmat.coeffRef(0,j) / pmat.coeffRef(0,0);
	}
	
	for( int d = 1; d < pmat.rows(); d++ ) // diagonal 
	{
		int i,j; 
	//				L						U
	//	|	1				|		|	1	25	81	169	|
	//	|	4	-64			|		|					|
	//	|	9	-176		|		|					|
	//	|	16	-336		|		|					|
	//	
		j = d;
		for( i = d;i < pmat.rows(); i++ )
		{
			float temp = 0;
			for( int k = 0; k < d; k++ )
			{
				temp += _L.coeffRef(i,k)*_U.coeffRef(k,j);
			}
			_L.coeffRef(i,j) = pmat.coeffRef(i,j) - temp;

		}
	//				L						U
	//	|	1				|		|	1	25	81	169	|
	//	|	4	-64			|		|		1	3.5	7.5	|
	//	|	9	-176		|		|					|
	//	|	16	-336		|		|					|
	//	
		i = d;
		for( j = d; j < pmat.cols(); j++ )
		{
			float temp = 0;
			for( int k = 0; k < d; k++ )
			{
				temp += _L.coeffRef(i,k)*_U.coeffRef(k,j);
			}
			_U.coeffRef(i,j) = ( pmat.coeffRef(i,j) - temp ) / _L.coeffRef(d,d);
		}
		
	}

	//if( CheckPivotNonZero() )
	//	return true;
	//else
	//	return false;
	return true;
}

template<> bool SparseLU<Eigen::SparseMatrix<float> >::CheckPivotNonZero()
{
	rPElement.clear();
	for( int i = 0; i < _L.rows(); i++ )
	{
		if( abs(_L.coeffRef(i,i)) < 0.00000001 )
		{
			rPElement.push_back(i);
		}
	}
	if( rPElement.size() > 0 )
	{
		std::cout << "CheckPivotNonZero：有" << rPElement.size() << "個斜角元素為零-";
		for( int i = 0; i < rPElement.size(); i++ )
		{
			std::cout << rPElement[i] << " ";
		}
		std::cout << std::endl;
		return false;
	}
	return true;
}

template<> bool SparseLU<Eigen::SparseMatrix<float> >::RandomP()
{
	for( int i = 0; i < rPElement.size(); i++ )
	{
		srand( (unsigned)time(NULL));
		int swapIndex = 0;
		do
		{
			swapIndex = rand() % _mat.rows();
		}while( swapIndex == rPElement[i] || swapIndex == 0 );
		MatrixType tempRP( _mat.rows() , _mat.cols() ); tempRP.setIdentity();

		tempRP.coeffRef(rPElement[i],rPElement[i]) = 0;			// 將原本那列的1去掉
		tempRP.coeffRef(swapIndex	,swapIndex	 ) = 0;			// 將要移過去的那列的1去掉
		tempRP.coeffRef(rPElement[i],swapIndex	 ) = 1;			// 將要移過去的那列放入原本那列
		tempRP.coeffRef(swapIndex	,rPElement[i]) = 1;			// 將原本那列放入要移過去的那列
		_P = tempRP * _P;					// 將矩陣加入_P矩陣中

	}
	return true;
}

template<> void SparseLU<Eigen::SparseMatrix<float> >::PrintPLU()
{
	//std::cout << "L:" << std::endl << _L << std::endl 
	//		  << "U:" << _U << std::endl;


	FILE* formulaFile = fopen("DebugLU.txt", "w");
	fprintf(formulaFile, "P: \n");
	for( int i = 0; i < _P.rows(); i++ )
	{
		for( int j = 0; j < _P.cols(); j++ )
		{
			fprintf(formulaFile, "%6.3f ", _P.coeffRef(i,j) );
		}
		fprintf(formulaFile, "\n");
	}
	fprintf(formulaFile, "L: \n");
	for( int i = 0; i < _L.rows(); i++ )
	{
		for( int j = 0; j < _L.cols(); j++ )
		{
			fprintf(formulaFile, "%6.3f ", _L.coeffRef(i,j) );
		}
		fprintf(formulaFile, "\n");
	}
	fprintf(formulaFile, "U: \n");
	for( int i = 0; i < _U.rows(); i++ )
	{
		for( int j = 0; j < _U.cols(); j++ )
		{
			fprintf(formulaFile, "%6.3f ", _U.coeffRef(i,j) );
		}
		fprintf(formulaFile, "\n");
	}

	fclose( formulaFile );

}

template<> Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> 
	SparseLU<Eigen::SparseMatrix<float> >::BackwardSubstitution_Lyb( 
		Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> b )
{
	//	http://www.stat.nctu.edu.tw/misg/SUmmer_Course/C_language/Ch06/LUdecomposition.htm
	//	
	typedef Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> Vector;
	Vector y(_L.rows());y.setZero();
	
	y(0) = b(0) / _L.coeffRef(0,0);

	for( int i = 1; i < _L.rows(); i++ )
	{
		Scalar temp = 0;
		for( int j = 0; j < i; j++ )
		{
			temp += _L.coeffRef(i,j) * y(j);
		}
		if( abs(_L.coeffRef(i,i)) < 0.00000001 )	// 分母
		{
			std::cout << "SparseLU::BackwardSubstitution_Lyb出錯：分母為0" << std::endl; 
		}
		else
		{
			y(i) = ( b(i) - temp ) /_L.coeffRef(i,i);
		}
	}
	return y;
}


template<> Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> 
	SparseLU<Eigen::SparseMatrix<float> >::BackwardSubstitution_Uxy( 
		Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> y )
{
	//	http://www.stat.nctu.edu.tw/misg/SUmmer_Course/C_language/Ch06/LUdecomposition.htm
	//	
	typedef Eigen::Matrix<SparseLU<Eigen::SparseMatrix<float> >::Scalar,Eigen::Dynamic,1> Vector;
	Vector x(_U.rows());x.setZero();

	x(_U.rows()-1) = y(_U.rows()-1);

	for( int i = _U.rows()-2; i >= 0; i-- )
	{
		Scalar temp = 0;
		for( int j = i+1; j < _U.rows(); j++ )
		{
			temp += _U.coeffRef(i,j) * x(j);
		}
		x(i) = y(i) - temp;
	}

	return x;

}


// =================================== double ========================================== //

template class SparseLU<Eigen::SparseMatrix<double> >;

template<> SparseLU<Eigen::SparseMatrix<double> >::SparseLU( Eigen::SparseMatrix<double> mat )
{
	_mat = mat;
	_P.resize( _mat.rows() , _mat.cols() );
	_L.resize( _mat.rows() , _mat.cols() );
	_U.resize( _mat.rows() , _mat.cols() );
	std::cout << "SparseLU：計算置換矩陣P" << std::endl;
	ComputeP();
	std::cout << "SparseLU：計算下上三角矩陣LU" << std::endl;
	ComputeLU();
	PrintPLU();
	int i = 0;
	MatrixType oriP = _P;
	while( !CheckPivotNonZero() )
	{
		std::cout << "SparseLU：第" << i << "次random" << std::endl;
		_P = oriP;
		RandomP();
		ComputeLU();
		i++;
	}
	std::cout << "SparseLU：完成" << std::endl;
}

template<> Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> 
	SparseLU<Eigen::SparseMatrix<double> >::Solve( 
		Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> b )
{
	// PAx=LUx=Pb
	typedef Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> Vector;
	Vector pb = _P * b;
	std::cout << "SparseLU:Solve：執行向前代入法 Ly=b" << std::endl;
	Vector y = BackwardSubstitution_Lyb(pb);
	std::cout << "SparseLU:Solve：執行向前代入法 Ux=y" << std::endl;
	Vector x = BackwardSubstitution_Uxy(y);
	return x;
}

template<> bool SparseLU<Eigen::SparseMatrix<double> >::ComputeP()
{
	_P.setIdentity();
	MatrixType tempMat = _mat;

	for( int i = 0; i < tempMat.rows(); i++ )
	{
		// 找出協角元素同直行的元素中 最大的
		Scalar max = tempMat.coeffRef(i,i);
		int maxi = i;
		for( int j = i+1; j < tempMat.cols(); j++ )
		{
			if( abs(tempMat.coeffRef(i,j)) > abs(max) )
			{
				max = tempMat.coeffRef(i,j);
				maxi = j;
			}
		}
		if( maxi == i )continue;

		// 建立交換矩陣
		MatrixType tempP( _mat.rows() , _mat.cols() ); tempP.setIdentity();
		tempP.coeffRef(i,i) = 0;			// 將原本那列的1去掉
		tempP.coeffRef(maxi,maxi) = 0;		// 將要移過去的那列的1去掉
		tempP.coeffRef(i,maxi) = 1;			// 將要移過去的那列放入原本那列
		tempP.coeffRef(maxi,i) = 1;			// 將原本那列放入要移過去的那列
		_P = tempP * _P;					// 將矩陣加入_P矩陣中

		// 將有最大直的列與原本的列交換
		// tempMat = tempP * tempMat;
		Scalar tempElement = 0;
		for( int j = 0; j < _mat.cols(); j++ )
		{
			tempElement = tempP.coeffRef(i,j);
			tempP.coeffRef(i,j) = tempP.coeffRef(maxi,j);
			tempP.coeffRef(maxi,j) = tempElement;
		}
		
	}
	return true;
}

template<> bool SparseLU<Eigen::SparseMatrix<double> >::ComputeLU()
{
	_L.setZero();
	_U.setIdentity();

	MatrixType pmat = _P * _mat;
	
	//      [,1] [,2] [,3] [,4]
	// [1,]    1   25   81  169
	// [2,]    4   36  100  196
	// [3,]    9   49  121  225
	// [4,]   16   64  144  256
	//
	//				L						U
	//	|	1				|		|	1	25	81	169	|
	//	|	4				|		|					|
	//	|	9				|		|					|
	//	|	16				|		|					|
	//	

	for( int i = 0; i < pmat.rows(); i++ )
	{
		_L.coeffRef(i,0) = pmat.coeffRef(i,0);
	}
	for( int j = 0; j < pmat.cols(); j++ )
	{
		_U.coeffRef(0,j) = pmat.coeffRef(0,j) / pmat.coeffRef(0,0);
	}
	
	for( int d = 1; d < pmat.rows(); d++ ) // diagonal 
	{
		int i,j; 
	//				L						U
	//	|	1				|		|	1	25	81	169	|
	//	|	4	-64			|		|					|
	//	|	9	-176		|		|					|
	//	|	16	-336		|		|					|
	//	
		j = d;
		for( i = d;i < pmat.rows(); i++ )
		{
			double temp = 0;
			for( int k = 0; k < d; k++ )
			{
				temp += _L.coeffRef(i,k)*_U.coeffRef(k,j);
			}
			double tempL = pmat.coeffRef(i,j) - temp;
			//if( abs(tempL) > 0.00000001 )
				_L.coeffRef(i,j) = tempL;

		}
	//				L						U
	//	|	1				|		|	1	25	81	169	|
	//	|	4	-64			|		|		1	3.5	7.5	|
	//	|	9	-176		|		|					|
	//	|	16	-336		|		|					|
	//	
		i = d;
		for( j = d; j < pmat.cols(); j++ )
		{
			double temp = 0;
			for( int k = 0; k < d; k++ )
			{
				temp += _L.coeffRef(i,k)*_U.coeffRef(k,j);
			}
			double tempU = ( pmat.coeffRef(i,j) - temp ) / _L.coeffRef(d,d);
			//if( abs(tempU) > 0.000000001 )
				_U.coeffRef(i,j) = tempU;
		}
		
	}

	//if( CheckPivotNonZero() )
	//	return true;
	//else
	//	return false;
	return true;
}

template<> bool SparseLU<Eigen::SparseMatrix<double> >::CheckPivotNonZero()
{
	rPElement.clear();
	for( int i = 0; i < _L.rows(); i++ )
	{
		if( abs(_L.coeffRef(i,i)) == 0 )
		{
			rPElement.push_back(i);
		}
	}
	if( rPElement.size() > 0 )
	{
		std::cout << "CheckPivotNonZero：有" << rPElement.size() << "個斜角元素為零-";
		for( int i = 0; i < rPElement.size(); i++ )
		{
			std::cout << rPElement[i] << " ";
		}
		std::cout << std::endl;
		return false;
	}
	return true;
}

template<> bool SparseLU<Eigen::SparseMatrix<double> >::RandomP()
{
	for( int i = 0; i < rPElement.size(); i++ )
	{
		srand( (unsigned)time(NULL));
		int swapIndex = 0;
		do
		{
			swapIndex = rand() % _P.rows();
		}while( swapIndex == rPElement[i] || swapIndex == 0 );
		MatrixType tempRP( _P.rows() , _P.cols() ); tempRP.setIdentity();

		tempRP.coeffRef(rPElement[i],rPElement[i]) = 0;			// 將原本那列的1去掉
		tempRP.coeffRef(swapIndex	,swapIndex	 ) = 0;			// 將要移過去的那列的1去掉
		tempRP.coeffRef(rPElement[i],swapIndex	 ) = 1;			// 將要移過去的那列放入原本那列
		tempRP.coeffRef(swapIndex	,rPElement[i]) = 1;			// 將原本那列放入要移過去的那列
		_P = tempRP * _P;					// 將矩陣加入_P矩陣中

	}
	return true;
}

template<> void SparseLU<Eigen::SparseMatrix<double> >::PrintPLU()
{
	//std::cout << "L:" << std::endl << _L << std::endl 
	//		  << "U:" << _U << std::endl;


	FILE* formulaFile = fopen("DebugLU.txt", "w");
	fprintf(formulaFile, "P: \n");
	for( int i = 0; i < _P.rows(); i++ )
	{
		for( int j = 0; j < _P.cols(); j++ )
		{
			fprintf(formulaFile, "%6.3f ", _P.coeffRef(i,j) );
		}
		fprintf(formulaFile, "\n");
	}
	fprintf(formulaFile, "L: \n");
	for( int i = 0; i < _L.rows(); i++ )
	{
		for( int j = 0; j < _L.cols(); j++ )
		{
			fprintf(formulaFile, "%6.3f ", _L.coeffRef(i,j) );
		}
		fprintf(formulaFile, "\n");
	}
	fprintf(formulaFile, "U: \n");
	for( int i = 0; i < _U.rows(); i++ )
	{
		for( int j = 0; j < _U.cols(); j++ )
		{
			fprintf(formulaFile, "%6.3f ", _U.coeffRef(i,j) );
		}
		fprintf(formulaFile, "\n");
	}

	fclose( formulaFile );

}

template<> Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> 
	SparseLU<Eigen::SparseMatrix<double> >::BackwardSubstitution_Lyb( 
		Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> b )
{
	//	http://www.stat.nctu.edu.tw/misg/SUmmer_Course/C_language/Ch06/LUdecomposition.htm
	//	
	typedef Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> Vector;
	Vector y(_L.rows());y.setZero();
	
	y(0) = b(0) / _L.coeffRef(0,0);

	for( int i = 1; i < _L.rows(); i++ )
	{
		Scalar temp = 0;
		for( int j = 0; j < i; j++ )
		{
			temp += _L.coeffRef(i,j) * y(j);
		}
		if( abs(_L.coeffRef(i,i)) < 0.00000001 )	// 分母
		{
			std::cout << "SparseLU::BackwardSubstitution_Lyb出錯：分母為0" << std::endl; 
		}
		else
		{
			y(i) = ( b(i) - temp ) /_L.coeffRef(i,i);
		}
	}
	return y;
}


template<> Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> 
	SparseLU<Eigen::SparseMatrix<double> >::BackwardSubstitution_Uxy( 
		Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> y )
{
	//	http://www.stat.nctu.edu.tw/misg/SUmmer_Course/C_language/Ch06/LUdecomposition.htm
	//	
	typedef Eigen::Matrix<SparseLU<Eigen::SparseMatrix<double> >::Scalar,Eigen::Dynamic,1> Vector;
	Vector x(_U.rows());x.setZero();

	x(_U.rows()-1) = y(_U.rows()-1);

	for( int i = _U.rows()-2; i >= 0; i-- )
	{
		Scalar temp = 0;
		for( int j = i+1; j < _U.rows(); j++ )
		{
			temp += _U.coeffRef(i,j) * x(j);
		}
		x(i) = y(i) - temp;
	}

	return x;

}


