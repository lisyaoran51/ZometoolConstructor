#include "VirtualForce.h"

/*int index;
	int color;
	int size;
	float scale;
	float cosAlpha;
	bool isSolved;
	float solution;*/

//=====================================Relation//=====================================
Relation::Relation( int rConnIndex , float coef , float cons )
{
	relatedConnIndex = rConnIndex;
	coefficient = coef;
	constant = cons;
}
//=====================================Relation//=====================================


//=====================================ConnectionData//=====================================竿子資料
ConnectionData::ConnectionData()
{};

ConnectionData::ConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection )
{
	index = i;
	sourceBall = sBall;
	isBall = true;
	connection = oConnection -> GetConn( i );
	//拿ballFace
	ballFace = -1;
	for( int fi = 0; fi < 62; fi++ )
	{
		if( sourceBall -> faceConnection[ fi ] != NULL )
		{
			if( sourceBall -> faceConnection[ fi ] == connection )
			{
				ballFace = fi;
			}
		}
	}
	if( ballFace == -1 )
	{
		std::cout << "ConnectionData::ConnectionData出錯：沒有找到來圓球ballFace" << std::endl;
	}


	//cosAlpha = cAlpha;

	color = connection -> color;
	size = connection -> size;
	//以下來自ZomeDirection裡GetColorLength()
	GLfloat length;
	if ( color == COLOR_BLUE )
	{
		if ( size == SIZE_S ) length = 1 * SCALE;//46.3
		if ( size == SIZE_M ) length = GOLDEN * SCALE;//74.9
		if ( size == SIZE_L ) length = GOLDEN * GOLDEN * SCALE;//121.2
	}
	else if ( color == COLOR_RED )
	{
		if ( size == SIZE_S ) length = sqrt( 2 + GOLDEN ) / 2 * SCALE;//43.8
		if ( size == SIZE_M ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * SCALE;//71
		if ( size == SIZE_L ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * GOLDEN * SCALE;//115
	}
	else if ( color == COLOR_YELLOW )
	{
		if ( size == SIZE_S ) length = sqrt(3) / 2 * SCALE;//40.1
		if ( size == SIZE_M ) length = sqrt(3) / 2 * GOLDEN * SCALE;//64.8
		if ( size == SIZE_L ) length = sqrt(3) / 2 * GOLDEN * GOLDEN * SCALE;//104.9
	}
	//=========
	scale = length / SCALE;
	isSolved = false;

}

ConnectionData::ConnectionData( int i , VirtualBall* sVirtualBall , ZomeConnection* oConnection )
{
	/*==================================================ｏ
	||連接虛擬球的竿子資料，有可能是反力，所以要特別判斷||
	*///================================================ｏ

	index = i;
	sourceVirtualBall = sVirtualBall;
	isBall = false;
	ballFace = -1;

	//接下來判斷這個竿子是不是代表反力，是的話他的index會<0
	if( i < -1 )
	{
		//這是一個反力，不是竿子
		connection = NULL;
		color = COLOR_BLUE;
		size = SIZE_S;
	}
	else
	{
		connection = oConnection -> GetConn( i );
		color = connection -> color;
		size = connection -> size;

		
		//拿ballFace
		for( int fi = 0; fi < 6; fi++ )
		{
			if( sourceVirtualBall -> conn[ fi ] != NULL )
			{
				if(  sourceVirtualBall -> conn[ fi ] == connection )
				{
					ballFace = sourceVirtualBall -> connectFace[fi];
				}
			}
		}
		if( ballFace == -1 )
		{
			std::cout << "ConnectionData::ConnectionData出錯：沒有找到來圓球ballFace" << std::endl;
		}
	}

	

	//cosAlpha = cAlpha;

	//以下來自ZomeDirection裡GetColorLength()
	GLfloat length;
	if ( color == COLOR_BLUE )
	{
		if ( size == SIZE_S ) length = 1 * SCALE;//46.3
		if ( size == SIZE_M ) length = GOLDEN * SCALE;//74.9
		if ( size == SIZE_L ) length = GOLDEN * GOLDEN * SCALE;//121.2
	}
	else if ( color == COLOR_RED )
	{
		if ( size == SIZE_S ) length = sqrt( 2 + GOLDEN ) / 2 * SCALE;//43.8
		if ( size == SIZE_M ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * SCALE;//71
		if ( size == SIZE_L ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * GOLDEN * SCALE;//115
	}
	else if ( color == COLOR_YELLOW )
	{
		if ( size == SIZE_S ) length = sqrt(3) / 2 * SCALE;//40.1
		if ( size == SIZE_M ) length = sqrt(3) / 2 * GOLDEN * SCALE;//64.8
		if ( size == SIZE_L ) length = sqrt(3) / 2 * GOLDEN * GOLDEN * SCALE;//104.9
	}
	//=========
	scale = length / SCALE;
	isSolved = false;

}
//=====================================ConnectionData//=====================================


//=====================================PolynomialConnectionData//=====================================竿子多項是資料
PolynomialConnectionData::PolynomialConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection , float c ):
ConnectionData( i , sBall , oConnection ) , coefficient( c )
{}

PolynomialConnectionData::PolynomialConnectionData( int i , VirtualBall* sVirtualBall , ZomeConnection* oConnection , float c ):
ConnectionData( i , sVirtualBall, oConnection ) , coefficient( c )
{}
//=====================================PolynomialConnectionData//=====================================


//=====================================VirtualWorkConnectionData//=====================================乾子需供資料
VirtualWorkConnectionData::VirtualWorkConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection ): ConnectionData( i , sBall , oConnection )
{
	cosAlpha = 0;
}

VirtualWorkConnectionData::VirtualWorkConnectionData( PolynomialConnectionData pConnectionData )
{

	index = pConnectionData .index;
	isBall = pConnectionData.isBall;

	if( isBall )
	{
		sourceBall = pConnectionData.sourceBall;
	}
	else
	{
		sourceVirtualBall = pConnectionData.sourceVirtualBall;
	}
	connection = pConnectionData.connection;
	//拿ballFace
	ballFace = pConnectionData.ballFace;
	color = pConnectionData.color;
	size = pConnectionData.size;
	scale = pConnectionData.scale;
	isSolved = false;

	cosAlpha = 0;
}

bool VirtualWorkConnectionData::ComputeCosAlpha( GLfloat* outerForceDirection )
{
	//cosine公式是 (V1 Dot V2)/(V1長度xV2長度)

	//(BallFace方向)
	//「   F (外力方向)
	//  \  ^
	//   \ |
	//    \|
	//     o
	//  夾角應該要是ballFace的反方向與外力的夾角，內力才會與外力箱反

	GLfloat innerForceDirection[3];
	if( ballFace >= 0 )
	{
		StaticZomeDirection::GetFaceDirection( ballFace , innerForceDirection );//拿方項
	}
	else
	{
		MathHelper::SetVector( innerForceDirection , 0 , -1 , 0 );
	}
	//MathHelper::VectorReverse( innerForceDirection );//把方向反轉

	cosAlpha = MathHelper::VectorDot( innerForceDirection , outerForceDirection ) / 
			   MathHelper::VectorAbsolution( innerForceDirection ) /
			   MathHelper::VectorAbsolution( outerForceDirection );//cosine公式是 (V1 Dot V2)/(V1長度xV2長度)

	return true;
}

//=====================================VirtualWorkConnectionData//=====================================



//=====================================BallVirtualForceData//=====================================球需供資料
BallVirtualForceData::BallVirtualForceData( PolynomialList* pDimension1 , PolynomialList* pDimension2 , PolynomialList* pDimension3 , ZomeStructure* b , ZomeConnection* oConnection )
{
	/*int ballIndex;
	std::vector<ConnectionData> connections[3];//三維的所有桿件做功資料
	float displacement[3];//三維的偏移
	float OuterForce[3];//三位的外力
	bool solved;*/


	
	isBall = true;
	ball = b;
	originalConn = oConnection;
	ballIndex = ball -> index;
	solved = false;
	
	std::cout << "建立第" << ballIndex << "顆球的多項式球資料" << std::endl;

	//把所有桿件編號輸入進去======
	for( std::map< int , float >::iterator i = pDimension1 -> polynomial.begin(); i != pDimension1 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension2 -> polynomial.begin(); i != pDimension2 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension3 -> polynomial.begin(); i != pDimension3 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 )connectionNum.insert( i -> first );
	}
	//=============================

	//件力第0維度
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension1 -> polynomial.find( *i ) == pDimension1 -> polynomial.end() )//如果多像是沒有，就建立一個cosA式0的ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , pDimension1 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
	}
	//件力第1維度
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension2 -> polynomial.find( *i ) == pDimension2 -> polynomial.end() )//如果多像是沒有，就建立一個cosA式0的ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , pDimension2 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
	}
	//件力第2維度
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension3 -> polynomial.find( *i ) == pDimension3 -> polynomial.end() )//如果多像是沒有，就建立一個cosA式0的ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , pDimension3 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
	}

	//輸入三維的外力
	///......

}

BallVirtualForceData::BallVirtualForceData( PolynomialList* pDimension1 , PolynomialList* pDimension2 , PolynomialList* pDimension3 , VirtualBall* vBall , ZomeConnection* oConnection )
{
	/*int ballIndex;
	std::vector<ConnectionData> connections[3];//三維的所有桿件做功資料
	float displacement[3];//三維的偏移
	float OuterForce[3];//三位的外力
	bool solved;*/
	
	isBall = false;
	virtualBall = vBall;
	originalConn = oConnection;
	ballIndex = virtualBall -> index;
	solved = false;
	
	std::cout << "建立第" << ballIndex << "顆虛擬球的多項式球資料" << std::endl;

	//把所有桿件編號輸入進去======
	for( std::map< int , float >::iterator i = pDimension1 -> polynomial.begin(); i != pDimension1 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 || i -> first < -1 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension2 -> polynomial.begin(); i != pDimension2 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 || i -> first < -1 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension3 -> polynomial.begin(); i != pDimension3 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 || i -> first < -1 )connectionNum.insert( i -> first );
	}
	//=============================

	//件力第0維度
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension1 -> polynomial.find( *i ) == pDimension1 -> polynomial.end() )//如果多像是沒有，就建立一個cosA式0的ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , pDimension1 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
	}
	//件力第1維度
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension2 -> polynomial.find( *i ) == pDimension2 -> polynomial.end() )//如果多像是沒有，就建立一個cosA式0的ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , pDimension2 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
	}
	//件力第2維度
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension3 -> polynomial.find( *i ) == pDimension3 -> polynomial.end() )//如果多像是沒有，就建立一個cosA式0的ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , pDimension3 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
	}

	//輸入三維的外力
	///......

}

bool BallVirtualForceData::MakeVirtualWorkConnectionData()
{
	//紀錄哪些竿子還沒有解，還有把多項是的資料輸入到虛功法竿子資料理
	
	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( !connections[0].at(j).isSolved ) 
		{
			virtualWorkConnections.push_back( VirtualWorkConnectionData( connections[0].at(j) ) );
		}
	}


	return true;
}

int BallVirtualForceData::GetVirtualWorkConnectionIndexFromConnIndex( int connIndex )
{
	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		if( virtualWorkConnections[i].index == connIndex )
		{
			return i;
		}
	}
	std::cout << "BallVirtualForceData::GetVirtualWorkConnectionIndexFromConnIndex出錯：找不到虛功法竿子有那個編號" << std::endl;
	return -1;

}

VirtualWorkConnectionData* BallVirtualForceData::GetVirtualWorkConnection( int index )
{
	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		if( virtualWorkConnections[i].index == index )
		{
			return &(virtualWorkConnections[i]);
		}
	}
}

bool BallVirtualForceData::ComputeCosAlpha()
{
	//找出外力合力方向

	GLfloat outerForceDirection[3];
	MathHelper::CopyVector( outerForceDirection , OuterForce );
	MathHelper::VectorNormalize( outerForceDirection );

	std::cout << "計算延外力方向的餘弦" << std::endl;

	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		if( !virtualWorkConnections[i].isSolved )
		{
			virtualWorkConnections[i].ComputeCosAlpha( outerForceDirection );

			std::cout << "   第" << virtualWorkConnections[i].index << "根竿子延外力方向的cosAlpha為" << virtualWorkConnections[i].cosAlpha << std::endl;
		}
		else
		{
			virtualWorkConnections[i].cosAlpha = 0;
		}
	}
	return true;
}

bool BallVirtualForceData::Analyze()
{
	if( solved )
	{
		std::cout << "BallVirtualForceData::Analyze()出錯：已經解過了" << std::endl;
		return false;
	}

	//紀錄哪些竿子還沒有解，還有把多項是的資料輸入到虛功法竿子資料理
	std::vector<int> solvedConnections;
	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( connections[0].at(j).isSolved ) 
		{
			solvedConnections.push_back( connections[0].at(j).index );
		}
	}
	

	//算出未解的竿子的夾角餘弦
	//ComputeCosAlpha();//百到外面去執行

	//算虛功法====================================================================
		//變形 v  = (Fl/EA)/( ( cos(a)2/Scale(a) + ... + cos(z)2/Scale(z) ))
		//受力 Fi = ( cos(i)/Scale(i) ) * F / 變形v
		//算出偏移v
	//累家每一個竿子造成的虛位移，
	float displacementDivisor = 0;
	float tempOuterForce = 0; //- Const(2) * Cos(2) - Const(3) * Cos(3)...
	//這邊要改
	for( int i = 0; i < CosAlphaOrder.size(); i++ )
	{
		//if( virtualWorkConnections.at(i).isSolved ) continue;//須公應該要只做有
		if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() != 0 )
		{
			if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() == 1 )
			{
				//Rate(2) * ( cos(1) / Scale(1) ) * cos(2)

				int relatedIndex = GetVirtualWorkConnectionIndexFromConnIndex( virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].relatedConnIndex ); 
				float debugcoefficient = virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].coefficient;
				float debugcosAlpha	   = virtualWorkConnections[   relatedIndex   ].cosAlpha;
				float debugscale       = virtualWorkConnections[   relatedIndex   ].scale;
				float debugcosAlpha2   = virtualWorkConnections[   relatedIndex   ].cosAlpha;

				float temp = debugcoefficient * debugcosAlpha / debugscale *debugcosAlpha2;

				displacementDivisor += virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].coefficient *
									   virtualWorkConnections[   relatedIndex   ].cosAlpha /
									   virtualWorkConnections[   relatedIndex   ].scale *
									   virtualWorkConnections[   relatedIndex   ].cosAlpha;

				//F - Const(2) * cos(2)
				tempOuterForce -= virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].constant * 
								  virtualWorkConnections[   relatedIndex   ].cosAlpha;
			}
			else
			{
				//要看一下一開始為什麼會造成兩個relation
				//		   F(i) +   Coef(j) * F(        j        ) +   Const  = 0  →F(i) = -Coef(j) * F(j) - Const
				//					  ∥ 				∥ 				∥ 
				//		        -coefficient    relatedConnIndex    -constant
				//							 ╳							|			
				//Relation( relatedConnIndex ,  coefficient ,        constant )

				//第i肝的受力是由第j肝推倒而得，兩乾的關係為 (i肝受力) = (coefficient) 成以 (j肝受力) 家 (const)
				
				//virtualWorkConnections[ CosAlphaOrder[i] ].relation[0] -> 第j肝的 coefficient 和 const


				std::cout << "BallVirtualForceData::Analyze()出錯：一個竿子有兩個觀系" << std::endl;
				return false;
			}

		}
		else
		{
			float temp = pow( virtualWorkConnections[ CosAlphaOrder[i] ].cosAlpha , 2 ) / 
							  virtualWorkConnections[ CosAlphaOrder[i] ].scale;
			// cos(a)2/Scale(a)
			displacementDivisor += pow( virtualWorkConnections[ CosAlphaOrder[i] ].cosAlpha , 2 ) / 
										virtualWorkConnections[ CosAlphaOrder[i] ].scale;
		}
	}
	if( displacementDivisor == 0 )
	{
		std::cout << "BallVirtualForceData::Analyze()出錯：分母為0，沒有未知的竿子" << std::endl;
	}

	//位移 = EAF / [ cos(a)2/Scale(a) + ... + cos(a)2/Scale(a) ]
	displacement = 1.0 / displacementDivisor;


	//算出解
	for( int i = 0; i < CosAlphaOrder.size(); i++ )//因為關係的順序是 F1 = 系數*F2 + 常數 所以解虛功的時候，順序要是3->2->1
	{//\\順序是大中小，觀系式是 小=a大+c ，所已先解大的在解小的
		if( virtualWorkConnections[ CosAlphaOrder[i] ].isSolved ) continue;
		//如果是有relation的竿子，就用另一個竿子的姐代進去
		if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() != 0 )
		{
			if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() == 1 )
			{
				//(i)  =  relatedConnIndex * coefficient + constant

				//										F- Const(2) * Cos(2) - Const(3) * Cos(3) - ...												    Cos(1)
		        // Fi =  ----------------------------------------------------------------------------------------------------------------- * Coef(i) * -------- + Const(i)
				//         Cos2(1) / Scale(1) + Coef(2) * ( Cos(1) / Scale(1) ) * Cos(2) + Coef(3) * ( Cos(1) / Scale(1) ) * Cos(3) + ...			   Scale(1)

				float relationCosAlpha;
				float relatedScale;
				float relatedConnForce;//相關的那個竿子的力
				//=========================================================================
				for( int j = 0; j < virtualWorkConnections.size(); j++ )//找出那個力
				{
					if( virtualWorkConnections[j].index == virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].relatedConnIndex )
					{
						if( !virtualWorkConnections[j].isSolved )
						{
							std::cout << "BallVirtualForceData::Analyze出錯：相關的那個竿子的姐沒有先解出來" << std::endl;
						}
						relationCosAlpha = virtualWorkConnections[j].cosAlpha;
						relatedScale	 = virtualWorkConnections[j].scale;
						relatedConnForce = virtualWorkConnections[j].solution;
					}
				}
				//==========================================================================

				/*//這一行跟下面那行的意思一樣
				virtualWorkConnections.at(i).solution = virtualWorkConnections[i].relation[0].coefficient *				  //Coef(i)
														relationCosAlpha / relatedScale *								  //cos(j)/Scale(j)
														( MathHelper::VectorAbsolution( OuterForce ) + tempOuterForce ) * //F - Const(2) * Cos(2) - Const(3) * Cos(3)
														displacement +													  //變形v
														virtualWorkConnections[i].relation[0].constant;					  //Const(i)
				*/
				virtualWorkConnections[ CosAlphaOrder[i] ].solution = 
						virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].coefficient * //Coef(i) *
						relatedConnForce +													 //Fj +
						virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].constant;	 //Const(i)
				virtualWorkConnections[ CosAlphaOrder[i] ].isSolved = true;

			}
			else
			{
				std::cout << "這邊還眉寫" << std::endl;
				//return false;
			}
		}
		else
		{
			//受力 Fi = ( cos(i)/Scale(i) ) * F * 變形v
			virtualWorkConnections[ CosAlphaOrder[i] ].solution = virtualWorkConnections[ CosAlphaOrder[i] ].cosAlpha / 
																  virtualWorkConnections[ CosAlphaOrder[i] ].scale * //cos(i)/Scale(i)
																  ( MathHelper::VectorAbsolution( OuterForce ) + tempOuterForce ) * //F - Const(2) * Cos(2) - Const(3) * Cos(3)
																  displacement;  //變形v
			virtualWorkConnections[ CosAlphaOrder[i] ].isSolved = true;
		}
	}

	if( isBall )
	{
		std::cout << "==================第" << ballIndex << "顆球的虛功法解" << "=================="<< std::endl;
	}
	else
	{
		std::cout << "==================第" << ballIndex << "個外接點的虛功法解" << "=================="<< std::endl;
	}

	//把三為的解加總
	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		/*if( std::find( solvedConnections.begin() , solvedConnections.end() , connections[0].at(j).index ) != 
			solvedConnections.end() )//在已解出的竿子裡面有找到，就跳過
		{
			continue;
		}*/

		//放進去解
		solutions.insert( std::pair<int,float>( virtualWorkConnections[ CosAlphaOrder[i] ].index , virtualWorkConnections[ CosAlphaOrder[i] ].solution ) );
		std::cout << "||第" << virtualWorkConnections[ CosAlphaOrder[i] ].index << "根竿子的解是" << virtualWorkConnections[ CosAlphaOrder[i] ].solution << "g" << std::endl;
		//////////////////這個式測式，之後要刪掉
	}
	
	std::cout << "===================================================="<< std::endl;


	/*
	//對三個為度都坐一次
	for( int i = 0; i < 3; i++ )
	{	
		//變形 v  = (Fl/EA)/( ( cos(a)2/Scale(a) + ... + cos(z)2/Scale(z) ))
		//受力 Fi = ( cos(i)/Scale(i) ) * F / 變形v
		//算出偏移v
		//displacement[0] = 0;
		float displacementDivisor = 0;
		for( int j = 0; j < connections[i].size(); j++ )
		{
			if( connections[i].at(j).isSolved ) continue;
			// cos(a)2/Scale(a)
			displacementDivisor += pow( connections[i].at(j).cosAlpha , 2 ) / connections[i].at(j).scale;
		}
		if( displacementDivisor == 0 )
		{
			std::cout << "BallVirtualForceData::Analyze()出錯：分母為0，沒有未知的竿子" << std::endl;
		}


		displacement[i] = 1.0 / displacementDivisor;

		//算出解
		for( int j = 0; j < connections[i].size(); j++ )
		{
			if( connections[i].at(j).isSolved ) continue;
			//受力 Fi = ( cos(i)/Scale(i) ) * F / 變形v
			connections[i].at(j).solution = connections[i].at(j).cosAlpha / connections[i].at(j).scale * //cos(i)/Scale(i)
											OuterForce[i] / //F
											displacement[i];  //變形v
			connections[i].at(j).isSolved = true;
		}

		
	}

	if( isBall )
	{
		std::cout << "==================第" << ballIndex << "顆球的虛功法解" << "=================="<< std::endl;
	}
	else
	{
		std::cout << "==================第" << ballIndex << "個外接點的虛功法解" << "=================="<< std::endl;
	}

	//把三為的解加總
	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( std::find( solvedConnections.begin() , solvedConnections.end() , connections[0].at(j).index ) != 
			solvedConnections.end() )//在已解出的竿子裡面有找到，就跳過
		{
			continue;
		}

		float tempSolutionAddedUp = 0.0;
		for( int i = 0; i < 1; i++ )///////3為改成1 測式一下
		{
			tempSolutionAddedUp += connections[i].at(j).solution;
		}
		//放進去解
		solutionsAddedUp.insert( std::pair<int,float>( connections[0].at(j).index , tempSolutionAddedUp ) );
		std::cout << "||第" << connections[0].at(j).index << "根竿子的解是" << tempSolutionAddedUp << "g" << std::endl;
		break;//////////////////這個式測式，之後要刪掉
	}
	
	std::cout << "===================================================="<< std::endl;*/
}

bool BallVirtualForceData::hasConnection( int connIndex )
{
	if( connectionNum.find( connIndex ) != connectionNum.end() )
	{
		return true;
	}
	else return false;
}

bool BallVirtualForceData::Substitute( int connIndex , float solution )
{
	//外力=外力-變數解*系數

	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( connections[0].at(j).index == connIndex && !connections[0].at(j).isSolved )
		{
			for( int i = 0; i < 3; i++ )
			{
				connections[i].at(j).solution = solution;
				connections[i].at(j).isSolved = true;
				OuterForce[i] -= solution * connections[i].at(j).coefficient;
			}
		}
	}
	return true;
}
//=====================================BallVirtualForceData//=====================================




//=====================================VirtualForceAnalyzer//=====================================
//public:
VirtualForceAnalyzer::VirtualForceAnalyzer( PolynomialList* fPolynomial , int bAmount , ZomeConnection* oConnection , ZomeStructure* oBall )
{
	//輸入時一定要是還沒有簡化過的多項式
	firstPolynomial = fPolynomial;
	ballAmount = bAmount;
	originalConn = oConnection;
	originalBall = oBall;

	//先把所有式子輸入，每三個式子要擺進去一個 BallVirtualForceData 終，每個式子隔ballAmount的下一個式子，就是他的下個維度的式子
	int ballCount = 0;
	for( PolynomialList* p = fPolynomial; p != NULL; p = p -> link )
	{
		if( ballCount == bAmount ) break;
		
		
		//找第二個維度的多項式
		PolynomialList* polynomialDimension2 = p;
		for(int i = 0; i < bAmount; i++ ) polynomialDimension2 = polynomialDimension2 -> link;
		//加入
		
		//找第三個維度的多項式
		PolynomialList* polynomialDimension3 = polynomialDimension2;
		for(int i = 0; i < bAmount; i++ ) polynomialDimension3 = polynomialDimension3 -> link;
		//加入
		//tempBallVirtualForce.AddDimensionData( 2 , polynomialDimension3 );
		if( p -> isBall )
		{
			BallVirtualForceData tempBallVirtualForce( p , polynomialDimension2 , polynomialDimension3 , p -> sourceBall , originalConn );
			ballVirtualForceData.push_back( tempBallVirtualForce );
		}
		else if( p -> isVirtualBall )
		{
			BallVirtualForceData tempBallVirtualForce( p , polynomialDimension2 , polynomialDimension3 , p -> sourceVirtualBall , originalConn );
			ballVirtualForceData.push_back( tempBallVirtualForce );
		}
		else
		{
			std::cout << "VirtualForceAnalyzer::VirtualForceAnalyzer出錯：沒有找到來源的球" << std::endl;
		}
		
		ballCount++;
	}
}

bool VirtualForceAnalyzer::Analyze()
{
	//先更新每個球的資訊
	Renew();

	bool debugOnce = true;/////////////////////debug用

	//尋找最少未知數的球
	while ( hasBallUnsolved() /*&& debugOnce*/ )
	{
		int leastUnsolvedBallIndex = GetLeastUnsolvedBallIndex();
		int leastUnsolvedBallVectorIndex;
		for( int i = 0; i < ballVirtualForceData.size(); i++ )
		{
			if( ballVirtualForceData[i].ballIndex == leastUnsolvedBallIndex )
			{
				leastUnsolvedBallVectorIndex = i;
			}
		}

	//姐出那個球，然後重新解一次所有多項式
		//分析
		if( !VirtualWorkAnalyze( leastUnsolvedBallIndex ) ) return false;
		//代入
 		SubstituteSolution( leastUnsolvedBallIndex );
		//解其他式子
		firstPolynomial -> SolvePolynomialList();

		//更新每個球的資訊
		Renew();
		//debugOnce = false;/////////////////////debug用
	}


	
	return true;
	//在找下一個球
}
//private:
bool VirtualForceAnalyzer::Renew()
{
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p -> polynomial.size() == 2 )
		{


			int parameter;
			float solution;
			p -> DivideScalar();
			for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
			{
				if( i -> first != -1 )
				{
					parameter = i -> first;

				}
				else
				{
					solution = -i -> second;
				}
			}

			if( std::find( solvedConnections.begin(), 
						   solvedConnections.end() , 
						   parameter ) == solvedConnections.end() )
			{
				//代入每一顆球
				for( int i = 0; i < ballVirtualForceData.size(); i++ )
				{
					if( ballVirtualForceData[i].hasConnection( parameter ) )
					{
						ballVirtualForceData[i].Substitute( parameter , solution );
					}
				}
				//紀錄這顆球已經輸入過
				solvedConnections.push_back( parameter );
			}
		}
	}
	return true;
}

bool VirtualForceAnalyzer::hasBallUnsolved()
{
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		for( int j = 0; j < ballVirtualForceData[i].connections[0].size(); j++ )
		{
			if( !(ballVirtualForceData[i].connections[0].at(j).isSolved) )
			{
				return true;
			}
			if( !(ballVirtualForceData[i].connections[1].at(j).isSolved) )
			{
				return true;
			}
			if( !(ballVirtualForceData[i].connections[2].at(j).isSolved) )
			{
				return true;
			}
		}
	}
	return false;
}

int VirtualForceAnalyzer::GetLeastUnsolvedBallIndex()
{
	int leastBallVectorIndex;
	int leastBallIndex;
	int leastBallUnsolvedAmount = 999;

	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		int unsolvedAmount = 0;
		if( ballVirtualForceData[i].ballIndex < 0 ) continue;
		for( int j = 0; j < ballVirtualForceData[i].connections[0].size(); j++ )
		{
			bool isUnsolved = false;

			if( !(ballVirtualForceData[i].connections[0].at(j).isSolved) )
			{
				isUnsolved = true;
			}
			if( !(ballVirtualForceData[i].connections[1].at(j).isSolved) )
			{
				isUnsolved = true;
			}
			if( !(ballVirtualForceData[i].connections[2].at(j).isSolved) )
			{
				isUnsolved = true;
			}

			if( isUnsolved )
			{
				unsolvedAmount++;
			}
		}

		if( unsolvedAmount < leastBallUnsolvedAmount && //比較少
			unsolvedAmount > 0						 && //??
			unsolvedAmount < ballVirtualForceData[i].connections[0].size() )//沒有解的竿子數不等於所有的竿子數(眉半根竿子有解出來)
		{
			leastBallVectorIndex = i;
			leastBallIndex = ballVirtualForceData[i].ballIndex;
			leastBallUnsolvedAmount = unsolvedAmount;
		}
	}
	std::cout << "現在要列出最少未知數的球的所有資訊：" << std::endl;
	std::cout << "||球邊號為：" <<  leastBallIndex << "。" << std::endl;
	std::cout << "||所有肝子編號為：";
	for( std::set<int>::iterator i = ballVirtualForceData[ leastBallVectorIndex ].connectionNum.begin(); 
		 i != ballVirtualForceData[ leastBallVectorIndex ].connectionNum.end();
		 i++ )
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
	std::cout << "||每個竿子的資訊為：" << std::endl;

	for( int j = 0; j < ballVirtualForceData[ leastBallVectorIndex ].connections[0].size(); j++ )
	{
		std::cout << "||第" << ballVirtualForceData[ leastBallVectorIndex ].connections[0].at(j).index << "號竿子：";
		if( ballVirtualForceData[ leastBallVectorIndex ].connections[0].at(j).isSolved )
		{
			std::cout << "X：" << ballVirtualForceData[ leastBallVectorIndex ].connections[0].at(j).solution << "，";
			std::cout << "Y：" << ballVirtualForceData[ leastBallVectorIndex ].connections[1].at(j).solution << "，";
			std::cout << "Z：" << ballVirtualForceData[ leastBallVectorIndex ].connections[2].at(j).solution << "。" << std::endl;
		}
		else
		{
			std::cout << "未解出答案" << std::endl;
		}
	}
	std::cout << "||球受到外力為：";
	std::cout << "X:" << ballVirtualForceData[ leastBallVectorIndex ].OuterForce[0] << ", ";
	std::cout << "Y:" << ballVirtualForceData[ leastBallVectorIndex ].OuterForce[1] << ", ";
	std::cout << "Z:" << ballVirtualForceData[ leastBallVectorIndex ].OuterForce[2] << "。" << std::endl;

	std::cout << "||球的偏移量為：" << ballVirtualForceData[ leastBallVectorIndex ].displacement << "mm" << std::endl;
	


	std::cout << "找出最少未知數的球：#" << leastBallIndex << "，未知數有" << leastBallUnsolvedAmount << "個" << std::endl;
	return leastBallIndex;
}

bool VirtualForceAnalyzer::MakeConnectionRelation( int ballIndex )
{
	//找出那顆球在陣列中的位置
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == ballIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}

	if( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size() == 0 )
	{
		std::cout << "VirtualForceAnalyzer::MakeConnectionRelation出錯：沒有虛功資料" << std::endl;
		return false;
	}
	//幫常用的東西寫個代號
	std::vector<int>* CosAlphaOrder = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder);
	std::vector<VirtualWorkConnectionData>* virtualWorkConnections = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections);
	
	//好像眉用
	std::vector<int> virtualWorkConnIndex;
	for( int i = 0; i < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); i++ )
	{
		virtualWorkConnIndex.push_back( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].index );
	}

	
	//要先去查說哪一個的cosAlpha最大，然後用那個竿子來觀聯，排列順序是從大到小
	//CheckCosAlphaOrder();
	for( int i = 0; i < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); i++ )//尋找 1 -> 2 3
	{
		//找最大的cosAlpha，大的在前，小的在後，放vector
		int maxCosAlphaIndex;
		float maxCosAlpha = -1;
		for( int j = 0; j < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); j++ )
		{
			if( std::find( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.begin() , 
						   ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.end() , 
						   j ) == 
				ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.end() )//
			{
				if( abs( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[j].cosAlpha ) > 
					maxCosAlpha )
				{
					maxCosAlpha = abs( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[j].cosAlpha );
					maxCosAlphaIndex = j;
				}
			}
		}
		ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.push_back( maxCosAlphaIndex );
	}
	//
	std::cout << "未支竿子的CosAlpha大小順序為：";
	for( int i = 0; i < (*CosAlphaOrder).size(); i++ )
	{
		std::cout << (*CosAlphaOrder)[i] << "(第" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "根竿子) ";
		if( i == (*CosAlphaOrder).size() - 1 )
		{
			std::cout << "(大到小)" << std::endl;
		}
		else
		{
			std::cout << "→";
		}
	}

//=======================================================================================================================================
	//ConnectRelation();
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p -> polynomial.size() == 3 )
		{
			//改成趙CosAlpha大小順序來做官聯，從小到大，因為CosAlphaOrder本來的順序是從大到小，所已i要從後面往前
			for( int i = (*CosAlphaOrder).size() - 1; i >= 0 ; i-- )//尋找 小->終  大
			{																											 //     終 -> 大   的關係
				for( int j = i - 1; j >= 0; j-- )// 如果i是小 j 就是 中→大
				{
					if( p -> HasUnknown( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index ) && //這個式子同時有兩個乾子當位知數
						p -> HasUnknown( (*virtualWorkConnections)[ (*CosAlphaOrder)[j] ].index ) )//意思就是 小 = a大 + c
					{
						p -> DivideScalar( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index ); 
						//式子原狀應該是 Coef(i) * (i) + Coef(j) * (j) + Const = 0
						//調整系數後變成    1    * (i) + Coef(j) * (j) + Const = 0
						//                  (i)  =  relatedConnIndex * coefficient + constant
						//在j裡面儲存 relatedConnIndex = j 
						//			  coefficient = -Coef(j)
						//			  constant = -Const
						
						//█████████████████████████████
						//簡查這個關係有沒有已經背書入過↓↓↓↓↓↓↓↓↓↓↓↓↓
						bool isAlreadyFound = false;
						for( int k = 0; k < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); k++ )
						{
							if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[k].relatedConnIndex == (*virtualWorkConnections)[ (*CosAlphaOrder)[j] ].index )
							{
								std::cout << "這個關係已背書入：第" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "竿到第" << 
																	   (*virtualWorkConnections)[         j           ].index << "竿" << std::endl;
								isAlreadyFound = true;
							}
						}
						if( isAlreadyFound ) continue;
						//===============================↑↑↑↑↑↑↑↑↑↑↑↑↑
						//█████████████████████████████


						// F(i) +   Coef(j)   * F(       j        ) +   Const   = 0
						//			  ∥ 				∥ 				  ∥ 
						//        -coefficient    relatedConnIndex    -constant

						Relation relation( (*virtualWorkConnections)[  (*CosAlphaOrder)[j] ].index ,						  //relatedConnIndex
										   -p -> GetTermParameter( (*virtualWorkConnections)[ (*CosAlphaOrder)[j] ].index ) , //coefficient
										   -p -> GetTermParameter( CONSTANT ) );											  //relatedConnIndex , coefficient , constant									//constant

						(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.push_back( relation );//將關係加入虛功資料中
						
						std::cout << "輸入新關係：#(" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << 
									 ") = " << relation.coefficient << " * #(" << relation.relatedConnIndex << ") + " << relation.constant << std::endl; 

					}
				}
			}
		}
	}

	if( ballIndex == 8 )
	{
		std::cout << "debug";
	}

	RedistributeDublicatedRelation( ballIndex );//把同一個竿子有兩個觀系給分出去
	/*
	//把同一個竿子有兩個觀系給分出去
	//如果前面在ConnectRelation時 ，i是小 j是中時建立一個小→中關係，j是大時又建立一個小→大關係
	//i就會建立兩個關係，這時就把一個關係分給 中 ，變成中→小關係
	//這個邏輯不好 應該要改成 中→大關係
	//RedistributeDublicatedRelation( ballIndex );
	for( int i = (*CosAlphaOrder).size() - 1; i >= 0 ; i-- )//尋找 小->終  大
	{
		if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size() > 1 )
		{

			//先找出所有關係中，CosAlpha最大的那個關係
			int maxCosAlphaIndex;
			float MaxCosAlpha = -1;
			VirtualWorkConnectionData* maxCosAlphaVirtualWorkConnection;
			Relation* maxCosAlphaRelation;//把最大的CosAlpha的關係給存起來，等一下用

			//這邊的順序是：小→小 小→中 小→大 ...
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )
			{
				VirtualWorkConnectionData* tempVirtualWorkConnection = 
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
					GetVirtualWorkConnection( 
					(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[ j ].relatedConnIndex );

				if( abs( tempVirtualWorkConnection -> cosAlpha ) > MaxCosAlpha )
				{
					MaxCosAlpha = abs( tempVirtualWorkConnection -> cosAlpha );
					maxCosAlphaIndex = tempVirtualWorkConnection -> index;
					maxCosAlphaRelation = &( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j] );//把最大的CosAlpha的關係給存起來，等一下用
					maxCosAlphaVirtualWorkConnection = tempVirtualWorkConnection;
				}
			}

			//將CosAlpha不是最大的那個關係，把這個關係轉換成這個乾子和CosAlpha最大那個竿子之間的關係
			//，並把目前的這個關係刪掉
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )//尋找這個竿子的鎂一個觀系
			{
				if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex != maxCosAlphaIndex )
				{
					//現在的關係要給j的竿子，讓他去與最大CosAlpha的桿子建立關係，式子是
					// i = coef(j) * j + const(j)
					// i = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)     
					// → coef(j) * j + const(j) = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)

					// → j = coef(MaxCosAlpha) / coef(j) * MaxCosAlpha + ( const(MaxCosAlpha) - const(j) ) / coef(j)
					//		  ¯¯¯¯¯¯¯¯¯¯¯¯↑¯¯¯¯¯¯¯¯¯¯¯¯¯				  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯↑¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
					//					newCoef												newConst

					//				coef(MaxCosAlpha) / coef(j)
					float newCoef = maxCosAlphaRelation -> coefficient / 
									(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//				( const(MaxCosAlpha) - const(j) ) / coef(j)
					float newConst = ( 
										maxCosAlphaRelation -> constant - 
									    (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant 
									 ) / 
									 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					
					//relation j 所只像的這個竿子
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
							GetVirtualWorkConnection( 
								(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex );

					//█████████████████████████████
					//簡查這個關係有沒有已經背書入過↓↓↓↓↓↓↓↓↓↓↓↓↓
					bool alreadyHaveRelation = false;
					for( int k = 0; k < tempVirtualWorkConnection -> relation.size(); k++ )
					{
						if( tempVirtualWorkConnection -> relation[k].relatedConnIndex == maxCosAlphaIndex )
						{
							//如果已經輸入過，要檢查已經輸入過的關係式不是數字都一樣，不一樣的話就有問題
							if( abs( tempVirtualWorkConnection -> relation[k].coefficient - newCoef ) < SMALL_NUMBER &&
								abs( tempVirtualWorkConnection -> relation[k].constant   - newConst ) < SMALL_NUMBER )
							{
								std::cout << "這個關係已背書入：第" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "竿到第" << 
																	   (*virtualWorkConnections)[         j           ].index << "竿" << std::endl;
							}
							else
							{
								std::cout << "RedistributeDublicatedRelation()出錯：轉換關係以後，發現與原有的關係數字不一樣" << std::endl;
								std::cout << "新建的關係： 第" << tempVirtualWorkConnection -> index << "竿 = Coeffifient( " << tempVirtualWorkConnection -> relation[k].coefficient <<
											 " ) × 第" << maxCosAlphaIndex << "竿 + Constant( " << tempVirtualWorkConnection -> relation[k].constant << " ) " << std::endl;
								std::cout << "舊的關係： 第" << tempVirtualWorkConnection -> index << "竿 = Coeffifient( " << newCoef <<
											 " ) × 第" << maxCosAlphaIndex << "竿 + Constant( " << newConst << " ) " << std::endl;
							}
							alreadyHaveRelation = true;
						}
					}
					if( alreadyHaveRelation ) continue;
					//===============================↑↑↑↑↑↑↑↑↑↑↑↑↑
					//█████████████████████████████

					// j → maxCosAlpha 的關係
					Relation tempRelation( maxCosAlphaIndex ,
										   newCoef,
										   newConst );

					//輸入j裡，要在CosAlpha大小順序正確的位置插入，relation的大小順序是 i = 0→n ， 大→小
					for( std::vector<Relation>::iterator it = tempVirtualWorkConnection -> relation.begin(); 
						 it != tempVirtualWorkConnection -> relation.end();
						 it++ )
					{
						//insert的擺放位置是，在哪一個位置插入，原本在那個位置的元素就會往外推一格
						//所以要找到第一個CosAlpha比他小的觀系，然後插入在那個關係的位置
						VirtualWorkConnectionData* tempVirtualWorkConnectionForSorting = 
							ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
								GetVirtualWorkConnection( it -> relatedConnIndex );
						if( abs( tempVirtualWorkConnectionForSorting -> cosAlpha ) < abs( maxCosAlphaVirtualWorkConnection -> cosAlpha ) )
						{
							tempVirtualWorkConnection -> relation.insert( it , tempRelation );
							break;
						}
					}
					//tempVirtualWorkConnection -> relation.push_back( tempRelation );


					//刪掉就的關係
					std::vector<Relation>::iterator relationToErase = (*virtualWorkConnections)[i].relation.begin() + j;
					(*virtualWorkConnections)[i].relation.erase( relationToErase );
					j--;//因為刪掉以後，relation會往後退一個，所以j也要-1



					/*  這一段是寫錯的，要改掉。
					//把原的式子等號左右對調，原本是
					// i = coef(j) * j + const(j)
					// 調整後變成
					// 1/coef(j) * i - const(j)/coef(j) = j
					//
					float newCoef = 1.0 / (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					float newConst =	 -(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant / 
										  (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//拿 關係j 所關聯的虛功桿件資料 的指標
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].GetVirtualWorkConnection( (*virtualWorkConnections)[i].relation[j].relatedConnIndex );

					//建立一個給 j桿件所使用的
					Relation tempRelation( (*virtualWorkConnections)[i].index , 
										   newCoef ,
										   newConst );
					tempVirtualWorkConnection -> relation.push_back( tempRelation );

					//刪掉就的關係
					std::vector<Relation>::iterator relationToErase;
					for( std::vector<Relation>::iterator it = (*virtualWorkConnections)[i].relation.begin(); 
						 it != (*virtualWorkConnections)[i].relation.end();
						 it++ )
					{
						if( it -> relatedConnIndex == (*virtualWorkConnections)[i].relation[j].relatedConnIndex )
						{
							relationToErase = it;
						}
					}
					(*virtualWorkConnections)[i].relation.erase( relationToErase );
					j--;//因為刪掉以後，relation會往後退一個，所以j也要-1
					*/ /*
				}
			}
		}
	}

	//return true;/////////測試用 之後要刪掉*/

	CombineChainRelation( ballIndex );//剪茶有沒有聯所的官系，有的話就把他門結合
	/* 
	//剪茶有沒有聯所的官系，有的話就把他門結合
	//CombineChainRelation();
	for( int i = 0; i < (*virtualWorkConnections).size(); i++ )//尋找每一個虛功竿子，看看他門每一個觀系
	{	
		bool foundChain = false;
		for( int j = 0; j < (*virtualWorkConnections)[i].relation.size(); j++ )//尋找這個竿子的鎂一個觀系
		{
			for( int k = 0; k < (*virtualWorkConnections).size(); k++ )//尋找這個觀系所對到的竿子，
			{
				if( (*virtualWorkConnections)[i].relation[j].relatedConnIndex != //i竿子的第j觀系，是否對到第k竿子
					(*virtualWorkConnections)[k].index )
					continue;
				for( int l = 0; l < (*virtualWorkConnections)[k].relation.size(); l++ )
				{
					//這邊來把第i竿子的第j觀系，會聯到第k竿子的第l觀系，把這兩個箱乘起來
					//式子原狀應該是 Coef(i) * (i) + Coef(j) * (j) + Const = 0
					//調整系數後變成    1    * (i) + Coef(j) * (j) + Const = 0

					// j = k
					// i = coef(j) * j + const(j)
					// k = coef(l) * l + const(l)
					// i = coef(j) * ( coef(l) * l + const(l) ) + const(j)
					//   = coef(j) * coef(l) * l + coef(j) * const(l) + const(j)
					

					Relation relation( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].relatedConnIndex , //l         //relatedConnIndex
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
									      ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].coefficient ) , //coef(l)   //coefficient
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].constant +      //const(l)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].constant ) );	 //const(j)  //constant	
					//relatedConnIndex , coefficient , constant									//constant

					//在j裡面儲存 relatedConnIndex = l
					//			  coefficient = coef(j) * coef(l)
					//			  constant = coef(j) * const(l) + const(j)
					//

					//修改完以後，原本的relation刪掉
					std::vector< Relation >::iterator relationToErase;
					for( relationToErase = ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.begin();
						 relationToErase != ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.end();
						 relationToErase++ )
					{
						if( (*relationToErase).relatedConnIndex == ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].relatedConnIndex )
							break;
					}

					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.erase( relationToErase );//刪除
					//書入修改好的結果
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.push_back( relation );//

				}
			}
		}
	}*/

	//PrintAllRelation();
	for( int i = 0; i < (*virtualWorkConnections).size(); i++ )
	{
		for( int j = 0; j < (*virtualWorkConnections)[i].relation.size(); j++ )
		{
			std::cout << "處理後關係：#(" << (*virtualWorkConnections)[i].index << 
									 ") = " << (*virtualWorkConnections)[i].relation[j].coefficient << 
									 " * #(" << (*virtualWorkConnections)[i].relation[j].relatedConnIndex << 
									 ") + " <<  (*virtualWorkConnections)[i].relation[j].constant << std::endl; 

		}
	}
	

	return true;
}

bool VirtualForceAnalyzer::VirtualWorkAnalyze( int leastUnsolvedBallIndex )
{
	//找出那顆球在陣列中的位置
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == leastUnsolvedBallIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}

	if( leastUnsolvedBallIndex == 23 )
	{
		std::cout << "debug" << std::endl;
		return false;//因為到23顆球會當機 所以先算到這就好
	}


	ballVirtualForceData[ leastUnsolvedBallVectorIndex ].MakeVirtualWorkConnectionData();
	//找出外力合力方向
	//算出所有竿子的cosA
	ballVirtualForceData[ leastUnsolvedBallVectorIndex ].ComputeCosAlpha();

	MakeConnectionRelation( leastUnsolvedBallIndex );//建立竿子與竿子之間的關係
	//用球做分析
	if( !ballVirtualForceData[ leastUnsolvedBallVectorIndex ].Analyze() )return false;
	return true;
}

bool VirtualForceAnalyzer::SubstituteSolution( int leastUnsolvedBallIndex )
{
	//找出那顆球在陣列中的位置
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == leastUnsolvedBallIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}
	//把每個眉代入過的味之樹找出來
	for( int i = 0; i < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); i++ )
	{
		int connIndex = ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.at(i).index;
		
		//沒有代入過
		//先建立新的多項式，裡面擺解，加入串列的最後
			
		PolynomialList* tempPolynomial = new PolynomialList( connIndex , ballVirtualForceData[ leastUnsolvedBallVectorIndex ].solutions.at( connIndex ) );//改成用加總後的姐來代入
		//PolynomialList* tempPolynomial = new PolynomialList( connIndex , firstPolynomial -> solvedConnections -> at( connIndex ) );//這個是什麼意思????????
		//PolynomialList* tempPolynomial = new PolynomialList( connIndex , ballVirtualForceData[ leastUnsolvedBallVectorIndex ].connections[0].at(i).solution );
		tempPolynomial -> AddToLink( firstPolynomial );
		std::cout << "根據虛功法，建立新的解：";
		tempPolynomial -> PrintPolynomialList();
		//代入所有式子
		tempPolynomial -> Substitute();

		//好像要把這個姐在推入solvedConnections裡?
		firstPolynomial -> solvedConnections -> push_back( connIndex );

		//break;////////////////////////////測試用 要改回來
		

		//也要把姐出來的編號更新到虛功法裡的solvedConnections
		if( std::find( solvedConnections.begin() , solvedConnections.end() , connIndex ) == solvedConnections.end() )
		{
			Renew();
		}

	}

	return true;
}

bool VirtualForceAnalyzer::RedistributeDublicatedRelation( int ballIndex )
{
	//找出那顆球在陣列中的位置
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == ballIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}
	
	//幫常用的東西寫個代號
	std::vector<int>* CosAlphaOrder = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder);
	std::vector<VirtualWorkConnectionData>* virtualWorkConnections = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections);
	
	//把同一個竿子有兩個觀系給分出去
	//如果前面在ConnectRelation時 ，i是小 j是中時建立一個小→中關係，j是大時又建立一個小→大關係
	//i就會建立兩個關係，這時就把一個關係分給 中 ，變成中→小關係
	//這個邏輯不好 應該要改成 中→大關係
	//RedistributeDublicatedRelation();
	for( int i = (*CosAlphaOrder).size() - 1; i >= 0 ; i-- )//尋找 小->終  大
	{
		if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size() > 1 )
		{

			//先找出所有關係中，CosAlpha最大的那個關係
			int maxCosAlphaIndex;
			float MaxCosAlpha = -1;
			VirtualWorkConnectionData* maxCosAlphaVirtualWorkConnection = NULL;
			Relation* maxCosAlphaRelation = NULL;//把最大的CosAlpha的關係給存起來，等一下用

			//這邊的順序是：小→小 小→中 小→大 ...
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )
			{
				VirtualWorkConnectionData* tempVirtualWorkConnection = 
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
						GetVirtualWorkConnection( 
							(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex );

				if( abs( tempVirtualWorkConnection -> cosAlpha ) > MaxCosAlpha )
				{
					MaxCosAlpha = abs( tempVirtualWorkConnection -> cosAlpha );
					maxCosAlphaIndex = tempVirtualWorkConnection -> index;
					maxCosAlphaRelation = &( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j] );//把最大的CosAlpha的關係給存起來，等一下用
					maxCosAlphaVirtualWorkConnection = tempVirtualWorkConnection;
				}
			}

			//將CosAlpha不是最大的那個關係，把這個關係轉換成這個乾子和CosAlpha最大那個竿子之間的關係
			//，並把目前的這個關係刪掉
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )//尋找這個竿子的鎂一個觀系
			{
				if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex != maxCosAlphaIndex )
				{
					//現在的關係要給j的竿子，讓他去與最大CosAlpha的桿子建立關係，式子是
					// i = coef(j) * j + const(j)
					// i = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)     
					// → coef(j) * j + const(j) = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)

					// → j = coef(MaxCosAlpha) / coef(j) * MaxCosAlpha + ( const(MaxCosAlpha) - const(j) ) / coef(j)
					//		  ¯¯¯¯¯¯¯¯¯¯¯¯↑¯¯¯¯¯¯¯¯¯¯¯¯¯				  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯↑¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
					//					newCoef												newConst

					//				coef(MaxCosAlpha) / coef(j)
					float newCoef = maxCosAlphaRelation -> coefficient / 
									(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//				( const(MaxCosAlpha) - const(j) ) / coef(j)
					float newConst = ( 
										maxCosAlphaRelation -> constant - 
									    (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant 
									 ) / 
									 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					
					//relation j 所只像的這個竿子
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
							GetVirtualWorkConnection( 
								(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex );

					//█████████████████████████████
					//簡查這個關係有沒有已經背書入過↓↓↓↓↓↓↓↓↓↓↓↓↓
					bool alreadyHaveRelation = false;
					for( int k = 0; k < tempVirtualWorkConnection -> relation.size(); k++ )
					{
						if( tempVirtualWorkConnection -> relation[k].relatedConnIndex == maxCosAlphaIndex )
						{
							//如果已經輸入過，要檢查已經輸入過的關係式不是數字都一樣，不一樣的話就有問題
							if( abs( tempVirtualWorkConnection -> relation[k].coefficient - newCoef ) < SMALL_NUMBER &&
								abs( tempVirtualWorkConnection -> relation[k].constant   - newConst ) < SMALL_NUMBER )
							{
								std::cout << "這個關係已背書入：第" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "竿到第" << 
																	   (*virtualWorkConnections)[         j           ].index << "竿" << std::endl;
							}
							else
							{
								std::cout << "RedistributeDublicatedRelation()出錯：轉換關係以後，發現與原有的關係數字不一樣" << std::endl;
								std::cout << "新建的關係： 第" << tempVirtualWorkConnection -> index << "竿 = Coeffifient( " << tempVirtualWorkConnection -> relation[k].coefficient <<
											 " ) × 第" << maxCosAlphaIndex << "竿 + Constant( " << tempVirtualWorkConnection -> relation[k].constant << " ) " << std::endl;
								std::cout << "舊的關係： 第" << tempVirtualWorkConnection -> index << "竿 = Coeffifient( " << newCoef <<
											 " ) × 第" << maxCosAlphaIndex << "竿 + Constant( " << newConst << " ) " << std::endl;

							}
							alreadyHaveRelation = true;
						}
					}
					if( alreadyHaveRelation ) continue;
					//===============================↑↑↑↑↑↑↑↑↑↑↑↑↑
					//█████████████████████████████

					// j → maxCosAlpha 的關係
					Relation tempRelation( maxCosAlphaIndex ,
										   newCoef,
										   newConst );

					//輸入j裡，要在CosAlpha大小順序正確的位置插入，relation的大小順序是 i = 0→n ， 大→小
					if( tempVirtualWorkConnection -> relation.size() == 0 )
					{
						//如果這個relation式空的，就直接擺在第一個
						tempVirtualWorkConnection -> relation.push_back( tempRelation );
						std::cout << "█轉換關係(舊的)：第" << 
										tempVirtualWorkConnection -> index << "竿 = Coeffifient( " << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient << " ) × 第" << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "竿 + Constant( " << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant << " ) " << std::endl;
						std::cout << "						↓↓↓↓↓	" << std::endl;
						std::cout << "█轉換至(新的)：第" << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "竿 = Coeffifient( " << 
										newCoef << " ) × 第" << 
										maxCosAlphaIndex << "竿 + Constant( " << 
										newConst << " ) " << std::endl;
					}
					else
					{
						for( std::vector<Relation>::iterator it = tempVirtualWorkConnection -> relation.begin(); 
							 it != tempVirtualWorkConnection -> relation.end();
							 it++ )
						{
							//insert的擺放位置是，在哪一個位置插入，原本在那個位置的元素就會往外推一格
							//所以要找到第一個CosAlpha比他小的觀系，然後插入在那個關係的位置
							VirtualWorkConnectionData* tempVirtualWorkConnectionForSorting = 
								ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
									GetVirtualWorkConnection( it -> relatedConnIndex );
							if( abs( tempVirtualWorkConnectionForSorting -> cosAlpha ) < abs( maxCosAlphaVirtualWorkConnection -> cosAlpha ) )
							{
								tempVirtualWorkConnection -> relation.insert( it , tempRelation );
								std::cout << "█轉換關係(舊的)：第" << tempVirtualWorkConnection -> index << "竿 = Coeffifient( " << 
											 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient << " ) × 第" << 
											 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "竿 + Constant( " << 
											 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant << " ) " << std::endl;
								std::cout << "						↓↓↓↓↓	" << std::endl;
								std::cout << "█轉換至(新的)：第" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "竿 = Coeffifient( " << 
											 newCoef << " ) × 第" << 
											 maxCosAlphaIndex << "竿 + Constant( " << 
											 newConst << " ) " << std::endl;
								break;
							}
						}
						//tempVirtualWorkConnection -> relation.push_back( tempRelation );
					}
					


					//刪掉就的關係
					std::vector<Relation>::iterator relationToErase = (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.begin() + j;
					/*
					std::vector<Relation>::iterator relationToErase = (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.begin();
					for( std::vector<Relation>::iterator it = (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.begin(); 
						 it != (*virtualWorkConnections)[i].relation.end();
						 it++ )
					{
						if( it -> relatedConnIndex == (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex )
						{
							relationToErase = it;
						}
					}*/
					(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.erase( relationToErase );
					j--;//因為刪掉以後，relation會往後退一個，所以j也要-1

					/*  這一段是寫錯的，要改掉。
					//把原的式子等號左右對調，原本是
					// i = coef(j) * j + const(j)
					// 調整後變成
					// 1/coef(j) * i - const(j)/coef(j) = j
					//
					float newCoef = 1.0 / (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					float newConst =	 -(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant / 
										  (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//拿 關係j 所關聯的虛功桿件資料 的指標
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].GetVirtualWorkConnection( (*virtualWorkConnections)[i].relation[j].relatedConnIndex );

					//建立一個給 j桿件所使用的
					Relation tempRelation( (*virtualWorkConnections)[i].index , 
										   newCoef ,
										   newConst );
					tempVirtualWorkConnection -> relation.push_back( tempRelation );

					//刪掉就的關係
					std::vector<Relation>::iterator relationToErase;
					for( std::vector<Relation>::iterator it = (*virtualWorkConnections)[i].relation.begin(); 
						 it != (*virtualWorkConnections)[i].relation.end();
						 it++ )
					{
						if( it -> relatedConnIndex == (*virtualWorkConnections)[i].relation[j].relatedConnIndex )
						{
							relationToErase = it;
						}
					}
					(*virtualWorkConnections)[i].relation.erase( relationToErase );
					j--;//因為刪掉以後，relation會往後退一個，所以j也要-1
					*/
				}
			}
		}
	}

	return true;
}

bool VirtualForceAnalyzer::CombineChainRelation( int ballIndex )
{
	//找出那顆球在陣列中的位置
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == ballIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}
	
	//幫常用的東西寫個代號
	std::vector<int>* CosAlphaOrder = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder);
	std::vector<VirtualWorkConnectionData>* virtualWorkConnections = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections);
	 
	//剪茶有沒有聯所的官系，有的話就把他門結合
	//CombineChainRelation();
	for( int i = 0; i < (*virtualWorkConnections).size(); i++ )//尋找每一個虛功竿子，看看他門每一個觀系
	{	
		bool foundChain = false;
		for( int j = 0; j < (*virtualWorkConnections)[i].relation.size(); j++ )//尋找這個竿子的鎂一個觀系
		{
			for( int k = 0; k < (*virtualWorkConnections).size(); k++ )//尋找這個觀系所對到的竿子，
			{
				if( (*virtualWorkConnections)[i].relation[j].relatedConnIndex != //i竿子的第j觀系，是否對到第k竿子
					(*virtualWorkConnections)[k].index )
					continue;
				for( int l = 0; l < (*virtualWorkConnections)[k].relation.size(); l++ )
				{
					//這邊來把第i竿子的第j觀系，會聯到第k竿子的第l觀系，把這兩個箱乘起來
					//式子原狀應該是 Coef(i) * (i) + Coef(j) * (j) + Const = 0
					//調整系數後變成    1    * (i) + Coef(j) * (j) + Const = 0

					// j = k
					// i = coef(j) * j + const(j)
					// k = coef(l) * l + const(l)
					// i = coef(j) * ( coef(l) * l + const(l) ) + const(j)
					//   = coef(j) * coef(l) * l + coef(j) * const(l) + const(j)
					

					Relation relation( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].relatedConnIndex , //l         //relatedConnIndex
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
									      ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].coefficient ) , //coef(l)   //coefficient
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].constant +      //const(l)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].constant ) );	 //const(j)  //constant	
					//relatedConnIndex , coefficient , constant									//constant

					//在j裡面儲存 relatedConnIndex = l
					//			  coefficient = coef(j) * coef(l)
					//			  constant = coef(j) * const(l) + const(j)
					//

					//修改完以後，原本的relation刪掉
					std::vector< Relation >::iterator relationToErase;
					for( relationToErase = ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.begin();
						 relationToErase != ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.end();
						 relationToErase++ )
					{
						if( (*relationToErase).relatedConnIndex == ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].relatedConnIndex )
							break;
					}

					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.erase( relationToErase );//刪除
					//書入修改好的結果
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.push_back( relation );//

				}
			}
		}
	}
	return true;
}

//=====================================VirtualForceAnalyzer//=====================================