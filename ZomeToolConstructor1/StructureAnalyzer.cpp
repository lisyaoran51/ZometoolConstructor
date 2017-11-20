#include "StructureAnalyzer.h"



StructureAnalyzer::StructureAnalyzer( Constructor* constructor )
{
	cons = constructor;
	zD = constructor -> zomeDirection;
}

bool StructureAnalyzer::FindLowestHeight()
{

	lowestEndHeight = 1000.0;
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex == -1 )
		{
			if( ( i -> endPosition )[1] < lowestEndHeight )
			{
				lowestEndHeight = ( i -> endPosition )[1];
			}
		}
	}
	std::cout << "最低的高度設為" << lowestEndHeight << std::endl;
	//把最低的幾個地方標出來
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex == -1 )
		{
			if( ( i -> endPosition )[1] < lowestEndHeight + BOTTOM_INTERVAL && i -> towardIndex == -1 )
			{
				Bottom.push_back( i -> index );
				std::cout << "第" << i -> index << "根竿子末端為接地，座標為(" 
						  <<  ( i -> endPosition )[0] << "," 
						  <<  ( i -> endPosition )[1] << "," 
						  <<  ( i -> endPosition )[2] << ")" << std::endl;
			}
		}
	}

	return true;
}

bool StructureAnalyzer::SetVirtualBalls()
{
	realBCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
		realBCount++;



	//建立連接表面的點的量
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex != -1 )continue;
		bool newVirtualBall = true;
		for(int j = 0; j < virtualBalls.size(); j++ )
		{
			VirtualBall* tempVirtualBall = virtualBalls.at( j );
			if( MathHelper::VectorApproximation( tempVirtualBall -> position ,
												 i -> endPosition ) )//如果這根棒子跟連接表面的點位置箱同，就把她家進去
			{
				newVirtualBall = false;
				//tempVirtualBall -> AddConn( i );
				tempVirtualBall -> connectionAmount++;
				if( tempVirtualBall -> connectionAmount > 5 )
				{
					
					std::cout << "Constructor::StructureAnalyze()出錯:連接表面的點所接的竿子超過6根" << std::endl;
					std::cout << "多出來的桿子編號是：" << i -> index << "，面是：" << zD -> GetOppositeFace( i -> fromFace ) << std::endl;
					errorBalls.push_back( i -> index );
					tempVirtualBall -> connectionAmount--;
					std::cout << "虛擬球座標：(" << tempVirtualBall -> position[0] << "," 
												 << tempVirtualBall -> position[1] << "," 
												 << tempVirtualBall -> position[2] << ")" << std::endl;
					continue;
				} 
				int towardFace = zD -> GetOppositeFace( i -> fromFace );
				tempVirtualBall -> connectFace[ tempVirtualBall -> connectionAmount ] = towardFace;//輸入與竿子連接的面編號
				tempVirtualBall -> connIndex[ tempVirtualBall -> connectionAmount ] = i -> index;//輸入竿子的編號
				tempVirtualBall -> conn[ tempVirtualBall -> connectionAmount ] = i;//輸入竿子的指標
				std::cout << "在第" << j << "個虛擬球(" << tempVirtualBall -> position[0] << "," 
						  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")中加入第" << i -> index << "號竿子，連接面是" << towardFace << "，目前共" 
						  << tempVirtualBall -> connectionAmount << "根竿子" << std::endl;
				bool debug = 0;
			}
		}
		if( newVirtualBall )
		{
			VirtualBall* tempVirtualBall = new VirtualBall( realBCount + virtualBalls.size() , i );//沒有這個點的話就新增一個點
			int towardFace = zD -> GetOppositeFace( i -> fromFace );
			tempVirtualBall -> connectFace[ 0 ] = towardFace;
			if( i -> endPosition[1] < lowestEndHeight + BOTTOM_INTERVAL )
			{
				tempVirtualBall -> isBottom = true;
			}
			virtualBalls.push_back( tempVirtualBall );
			std::cout << "新增第" << virtualBalls.size() << "個虛擬球(" << tempVirtualBall -> position[0] << ","
					  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")，加入第" 
					  << i -> index << "號竿子，連接面是" << towardFace << "，目前共" 
					  << tempVirtualBall -> connectionAmount << "根竿子" << std::endl;
			
		}
	}
	return true;
}

bool StructureAnalyzer::SDSetVirtualBalls()
{
	realBCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
		realBCount++;



	//建立連接表面的點的量
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex != -1 )continue;
		bool newVirtualBall = true;
		for(int j = 0; j < virtualBalls.size(); j++ )
		{
			VirtualBall* tempVirtualBall = virtualBalls.at( j );
			if( MathHelper::VectorApproximation( tempVirtualBall -> position ,
												 i -> endPosition ) )//如果這根棒子跟連接表面的點位置箱同，就把她家進去
			{
				newVirtualBall = false;
				//tempVirtualBall -> AddConn( i );
				bool dubConn = false;
				for( int c = 0; c <= tempVirtualBall -> connectionAmount; c++ )
				{
					GLfloat* connPos = tempVirtualBall -> conn[c] -> position;
					if( MathHelper::VectorApproximation( i -> position , connPos ) )
					{
						dubConn = true;
						std::cout << "重複竿子編號" << i -> index << std::endl;
					}
				}
				if(dubConn) continue;

				tempVirtualBall -> connectionAmount++;
				if( tempVirtualBall -> connectionAmount > 5 )
				{
					
					std::cout << "Constructor::StructureAnalyze()出錯:連接表面的點所接的竿子超過6根" << std::endl;
					std::cout << "多出來的桿子編號是：" << i -> index << "，面是：" << zD -> GetOppositeFace( i -> fromFace ) << std::endl;
					errorBalls.push_back( i -> index );
					tempVirtualBall -> connectionAmount--;
					std::cout << "虛擬球座標：(" << tempVirtualBall -> position[0] << "," 
												 << tempVirtualBall -> position[1] << "," 
												 << tempVirtualBall -> position[2] << ")" << std::endl;
					continue;
				} 
				int towardFace = zD -> GetOppositeFace( i -> fromFace );
				tempVirtualBall -> connectFace[ tempVirtualBall -> connectionAmount ] = towardFace;//輸入與竿子連接的面編號
				tempVirtualBall -> connIndex[ tempVirtualBall -> connectionAmount ] = i -> index;//輸入竿子的編號
				tempVirtualBall -> conn[ tempVirtualBall -> connectionAmount ] = i;//輸入竿子的指標
				std::cout << "在第" << j << "個虛擬球(" << tempVirtualBall -> position[0] << "," 
						  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")中加入第" << i -> index << "號竿子，連接面是" << towardFace << "，目前共" 
						  << tempVirtualBall -> connectionAmount << "根竿子" << std::endl;
				bool debug = 0;
			}
		}
		if( newVirtualBall )
		{
			VirtualBall* tempVirtualBall = new VirtualBall( realBCount + virtualBalls.size() , i );//沒有這個點的話就新增一個點
			int towardFace = zD -> GetOppositeFace( i -> fromFace );
			tempVirtualBall -> connectFace[ 0 ] = towardFace;
			if( i -> endPosition[1] < lowestEndHeight + BOTTOM_INTERVAL )
			{
				tempVirtualBall -> isBottom = true;
			}
			virtualBalls.push_back( tempVirtualBall );
			std::cout << "新增第" << virtualBalls.size() << "個虛擬球(" << tempVirtualBall -> position[0] << ","
					  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")，加入第" 
					  << i -> index << "號竿子，連接面是" << towardFace << "，目前共" 
					  << tempVirtualBall -> connectionAmount << "根竿子" << std::endl;
			
		}
	}
	return true;
}

bool StructureAnalyzer::SetMatrix()
{

//算總共有幾顆球
	realBCount = 0;
	bCount = 0;
	cCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
		realBCount++;
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		cCount++;
	}
	bCount = realBCount + virtualBalls.size();
//===============
	
	stiffMat.resize(bCount*3,bCount*3);		stiffMat.setZero();
	f.resize(bCount*3);						f		.setZero();					
	disp.resize(bCount*3);					disp	.setZero();
	innerF.resize(cCount);					innerF	.setZero();

	std::cout << "SetMatrix：矩陣為" << bCount*3 << "x" << bCount*3 << "維。" << std::endl;

//改成對每個竿子寫一段Matrix
	//SetStiffness();
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		int fromB = i -> fromIndex;
		int toB = GetTowardBallIndex( i );
		float s = GetStiffness( i );
//rotation式從來原面的三軸分量
		float res[3] = { i -> rotation[0] ,
						 i -> rotation[1] ,
						 i -> rotation[2] };

		SM tempM( bCount*3 , bCount*3 );
		VT tempVT;

/*
勁度矩陣：
					from		 to
				  點1位移	   點2位移
				   / |  \      /  | \
			|    xx	 xy	 xz	-xx	-xy	-xz	|   \
			|	 xy	 yy	 yz	-xy	-yy	-yz |   - 點一施力 from
			|	 xz	 yz	 zz	-xz	-yz	-zz |   /
			|	-xx	-xy	-xz	 xx	 xy	 xz	|	\
			|  	-xy	-yy	-yz	 xy	 yy	 yz	|	- 點2師力   to
			|  	-xz	-yz	-zz	 xz	 yz	 zz	|	/
*/
		//float EAdivL = YOUNG_MODULUS * 10.0 / zD -> GetFaceLength( i -> fromFace , i -> size ); //10.0是截面積 
//(第幾橫列,第幾直行)
		for( int j = 0; j < 3; j++ )
		{
			for( int k = 0; k < 3; k++ )
			{
//第j列第k行 左上  (from, from)
				tempVT.push_back( T( fromB*3+j , fromB*3+k ,  res[j]*res[k] * s ) );
//第j列第k行 右上  (from, to)
				tempVT.push_back( T( fromB*3+j , toB*3+k   , -res[j]*res[k] * s ) );
//第j列第k行 左下  (to, from)
				tempVT.push_back( T( toB*3+j   , fromB*3+k , -res[j]*res[k] * s ) );
//第j列第k行 右下  (to, to)
				tempVT.push_back( T( toB*3+j   , toB*3+k   ,  res[j]*res[k] * s ) );
			}
		}
//將每一個勁度加到稀疏矩陣，在將稀疏矩陣累加
		tempM.setFromTriplets( tempVT.begin() , tempVT.end() );
		stiffMat += tempM;

//將竿子的重量加入force中
		f[ fromB*3 + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f; 
		f[ toB*3   + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f;	// 如果這個點屬於底部，後面會將他的受力歸零

		totalWeight -= zD -> GetWeight( i -> color ,i -> size );				// 總重量
	}


//設定位移和受力
	//SetOuterForce();
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
	{
		f[ b->index*3 + 1 ] -= zD -> GetWeight( WEIGHT_WHITE , 0 ) ;
		totalWeight -= zD -> GetWeight( WEIGHT_WHITE , 0 );			// 總重量
	}
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( !virtualBalls[i] -> isBottom )
		{	
			f[ virtualBalls[i]->index*3 + 1 ] -= SEGMENT_WEIGHT ;	// 不在底部的碎片，加上重量
			totalWeight -= SEGMENT_WEIGHT;							// 總重量
		}
		else
		{
			totalWeight -= f[ virtualBalls[i]->index*3 + 1 ];	// 把底部的碎片的重量扣掉
			f[ virtualBalls[i]->index*3 + 1 ] = 0;
		}
	}

//設定已知受力和位移
	//SetIfKnown();
	knF = new bool[ bCount*3 ];		for(int i=0;i<bCount*3;i++)knF[i]=true;    //大多外力都已知
	knDisp = new bool[ bCount*3 ];	for(int i=0;i<bCount*3;i++)knDisp[i]=false;//大多位移都未知
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{	
			knF[ realBCount*3 + i*3 + 1 ] = false;		//底部的y方向外力未知
			knDisp[ realBCount*3 + i*3 + 1 ] = true;	//底部的y方向位移已知
		}
	}
//拿第一個底部VirtualBall當作固定點
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{
			knF[ realBCount*3 + i*3 ] = false;		// 底部固定點的外力未知
			knF[ realBCount*3 + i*3 + 1 ] = false;
			knF[ realBCount*3 + i*3 + 2 ] = false;
			knDisp[ realBCount*3 + i*3 ] = true;		// 底部固定點的位移已知
			knDisp[ realBCount*3 + i*3 + 1 ] = true;
			knDisp[ realBCount*3 + i*3 + 2 ] = true;
			break;
		}
	}
	knFAmount = 0;
	for( int i = 0; i < bCount*3; i++ )
	{
		if( !knF[i] ) knFAmount++;
	}


	return true;
}

bool StructureAnalyzer::SDSetMatrix()
{

//算總共有幾顆球
	realBCount = 0;
	bCount = 0;
	cCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
	{
		realBCount++;
	
	}
		
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		cCount++;
	}
	bCount = realBCount + virtualBalls.size();
//===============
	
	stiffMat.resize(bCount*3,bCount*3);		stiffMat.setZero();
	f.resize(bCount*3);						f		.setZero();					
	disp.resize(bCount*3);					disp	.setZero();
	innerF.resize(cCount);					innerF	.setZero();

	std::cout << "SetMatrix：矩陣為" << bCount*3 << "x" << bCount*3 << "維。" << std::endl;

//改成對每個竿子寫一段Matrix
	//SetStiffness();
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{

		int fromB = i -> fromIndex;
		int toB = GetTowardBallIndex( i );
		float s = GetStiffness( i );
		
//rotation式從來原面的三軸分量
		float res[3] = {0,0,0};
		zD -> GetFaceDirection( i -> fromFace , res );

		SM tempM( bCount*3 , bCount*3 );
		VT tempVT;

/*
勁度矩陣：
					from		 to
				  點1位移	   點2位移
				   / |  \      /  | \
			|    xx	 xy	 xz	-xx	-xy	-xz	|   \
			|	 xy	 yy	 yz	-xy	-yy	-yz |   - 點一施力 from
			|	 xz	 yz	 zz	-xz	-yz	-zz |   /
			|	-xx	-xy	-xz	 xx	 xy	 xz	|	\
			|  	-xy	-yy	-yz	 xy	 yy	 yz	|	- 點2師力   to
			|  	-xz	-yz	-zz	 xz	 yz	 zz	|	/
*/
		//float EAdivL = YOUNG_MODULUS * 10.0 / zD -> GetFaceLength( i -> fromFace , i -> size ); //10.0是截面積 
//(第幾橫列,第幾直行)
		for( int j = 0; j < 3; j++ )
		{
			for( int k = 0; k < 3; k++ )
			{
//第j列第k行 左上  (from, from)
				tempVT.push_back( T( fromB*3+j , fromB*3+k ,  res[j]*res[k] * s ) );
//第j列第k行 右上  (from, to)
				tempVT.push_back( T( fromB*3+j , toB*3+k   , -res[j]*res[k] * s ) );
//第j列第k行 左下  (to, from)
				tempVT.push_back( T( toB*3+j   , fromB*3+k , -res[j]*res[k] * s ) );
//第j列第k行 右下  (to, to)
				tempVT.push_back( T( toB*3+j   , toB*3+k   ,  res[j]*res[k] * s ) );
			}
		}
//將每一個勁度加到稀疏矩陣，在將稀疏矩陣累加
		tempM.setFromTriplets( tempVT.begin() , tempVT.end() );
		stiffMat += tempM;

//將竿子的重量加入force中
		f[ fromB*3 + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f; 
		f[ toB*3   + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f;	// 如果這個點屬於底部，後面會將他的受力歸零

		totalWeight -= zD -> GetWeight( i -> color ,i -> size );				// 總重量
	}


//設定位移和受力
	//SetOuterForce();
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
	{
		f[ b->index*3 + 1 ] -= zD -> GetWeight( WEIGHT_WHITE , 0 ) ;
		totalWeight -= zD -> GetWeight( WEIGHT_WHITE , 0 );			// 總重量

		if( b -> index == 12 || b -> index == 16 )
		{
			f[ b->index*3 + 1 ] -= 500.0;
			totalWeight -= 500.0;
		}
	}
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( !virtualBalls[i] -> isBottom )
		{	
			f[ virtualBalls[i]->index*3 + 1 ] -= SEGMENT_WEIGHT ;	// 不在底部的碎片，加上重量
			totalWeight -= SEGMENT_WEIGHT;							// 總重量
		}
		else
		{
			totalWeight -= f[ virtualBalls[i]->index*3 + 1 ];	// 把底部的碎片的重量扣掉
			f[ virtualBalls[i]->index*3 + 1 ] = 0;
		}
	}

//設定已知受力和位移
	//SetIfKnown();
	knF = new bool[ bCount*3 ];		for(int i=0;i<bCount*3;i++)knF[i]=true;    //大多外力都已知
	knDisp = new bool[ bCount*3 ];	for(int i=0;i<bCount*3;i++)knDisp[i]=false;//大多位移都未知
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{	
			knF[ realBCount*3 + i*3 + 1 ] = false;		//底部的y方向外力未知
			knDisp[ realBCount*3 + i*3 + 1 ] = true;	//底部的y方向位移已知
		}
	}

	// SD
	//knF[ 0 ] = false;		//底部的y方向外力未知
	//knF[ 1 ] = false;		//底部的y方向外力未知
	//knF[ 2 ] = false;		//底部的y方向外力未知
	//knF[ 4 ] = false;		//底部的y方向外力未知
	//knF[ 7 ] = false;		//底部的y方向外力未知
	//knDisp[ 0 ] = true;		//底部的y方向位移已知
	//knDisp[ 1 ] = true;		//底部的y方向位移已知
	//knDisp[ 2 ] = true;		//底部的y方向位移已知
	//knDisp[ 4 ] = true;		//底部的y方向位移已知
	//knDisp[ 7 ] = true;		//底部的y方向位移已知
	//f[ 1 ] = 0;
	//f[ 4 ] = 0;
	//f[ 7 ] = 0;
	totalWeight = f[ 10 ];	// 只計算頂球



//拿第一個底部VirtualBall當作固定點
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{
			knF[ realBCount*3 + i*3 ] = false;		// 底部固定點的外力未知
			knF[ realBCount*3 + i*3 + 1 ] = false;
			knF[ realBCount*3 + i*3 + 2 ] = false;
			knDisp[ realBCount*3 + i*3 ] = true;		// 底部固定點的位移已知
			knDisp[ realBCount*3 + i*3 + 1 ] = true;
			knDisp[ realBCount*3 + i*3 + 2 ] = true;
			break;
		}
	}
	knFAmount = 0;
	for( int i = 0; i < bCount*3; i++ )
	{
		if( !knF[i] ) knFAmount++;
	}


	return true;
}

;	int StructureAnalyzer::GetTowardBallIndex( ZomeConnection* c )
{
	if( c -> towardIndex == -1 )
	{
		for( int i = 0; i < virtualBalls.size(); i++ )
		{
			if( virtualBalls[i] -> conn[0] == c ||
				virtualBalls[i] -> conn[1] == c ||
				virtualBalls[i] -> conn[2] == c ||
				virtualBalls[i] -> conn[3] == c )
			{
				return virtualBalls[i] -> index;
			}
		}
		std::cout << "int StructureAnalyzer::GetTowardBallIndex出錯：沒有找到virtualBall" << std::endl;
	}
	return c -> towardIndex;
}

;	float StructureAnalyzer::GetStiffness( ZomeConnection* c )
{
	// E * A / L
	//return 1. / zD -> GetFaceLength( c -> fromFace , c -> size ) * SCALE;
	return YOUNG_MODULUS * 10.0 / zD -> GetFaceLength( c -> fromFace , c -> size );
}

bool StructureAnalyzer::SolveMatrix()
{


//		k =
//		|	K(已知外力，不知位移)	|	K(已知外力，已知位移)	|	| 不知位移	| =	|已知外力|
//		|	K(不知外力，不知位移)	|	K(不知外力，已知位移)	|	|	  0		| 	|不知外力|
//
//		先把下面兩個刪掉，再把右上刪掉，再解方程式

	//DebugPrintStiffness( stiffMat , f );

//先刪去不用的列(該列的受力未知，因此沒有變形)
	//DeleteUnknownForce();
	/*
	SM tempM = *stiffMat;
	V tempF = *f;
	for( int i = 0; i < bCount * 3; i++ )
	{
		if( !knF[i] )//受力未知，所以變形已知(無變形)，舊不需要計算變形
		{
			tempM.row(i) = tempM.row(i-1);
			tempF[i] = tempF[i-1];
		}
	}
//	要把已知位移為0的部分，系數歸零
	for( int j = 0; j < disp -> size(); j++ )
	{
		if( knDisp[j] )
		{
			for( int i = 0; i < f -> size(); i++ )
			{
				tempM.coeffRef(i,j) = 0;
			}
		}
	}
	*/
	//ExtractKnownFUnknownDisp();
	Eigen::SparseMatrix<double> tempM( stiffMat.rows()-knFAmount , stiffMat.rows()-knFAmount );
	tempM.makeCompressed();
	std::vector< Eigen::Triplet<double> > tempMCoef;
	Eigen::VectorXd tempF( stiffMat.rows()-knFAmount );
	Eigen::VectorXd tempDisp;
	int ii=0;
	for( int i = 0; i < stiffMat.rows(); i++ )
	{
		if( !knF[i] ) continue;
		int jj = 0;
		for( int j = 0; j < stiffMat.cols(); j++ )
		{
			if( !knF[j] ) continue;
			if( stiffMat.coeffRef(i,j) != 0 )
			{
				tempMCoef.push_back( Eigen::Triplet<double>( ii,jj,stiffMat.coeffRef(i,j) ) );
			}
			jj++;
		}
		tempF[ii] = f[i];
		ii++;
	}
	std::cout << "外力：" << tempF;
	tempM.setFromTriplets(tempMCoef.begin(),tempMCoef.end());
	DebugPrintStiffness( &tempM , &tempF );


// 自己寫一個LU
	//StiffMatrixLU();
	SparseLU<Eigen::SparseMatrix<double> > sparseLU(tempM);
	sparseLU.PrintPLU();
	
	
	tempDisp = sparseLU.Solve(tempF);


	std::cout << "位移：" << tempDisp;

	//PutBackDisp();
	ii=0;
	for( int i = 0; i < disp.size(); i++ )
	{
		if( !knF[i] ) continue;
		disp[i] = tempDisp[ii];
		ii++;
	}


	f = stiffMat * disp;

	   
	std::cout << "位移：" << disp;
	std::cout << "外力：" << f;

	float debugWeight = 0;
	for( int i = 0; i < f.size(); i++ )
	{
		if( !knF[i] )
		{
			debugWeight += f[i];
		}
	}
	std::cout << "總重量為" << totalWeight << "g，反力的總量是" << debugWeight << "g" << std::endl;


	return true;
}

;	bool StructureAnalyzer::DebugPrintStiffness( Eigen::SparseMatrix<double>* tempM , Eigen::VectorXd* v )
	{
		FILE* formulaFile = fopen("DebugStiffness.txt", "w");
		for( int i = 0; i < tempM -> rows(); i++ )
		{
			for( int j = 0; j < tempM -> cols(); j++ )
			{
				fprintf(formulaFile, "%6.6f ", tempM -> coeffRef(i,j) );
			}
			fprintf(formulaFile, "\n");
		}

		for( int i = 0; i < v -> size(); i++ )
		{
			fprintf(formulaFile, "%6.3f\n", (*v)(i) );
		}

		fclose( formulaFile );
		return true;


	}

bool StructureAnalyzer::SolveInnerForce()
{
	int debugCount = 0;
	//拿每個竿子的兩端
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		//std::cout << "debugCount" << debugCount << std::endl;


// 取他的三軸未移
		Eigen::RowVectorXf disp6(6);	// (投x,投y,投z,尾x,尾y,尾z)
		int from = i -> fromIndex;		//
		int to = GetTowardBallIndex(i);		//

		std::cout << "第" << i -> index		<< "根竿子頭位移為(" 
				  << disp[ from*3     ]		<< "," 
				  << disp[ from*3 + 1 ]		<< ","
				  << disp[ from*3 + 2 ]		<< "),尾位移為("
				  << disp[ to*3       ]		<< ","
				  << disp[ to*3 + 1   ]		<< ","
				  << disp[ to*3 + 2   ]		<< ")" << std::endl;

		disp6 << disp[ from*3 ] , disp[ from*3 + 1 ] , disp[ from*3 + 2 ] ,
				 disp[ to*3   ] , disp[ to*3 + 1   ] , disp[ to*3 + 2   ];

// 算出他的k'T
		/*     
		p' = k'T u (竿子內力 = k'T * 竿子未移)
		k'T =	|	x	y	z	-x	-y	-z	|
				|	-x	-y	-z	x	y	z	|
		只取第一列
		*/
		float res[3] = { i -> rotation[0] ,
						 i -> rotation[1] ,
						 i -> rotation[2] };

		Eigen::RowVectorXf localK_T(6);
		localK_T << res[0] , res[1] , res[2] , 
					-res[0] , -res[1] , -res[2];



//算出內力
		// p' = k'T u (竿子內力 = k'T * 竿子未移)
		innerF[ i -> index ] = disp6.dot( localK_T ) * GetStiffness(i);

		std::cout << "第" << i -> index << "根竿子的內力為" << innerF[ i -> index ] << "g" << std::endl;
		debugCount++;
	}

	return true;
}

bool StructureAnalyzer::SDSolveInnerForce()
{
	int debugCount = 0;
	//拿每個竿子的兩端
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{

// 取他的三軸未移
		Eigen::RowVectorXd disp6(6);	// (投x,投y,投z,尾x,尾y,尾z)
		int from = i -> fromIndex;		//
		int to = GetTowardBallIndex(i);		//

		std::cout << "第" << i -> index		<< "根竿子頭位移為(" 
				  << disp[ from*3     ]		<< "," 
				  << disp[ from*3 + 1 ]		<< ","
				  << disp[ from*3 + 2 ]		<< "),尾位移為("
				  << disp[ to*3       ]		<< ","
				  << disp[ to*3 + 1   ]		<< ","
				  << disp[ to*3 + 2   ]		<< ")" << std::endl;

		disp6 << disp[ from*3 ] , disp[ from*3 + 1 ] , disp[ from*3 + 2 ] ,
				 disp[ to*3   ] , disp[ to*3 + 1   ] , disp[ to*3 + 2   ];

// 算出他的k'T
		/*     
		p' = k'T u (竿子內力 = k'T * 竿子未移)
		k'T =	|	x	y	z	-x	-y	-z	|
				|	-x	-y	-z	x	y	z	|
		只取第一列
		*/
		float res[3] = {0,0,0};
		zD -> GetFaceDirection( i -> fromFace , res );

		Eigen::RowVectorXd localK_T(6);
		localK_T << res[0] , res[1] , res[2] , 
					-res[0] , -res[1] , -res[2];



//算出內力
		// p' = k'T u (竿子內力 = k'T * 竿子未移)
		innerF[ i -> index ] = disp6.dot( localK_T ) * GetStiffness(i);

		std::cout << "第" << i -> index << "根竿子的內力為" << innerF[ i -> index ] << "g" << std::endl;
		debugCount++;
	}

	return true;
}

bool StructureAnalyzer::PrintAnalysis()
{
	//debug輸出結果
	//PrintPolynomialList();
	//debug
	FILE* formulaFile = fopen("StructureFormulaSolved.txt", "w");
	fprintf(formulaFile,"節點位移\n");
	for( int i = 0; i < disp.size(); i++ )
	{
		fprintf(formulaFile, "%d : %12.12lf \n", i , disp[i] );
	}

	fprintf(formulaFile,"節點受力(驗算)\n");
	for( int i = 0; i < disp.size(); i++ )
	{
		fprintf(formulaFile, "%d : %12.12lf \n", i , f[i] );
	}

	fprintf(formulaFile,"竿子內力\n");
	for( int i = 0; i < innerF.size(); i++ )
	{
		fprintf(formulaFile, "%d : %12.12lf g\n", i , innerF[i] );
	}

	
	fclose( formulaFile );
	return true;
}

bool StructureAnalyzer::MapToMaterial()
{

	for( int i = 0; i < innerF.size(); i++ )
	{
		
		for( std:: map<int,int>::iterator it = cons -> matIndex2Index -> begin(); 
					 it != cons -> matIndex2Index -> end();
					 it++ )
		{
			if( i == it -> second/*(obj檔裡面的面數編號,ZomeConnetion編號)*/ )
			{
				matForce.insert( std::pair< int , float >( it -> first , innerF[i] * FORCE_SCALE ) );//(obj檔裡面的面數編號,受力大小)
				//將這根桿件的受力，編上材質順序的編號。

				std::cout << "將第" << i << "根竿子找到他在材質上的順序編號為" 
							<< it -> first << "，然後輸入其受力為" << innerF[i] << "g" << std::endl;
			}
		}
	}


	//	matIndex2Index (obj檔裡面的面數編號,ZomeConnetion編號)
	//	matForce	   (obj檔裡面的面數編號,受力大小)

// 這邊尋找沒有編號受力的竿子，並把他的受力改為999
	for( ZomeConnection* z = cons -> originalConn; z != NULL; z = z -> link )
	{
		bool existInMatForce = false;
		//先找出這一個竿子在obj中的面數編號
		int faceIndex = -1;
		for( std:: map<int,int>::iterator it = cons -> matIndex2Index -> begin(); 
			it != cons -> matIndex2Index -> end();
			it++ )/*(obj檔裡面的面數編號,ZomeConnetion編號)*/ 
		{
			if( it -> second == z -> index )
			{
				faceIndex = it -> first;
				break;
			}
		}
		if( faceIndex == -1 )
		{
			std::cout << "ZomeGenerator::ZomeStructureAnalyze()出錯：沒有找到這個竿子在obj中的面數編號" << std::endl;
		}
		//在找出這個面數編號是否有在matForce裡，儲存受力大小
		for( std:: map<int,float>::iterator it = matForce.begin(); 
			 it != matForce.end();
			 it++ )/*(obj檔裡面的面數編號,受力大小)*/ 
		{
			if( it -> first == faceIndex )//竿子編號
			{
				existInMatForce = true;
			}
		}
		//如果沒有這個編號，代表眉算出他的受力
		if( !existInMatForce )//
		{
			matForce.insert( std::pair< int , float >( faceIndex , 1000.0 ) );
			std::cout << "ZomeGenerator::ZomeStructureAnalyze()出錯：第" << z -> index << "跟竿子，其面數編號為" 
					  << faceIndex << "，沒有算出受力大小" << std::endl;
		}

	}
	return true;
}

bool StructureAnalyzer::Analyze()
{
	
	FindLowestHeight();
	
	//建立連接表面的點的量
	SetVirtualBalls();
	
	//開始列多項式，對每一科求建立一個三個式子，每個點分別網x、y、z各建一個式子
	SetMatrix();
	
	//解出位移解出外力
	SolveMatrix();

	//用未移算出內力
	SolveInnerForce();

	//輸出到constructor
	PrintAnalysis();

	//轉換成才職
	MapToMaterial();

	return true;
}

bool StructureAnalyzer::SDAnalyze()
{
	
	
	//建立連接表面的點的量
	//SetVirtualBalls();
	SDSetVirtualBalls();

	//開始列多項式，對每一科求建立一個三個式子，每個點分別網x、y、z各建一個式子
	SDSetMatrix();
	
	//解出位移解出外力
	SolveMatrix();

	//用未移算出內力
	SDSolveInnerForce();

	//輸出到constructor
	PrintAnalysis();

	//轉換成才職
	MapToMaterial();

	return true;
}