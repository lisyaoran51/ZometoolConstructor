#include "ZomeGenerator.h"



ZomeGenerator::ZomeGenerator( Model* m )
{
	model = m;
	zomeDirection = new ZomeDirection();
	nearestPointOnSurface = NULL;
	firstBranchEnd = NULL;
	count = 0;

	int sampleNumber = GetSampleNumber( model );

	std::cout << "模型取樣點數共有" << sampleNumber << "點" << std::endl;

	for( int i = 0; i < 1; i++ )
	{
		std::cout << "目前取樣第" << i << "點" << std::endl;

		Point* centerPoint = new Point();
		//RandomInnerPoint( model , centerPoint );//隨機生成一個裡面的點
		//centerPoint -> SetPosition( 0 , zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) , 0 );//kitten//Head
		//centerPoint -> SetPosition( 0 , 0 , 0 );//column
		//centerPoint -> SetPosition( 0 , -SCALE / 2.0 , SCALE / 2.0 );//Trimmed Ball
		centerPoint -> SetPosition( 0 , -SCALE / 4.0 , SCALE / 2.0 );//TestBall1208
		//centerPoint -> SetPosition( SCALE / 4.0 , -SCALE / 2.0 , 0 );//TestBall0208
		Cone* cone = new Cone( centerPoint );
		if ( !IfConeCollide( cone ) )
		{
			std::cout << "生成新的Cone串" << std::endl;
			//若還沒有生成點 這個點就當第一個點
			if ( firstCone == NULL )
			{
				firstCone = cone;
			}
			else//有的話，就把最後一個方塊連到這個第一個方塊
			{
				tempCone -> link = cone;
			}

			//把最後一個金字塔tempPyramidGroup更新
			Cone* lastCone;
			for ( Cone* j = cone; j != NULL; j = j -> link )
			{
				j -> firstCone = firstCone;
				if ( j -> link == NULL )
					tempCone = j;
			}

		}
		else
		{//把整串pyramidGroup都刪掉
			std::cout << "這串金字塔與之前重複" << std::endl;
			DeleteCone( cone );
		}
	}

	ComputeEngagedDistance();
	if( FIX_FAILURE_POINT )
	{
		FixFailurePoint();
	}
	

	if( CONSTRUCTOR_MODE == MAKE_OUTERCONE || 
		CONSTRUCTOR_MODE == MAKE_BRANCH ||
		CONSTRUCTOR_MODE == MAKE_BRANCH_END	)
	{
		GenerateOuterCone();
	}
	
	GenerateConeStructure();
	GenerateOuterConeStructure();

	if( CONSTRUCTOR_MODE == MAKE_OUTERCONE || 
		CONSTRUCTOR_MODE == MAKE_BRANCH ||
		CONSTRUCTOR_MODE == MAKE_BRANCH_END	)
	{
		if( STRUCTURAL_ANALYSIS && CONSTRUCTOR_MODE == MAKE_OUTERCONE )
		{
			ZomeStructureAnalyze();
		}
	}

	GenerateSocket();
	//GenerateSocketModel();
	
}

ZomeGenerator::ZomeGenerator()
{
	zomeDirection = new ZomeDirection();
	nearestPointOnSurface = NULL;
	firstBranchEnd = NULL;
	count = 0;

	
	Point* centerPoint = new Point();
	//RandomInnerPoint( model , centerPoint );//隨機生成一個裡面的點
	//centerPoint -> SetPosition( 0 , zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) , 0 );//kitten//Head
	//centerPoint -> SetPosition( 0 , 0 , 0 );//column
	//centerPoint -> SetPosition( 0 , -SCALE / 2.0 , SCALE / 2.0 );//Trimmed Ball
	centerPoint -> SetPosition( 0 , -SCALE / 4.0 , SCALE / 2.0 );//TestBall1208
	//centerPoint -> SetPosition( SCALE / 4.0 , -SCALE / 2.0 , 0 );//TestBall0208
	firstCone = new Cone( centerPoint );

	

	
	//GenerateConeStructure();
	//GenerateOuterConeStructure();

	
	//ZomeStructureAnalyze();
	
}

ZomeGenerator::~ZomeGenerator()
{
	DeleteCone( firstCone );
	DeleteOuterCone( firstOuterCone );
	delete zomeDirection;
	delete constructor;
	Delete( pointList );
	delete socketConstructor;
	Delete( nearestPointOnSurface );
}

int ZomeGenerator::GetSampleNumber( Model* model )
{
	GLfloat sizeOfModel = 0.f;
	

	for ( int i = 0 ; i < model -> verticeSize; i++ )
	{
		if( abs( ( ( *( model -> vertices ) )[ i ] ) ) > abs( sizeOfModel ) )//這邊出錯
		{
			sizeOfModel = ( *( model -> vertices ) )[ i ];
		}
	}
	int sampleNumber = pow( (int)sizeOfModel / 100 , 3 );//每50mm取樣一個點

	return sampleNumber;

}

void ZomeGenerator::RandomInnerPoint( Model* model , Point* randomPoint )
{
	//先取模行犯微的上下線
	GLfloat minX = 999;
	GLfloat maxX = -999;
	GLfloat minY = 999;
	GLfloat maxY = -999;
	GLfloat minZ = 999;
	GLfloat maxZ = -999;
	for ( int i = 0 ; i < model -> verticeSize / 3; i++ )
	{
		if( ( ( *( model -> vertices ) )[ i * 3 ] ) > maxX )
			maxX = ( *( model -> vertices ) )[ i * 3 ];
		if( ( ( *( model -> vertices ) )[ i * 3 ] ) < minX )
			minX = ( *( model -> vertices ) )[ i * 3 ];
		if( ( ( *( model -> vertices ) )[ i * 3 + 1 ] ) > maxY )
			maxY = ( *( model -> vertices ) )[ i * 3 + 1 ];
		if( ( ( *( model -> vertices ) )[ i * 3 + 1 ] ) < minY )
			minY = ( *( model -> vertices ) )[ i * 3 + 1 ];
		if( ( ( *( model -> vertices ) )[ i * 3 + 2 ] ) > maxZ )
			maxZ = ( *( model -> vertices ) )[ i * 3 + 2 ];
		if( ( ( *( model -> vertices ) )[ i * 3 + 2 ] ) < minZ )
			minZ = ( *( model -> vertices ) )[ i * 3 + 2 ];
	}
	maxX = (int)( floor( maxX / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) - 1;
	minX = (int)( ceil( minX / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) + 1;
	maxY = (int)( floor( maxY / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) - 1;
	minY = (int)( ceil( minY / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) + 1;
	maxZ = (int)( floor( maxZ / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) - 1;
	minZ = (int)( ceil( minZ / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) + 1;

	GLfloat xLength = maxX - minX;
	GLfloat yLength = maxY - minY;
	GLfloat zLength = maxZ - minZ;

	srand( ( unsigned ) time ( NULL) );

	bool isInside = false;
	Point* innerPoint = new Point();

	while( !isInside )
	{
		std::cout << "重新取樣中" << std::endl;
		GLfloat x,y,z;
		//x = GLfloat( rand() % (int)( sizeOfModel / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) * zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) * GLfloat( ( rand() % 2 == 0 ) ? -1 : 1 );
		//y = GLfloat( rand() % (int)( sizeOfModel / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) * zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) * GLfloat( ( rand() % 2 == 0 ) ? -1 : 1 );
		//z = GLfloat( rand() % (int)( sizeOfModel / zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) ) ) * zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) * GLfloat( ( rand() % 2 == 0 ) ? -1 : 1 );
		x = GLfloat( rand() % ((int)xLength+1) + minX ) * zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
		y = GLfloat( rand() % ((int)yLength+1) + minY ) * zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
		z = GLfloat( rand() % ((int)zLength+1) + minZ ) * zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
		delete innerPoint;
		innerPoint = NULL;
		innerPoint = new Point( (GLfloat)x , (GLfloat)y , (GLfloat)z );
		

		GLfloat minDistance = 999999999;
		GLuint minIndex;
		
		for( int i = 0; i < model -> verticeSize / 3; i++ )
		{
			int tempDistance = pow( ( *( model -> vertices ) )[ i * 3 ] - innerPoint -> position[0] , 2 ) + 
							   pow( ( *( model -> vertices ) )[ i * 3 + 1 ] - innerPoint -> position[1] , 2 ) + 
							   pow( ( *( model -> vertices ) )[ i * 3 + 2 ] - innerPoint -> position[2] , 2 );
			if ( tempDistance < minDistance )
			{
				minIndex = i;
				minDistance = tempDistance;
			}
		}
		//minVector是從點到模型點的最小距離
		
		Point* minVector = new Point( ( *( model -> vertices ) )[ minIndex * 3 ] - innerPoint -> position[0] , 
									  ( *( model -> vertices ) )[ minIndex * 3 + 1 ] - innerPoint -> position[1] , 
									  ( *( model -> vertices ) )[ minIndex * 3 + 2 ] - innerPoint -> position[2] );
		//從隨機點到mesh的向量
		
		//如果隨機點太靠近mesh，就重選一個點
		if ( minVector -> Absolution() < zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S ) )
		{
			delete minVector;
			delete innerPoint;
			innerPoint = NULL;
			continue;
		}

		GLfloat minCos = 0;
		GLuint minFace;
		Point* minCross = new Point();
		GLfloat minDist2Surface = 999999999;
		
		bool isTooCloseToFace = false;
		//std::cout << "查看是不是模型內的點";
		for( int i = 0; i < model -> indiceSize / 3; i++ )
		{
			if ( ( *( model -> indices ) )[ i * 3 ] == minIndex || 
				 ( *( model -> indices ) )[ i * 3 + 1 ] == minIndex || 
				 ( *( model -> indices ) )[ i * 3 + 2 ] == minIndex )
			{
				
				/*
				std::cout << "取樣點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
				std::cout << "取樣點2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 2 ] << std::endl;
				std::cout << "取樣點3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 2 ] << std::endl;
										   */
				//std::cout << ".";
				//p1p2 和 p1p3 做成 v1 和 v2
				Point* vector1 = new Point( ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 1 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 2 ] );

				Point* vector2 = new Point( ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 1 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 2 ] );
				//計算點到平面的距離
				//d = N * P - N * P1
				//N = P2-P1 * P3-P1 Normalize
				Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();

				

				if( abs( normal -> Dot( innerPoint -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] ) ) < sqrt(3) * GRID_SCALE )
				{
					isTooCloseToFace = true; 
					delete vector1;
					delete vector2; 
					delete normal;
					break;
				}
				
				//取三個點的法向量
				Point* normal1 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
				Point* normal2 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
				Point* normal3 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
				//平均成一個
				Point* averageNormal = new Point( normal1 -> position[0] +  normal2 -> position[0] + normal3 -> position[0] ,
												  normal1 -> position[1] +  normal2 -> position[1] + normal3 -> position[1] ,
												  normal1 -> position[2] +  normal2 -> position[2] + normal3 -> position[2] );
				
				
				//外基v1 v2 得到該面法向量cross
				Point* cross = new Point();
				MathHelper::VectorCorss( cross -> position , vector1 -> position , vector2 -> position );
				cross -> Normalize();

				if ( !cross -> Dot( averageNormal ) > 0 )
				{
					cross -> Reverse();
				}

				cross -> Normalize();
				minVector -> Normalize();
				GLfloat tempCos = cross -> Dot( minVector );


				/*
				if ( abs( cross -> Dot( minVector ) ) > minCos )//如果法向量與該點到mesh的向量內積最大，代表示最靠近的面
				{												//在把這個面的法向量內積看是正還是負，就知道在內還在外
					minCos = abs( cross -> Dot( minVector ) );
					minFace = i;
					minCross -> Copy( cross );
				}*/
				//這邊才是對的 取距離最小的面 當做最接近的面
				if( abs( normal -> Dot( innerPoint -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] ) ) < minDist2Surface )
				{
					minDist2Surface = abs( normal -> Dot( innerPoint -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] ) );
					minFace = i;
					minCross -> Copy( cross );
				}
				
				delete vector1;
				delete vector2;
				delete normal;
				delete normal1;
				delete normal2;
				delete normal3; 
				delete averageNormal;
				delete cross;
			}
		}
		//std::cout << std::endl;
		//在把這個面的法向量內積看是正還是負，就知道在內還在外
		if ( minCross -> Dot( minVector ) > 0 )//正的就是點在模型內
		{
			if( !isTooCloseToFace ) isInside = true;
		}

		delete minCross;
		delete minVector;
	}
	randomPoint -> Copy( innerPoint );
	std::cout << "random的初始點位置式：" << randomPoint -> position[0] << "," << randomPoint -> position[1] << "," << randomPoint -> position[2] << std::endl; 
	delete innerPoint;
}

bool ZomeGenerator::IfConeCollide( Cone* cone )
{

	for( Cone* j = cone -> firstCone; j -> link != NULL; j = j -> link )
	{
		if( cone -> position[0] == j -> position[0] && 
			cone -> position[1] == j -> position[1] && 
			cone -> position[2] == j -> position[2] )
			std::cout << "bug";
	}


	std::cout<<"->";
	bool isCollided = false;
	for( int i = 0; i < 4; i++ )
	{
		
		//往6個方向新增 如果新增過或接觸到外面 就停
		if( !cone -> facePropagated[i] && !cone -> engagedFace[i] && !cone -> engaged && count < 4000 )
		{
			//std::cout<<i;
			count++;
			std::cout << count;
			std::cout << ":建新cone，從第" << cone -> index << "建過來，方向是" << i << std::endl;
			Cone* conePropagated = new Cone( cone , i );//這行裡面 如果這個方塊沒有與之前的方塊重付
														//就會自動把他與前面的方塊相連 並且把相鄰邊都對好
			
			//檢查有沒有重複
			if ( conePropagated -> dublicated )
			{
				std::cout << "X" << std::endl;
				DeleteCone( conePropagated );
				continue;
			}
			else
			{ 
				if( !conePropagated )
					std::cout<<"?";
				if( !conePropagated -> position )
					std::cout<<"?";

				std::cout << ":" << conePropagated -> position[0] << " , " << conePropagated -> position[1] << " , " << conePropagated -> position[2];
				//這個方塊沒有重複
				//查看這個方塊有沒有碰到邊 並且把有沒有碰到邊輸入到engaged值裡面
				CheckEngaged( conePropagated );
				//CheckBallEngaged( conePropagated );

				for( Cone* j = firstCone; j != NULL; j = j -> link )
				{//查看有沒有之前別的取樣點的方塊
					if( j -> link )
					{
						if( conePropagated -> Compare ( j -> link ) )
							return true;
					}
				}
				std::cout << std::endl;
				//std::cout << "這個grid的engaged式：" << grid -> engaged << std::endl;
				
				isCollided |= IfConeCollide( conePropagated );
			}
		}
		else if( count >= 4000 )
		{
			std::cout << "ZomeGenerator::IfConeCollide出錯：產生三角錐數目太多" << std::endl;
		}
	}

	return isCollided;
}

bool ZomeGenerator::CheckEngaged( Cone* cone )
{
	//std::cout<<"C";
	bool isEngaged = false;
	GLfloat tempNearestPoint[3];
	//一個cone的最大距離maxConeDistance
	GLfloat maxConeDistance = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
	//std::cout << "查看點相交";
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		/*
		std::cout << "搜尋面1:" << ( *( model -> indices ) )[ i * 3 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 1 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 2 ] << std::endl;
		std::cout << "搜尋點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
		*/
		//如果模型中的這個面離方塊不到1mm
		//這邊寫錯了 應該是平面到點的距離不到根號3 布是點到點
		//d = N * P - N * P1
		//N = P2-P1 * P3-P1 Normalize
		Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		


		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		if( 0 && normal -> position[0] == 0 && normal -> position[1] == 0 )
		{
			std::cout << "(" << ( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[0] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[1] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[2] << "),(" <<
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 )[0] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 )[1] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 )[2] << "),(" <<
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 )[0] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 )[1] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 )[2] << ")" << std::endl;
			std::cout << "123";
		}
		/*
		GLfloat dddd = abs( normal -> Dot( grid -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) );
		GLfloat vvvv = ( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[0];
		*/
		//與玉測試的平面的距離 小於一個cone的最大距離
		if( abs( normal -> Dot( cone -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) ) 
			< maxConeDistance /*zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S )*/ )
		{
			//std::cout << ".";
			/*
			std::cout << "查看點相交:點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 2 ] << std::endl;
			
			std::cout << "查看點相交:點2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 2 ] << std::endl;
			std::cout << "查看點相交:點3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 2 ] << std::endl;
				*/						   
			//寫錯了
			
			isEngaged |= cone -> CheckIntersectConeOnTri ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											 			   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] );
			/*
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] );
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			*/
			for( int j = 0; j < 4; j++ )
			{
				GLfloat distance = MathHelper::CheckDistanceToTriangle( cone -> corner[j] -> position ,
														 &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
														 &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
														 &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
														 &( *( model -> normals ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
														 &( *( model -> normals ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
														 &( *( model -> normals ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
														 tempNearestPoint );

				if( distance < NODE_DIAMETER / 2.0 + SURFACE_THICKNESS && distance > -1.0 )
				{
					isEngaged = true;
					break;
				}
			}

		}
		delete vector1;
		delete vector2;
		delete normal;

		if( isEngaged )
		{
			break;
		}
	}
	if( isEngaged ) 
		std::cout << "，有香蕉";
	cone -> engaged = isEngaged;
	return isEngaged;
	//grid -> CheckIntersect( )
}

bool ZomeGenerator::CheckBallEngaged( Cone* cone )
{
	bool isEngaged = false;
	GLfloat tempNearestPoint[3];

	GLfloat maxConeDistance = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
	//std::cout << "查看點相交";
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		//如果模型中的這個面離方塊不到1mm
		//這邊寫錯了 應該是平面到點的距離不到根號3 布是點到點
		//d = N * P - N * P1
		//N = P2-P1 * P3-P1 Normalize
		Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		


		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		if( normal -> position[0] == 0 && normal -> position[1] == 0 )
		{
			std::cout << "(" << ( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[0] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[1] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[2] << "),(" <<
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 )[0] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 )[1] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 )[2] << "),(" <<
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 )[0] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 )[1] << "," << 
								( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 )[2] << ")" << std::endl;
			std::cout << "123";
		}
		/*
		GLfloat dddd = abs( normal -> Dot( grid -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) );
		GLfloat vvvv = ( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[0];
		*/
		//與玉測試的平面的距離 小於一個cone的最大距離
		if( abs( normal -> Dot( cone -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) ) 
			< maxConeDistance /*zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S )*/ )
		{
			//std::cout << ".";
			/*
			std::cout << "查看點相交:點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 2 ] << std::endl;
			
			std::cout << "查看點相交:點2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 2 ] << std::endl;
			std::cout << "查看點相交:點3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 2 ] << std::endl;
				*/						   
			//寫錯了
			for( int j = 0; j < 4; j++ )
			{
				if( MathHelper::CheckDistanceToTriangle( cone -> corner[j] -> position ,
														 &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
														 &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
														 &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
														 &( *( model -> normals ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
														 &( *( model -> normals ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
														 &( *( model -> normals ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
														 tempNearestPoint ) )
				{
					isEngaged = true;
					break;
				}
			}
		}
		delete vector1;
		delete vector2;
		delete normal;

		if( isEngaged )
		{
			break;
		}
	}
	if( isEngaged ) 
		std::cout << "，有香蕉";
	cone -> engaged = isEngaged;
	return isEngaged;
	//grid -> CheckIntersect( )
}

int ZomeGenerator::CheckEngagedDistance( Cone* cone , int maxDistance )
{
	if( cone == NULL ) return -1;

	if( cone -> engaged )
	{
		return 0;
	}
	else
	{
		if( maxDistance >= 0 )
		{
			int minDistance = 9999;

			for( int i = 0; i < 4; i++ )
			{
				int tempDistance = CheckEngagedDistance( cone -> neighborCone[i] , maxDistance - 1 );
				if( tempDistance < minDistance && tempDistance >= 0 )
				{
					minDistance = tempDistance;
				}
			}
			if ( minDistance != 9999 )
			{
				return minDistance + 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
}

void ZomeGenerator::ComputeEngagedDistance()
{
	for( Cone* i = firstCone; i != NULL; i = i -> link )
	{
		i -> engagedDistance = CheckEngagedDistance( i , 6 );
		std::cout << "第" << i -> index << "顆球的距離是" << i -> engagedDistance << std::endl;
		//if( i -> engagedDistance > 2 || i -> engagedDistance == -1 ) i -> activated = true;
		if( i -> engagedDistance == CONE_ENGAGED_DISTANCE ) i -> activated = true;
		if( SOLID_CONE == true )
		{
			if( i -> engagedDistance > CONE_ENGAGED_DISTANCE || i -> engagedDistance == -1 ) i -> activated = true;
		}
	}
}

void ZomeGenerator::FixFailurePoint()
{
	//1.先查覺破壞點在哪
	std::cout << "開始分析破壞點" << std::endl;
	FailurePointFixer* failurePointFixer = new FailurePointFixer( firstCone );
	failurePointFixer -> Assort();
	failurePointFixer -> FindFailurePoint();



	//列出所有三角錐，再把穩定相連的三角錐結合到同一個串列中
	//最後無法結合的兩團，找出他門破壞點在哪
	

	//2.在去百破壞點補起來

	if( !failurePointFixer -> TryFillCone() )
	{
		std::cout << "ZomeGenerator::FixFailurePoint()出錯：沒有辦法把破壞點填補起來" << std::endl;
	}

	//嘗試各種方法，看哪一個可以補起破壞點
}

void ZomeGenerator::DeleteCone( Cone* cone )
{
	for( Cone* i = cone; i != NULL; )
	{
		Cone* j = i -> link;
		delete i;
		i = j;
	}
}

void ZomeGenerator::DeleteOuterCone( OuterCone* outerCone )
{
	for( OuterCone* i = outerCone; i != NULL; )
	{
		OuterCone* j = i -> link;
		delete i;
		i = j;
	}
}

void ZomeGenerator::Delete( Point* point )
{
	for( Point* i = point; i != NULL; )
	{
		Point* j = i -> link;
		delete i;
		i = j;
	}
}

void ZomeGenerator::Delete( PointList* pointList )
{
	for( PointList* i = pointList; i != NULL; )
	{
		PointList* j = i -> link;
		delete i;
		i = j;
	}
}
//當其他位子有建模型，但要在中間沒有相連的新位子建新的模型時，建立一顆新的球
bool ZomeGenerator::ConstructNewBall( Point* point , int size )
{
	//看這顆球以前有沒有
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::CompareVector( i -> position , point -> position ) )
		{
			std::cout << "這顆球已經被建過了，出錯:" << point -> position[0] << "," << point -> position[1] << "," <<  point -> position[2] << std::endl;
			return false;
		}
	}

	bool hasBall = false;
	int index = 0;
	//查看這顆要新建的球旁邊有沒有相鄰的球，有的話就從那顆球建過來，然後把hasNeighbor變成true
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		int faceNumber = zomeDirection -> GetDirectionFace( i -> position , point -> position , size );
		if( faceNumber >= 0 )
		{
			//已經有建球
			if( hasBall )
			{
				constructor -> Construct( i -> index , faceNumber , 0 , TYPE_TRUSS );
			}
			else
			{//還呒鍵球
				index = constructor -> Construct( i -> index , faceNumber , 0 , TYPE_PIN );
				hasBall = true;
			}
		}
	}

	//進入這行的話，就代表這顆球完全沒有任何相鄰的球，要獨自新建一顆
	if( !hasBall )
	{
		index = constructor -> NewBall( point -> position );
	}

	pointList -> Add( point , index );

	//這行不需要
	if( !hasBall && false )
	{
		std::cout << "ConstructNewBall出錯在" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << "連布起來" << std::endl;
	}

	return true;


}
//鍵球和竿子，其他東西已經有了
bool ZomeGenerator::ConstructNeighborBall( Point* point , int size )
{
	//看這顆球以前有沒有
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::CompareVector( i -> position , point -> position ) )
		{
			std::cout << "這顆球已經被建過了，出錯:" << point -> position[0] << "," << point -> position[1] << "," <<  point -> position[2] << std::endl;
			return false;
		}
	}

	bool hasBall = false;
	int index = 0;
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		int faceNumber = zomeDirection -> GetDirectionFace( i -> position , point -> position , size );
		if( faceNumber >= 0 )
		{
			//已經有建球
			if( hasBall )
			{
				constructor -> Construct( i -> index , faceNumber , 0 , TYPE_TRUSS );
			}
			else
			{//還呒鍵球
				index = constructor -> Construct( i -> index , faceNumber , 0 , TYPE_PIN );
				hasBall = true;
			}
		}
	}
	pointList -> Add( point , index );

	if( !hasBall )
	{
		std::cout << "ConstructNeighborBall出錯在" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << "連布起來" << std::endl;
	}

	return true;
}
//意思是指從這個cone的四個點來長出支架
bool ZomeGenerator::ConstructConeNeighborBall( Cone* cone , Point* point , int size )
{
	//看這顆球以前有沒有
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::CompareVector( i -> position , point -> position ) )
		{
			std::cout << "這顆球已經被建過了，出錯:" << point -> position[0] << "," << point -> position[1] << "," <<  point -> position[2] << std::endl;
			return false;
		}
	}

	bool hasBall = false;
	int index = 0;
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		int faceNumber = zomeDirection -> GetDirectionFace( i -> position , point -> position , size );
		if( faceNumber >= 0 )
		{
			//如果這個點不是這個cone上的，就不要隨便建之價
			if( !MathHelper::VectorApproximation( cone -> corner[0] -> position , i -> position ) &&
				!MathHelper::VectorApproximation( cone -> corner[1] -> position , i -> position ) &&
				!MathHelper::VectorApproximation( cone -> corner[2] -> position , i -> position ) &&
				!MathHelper::VectorApproximation( cone -> corner[3] -> position , i -> position ) )
			{
				continue;
			}
			//已經有建球
			if( hasBall )
			{
				constructor -> Construct( i -> index , faceNumber , 0 , TYPE_TRUSS );
			}
			else
			{//還呒鍵球
				index = constructor -> Construct( i -> index , faceNumber , 0 , TYPE_PIN );
				hasBall = true;
			}
		}
	}
	pointList -> Add( point , index );

	if( !hasBall )
	{
		std::cout << "ConstructNeighborBall出錯在" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << "連布起來" << std::endl;
	}

	return true;
}

bool ZomeGenerator::ConstructConeConn( Cone* cone , Point* point , int size )
{
	//看這顆球以前有沒有
	bool hasBall = false;
	for( PointList* i = pointList -> firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::VectorApproximation( i -> position , point -> position ) )
		{
			hasBall = true;
		}
	}
	if( !hasBall )
	{
		std::cout << "ZomeGenerator::ConstructConeConn出錯：這顆球還沒被建過--(" << point -> position[0] << "," << point -> position[1] << "," <<  point -> position[2] << ")" << std::endl;
		return false;
	}


	for( PointList* p = pointList -> firstPointList; p != NULL; p = p -> link )//要在以有的點裡
	{
		if( MathHelper::VectorApproximation( cone -> corner[0] -> position , p -> position ) ||
			MathHelper::VectorApproximation( cone -> corner[1] -> position , p -> position ) ||
			MathHelper::VectorApproximation( cone -> corner[2] -> position , p -> position ) ||
			MathHelper::VectorApproximation( cone -> corner[3] -> position , p -> position ) )//要是三角錐的其中一點
		{
			int faceNumber = zomeDirection -> GetDirectionFace( p -> position , point -> position , size );
			if( faceNumber >= 0 )//要能夠和其中一點相連
			{
				if( !constructor -> ContainConn( p -> index , faceNumber ) )//要沒有被見過支架
				{
					constructor -> Construct( p -> index , faceNumber , 0 , TYPE_TRUSS );
				}
			}
		}
	}
	return true;
}

void ZomeGenerator::GenerateConeStructure()
{
	pointList = new PointList();
	bool hasFirstBall = false;


	for( Cone* i = firstCone; i != NULL; i = i -> link )
	{
		//當距離是0或1 就跳過不建
		//if( i -> engagedDistance < 3 && i -> engagedDistance != -1 )continue;
		//當距離不是2實就跳過
		//if( i -> engagedDistance != 3 )continue;
		if( !i -> activated )continue;
		//當第一次建立內支架，球的周邊沒有相鄰球
		if( !pointList -> CloselyContain( i -> corner[0] ) && 
			!pointList -> CloselyContain( i -> corner[1] ) &&
			!pointList -> CloselyContain( i -> corner[2] ) &&
			!pointList -> CloselyContain( i -> corner[3] ) &&
			!hasFirstBall )
		{
			std::cout << "第一次建內支架" << std::endl;
			hasFirstBall = true;
			//第一次
			//把球的原點的位置移到第一個corner上
			//這邊debug嘗試把firstCone改乘i
			constructor = new Constructor( i -> corner[0] -> position );

			int index = 0;
			pointList -> Add( i -> corner[0] , index );

			ConstructConeNeighborBall( i , i -> corner[1] , SIZE_S );

			ConstructConeNeighborBall( i , i -> corner[2] , SIZE_S );

			ConstructConeNeighborBall( i , i -> corner[3] , SIZE_S );
		}
		//並非第一次建立支架，但是球的周邊沒有相鄰的球
		else if( !pointList -> CloselyContain( i -> corner[0] ) && 
				 !pointList -> CloselyContain( i -> corner[1] ) &&
				 !pointList -> CloselyContain( i -> corner[2] ) &&
				 !pointList -> CloselyContain( i -> corner[3] ) &&
				 hasFirstBall ) 
		{
			std::cout << "在一個新的位置建內支架，周邊不知道有沒有相鄰的球" << std::endl;

			ConstructNewBall( i -> corner[0] , SIZE_S );
			ConstructConeNeighborBall( i , i -> corner[1] , SIZE_S );
			ConstructConeNeighborBall( i , i -> corner[2] , SIZE_S );
			ConstructConeNeighborBall( i , i -> corner[3] , SIZE_S );

		}
		//不是第一次建立支架，三角錐的其中幾點已經有建過
		else
		{
			if( !pointList -> CloselyContain( i -> corner[0] ) )//沒有球就建球，有球就建支架
			{
				std::cout << "往" << 0 << "建內支架" << std::endl;
				ConstructConeNeighborBall( i , i -> corner[0] , SIZE_S );
			}
			else
			{
				std::cout << "往" << 0 << "方向的球與這顆球建支架相連" << std::endl;
				ConstructConeConn( i , i -> corner[0] , SIZE_S );
			}
			if( !pointList -> CloselyContain( i -> corner[1] ) )
			{
				std::cout << "往" << 1 << "建內支架" << std::endl;
				ConstructConeNeighborBall( i , i -> corner[1] , SIZE_S );
			}
			else
			{
				std::cout << "往" << 1 << "方向的球與這顆球建支架相連" << std::endl;
				ConstructConeConn( i , i -> corner[1] , SIZE_S );
			}
			if( !pointList -> CloselyContain( i -> corner[2] ) )
			{
				std::cout << "往" << 2 << "建內支架" << std::endl;
				ConstructConeNeighborBall( i , i -> corner[2] , SIZE_S );
			}
			else
			{
				std::cout << "往" << 2 << "方向的球與這顆球建支架相連" << std::endl;
				ConstructConeConn( i , i -> corner[2] , SIZE_S );
			}
			if( !pointList -> CloselyContain( i -> corner[3] ) )
			{
				std::cout << "往" << 3 << "建內支架" << std::endl;
				ConstructConeNeighborBall( i , i -> corner[3] , SIZE_S );
			}
			else
			{
				std::cout << "往" << 3 << "方向的球與這顆球建支架相連" << std::endl;
				ConstructConeConn( i , i -> corner[3] , SIZE_S );
			}
		}

	}
}

//=================================================================================
/*
以下室outerCone以下室outerCone以下室outerCone以下室outerCone以下室outerCone以下室outerCone以下室outerConeCheckIntersectWithConeFace
以下室outerCone以下室outerCone以下室outerCone以下室outerCone以下室outerCone以下室outerCone以下室outerCone
*/
//=================================================================================
bool ZomeGenerator::CheckEngaged( OuterCone* outerCone )
{
	bool isEngaged = false;
	//一個cone的最大距離maxConeDistance
	//GLfloat maxConeDistance = zomeDirection -> GetColorLength( COLOR_RED , SIZE_L );
	GLfloat maxConeDistance = 150.0;

	//std::cout << "查看點相交";
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		/*
		std::cout << "搜尋面1:" << ( *( model -> indices ) )[ i * 3 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 1 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 2 ] << std::endl;
		std::cout << "搜尋點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
		*/
		//如果模型中的這個面離方塊不到1mm
		//這邊寫錯了 應該是平面到點的距離不到根號3 布是點到點
		//d = N * P - N * P1
		//N = P2-P1 * P3-P1 Normalize
		Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		/*
		GLfloat dddd = abs( normal -> Dot( grid -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) );
		GLfloat vvvv = ( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[0];
		*/
		//與玉測試的平面的距離 小於一個cone的最大距離
		if( abs( normal -> Dot( outerCone -> sourceCorner[0] -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) ) 
			< maxConeDistance )
		{
			//std::cout << ".";
			/*
			std::cout << "查看點相交:點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 2 ] << std::endl;
			
			std::cout << "查看點相交:點2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 2 ] << std::endl;
			std::cout << "查看點相交:點3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 2 ] << std::endl;
				*/						   
			//寫錯了
			
			isEngaged |= outerCone -> CheckIntersectOuterConeOnTri ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
																     &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											 					     &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] );
			/*
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] );
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			*/
		}
		delete vector1;
		delete vector2;
		delete normal;

		if( isEngaged )
		{
			break;
		}
	}
	if( isEngaged ) 
		std::cout << "這個outerCone" << outerCone << "的第" << outerCone -> type << "種組法有香蕉" << std::endl;
		//std::cout << "，有香蕉";
	outerCone -> engaged = isEngaged;
	return isEngaged;
}

bool ZomeGenerator::CheckDistanceToSurface( OuterCone* outerCone )
{//去查每個outercone到外表的距離
	/*
	float dist_Point_to_Plane( Point P, Plane PL, Point* B)
	{
	    //http://geomalgorithms.com/a04-_planes.html
		//    Input:  P  = a 3D point
		//            PL = a  plane with point V0 and normal n
		//    Output: *B = base point on PL of perpendicular from P
		//    Return: the distance from P to the plane PL
		//    d norm 距離
		float    sb, sn, sd;

		sn = -dot( PL.n, (P - PL.V0));
		sd = dot(PL.n, PL.n);
		sb = sn / sd;

		*B = P + sb * PL.n;
		return d(P, *B);
	}*/
	//算出三個末端的中心
	/*
	GLfloat* endPointCenter = new GLfloat[3];
	endPointCenter[0] = ( outerCone -> cornerEnd[0] -> position[0] + 
						  outerCone -> cornerEnd[1] -> position[0] + 
						  outerCone -> cornerEnd[2] -> position[0] ) / 3.0;
	endPointCenter[1] = ( outerCone -> cornerEnd[0] -> position[1] + 
						  outerCone -> cornerEnd[1] -> position[1] + 
						  outerCone -> cornerEnd[2] -> position[1] ) / 3.0;
	endPointCenter[2] = ( outerCone -> cornerEnd[0] -> position[2] + 
						  outerCone -> cornerEnd[1] -> position[2] + 
						  outerCone -> cornerEnd[2] -> position[2] ) / 3.0;*/
	Point* endPointCenter = new Point( outerCone -> cornerEnd[0] -> position );
	endPointCenter -> Plus( outerCone -> cornerEnd[1] );
	endPointCenter -> Plus( outerCone -> cornerEnd[2] );
	endPointCenter -> Multiply( 1.0 / 3.0 );
	if( outerCone -> cornerEnd[0] -> DistanceToPoint( outerCone -> cornerEnd[1] -> position ) > NODE_DIAMETER * 3.0 )
	{
		std::cout << "ZomeGenerator::CheckDistanceToSurface出錯：三個endPoint距離太遠(" <<
			outerCone -> cornerEnd[0] -> position[0] << "," <<
			outerCone -> cornerEnd[0] -> position[1] << "," <<
			outerCone -> cornerEnd[0] -> position[2] << ")(" <<
			outerCone -> cornerEnd[1] -> position[0] << "," <<
			outerCone -> cornerEnd[1] -> position[1] << "," <<
			outerCone -> cornerEnd[1] -> position[2] << ")(" <<
			outerCone -> cornerEnd[2] -> position[0] << "," <<
			outerCone -> cornerEnd[2] -> position[1] << "," <<
			outerCone -> cornerEnd[2] -> position[2] << ")" << std::endl;
	}


	Point* tempNearstPoint = new Point();

	//紀錄最小距離
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		//算normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		/*
		GLfloat* vertexNormalAverage = new GLfloat[3];//vertex normal平均值
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		MathHelper::VectorMutiply( vertexNormalAverage , 1.0 / 3.0 );*/
		//( Normal 1 + Normal 2 + Normal 3 ) / 3
		Point* vertexNormalAverage = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		vertexNormalAverage -> Plus( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
		vertexNormalAverage -> Plus( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		vertexNormalAverage -> Multiply( 1.0 / 3.0 );
		//如果和點法向量相反方向，就把平面normal反轉
		if( vertexNormalAverage -> Dot( normal -> position ) < 0 ) normal -> Reverse();
		
		//從末端到表面 與normal同方向
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		GLfloat sb = normal -> Dot( distancePtoV0 );//是正的 如果點在模型裡，也就是距離
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//是正的 如果點在模型裡，也就是距離

		if( sb < minDistance && sb > -0.1 )
		{
			//如果這個面與點的距離小於目前的最小距離，則來看
			//1.這個點投影到面上時，會在三角型三個點之內，則她的距離就試與平面的距離
			//2.這個點投影到面上時，在三角型三個點之外，則她的距離就是與三個點的最小距離(比與平面的距離大)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//這邊算投影的點
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			if( abs( sb ) > SMALL_NUMBER )
			{
				normal -> Multiply( sb );
				pointProjectOnPlane -> Plus( normal );
				normal -> Multiply( 1.0 / sb );
			}

			//算第一個做標 第一個是三角型總面積x2 第二個是三角型點0到投影點的距離 第三個是投影點和編1所包的面積
			Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , pointProjectOnPlane -> position );
			Point* vertex1ToPoint = new Point(); vertex1ToPoint -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , pointProjectOnPlane -> position );
			Point* crossArea1 = new Point(); crossArea1 -> Cross( vector1 , vertex0ToPoint );
			Point* crossArea2 = new Point(); crossArea2 -> Cross( vertex1ToPoint , vector2 );
			
			//算第一個做標漢第二個做標
			GLfloat TotalArea = crossTotalArea -> Dot( normal );
			GLfloat Area1 = crossArea1 -> Dot( normal );
			GLfloat Area2 = crossArea2 -> Dot( normal );
			if( ( Area1 >= 0 && Area2 >= 0 ) || ( Area1 <= 0 && Area2 <= 0 ) )//這行代表一個正一個負，就是這個點未在三角形中
			{
				if( abs( TotalArea ) > abs( Area1 ) + abs( Area2 ) )
				{
					//有在三角形上
					insideTriangle = true;
				}
			}

			if( insideTriangle )
			{
				minDistance = sb;
				tempNearstPoint -> Copy( pointProjectOnPlane );
			}
			else
			{
				GLfloat* distance = new GLfloat[3];
				distance[0] = endPointCenter -> DistanceToPoint( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
				distance[1] = endPointCenter -> DistanceToPoint( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
				distance[2] = endPointCenter -> DistanceToPoint( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
				int min = 0;

				//找出最近的點
				if( distance[0] < distance[1] )
					if( distance[0] < distance[2] )
						min = 0;
					else
						min = 2;
				else
					if( distance[1] < distance[2] )
						min = 1;
					else
						min = 2;

				//把這個點的位置給紀錄下來 寄在tempNearstPoint裡
				if( distance[ min ] < minDistance )
				{
					minDistance = distance[ min ];
					tempNearstPoint -> Copy( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + min ] * 3 );
				}
				delete[] distance;
			}
			delete crossTotalArea;
			delete vertex0ToPoint;
			delete vertex1ToPoint;
			delete crossArea1;
			delete crossArea2;
			delete pointProjectOnPlane;
		}
	}
	//這行去看他和另一個端點距離多近
	GLfloat distanceToNearest = 99999;
	GLfloat minDistance_DistanceToNearest;
	GLfloat distanceBetweenEnd = 9999;
	for( OuterCone* o = firstOuterCone; o != NULL; o = o -> link )
	{


		if( o == outerCone ) continue;
		if( o -> type == 0 ) continue;
		if( o -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position ) < distanceToNearest )
		{
			distanceToNearest = o -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position );
		}
		if( o -> cornerEnd[0] -> DistanceToPoint( outerCone -> cornerEnd[0] -> position ) < distanceBetweenEnd )
		{
			distanceBetweenEnd = o -> cornerEnd[0] -> DistanceToPoint( outerCone -> cornerEnd[0] -> position );
		}
	}
	
	minDistance_DistanceToNearest = minDistance;
	if( distanceToNearest < 100 )
	{
		minDistance_DistanceToNearest = minDistance + pow( ( 100.0 - distanceToNearest ) , DISTANCE_PARAM ) / 50.0;
		if( outerCone -> type == 1 ) minDistance_DistanceToNearest *= 10.0;
	}
	//如果兩個相鄰點泰近，就刪掉一個點
	if( distanceToNearest < SOCKET_RADIUS * 2.0 || distanceBetweenEnd < SOCKET_RADIUS * 2.0 )
	{
		std::cout <<  "這個outerCone" << outerCone << "的第" << outerCone -> type << "種組法，與其他outerCone距離太近(" << distanceToNearest << ")所以刪掉" << std::endl;
		delete tempNearstPoint;
		return false;
	}
	std::cout << "這個outerCone" << outerCone << "的第" << outerCone -> type << "種組法，與其他outerCone距離" << distanceToNearest << "，加權總距離是" << minDistance_DistanceToNearest << std::endl;
	outerCone -> distanceToSurface[ outerCone -> type ] = minDistance;
	outerCone -> weightedDistance[ outerCone -> type ] = minDistance_DistanceToNearest;

	
	//這邊紀錄最靠近外支架的點的位置，先考量她是布是目前最靠近的外支架建立方式，試的話就把點給紀錄下來
	bool theNearest = true;
	for( int i = 0; i < TYPE_AMOUNT; i++ )
	{
		if( outerCone -> weightedDistance[i] < minDistance_DistanceToNearest && outerCone -> distanceToSurface[i] != -1.0 )
		{
			theNearest = false;
		}
	}
	if( theNearest )
	{
		//把這個最近點書進outercone裡，並擺index改乘她的type
		outerCone -> nearestPointOnSurface -> Copy( tempNearstPoint );
		outerCone -> nearestPointOnSurface -> index = outerCone -> type;
	}
	delete tempNearstPoint;
	return true;
}

bool ZomeGenerator::TryDifferentType( OuterCone* outerCone )
{
	std::cout << "OuterCone" << outerCone << "的三個源點的方向是" << outerCone -> coneDirection << "，來源面是" << outerCone -> sourceConeFace << std::endl <<
		"(" << outerCone -> sourceCorner[0] -> position[0] << "," << 
		outerCone -> sourceCorner[0] -> position[1] << "," << 
		outerCone -> sourceCorner[0] -> position[2] << ")," << std::endl;
	std::cout << "(" << outerCone -> sourceCorner[1] -> position[0] << "," << 
		outerCone -> sourceCorner[1] -> position[1] << "," << 
		outerCone -> sourceCorner[1] -> position[2] << ")," << std::endl;
	std::cout << "(" << outerCone -> sourceCorner[2] -> position[0] << "," << 
		outerCone -> sourceCorner[2] -> position[1] << "," << 
		outerCone -> sourceCorner[2] -> position[2] << ")," << std::endl;

	GLfloat debugCorner0[3] = {-23,12,47};
	GLfloat debugCorner1[3] = {-23,12,0};
	GLfloat debugCorner2[3] = {0,35,23};
	if( MathHelper::VectorDistance( debugCorner0 , outerCone -> sourceCorner[0] -> position ) < 5.0 ||
		MathHelper::VectorDistance( debugCorner0 , outerCone -> sourceCorner[1] -> position ) < 5.0 ||
		MathHelper::VectorDistance( debugCorner0 , outerCone -> sourceCorner[2] -> position ) < 5.0 )
	{
		std::cout << "debug:群找重複的外部錐，在(-23,12,47)" << std::endl;
		if( MathHelper::VectorDistance( debugCorner1 , outerCone -> sourceCorner[0] -> position ) < 5.0 ||
			MathHelper::VectorDistance( debugCorner1 , outerCone -> sourceCorner[1] -> position ) < 5.0 ||
			MathHelper::VectorDistance( debugCorner1 , outerCone -> sourceCorner[2] -> position ) < 5.0 )
		{
			std::cout << "debug:群找重複的外部錐，在(-23,12,0)" << std::endl;
			if( MathHelper::VectorDistance( debugCorner2 , outerCone -> sourceCorner[0] -> position ) < 5.0 ||
				MathHelper::VectorDistance( debugCorner2 , outerCone -> sourceCorner[0] -> position ) < 5.0 ||
				MathHelper::VectorDistance( debugCorner2 , outerCone -> sourceCorner[0] -> position ) < 5.0 )
			{
				std::cout << "debug:群找重複的外部錐，在(0,35,23)" << std::endl;
			}
		}
	}


	//把所有可能打健髮都是一遍 然後選出最好的方法重建
	CheckEngaged( outerCone );
	if( !outerCone -> engaged && 
		!outerCone -> dublicatedWithCone && 
		!outerCone -> dublicatedWithOuterCone &&
		!outerCone -> intersected )
	{
		//outerCone -> CheckDistanceToSurface();
		CheckDistanceToSurface( outerCone );
	}
	
	//如果這個outercone建起來的時候重複了，代表她的方向上被其他的cone包為，但那個方向沒有本身的cone，
	//這個時候這個outercone應該直接部件
	if( outerCone -> dublicatedWithCone )
	{
		outerCone -> TryDifferentType( 0 );
		return true;
	}


	for( int i = 2; i < TYPE_AMOUNT; i++ )//
	{
		if( !outerCone -> TryDifferentType( i ) ) continue;
		CheckEngaged( outerCone );
		if( !outerCone -> engaged && 
			!outerCone -> dublicatedWithCone && 
			!outerCone -> dublicatedWithOuterCone &&
			!outerCone -> intersected )
		{
			//outerCone -> CheckDistanceToSurface();
			CheckDistanceToSurface( outerCone );
		}
		else
		{
			if( outerCone -> engaged )
			{
				std::cout << "第" << i << "種組法支架與其他Cone重疊" << std::endl;
			}
			if( outerCone -> dublicatedWithCone )
			{
				std::cout << "第" << i << "種組法支架與其他Cone重疊" << std::endl;
			}
		}
	}

	GLfloat minWeightedDistance = 999999.9;
	int minDistanceType = 0;
	for( int i = 1; i < TYPE_AMOUNT; i++ )
	{
		if( outerCone -> distanceToSurface[i] != -1.0 && outerCone -> weightedDistance[i] < minWeightedDistance )
		{
			minWeightedDistance = outerCone -> weightedDistance[i];
			minDistanceType = i;
		}
	}

	if( minDistanceType == 0 )
	{
		std::cout << "ZomeGenerator::TryDifferentType出錯：沒有任何組法可用" << std::endl;
	}

	//試用
	//if( outerCone -> distanceToSurface[5] != -1.0 ) minDistanceType = 2;
	//else minDistanceType = 0;
	//minDistanceType = 1;
	outerCone -> TryDifferentType( minDistanceType );
	CheckDistanceToSurface( outerCone );
	//if( minDistanceType != 0 && outerCone -> cornerEnd[0] -> DistanceToPoint( outerCone -> cornerEnd[1] -> position ) > 20 ) 
	//{
	//	std::cout<<"出錯";
	//}
	return true;
}

bool ZomeGenerator::GenerateOuterCone()
{
	for( Cone* i = firstCone; i != NULL; i = i -> link )
	{
		//當距離是0或1 就跳過不建
		if( i -> engagedDistance == CONE_ENGAGED_DISTANCE )
		{
			for( int j = 0; j < 4; j++ )
			{
				if( i -> neighborCone[j] )
				{
					if( i -> neighborCone[j] -> engagedDistance < CONE_ENGAGED_DISTANCE &&
						i -> neighborCone[j] -> engagedDistance >= 0 )
					{
						if( firstOuterCone == NULL )
						{
							firstOuterCone = new OuterCone( i , j , 1 );
							TryDifferentType( firstOuterCone );
							if( firstOuterCone -> type == 0 )
							{
								std::cout << "ZomeGenerator::GenerateOuterCone出錯：(" << firstOuterCone -> sourceCorner[0] -> position[0] << "," <<
									firstOuterCone -> sourceCorner[0] -> position[1] << "," <<
									firstOuterCone -> sourceCorner[0] -> position[2] << "),(" <<
									firstOuterCone -> sourceCorner[1] -> position[0] << "," <<
									firstOuterCone -> sourceCorner[1] -> position[1] << "," <<
									firstOuterCone -> sourceCorner[1] -> position[2] << "),(" <<
									firstOuterCone -> sourceCorner[2] -> position[0] << "," <<
									firstOuterCone -> sourceCorner[2] -> position[1] << "," <<
									firstOuterCone -> sourceCorner[2] -> position[2] << ")無法建立" << std::endl;
							}
						}
						else
						{
							OuterCone* tempOuterCone = new OuterCone( i , j , 1 , firstOuterCone );
							TryDifferentType( tempOuterCone );
							if( tempOuterCone -> type == 0 )
							{
								std::cout << "ZomeGenerator::GenerateOuterCone出錯：(" << tempOuterCone -> sourceCorner[0] -> position[0] << "," <<
									tempOuterCone -> sourceCorner[0] -> position[1] << "," <<
									tempOuterCone -> sourceCorner[0] -> position[2] << "),(" <<
									tempOuterCone -> sourceCorner[1] -> position[0] << "," <<
									tempOuterCone -> sourceCorner[1] -> position[1] << "," <<
									tempOuterCone -> sourceCorner[1] -> position[2] << "),(" <<
									tempOuterCone -> sourceCorner[2] -> position[0] << "," <<
									tempOuterCone -> sourceCorner[2] -> position[1] << "," <<
									tempOuterCone -> sourceCorner[2] -> position[2] << ")無法建立" << std::endl;
							}
							tempOuterCone -> AddToLink();
						}
					}
				}
			}
		}
	}
	for( int i = 0; i < REARRANGE_TIMES - 1; i++ )
	{
		for( OuterCone* j = firstOuterCone; j != NULL; j = j -> link )
		{
			//debug
			float debugPos1[3] = { 23 , -23 , 23 };
			float debugPos2[3] = { 47 , -47 , 47 };
			if( MathHelper::VectorDistance( j -> sourceCone -> corner[0] -> position , debugPos1 ) < 3 || 
				MathHelper::VectorDistance( j -> sourceCone -> corner[1] -> position , debugPos1 ) < 3 ||
				MathHelper::VectorDistance( j -> sourceCone -> corner[2] -> position , debugPos1 ) < 3 ||
				MathHelper::VectorDistance( j -> sourceCone -> corner[3] -> position , debugPos1 ) < 3 )
			{
				if( MathHelper::VectorDistance( j -> sourceCone -> corner[0] -> position , debugPos2 ) < 3 ||
					MathHelper::VectorDistance( j -> sourceCone -> corner[1] -> position , debugPos2 ) < 3 ||
					MathHelper::VectorDistance( j -> sourceCone -> corner[2] -> position , debugPos2 ) < 3 ||
					MathHelper::VectorDistance( j -> sourceCone -> corner[3] -> position , debugPos2 ) < 3 )
				{
					std::cout << "找到出錯的外三角錐，四個角為：" << std::endl;
					std::cout << "(" << j -> sourceCone -> corner[0] -> position[0] << "," <<
										j -> sourceCone -> corner[0] -> position[1] << "," <<
										j -> sourceCone -> corner[0] -> position[2] << ")" << std::endl;
					std::cout << "(" << j -> sourceCone -> corner[1] -> position[0] << "," <<
										j -> sourceCone -> corner[1] -> position[1] << "," <<
										j -> sourceCone -> corner[1] -> position[2] << ")" << std::endl;
					std::cout << "(" << j -> sourceCone -> corner[2] -> position[0] << "," <<
										j -> sourceCone -> corner[2] -> position[1] << "," <<
										j -> sourceCone -> corner[2] -> position[2] << ")" << std::endl;
					std::cout << "(" << j -> sourceCone -> corner[3] -> position[0] << "," <<
										j -> sourceCone -> corner[3] -> position[1] << "," <<
										j -> sourceCone -> corner[3] -> position[2] << ")，他的型為第" << j -> type << "型" << std::endl;
					

					std::cout << "第1根竿子末端為(" << j ->cornerEnd[0] -> position[0] << "," <<
													   j ->cornerEnd[0] -> position[1] << "," <<
													   j ->cornerEnd[0] -> position[2] << ")" << std::endl;
					std::cout << "第2根竿子末端為(" << j ->cornerEnd[1] -> position[0] << "," <<
													   j ->cornerEnd[1] -> position[1] << "," <<
													   j ->cornerEnd[1] -> position[2] << ")" << std::endl;
					std::cout << "第3根竿子末端為(" << j ->cornerEnd[2] -> position[0] << "," <<
													   j ->cornerEnd[2] -> position[1] << "," <<
													   j ->cornerEnd[2] -> position[2] << ")" << std::endl;
				}

			}


			j -> RefreshDistanceToSurface();
			TryDifferentType( j );
			if( j -> type == 0 )
			{
				std::cout << "ZomeGenerator::GenerateOuterCone出錯：(" << j -> sourceCorner[0] -> position[0] << "," <<
					j -> sourceCorner[0] -> position[1] << "," <<
					j -> sourceCorner[0] -> position[2] << "),(" <<
					j -> sourceCorner[1] -> position[0] << "," <<
					j -> sourceCorner[1] -> position[1] << "," <<
					j -> sourceCorner[1] -> position[2] << "),(" <<
					j -> sourceCorner[2] -> position[0] << "," <<
					j -> sourceCorner[2] -> position[1] << "," <<
					j -> sourceCorner[2] -> position[2] << ")無法建立" << std::endl;
			}


			
		}
	}
	return true;
}

void ZomeGenerator::GenerateOuterConeStructure()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 ) continue;
		//( i -> coneDirection != 3 ) continue;
		if( MathHelper::VectorApproximation( i -> sourceCorner[0] -> position , i -> sourceCorner[1] -> position ) ||
			MathHelper::VectorApproximation( i -> sourceCorner[0] -> position , i -> sourceCorner[2] -> position ) ||
			MathHelper::VectorApproximation( i -> sourceCorner[2] -> position , i -> sourceCorner[1] -> position ) )
		{
			std::cout << "兩個之價從同一個點長出來，出錯" << std::endl;
		}
		for( PointList* j = pointList -> firstPointList; j != NULL; j = j -> link )
		{
			for( int k = 0; k < 3; k++ )
			{
				//如果和pointlist裡面一樣，就長出來
				if( MathHelper::VectorApproximation( i -> sourceCorner[k] -> position , j -> position ) )
				{
					
					////////////////////////////////////
					std::cout << "在第" << j -> index << "號球上建立外支架，朝向第" << i -> constructedHole[k] << "面" << std::endl;
					if( i -> constructedHole[k] == -1 )
					{
						std::cout << "建外支架的面出錯！！！！！！" << std::endl;
					}
					////////////////////////////////////
					GLfloat* tempPosition = new GLfloat[3];
					GLfloat length = zomeDirection -> GetFaceLength ( i -> constructedHole[k] , SIZE_S );
					MathHelper::SetVector( tempPosition , 
										   i -> sourceCorner[k] -> position[0] + zomeDirection -> faceDirection[ i -> constructedHole[k] * 3 ] * length ,
										   i -> sourceCorner[k] -> position[1] + zomeDirection -> faceDirection[ i -> constructedHole[k] * 3 + 1 ] * length ,
										   i -> sourceCorner[k] -> position[2] + zomeDirection -> faceDirection[ i -> constructedHole[k] * 3 + 2 ] * length );
					for( PointList* l = pointList -> firstPointList; l != NULL; l = l -> link )
					{
						if(  MathHelper::VectorApproximation( tempPosition , l -> position ) )
						{
							std::cout << "建外支架的支架出錯，該支架方向連向內部而非向外！！！！！！" << std::endl;
						}
					}
					delete tempPosition;
					///////////////////////////////////////
					
					constructor -> Construct( j -> index , i -> constructedHole[k] , i -> constructedSize[k] , TYPE_TRUSS );

				}
			}
		}
	}



}

void ZomeGenerator::ZomeStructureAnalyze()
{
	structureAnalyzer = new StructureAnalyzer( constructor );


	structureAnalyzer -> Analyze();
	
}

//=======================================================================
//以下室socket
//=======================================================================

bool ZomeGenerator::GenerateSocket()
{
	socketConstructor = new SocketConstructor();
	//PointList* socketPointList = new PointList();
	//這邊是外佈三角錐
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		//GLfloat test[3] = { -22.342091 , -80.901695 , 34.948772 };
		//if( MathHelper::VectorDistance( i -> nearestPointOnSurface-> position , test ) < 1.0 )
		//{
		//	std::cout << "debug";
		//}

		if( i -> type != 0 )
		{
			if( i -> distanceToSurface[ i -> type ] > SOCKET_RADIUS )//超過12mm才會生乘
			{
				socketConstructor -> NewSocket( i );
				Point* tempSurfacePoint = new Point( i -> nearestPointOnSurface -> position );

				if( nearestPointOnSurface == NULL )
				{
					nearestPointOnSurface = tempSurfacePoint;
					nearestPointOnSurface -> firstPoint = tempSurfacePoint;
				}
				else
				{
					nearestPointOnSurface -> AddToLink( tempSurfacePoint );
				}
			}
			else
			{
				//socketConstructor -> NewSocket( i );//在這見一顆半圓型
				socketConstructor -> NewOriginalSocket( i );
				Point* tempSurfacePoint = new Point( i -> nearestPointOnSurface -> position );

				if( nearestPointOnSurface == NULL )
				{
					nearestPointOnSurface = tempSurfacePoint;
					nearestPointOnSurface -> firstPoint = tempSurfacePoint;
				}
				else
				{
					nearestPointOnSurface -> AddToLink( tempSurfacePoint );
				}
			}
		}
	}

	return true;
}

bool ZomeGenerator::GenerateBranchSocket()
{
	//這邊是樹枝
	for( std::map< Segment* , Branch* >::iterator i = rootList.begin(); i != rootList.end(); i++ )
	{
		Branch* iteratorBranch = i -> second;

		for( Branch* b = iteratorBranch -> firstBranch ; b != NULL; b = b -> link )
		{
			if( /*b -> isEnd*/ 0 )//不在數的最後一根接了
			{
				if( b -> distanceToSurface[ 1 ] > SOCKET_RADIUS )//超過12mm才會生乘
				{
					socketConstructor -> NewSocket( b );
					Point* tempSurfacePoint = new Point( b -> nearestPointOnSurface -> position );

					if( nearestPointOnSurface == NULL )
					{
						nearestPointOnSurface = tempSurfacePoint;
						nearestPointOnSurface -> firstPoint = tempSurfacePoint;
					}
					else
					{
						nearestPointOnSurface -> AddToLink( tempSurfacePoint );
					}
				}
				else
				{
					//socketConstructor -> NewSocket( i );//在這見一顆半圓型
					socketConstructor -> NewOriginalSocket( b );
					Point* tempSurfacePoint = new Point( b -> nearestPointOnSurface -> position );

					if( nearestPointOnSurface == NULL )
					{
						nearestPointOnSurface = tempSurfacePoint;
						nearestPointOnSurface -> firstPoint = tempSurfacePoint;
					}
					else
					{
						nearestPointOnSurface -> AddToLink( tempSurfacePoint );
					}
				}
			}
		}
	}

	//這邊是數枝末梢
	for( BranchEnd* b = firstBranchEnd ; b != NULL; b = b -> link )
	{
		if( b -> type == 0 ) continue;

		if( b -> distance[ b -> type ][ b -> branchEndDirection ] > SOCKET_RADIUS )//超過12mm才會生乘
		{
			socketConstructor -> NewSocket( b );
			Point* tempSurfacePoint = new Point( b -> nearestPointOnSurface -> position );

			if( nearestPointOnSurface == NULL )
			{
				nearestPointOnSurface = tempSurfacePoint;
				nearestPointOnSurface -> firstPoint = tempSurfacePoint;
			}
			else
			{
				nearestPointOnSurface -> AddToLink( tempSurfacePoint );
			}
		}
		else
		{
			//socketConstructor -> NewSocket( i );//在這見一顆半圓型
			socketConstructor -> NewOriginalSocket( b );
			Point* tempSurfacePoint = new Point( b -> nearestPointOnSurface -> position );

			if( nearestPointOnSurface == NULL )
			{
				nearestPointOnSurface = tempSurfacePoint;
				nearestPointOnSurface -> firstPoint = tempSurfacePoint;
			}
			else
			{
				nearestPointOnSurface -> AddToLink( tempSurfacePoint );
			}
		}

	}
	return true;
}

bool ZomeGenerator::GenerateSurfacePoint( Point* sPoint )
{
	sPoint = NULL;
	//這邊是外佈三角錐
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type != 0 && i -> distanceToSurface[ i -> type ] > SOCKET_RADIUS )//超過8mm才會生乘
		{
			if( sPoint == NULL )
			{
				sPoint = new Point( i -> nearestPointOnSurface -> position );
				sPoint -> firstPoint = sPoint;
			}
			else
			{
				Point* tempPoint = new Point( i -> nearestPointOnSurface -> position );
				sPoint -> AddToLink( tempPoint );//還沒寫
			}
		}
	}
	return true;
}

bool ZomeGenerator::GetNearestPointOnSurface( Point* pointList )
{
	if( !nearestPointOnSurface )
	{
		std::cout << "ZomeGenerator::GetNearestPointOnSurface出錯：沒有點" << std::endl;
		return false;
	}

	pointList -> Copy( nearestPointOnSurface );

	for( Point* p = nearestPointOnSurface -> link; p != NULL; p = p -> link )
	{
		Point* temp = new Point( p -> position );
		pointList -> AddToLink( temp );
	}


	return true;
}

bool ZomeGenerator::ExportNearestPointOnSurface( Point* pointList )
{
	if( !nearestPointOnSurface )
	{
		std::cout << "ZomeGenerator::GetNearestPointOnSurface出錯：沒有點" << std::endl;
		return false;
	}

	FILE* ObjFile = fopen( "NearestPointOnSurface.txt" ,"w");
	int count = 0;
	for( Point* p = nearestPointOnSurface -> link; p != NULL; p = p -> link )
	{
		count++;
		fprintf(ObjFile, "%f %f %f \n", (float)p -> position[0] , (float)p -> position[1] , (float)p -> position[2] );
		std::cout << "書出表面上的點:(" <<  p -> position[0] << "," << p -> position[1] << "," << p -> position[2] << ")" << std::endl;
	}
	fclose(ObjFile);
	std::cout << "共產生了" << count << "個點" << std::endl;
}

//=======================================================================
//以下室branch
//=======================================================================

bool ZomeGenerator::GenerateBranch( Polyhedron* p , Segmenter* s , OuterCone* oCone )
{
	polyhedron = p;
	segmenter = s;

	//
	for( Segment* se = s -> segment; se != NULL; se = se -> link )
	{
		if( CheckSegmentTooBig( se ) )
		{
			OuterCone* outerCone = GetOuterCone( se , oCone );
			//取得與該segment相連的outercone
			Point* farPoint = se -> GetFarPoint( outerCone -> nearestPointOnSurface );

			std::cout << "最遠點位在(" << farPoint -> position[0] << "," << farPoint -> position[1] << "," << farPoint -> position[2] << ")" << std::endl;

			Branch* tempBranch[3] = { GrowBranch( outerCone -> sourceCorner[0] , outerCone , farPoint ) ,
									  GrowBranch( outerCone -> sourceCorner[1] , outerCone , farPoint ) ,
									  GrowBranch( outerCone -> sourceCorner[2] , outerCone , farPoint ) };

			GLfloat endDistance[3] = { -1 , -1 , -1 };

			int efficiency[3] = { CheckEfficientPath( tempBranch[0] , farPoint , outerCone , polyhedron , &(endDistance[0]) ) ,
								  CheckEfficientPath( tempBranch[1] , farPoint , outerCone , polyhedron , &(endDistance[1]) ) ,
								  CheckEfficientPath( tempBranch[2] , farPoint , outerCone , polyhedron , &(endDistance[2]) ) };
			
			efficiency[0] += int( endDistance[0] / EFFICIENCY_PARAM );
			efficiency[1] += int( endDistance[1] / EFFICIENCY_PARAM );
			efficiency[2] += int( endDistance[2] / EFFICIENCY_PARAM );

			Branch* bestBranch = tempBranch[0];
			//取效率最高的
			if( efficiency[0] <= efficiency[1] && efficiency[0] <= efficiency[2] && !(efficiency[0] > 999) )
			{
				Delete( tempBranch[1] );
				Delete( tempBranch[2] );
				bestBranch = tempBranch[0];
				//AddToLink( tempBranch[0] );
			}
			else if( efficiency[1] <= efficiency[0] && efficiency[1] <= efficiency[2] && !(efficiency[1] > 999) )
			{
				Delete( tempBranch[0] );
				Delete( tempBranch[2] );
				bestBranch = tempBranch[1];
				//AddToLink( tempBranch[1] );
			}
			else if( efficiency[2] <= efficiency[0] && efficiency[2] <= efficiency[1] && !(efficiency[2] > 999) )
			{
				Delete( tempBranch[0] );
				Delete( tempBranch[1] );
				bestBranch = tempBranch[2];
				//AddToLink( tempBranch[2] );
			}
			else
			{
				//這一次完全沒長出樹支
				std::cout << "ZomeGenerator::GenerateBranch出錯：該碎片完全沒長出樹枝" << std::endl;
				continue;
			}
			if( bestBranch == NULL ) continue;

			rootList.insert( std::pair< Segment* , Branch* >( se , bestBranch ) );


			//GrowBranchEnd( bestBranch ); 

		}
	}

	return true;
}
//如果碎片泰大，就要來在裡面多生成支架，來縮小他
bool ZomeGenerator::CheckSegmentTooBig( Segment* s )
{
	if( s -> GetArea() > SEGMENT_AREA_LIMIT_TO_BRANCH ) //超過這個數字，就要來生支架
	{
		return true;
	}
	return false;
}

Branch* ZomeGenerator::GrowBranch( Point* point , OuterCone* outerCone , Point* fPoint )
{
	Branch* tempFirstBranch = NULL;
	std::vector< Branch* > newBranch;

	for( int i = 0; i < 12; i++ )
	{
		Branch* tempBranch = new Branch( point -> position , i , outerCone , fPoint -> position );
		std::cout << "從(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
							  tempBranch -> sourceCorner[0] -> position[1] << "," << 
							  tempBranch -> sourceCorner[0] -> position[2] << ")向第" << tempBranch -> connectFace[0] << "面建出第-----" << std::endl;

		if( CheckEngaged( tempBranch ) )
		{
			std::cout << "(" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << ")的第" << i << "面碰到模型表面，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( tempBranch -> CheckIntersectWithConeFace() )
		{
			std::cout << "(" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << ")的第" << i << "面碰到插入三角錐，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( tempBranch -> CheckIntersectWithConnector() )
		{
			std::cout << "(" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << ")的第" << i << "面與其他outercone泰靠近，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( CheckIntersectWithBranch( tempBranch ) || tempBranch -> CheckIntersectWithBranch( tempFirstBranch ) )//與 錢以精確定建好的數支測試香蕉
		{//grownext也要改
			std::cout << "(" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << ")的第" << i << "面碰到與其他樹枝，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( tempBranch -> CheckDuplicated( tempFirstBranch ) )//沒有和目前這一組重疊
		{
			std::cout << "(" << point -> position[0] << "," << point -> position[1] << "," << point -> position[2] << ")的第" << i << "面與其他樹枝重複，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}

		CheckDistanceToSurface( tempBranch );
		if( abs( tempBranch -> distanceToSurface[0] ) < NODE_DIAMETER / 2.0 ||
			abs( tempBranch -> distanceToSurface[1] ) < NODE_DIAMETER / 2.0 )
		{
			delete tempBranch;
			continue;
		}
		//這編要看樹之有沒有長太遠，工是是 樹之道原點平方 + 樹之道遠點平方 < 原點到遠點平方+1000
		tempBranch -> endDistanceToFarPoint = MathHelper::VectorDistance( tempBranch -> farPoint , tempBranch -> sourceCorner[1] -> position );
		GLfloat OCtoBranch = MathHelper::VectorDistance( tempBranch -> sourceOuterCone -> sourceCorner[0] -> position , tempBranch -> sourceCorner[1] -> position );
		GLfloat distanceThreshold = MathHelper::VectorDistance( tempBranch -> farPoint , tempBranch -> sourceOuterCone -> sourceCorner[0] -> position );
		if( pow( distanceThreshold , 2 ) + DISTANCE_RELAX < pow( OCtoBranch , 2 ) + pow( tempBranch -> endDistanceToFarPoint , 2 ) )
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面偏離太遠，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}


		//如果可以建，就建出來，加入支錢的branch
		if( tempFirstBranch == NULL )
		{
			std::cout << "建立新的樹枝頭" << std::endl;
			tempBranch -> index = 0;
			tempBranch -> firstBranch = tempBranch;
			tempFirstBranch = tempBranch;
		}
		else
		{
			//線在這個樹支去和支錢的樹支連結
			tempBranch -> ConnectWithNeighborBranch( tempFirstBranch );
			tempFirstBranch -> AddToLink( tempBranch );
		}

		std::cout << "從(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
							  tempBranch -> sourceCorner[0] -> position[1] << "," << 
							  tempBranch -> sourceCorner[0] -> position[2] << ")向第" << tempBranch -> connectFace[0] << "面建出第" << tempBranch -> index << "跟樹枝" << std::endl;


		MathHelper::CopyVector( tempBranch -> farPoint , fPoint -> position );
		tempBranch -> endDistanceToFarPoint = MathHelper::VectorDistance( tempBranch -> farPoint , tempBranch -> sourceCorner[1] -> position );

		newBranch.push_back( tempBranch );
		/*
		if( !tempBranch -> CheckConnectToOtherHead() )
		{
			GrowNextBranch( tempBranch , BRANCH_LENGTH_LIMIT );
		}
		*/
	} 

	//廣度優先
	for( const auto &i : newBranch ) {
		GrowNextBranch( i , BRANCH_LENGTH_LIMIT );
	}


	return tempFirstBranch;
}

bool ZomeGenerator::GrowNextBranch( Branch* lastBranch , int iterateTimes )
{
	//如果長了10次就放器
	if( iterateTimes == 0 ) return false;

	std::vector< Branch* > newBranch;//這行用來弄廣度優先
	//Branch* tempFirstBranch = NULL;
	for( int i = 0; i < 6; i++ )
	{
		Branch* tempBranch = new Branch( i , lastBranch );

		if( CheckEngaged( tempBranch ) )
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面碰到模型表面，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( tempBranch -> CheckIntersectWithConeFace() )
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面碰到插入三角錐，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( tempBranch -> CheckIntersectWithConnector() )
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面與其他outercone泰靠近，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( CheckIntersectWithBranch( tempBranch ) || tempBranch -> CheckIntersectWithBranch( lastBranch -> firstBranch ) )//與 錢以精確定建好的數支測試香蕉
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面碰到與其他樹枝，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		else if( tempBranch -> CheckDuplicated( lastBranch -> firstBranch ) )//沒有和目前這一組重疊
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面與其他樹枝重複，所以刪除" << std::endl;
			delete tempBranch;
			continue;
		}
		//這邊看編編有沒有到表面
		CheckDistanceToSurface( tempBranch );
		if( abs( tempBranch -> distanceToSurface[0] ) < NODE_DIAMETER / 2.0 ||
			abs( tempBranch -> distanceToSurface[1] ) < NODE_DIAMETER / 2.0 )
		{
			delete tempBranch;
			continue;
		}
		//這編要看樹之有沒有長太遠，工是是 樹之道原點平方 + 樹之道遠點平方 < 原點到遠點平方+1000
		tempBranch -> endDistanceToFarPoint = MathHelper::VectorDistance( tempBranch -> farPoint , tempBranch -> sourceCorner[1] -> position );
		GLfloat OCtoBranch = MathHelper::VectorDistance( tempBranch -> sourceOuterCone -> sourceCorner[0] -> position , tempBranch -> sourceCorner[1] -> position );
		GLfloat distanceThreshold = MathHelper::VectorDistance( tempBranch -> farPoint , tempBranch -> sourceOuterCone -> sourceCorner[0] -> position );
		if( pow( distanceThreshold , 2 ) + DISTANCE_RELAX < pow( OCtoBranch , 2 ) + pow( tempBranch -> endDistanceToFarPoint , 2 ) )
		{
			std::cout << "(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
								tempBranch -> sourceCorner[0] -> position[1] << "," << 
								tempBranch -> sourceCorner[0] -> position[2] << ")的第" << i << "面偏離太遠，所以刪除" << std::endl;
			//std::cout << "總距離:" << distanceThreshold << ",原點到樹枝距離:" << OCtoBranch << ",樹枝到最遠點:" << tempBranch -> endDistanceToFarPoint << std::endl;
			delete tempBranch;
			continue;
		}

		//和其他相連的樹支連起來
		tempBranch -> ConnectWithNeighborBranch( lastBranch );

		//如果可以建，就建出來，加入支錢的branch
		lastBranch -> AddToLink( tempBranch );
		
		std::cout << "從(" << tempBranch -> sourceCorner[0] -> position[0] << "," << 
							  tempBranch -> sourceCorner[0] -> position[1] << "," << 
							  tempBranch -> sourceCorner[0] -> position[2] << ")向第" << tempBranch -> connectFace[0] << "面建出第" << tempBranch -> index << "跟樹枝" << std::endl;

		newBranch.push_back( tempBranch );
		/*
		if( !tempBranch -> CheckConnectToOtherHead() )
		{
			GrowNextBranch( tempBranch , iterateTimes - 1 );
		}*/
	} 

	//廣度優先
	for( const auto &i : newBranch ) {
		GrowNextBranch( i , iterateTimes - 1 );
	}


	return true;
}

OuterCone* ZomeGenerator::GetOuterCone( Segment* segment , OuterCone* outerCone )
{
	if( segment -> surfacePoint == NULL )
	{
		return NULL;
	}


	for( OuterCone* o = outerCone -> firstOuterCone; o != NULL; o = o -> link )
	{
		if( segment -> surfacePoint -> CloselyCompare( o -> nearestPointOnSurface ) )
		{
			return o;
		}
	}
	//segment -> surfacePoint

	std::cout << "ZomeGenerator::GetOuterCone出錯：沒有找到有這一個表面點的外部三角錐" << std::endl;
	return NULL;
}

bool ZomeGenerator::CheckEngaged( Branch* branch )
{
	//先算出頭尾端位置
	GLfloat endPoint[3] , initPoint[3];
	MathHelper::VectorMinus( endPoint , branch -> sourceCorner[1] -> position , branch -> sourceCorner[0] -> position );
	MathHelper::VectorNormalize( endPoint );
	MathHelper::CopyVector( initPoint , endPoint );
	MathHelper::VectorMutiply( initPoint , 0.01 );//從頭往後0.01mm再開始當作樹枝頭，比較不容易在這邊超出模型
	MathHelper::VectorPlus( initPoint , branch -> sourceCorner[0] -> position );
	MathHelper::VectorMutiply( endPoint ,  zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - 0.01 /*- NODE_DIAMETER / 2.0 + LATCH*/ );//如果不算全長，可能會再尾巴穿出模型，但又繼續增長
	MathHelper::VectorPlus( endPoint , branch -> sourceCorner[0] -> position );


	bool isEngaged = false;
	//一個cone的最大距離maxConeDistance
	GLfloat maxConeDistance = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
	//std::cout << "查看點相交";
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		/*
		std::cout << "搜尋面1:" << ( *( model -> indices ) )[ i * 3 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 1 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 2 ] << std::endl;
		std::cout << "搜尋點1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
		*/
		//如果模型中的這個面離方塊不到1mm
		//這邊寫錯了 應該是平面到點的距離不到根號3 布是點到點
		//d = N * P - N * P1
		//N = P2-P1 * P3-P1 Normalize
		Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		


		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		//與玉測試的平面的距離 小於一個cone的最大距離
		if( abs( normal -> Dot( initPoint ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) ) 
			< maxConeDistance /*zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S )*/ )
		{
			isEngaged |= MathHelper::CheckIntersectOnTriangle( initPoint ,
															   endPoint ,
															   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
															   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											 				   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] );
			
		}
		delete vector1;
		delete vector2;
		delete normal;

		if( isEngaged )
		{
			break;
		}
	}
	if( isEngaged ) 
		std::cout << "新的樹枝有香蕉" << std::endl;
	//branch -> engaged = isEngaged;
	return isEngaged;
	//grid -> CheckIntersect( )
}

bool ZomeGenerator::CheckIntersectWithBranch( Branch* branch )
{
	for( std::map< Segment* , Branch* >::iterator i = rootList.begin(); i != rootList.end(); i++ )
	{
		Branch* iteratorBranch = i -> second;

		if( branch -> CheckIntersectWithBranch( iteratorBranch ) )
		{
			return true;
		}
	}
	return false;
}

bool ZomeGenerator::CheckDistanceToSurface( Branch* branch )
{//去查每個outercone到外表的距離
	/*
	float dist_Point_to_Plane( Point P, Plane PL, Point* B)
	{
	    //http://geomalgorithms.com/a04-_planes.html
		//    Input:  P  = a 3D point
		//            PL = a  plane with point V0 and normal n
		//    Output: *B = base point on PL of perpendicular from P
		//    Return: the distance from P to the plane PL
		//    d norm 距離
		float    sb, sn, sd;

		sn = -dot( PL.n, (P - PL.V0));
		sd = dot(PL.n, PL.n);
		sb = sn / sd;

		*B = P + sb * PL.n;
		return d(P, *B);
	}*/
	Point* tempNearstPoint = new Point();

	//紀錄最小距離
	GLfloat minDistance[2] = { 9999999.9 , 9999999.9 };
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		GLfloat tempMinDistance0 = 
			MathHelper::CheckDistanceToTriangle( branch -> sourceCorner[0] -> position ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 , 
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 ,
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ,
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 , 
												 tempNearstPoint -> position );
		if( tempMinDistance0 < minDistance[0] && tempMinDistance0 > 0 )
			minDistance[0] = tempMinDistance0;

		GLfloat tempMinDistance1 = 
			MathHelper::CheckDistanceToTriangle( branch -> sourceCorner[1] -> position ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 , 
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 ,
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ,
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 , 
												 tempNearstPoint -> position );
		if( tempMinDistance1 < minDistance[1] && tempMinDistance1 > -0.1 )
		{
			minDistance[1] = tempMinDistance1;
			//把這個最近點書進樹枝裡，並擺index改乘她的type
			branch -> nearestPointOnSurface -> Copy( tempNearstPoint );
		}
		
	}
	branch -> distanceToSurface[0] = minDistance[0];
	branch -> distanceToSurface[1] = minDistance[1];

	delete tempNearstPoint;
	return true;
}

int ZomeGenerator::CheckEfficientPath( Branch*& branch , Point* farPoint , OuterCone* outerCone , Polyhedron* p , GLfloat* endDistance )
{
	if( branch == NULL )
	{
		std::cout << "樹支" << branch << "沒有長出任何樹支" << std::endl;
		return 99999;
	}

	Branch* endBranch = NULL;
	GLfloat minDistance = 99999;
	//先把把最靠近終點的點找出來
	for( Branch* b = branch -> firstBranch; b != NULL; b = b -> link )
	{
		if( b -> endDistanceToFarPoint < minDistance )
		{
			minDistance = b -> endDistanceToFarPoint;
			endBranch = b;
		}
	}
	if( endBranch == NULL )
	{
		std::cout << "樹支" << branch << "沒有長出任何樹支" << std::endl;
		return 99999;
	}

	*endDistance = endBranch -> endDistanceToFarPoint;

	//把每一個樹支距離中點多遠都算出來
	endBranch -> distance = 0;
	endBranch -> isEnd = true;
	for( int i = 0; i < 12; i++ )
	{
		//計算網投那邊多遠
		if( endBranch -> neighborBranch[i][ BRANCH_HEAD ] != NULL )
		{
			//從最尾巴的樹枝的頭方向樹枝往回算
			if( endBranch -> neighborBranch[i][ BRANCH_HEAD ] -> distance == -1 )
			{
				endBranch -> pathDirection = TOWARD_TAIL;
				//檢查這個樹支網哪個方向長
				//   最後一根樹枝的頭方向相鄰樹枝的相接點座標    等於            最後一根樹枝的頭方向相鄰樹枝           的頭的座標
				if( endBranch -> neighborCorner[i][ BRANCH_HEAD ] == endBranch -> neighborBranch[i][ BRANCH_HEAD ] -> sourceCorner[0] )//意思就是與相鄰樹枝的頭相連
				{
					//TOWRAD_END
					std::cout << "最後一根樹枝往頭方向設定距離，下一個樹枝要往他的尾巴走到現在這根樹枝" << std::endl;
					endBranch -> neighborBranch[i][ BRANCH_HEAD ] -> SetPathDistance( 0 , TOWARD_HEAD );
				}
				//        最後一根樹枝的頭方向相鄰樹枝的相接點座標    等於              最後一根樹枝的頭方向相鄰樹枝        的尾巴的座標
				else if( endBranch -> neighborCorner[i][ BRANCH_HEAD ] == endBranch -> neighborBranch[i][ BRANCH_HEAD ] -> sourceCorner[1] )//意思就是與相鄰樹枝的尾巴相連
				{
					//TOWARD_HEAD
					std::cout << "最後一根樹枝往頭方向設定距離，下一個樹枝要往他的頭走到現在這根樹枝" << std::endl;
					endBranch -> neighborBranch[i][ BRANCH_HEAD ] -> SetPathDistance( 0 , TOWARD_TAIL );
				}
				else
				{
					std::cout << "ZomeGenerator::CheckEfficientPath出錯" << std::endl;
				}
			}
			/*
			//從最尾巴的樹枝的尾巴往回算
			if( endBranch -> neighborBranch[i][ BRANCH_TAIL ] -> distance == -1 )
			{
				//檢查這個樹支網哪個方向長
				//   最後一根樹枝的尾巴方向相鄰樹枝的相接點座標   等於         最後一根樹枝的尾巴方向相鄰樹枝           的頭的座標
				if( endBranch -> neighborCorner[i][ BRANCH_TAIL ] == endBranch -> neighborBranch[i][ BRANCH_TAIL ] -> sourceCorner[0] )
				{
					//TOWRAD_END
					std::cout << "最後一根樹枝往尾巴方向設定距離，下一個樹枝要往他的尾巴走到下下一根樹枝" << std::endl;
					endBranch -> neighborBranch[i][ BRANCH_TAIL ] -> SetPathDistance( 0 , TOWARD_TAIL );
				}
				//       最後一根樹枝的尾巴方向相鄰樹枝的相接點座標   等於        最後一根樹枝的尾巴方向相鄰樹枝           的頭的座標
				else if( endBranch -> neighborCorner[i][ BRANCH_TAIL ] == endBranch -> neighborBranch[i][ BRANCH_TAIL ] -> sourceCorner[1] )
				{
					//TOWARD_HEAD
					std::cout << "最後一根樹枝往尾巴方向設定距離，下一個樹枝要往他的頭走到下下一根樹枝" << std::endl;
					endBranch -> neighborBranch[i][ BRANCH_TAIL ] -> SetPathDistance( 0 , TOWARD_HEAD );
				}
				else
				{
					std::cout << "ZomeGenerator::CheckEfficientPath出錯" << std::endl;
				}
			}
			*/
		}
		//往委應該不用計算
		/*
		if( endBranch -> neighborBranch[i][1] != NULL )
		{
			if( endBranch -> neighborBranch[i][1] -> distance == -1 )
			{
				if( endBranch -> neighborCorner[i][1] == endBranch -> neighborBranch[i][1] -> sourceCorner[0] )
				{
					//TOWRAD_END
					endBranch -> neighborBranch[i][1] -> SetPathDistance( 0 , TOWRAD_END );
				}
				else if( endBranch -> neighborCorner[i][1] == endBranch -> neighborBranch[i][1] -> sourceCorner[1] )
				{
					//TOWARD_HEAD
					endBranch -> neighborBranch[i][1] -> SetPathDistance( 0 , TOWARD_HEAD );
				}
				else
				{
					std::cout << "ZomeGenerator::CheckEfficientPath出錯" << std::endl;
				}
			}
		}
		*/
	}

	//找處最短的路並且做記號，方便之後把其他常的錄刪掉
	Branch* branchOnOuterCone = NULL;
	for( Branch* b = branch -> firstBranch; b != NULL; b = b -> link )
	{
		if( b -> distance != -1 )
		{
			//如果這根的頭連像外層三角錐
			if( b -> sourceCorner[0] -> CloselyCompare( b -> sourceOuterCone -> sourceCorner[0] ) ||
				b -> sourceCorner[0] -> CloselyCompare( b -> sourceOuterCone -> sourceCorner[1] ) ||
				b -> sourceCorner[0] -> CloselyCompare( b -> sourceOuterCone -> sourceCorner[2] ) )
			{
				//且這根樹枝是從頭往尾巴走
				if( b -> pathDirection == TOWARD_TAIL )
				{
					if( branchOnOuterCone == NULL )
					{
						branchOnOuterCone = b;
					}
					else if( b -> distance < branchOnOuterCone -> distance )
					{
						branchOnOuterCone = b;
					}
				}
				else
				{
					std::cout << "ZomeGenerator::CheckEfficientPath出錯：找第一根樹枝時，第一根樹枝的起點不在外部三角形上" << std::endl;
				}
			}
			//如果這根的尾八連像外層三角錐
			if( b -> sourceCorner[1] -> CloselyCompare( b -> sourceOuterCone -> sourceCorner[0] ) ||
				b -> sourceCorner[1] -> CloselyCompare( b -> sourceOuterCone -> sourceCorner[1] ) ||
				b -> sourceCorner[1] -> CloselyCompare( b -> sourceOuterCone -> sourceCorner[2] ) )
			{
				//且這根樹枝是從尾巴往頭走
				if( b -> pathDirection == TOWARD_HEAD )
				{
					if( branchOnOuterCone == NULL )
					{
						branchOnOuterCone = b;
					}
					else if( b -> distance < branchOnOuterCone -> distance )
					{
						branchOnOuterCone = b;
					}
				}
				else
				{
					std::cout << "ZomeGenerator::CheckEfficientPath出錯：找第一根樹枝時，第一根樹枝的起點不在外部三角形上" << std::endl;
				}
			}
		}
	}
	if( branchOnOuterCone == NULL )
	{
		std::cout << "ZomeGenerator::CheckEfficientPath出錯：沒有找到第一根樹枝" << std::endl;
		return 99999;
	}
	branchOnOuterCone -> SetShortestPath( branchOnOuterCone -> distance + 1 );
	

	for( Branch* b = branch -> firstBranch; b != NULL;)
	{
		if( !b -> onPath )//要刪的話就先把戀結指項下一個再刪
		{
			if( b == branch )
			{
				branch = b -> link;
			}
			Branch* tempBranch = b -> link;
			DeleteFromList( b );
			b = tempBranch;
		}
		else//部山的話就直接指項下一個
		{
			b = b -> link;
		}
	}

	return branchOnOuterCone -> distance;
}

bool ZomeGenerator::ConstructBranch()
{
	//
	//int count = 0;
	for( std::map< Segment* , Branch* >::iterator i = rootList.begin(); i != rootList.end(); i++ )
	{
		//debug
		//count++;
		//if( count == 2 )break;
		Branch* iteratorBranch = i -> second;

		Branch* endBranch = NULL;
		GLfloat minDistance = 99999;
		//先把把最靠近終點的點找出來
		for( Branch* b = iteratorBranch -> firstBranch; b != NULL; b = b -> link )
		{
			if( b -> endDistanceToFarPoint < minDistance )
			{
				minDistance = b -> endDistanceToFarPoint;
				endBranch = b;
			}
		}


		for( Branch* b = iteratorBranch -> firstBranch ; b != NULL; b = b -> link )
		{
			bool hasHeadBall = false;
			int headBallIndex = -1;
			bool hasTailBall = false;
			int tailBallIndex = -1;
			//
			for( PointList* pl = pointList -> firstPointList; pl != NULL; pl = pl -> link )
			{
				if( MathHelper::VectorApproximation( b -> sourceCorner[0] -> position , pl -> position ) )
				{
					hasHeadBall = true;
					headBallIndex = pl -> index;
				}
				if( MathHelper::VectorApproximation( b -> sourceCorner[1] -> position , pl -> position ) )
				{
					hasTailBall = true;
					tailBallIndex = pl -> index;
				}
			}

			//
			if( hasHeadBall && !hasTailBall )//有頭無尾
			{//把end branch末端也聲球
				if( /*b == endBranch*/ 0 )//如果這個點是最後一根，就不建另一端
				{
					constructor -> Construct( headBallIndex , b -> connectFace[0] , SIZE_S , TYPE_TRUSS );
				}else{
					tailBallIndex = constructor -> Construct( headBallIndex , b -> connectFace[0] , SIZE_S , TYPE_PIN );
					pointList -> Add( b -> sourceCorner[1] , tailBallIndex );
				}
			}
			else if( !hasHeadBall && hasTailBall )//有尾無頭
			{
				if( /*b == endBranch*/ 0 )//如果這個點是最後一根，就不建另一端
				{
					constructor -> Construct( tailBallIndex , b -> connectFace[1] , SIZE_S , TYPE_TRUSS );
				}else{
					headBallIndex = constructor -> Construct( tailBallIndex , b -> connectFace[1] , SIZE_S , TYPE_PIN );
					pointList -> Add( b -> sourceCorner[0] , headBallIndex );
				}
			}
			else if( hasHeadBall && hasTailBall )//有頭有尾
			{
				constructor -> Construct( headBallIndex , b -> connectFace[0] , SIZE_S , TYPE_TRUSS );
			}
			else if( !hasHeadBall && !hasTailBall )//眉頭眉尾
			{
				if( /*b == endBranch*/ 0 )//如果這個點是最後一根，就不建另一端
				{	
					if( b -> pathDirection == TOWARD_HEAD )
					{
						tailBallIndex = constructor -> NewBall( b -> sourceCorner[1] -> position );
						pointList -> Add( b -> sourceCorner[1] , tailBallIndex );
						constructor -> Construct( tailBallIndex , b -> connectFace[1] , SIZE_S , TYPE_TRUSS );
					}
					else if( b -> pathDirection == TOWARD_TAIL )
					{
						headBallIndex = constructor -> NewBall( b -> sourceCorner[0] -> position );
						pointList -> Add( b -> sourceCorner[0] , headBallIndex );
						constructor -> Construct( headBallIndex , b -> connectFace[0] , SIZE_S , TYPE_TRUSS );
					}
				}
				else
				{
					tailBallIndex = constructor -> NewBall( b -> sourceCorner[1] -> position );
					headBallIndex = constructor -> Construct( tailBallIndex , b -> connectFace[1] , SIZE_S , TYPE_PIN );
					pointList -> Add( b -> sourceCorner[0] , headBallIndex );
					pointList -> Add( b -> sourceCorner[1] , tailBallIndex );
				}
			}

			std::cout << "樹枝從(" << b -> sourceCorner[0] -> position[0] << "," << 
									  b -> sourceCorner[0] -> position[1] << "," << 
									  b -> sourceCorner[0] -> position[2] << ")向第" << b -> connectFace[0] << "建樹枝" << std::endl;

		}
	}
	
	return true;
}

bool ZomeGenerator::Delete( Branch* branch )
{
	if( branch == NULL )
	{
		return true;
	}

	if( branch -> firstBranch == NULL )
	{
		return true;
	}

	for( Branch* b = branch -> firstBranch; b != NULL; )
	{
		Branch* tempBranch = b -> link;
		delete b;
		b = tempBranch;
	}
	return true;
}

bool ZomeGenerator::DeleteFromList( Branch* branch )
{
	std::cout << "將第" << branch -> index << "根樹枝刪掉" << std::endl;
  
	if( branch == branch -> firstBranch )
	{
		for( Branch* b = branch -> link; b != NULL; b = b -> link )
		{
			b -> firstBranch = branch -> link;
		}
	}
	else
	{
		Branch* lastBranch = new Branch();
		for( Branch* b = branch -> firstBranch; b != NULL; b = b -> link )
		{
			if( b -> link == branch )
			{
				delete lastBranch;
				lastBranch = b;
				break;
			}
			if( b -> link == NULL )
			{
				std::cout << "ZomeGenerator::DeleteFromList出錯：沒有找到該樹枝" << std::endl;
			}
		}
	
		//找到上一根樹枝，然後跟下一根相連
		if( lastBranch != NULL )
		{
			lastBranch -> link = branch -> link;
		}
	}
	

	//把相鄰的樹枝的連結改微NULL
	for( int i = 0; i < 12; i++ )
	{
		//先改頭的那邊
		if( branch -> neighborBranch[i][ BRANCH_HEAD ] != NULL )
		{
			Branch* tempBranch = branch -> neighborBranch[i][ BRANCH_HEAD ];
			int tempIndex = branch -> GetFaceBranchIndex( branch -> connectFace[ BRANCH_HEAD ] );
			int cornerNumber;
			if( branch -> sourceCorner[ BRANCH_HEAD ] == tempBranch -> neighborCorner[ tempIndex ][ BRANCH_HEAD ] )
			{
				cornerNumber = BRANCH_HEAD;
			}
			else if( branch -> sourceCorner[ BRANCH_HEAD ] == tempBranch -> neighborCorner[ tempIndex ][ BRANCH_TAIL ] )
			{
				cornerNumber = BRANCH_TAIL;
			}
			else
			{
				std::cout << "ZomeGenerator::DeleteFromList出錯：沒有找到已經被記錄的樹枝鄰居" << std::endl; 
				continue;
			}
			tempBranch -> neighborBranch[ tempIndex ][ cornerNumber ] = NULL;
			tempBranch -> neighborCorner[ tempIndex ][ cornerNumber ] = NULL;
		}
		//在改委的那邊
		if( branch -> neighborBranch[i][ BRANCH_TAIL ] != NULL )
		{
			Branch* tempBranch = branch -> neighborBranch[i][ BRANCH_TAIL ];
			int tempIndex = branch -> GetFaceBranchIndex( branch -> connectFace[ BRANCH_TAIL ] );
			int cornerNumber;
			if( branch -> sourceCorner[ BRANCH_TAIL ] == tempBranch -> neighborCorner[ tempIndex ][ BRANCH_HEAD ] )
			{
				cornerNumber = BRANCH_HEAD;
			}
			else if( branch -> sourceCorner[ BRANCH_TAIL ] == tempBranch -> neighborCorner[ tempIndex ][ BRANCH_TAIL ] )
			{
				cornerNumber = BRANCH_TAIL;
			}
			else
			{
				std::cout << "ZomeGenerator::DeleteFromList出錯：沒有找到已經被記錄的樹枝鄰居" << std::endl; 
				continue;
			}
			tempBranch -> neighborBranch[ tempIndex ][ cornerNumber ] = NULL;
			tempBranch -> neighborCorner[ tempIndex ][ cornerNumber ] = NULL;
		}
	}

	delete branch;
	return true;
}
//給public用，生成樹支和書出
bool ZomeGenerator::MakeBranch( Polyhedron* p , Segmenter* s )
{
	GenerateBranch( p , s , firstOuterCone );
	if( CONSTRUCTOR_MODE == MAKE_BRANCH_END )
	{
		GenerateBranchEnd();
	}
	ConstructBranch();
	ConstructBranchEnd();
	GenerateBranchSocket();
	return true;
}

//================BranchEnd//================
//================BranchEnd//================
//================BranchEnd//================

bool ZomeGenerator::GenerateBranchEnd()
{
	firstBranchEnd = NULL;
	//對每一根樹枝建末端
	for( std::map< Segment* , Branch* >::iterator i = rootList.begin(); i != rootList.end(); i++ )
	{
		Branch* iteratorBranch = i -> second;

		for( Branch* b = iteratorBranch -> firstBranch; b != NULL; b = b -> link )
		{
			if( /*!b -> isEnd*/ true )//不是最後一根，就建末梢
			{
				BranchEnd* tempBranchEnd = new BranchEnd( b , firstBranchEnd );

				TryDifferentType( tempBranchEnd );

				if( tempBranchEnd -> type != 0 )
				{
					AddToLink( tempBranchEnd );
				}
				else
				{
					delete tempBranchEnd;
				}
			}
		}
	}

	return true;
}

bool ZomeGenerator::TryDifferentType( BranchEnd* branchEnd )
{
	std::cout << "數枝末梢" << branchEnd << "的兩個源點的方向是" << branchEnd -> branchDirection << std::endl <<
		"(" << branchEnd -> sourceCorner[0] -> position[0] << "," << 
		branchEnd -> sourceCorner[0] -> position[1] << "," << 
		branchEnd -> sourceCorner[0] -> position[2] << ")," << std::endl;
	std::cout << "(" << branchEnd -> sourceCorner[1] -> position[0] << "," << 
		branchEnd -> sourceCorner[1] -> position[1] << "," << 
		branchEnd -> sourceCorner[1] -> position[2] << ")," << std::endl;


	//每種都建一遍梁距離
	for( int i = 1; i < BRANCH_END_TYPE_AMOUNT; i++ )
	{
		for( int j = 0; j < BRANCH_END_DIRECTION_AMOUNT; j++ )
		{
			//std::cout<<"debug:i=" << i << ",j=" << j << std::endl;
			if( !branchEnd -> TryDifferentType( i , j ) ) continue;


			if( CheckEngaged( branchEnd ) )
			{
				std::cout << "這個數之末梢" << branchEnd << "的第" << i << "種組法的第" << j << "個方向會碰到模型邊原，所以刪除" << std::endl;
				
				continue;
			}
			if( CheckIntersectWithBranch( branchEnd ) )
			{
				std::cout << "這個數之末梢" << branchEnd << "的第" << i << "種組法的第" << j << "個方向會碰到其他樹枝，所以刪除" << std::endl;
				
				continue;
			}
			//確認這之樹之末稍可以裝上去以後，就來計算他和表面的距離
			if( !CheckDistanceToSurface( branchEnd ) )//回傳錯就代表跟其他點距離太近
			{
				branchEnd -> weightedDistance[i][j] = -1;
				continue;
			}
			std::cout << "成功建立數之末梢" << branchEnd << "的第" << i << "種組法的第" << j << "個方向" << std::endl;
			

		}
	}
	//選最短的距離建
	GLfloat minWeightedDistance = 99999;
	int minType = -1;
	int minDirection = -1;
	for( int i = 1; i < BRANCH_END_TYPE_AMOUNT; i++ )
	{
		for( int j = 0; j < BRANCH_END_DIRECTION_AMOUNT; j++ )
		{
			if( branchEnd -> weightedDistance[i][j] < minWeightedDistance && 
				branchEnd -> weightedDistance[i][j] != -1 )
			{
				minType = i;
				minDirection = j;
				minWeightedDistance = branchEnd -> weightedDistance[i][j];
			}
		}
	}

	if( minType == -1 )
	{
		branchEnd -> TryDifferentType( 0 , 0 );
	}
	else
	{
		branchEnd -> TryDifferentType( minType , minDirection );
	}
	return true;
}

bool ZomeGenerator::CheckEngaged( BranchEnd* branchEnd )
{//-158 162 0
	
	

	for( int i = 0; i < 2; i++ )
	{
		//debug
		GLfloat test[3] = { -138 , 32 , 79 };
		if( MathHelper::VectorDistance( test , branchEnd -> cornerEnd[i] -> position ) < 10.0 )
		{
			std::cout << "該樹枝末端在(" << branchEnd -> cornerEnd[i] -> position[0] << "," <<
											branchEnd -> cornerEnd[i] -> position[1] << "," <<
											branchEnd -> cornerEnd[i] -> position[2] << ")" << std::endl;
		}
		/*
		std::cout << "該樹枝根在(" << branchEnd -> sourceCorner[i] -> position[0] << "," <<
									  branchEnd -> sourceCorner[i] -> position[1] << "," <<
									  branchEnd -> sourceCorner[i] -> position[2] << ")" << std::endl;
		std::cout << "該樹枝末端在(" << branchEnd -> cornerEnd[i] -> position[0] << "," <<
										branchEnd -> cornerEnd[i] -> position[1] << "," <<
										branchEnd -> cornerEnd[i] -> position[2] << ")" << std::endl;
		*/

		//先算出頭尾端位置
		GLfloat endPoint[3] , initPoint[3];
		MathHelper::VectorMinus( endPoint , branchEnd -> cornerEnd[i] -> position , branchEnd -> sourceCorner[i] -> position );
		MathHelper::VectorNormalize( endPoint );
		MathHelper::CopyVector( initPoint , endPoint );
		MathHelper::VectorMutiply( initPoint , 0.1 );//從頭往後0.1mm再開始當作樹枝頭，比較不容易在這邊超出模型
		MathHelper::VectorPlus( initPoint , branchEnd -> sourceCorner[i] -> position );
		MathHelper::VectorMutiply( endPoint , -0.1 );//如果不算全長，可能會再尾巴穿出模型，但又繼續增長
		MathHelper::VectorPlus( endPoint , branchEnd -> cornerEnd[i] -> position );

		bool isEngaged = false;
		//一個cone的最大距離maxConeDistance
		GLfloat maxConeDistance = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S );
		//std::cout << "查看點相交";
		for( int j = 0; j < model -> indiceSize / 3; j++ ) //出錯
		{
			//如果模型中的這個面離方塊不到1mm
			//這邊寫錯了 應該是平面到點的距離不到根號3 布是點到點
			//d = N * P - N * P1
			//N = P2-P1 * P3-P1 Normalize
			Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ j * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ j * 3 + 2 ] * 3 );
			Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ j * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ j * 3 + 2 ] * 3 );
		


			Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
			//與玉測試的平面的距離 小於一個cone的最大距離
			if( abs( normal -> Dot( initPoint ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ j * 3 ] * 3 ) ) 
				< maxConeDistance /*zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_S )*/ )
			{
				isEngaged |= MathHelper::CheckIntersectOnTriangle( initPoint ,
																   endPoint ,
																   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ j * 3 ] * 3 ] ,
																   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ j * 3 + 1 ] * 3 ] ,
											 					   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ j * 3 + 2 ] * 3 ] );
			
			}
			delete vector1;
			delete vector2;
			delete normal;

			if( isEngaged )
			{
				break;
			}
		}
		if( isEngaged ) 
		{
			std::cout << "新的樹枝末梢有香蕉" << std::endl;
			return isEngaged;
		}
			
		//branch -> engaged = isEngaged;
		//grid -> CheckIntersect( )
	}
	return false;
}
	
bool ZomeGenerator::CheckIntersectWithBranch( BranchEnd* branchEnd )
{
	for( std::map< Segment* , Branch* >::iterator i = rootList.begin(); i != rootList.end(); i++ )
	{
		Branch* iteratorBranch = i -> second;

		if( branchEnd -> CheckIntersectWithBranch( iteratorBranch ) )
		{
			return true;
		}
	}
	return false;
}

bool ZomeGenerator::CheckDistanceToSurface( BranchEnd* branchEnd )
{
	//debug
	GLfloat test[3] = { -151 , 94 , -21 };
	if( MathHelper::VectorDistance( test , branchEnd -> cornerEnd[0] -> position ) < 10.0 )
	{
		std::cout << "debug" << std::endl;
	}


	Point* tempNearstPoint = new Point();
	//紀錄最小距離
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	{
		GLfloat tempNearestPointOnSurface[3];
		GLfloat tempMinDistance = 
			MathHelper::CheckDistanceToTriangle( branchEnd -> cornerEnd[0] -> position ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ,
												 *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 , 
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 ,
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ,
												 *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 , 
												 tempNearestPointOnSurface );
		if( tempMinDistance < minDistance && tempMinDistance > -0.1 )
		{
			minDistance = tempMinDistance;
			MathHelper:: CopyVector( tempNearstPoint -> position , tempNearestPointOnSurface );
		}
	}
	

	//這行去看他和另一個端點距離多近
	GLfloat distanceToNearest = 99999;
	GLfloat minDistance_DistanceToNearest;
	GLfloat distanceBetweenEnd = 9999;
	//先看outercone
	for( OuterCone* o = firstOuterCone; o != NULL; o = o -> link )
	{
		if( o -> type == 0 ) continue;
		if( o -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position ) < distanceToNearest )
		{
			distanceToNearest = o -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position );
		}
		if( o -> cornerEnd[0] -> DistanceToPoint( branchEnd -> cornerEnd[0] -> position ) < distanceBetweenEnd )
		{
			distanceBetweenEnd = o -> cornerEnd[0] -> DistanceToPoint( branchEnd -> cornerEnd[0] -> position );
		}
	}
	//在看branch
	for( std::map< Segment* , Branch* >::iterator i = rootList.begin(); i != rootList.end(); i++ )
	{
		Branch* iteratorBranch = i -> second;
		for( Branch* b = iteratorBranch; b != NULL; b = b -> link )
		{
			if( /*b -> isEnd*/ 0 )
			{
				if( b -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position ) < distanceToNearest )
				{
					distanceToNearest = b -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position );
				}
				if( b -> sourceCorner[0] -> DistanceToPoint( branchEnd -> cornerEnd[0] -> position ) < distanceBetweenEnd )
				{
					distanceBetweenEnd = b -> sourceCorner[0] -> DistanceToPoint( branchEnd -> cornerEnd[0] -> position );
				}
			}
		}
	}
	//在看branchEnd
	for( BranchEnd* b = firstBranchEnd; b != NULL; b = b -> link )
	{
		if( b -> type == 0 ) continue;

		if( b -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position ) < distanceToNearest )
		{
			distanceToNearest = b -> nearestPointOnSurface -> DistanceToPoint( tempNearstPoint -> position );
		}
		if( b -> sourceCorner[0] -> DistanceToPoint( branchEnd -> cornerEnd[0] -> position ) < distanceBetweenEnd )
		{
			distanceBetweenEnd = b -> sourceCorner[0] -> DistanceToPoint( branchEnd -> cornerEnd[0] -> position );
		}
	}
	//計算加全距離
	minDistance_DistanceToNearest = minDistance;
	if( distanceToNearest < 100 )
	{
		minDistance_DistanceToNearest = minDistance + pow( ( 100.0 - distanceToNearest ) , DISTANCE_PARAM ) / 50.0;
	}
	//如果兩個相鄰點泰近，就刪掉一個點
	if( distanceToNearest < SOCKET_RADIUS * 2.0 || distanceBetweenEnd < SOCKET_RADIUS * 2.0 )
	{
		std::cout <<  "這個數枝末梢" << branchEnd << "的第" << branchEnd -> type << "種組法的第" << branchEnd -> branchEndDirection << "個方向，與其他outerCone或樹枝距離太近(" << distanceToNearest << ")所以刪掉" << std::endl;
		delete tempNearstPoint;
		return false;
	}
	std::cout << "這個數枝末梢" << branchEnd << "的第" << branchEnd -> type << "種組法的第" << branchEnd -> branchEndDirection << "個方向，與其他outerCone距離" << distanceToNearest << "，加權總距離是" << minDistance_DistanceToNearest << std::endl;
	branchEnd -> distance[ branchEnd -> type ][ branchEnd -> branchEndDirection ] = minDistance;
	branchEnd -> weightedDistance[ branchEnd -> type ][ branchEnd -> branchEndDirection ] = minDistance_DistanceToNearest;

	//這邊紀錄最靠近外支架的點的位置，先考量她是布是目前最靠近的外支架建立方式，試的話就把點給紀錄下來
	bool theNearest = true;
	for( int i = 0; i < BRANCH_END_TYPE_AMOUNT; i++ )
	{
		for( int j = 0; j < BRANCH_END_DIRECTION_AMOUNT; j++ )
		{
			if( branchEnd -> weightedDistance[i][j] < minDistance_DistanceToNearest && branchEnd -> distance[i][j] != -1.0 )
			{
				theNearest = false;
			}
		}
	}
	if( theNearest )
	{
		//把這個最近點書進outercone裡，並擺index改乘她的type
		branchEnd -> nearestPointOnSurface -> Copy( tempNearstPoint );
	}
	delete tempNearstPoint;



	return true;
}

bool ZomeGenerator::ConstructBranchEnd()
{
	for( BranchEnd* b = firstBranchEnd ; b != NULL; b = b -> link )
	{
		if( b -> type == 0 ) continue;

		int headBallIndex = -1;
		int tailBallIndex = -1;
		
		for( PointList* pl = pointList -> firstPointList; pl != NULL; pl = pl -> link )
		{
			if( MathHelper::VectorApproximation( b -> sourceCorner[0] -> position , pl -> position ) )
			{
				headBallIndex = pl -> index;
			}
			if( MathHelper::VectorApproximation( b -> sourceCorner[1] -> position , pl -> position ) )
			{
				tailBallIndex = pl -> index;
			}
		}
		//建出兩根枝價
		if( headBallIndex == -1 )
		{
			std::cout << "ZomeGenerator::ConstructBranchEnd出錯：沒有找到數枝末梢的那顆球" << std::endl;
		}
		else
		{
			constructor -> Construct( headBallIndex , b -> constructedHole[0] , b -> constructedSize[0] , TYPE_TRUSS );
		}

		if( tailBallIndex == -1)
		{
			std::cout << "ZomeGenerator::ConstructBranchEnd出錯：沒有找到數枝末梢的那顆球" << std::endl;
		}
		else
		{
			constructor -> Construct( tailBallIndex , b -> constructedHole[1] , b -> constructedSize[1] , TYPE_TRUSS );
		}
		
		std::cout << "樹枝從(" << b -> sourceCorner[0] -> position[0] << "," << 
									b -> sourceCorner[0] -> position[1] << "," << 
									b -> sourceCorner[0] -> position[2] << ")向第" << b -> constructedHole[0] << "面建出末梢" << std::endl;
		std::cout << "樹枝從(" << b -> sourceCorner[1] -> position[0] << "," << 
									b -> sourceCorner[1] -> position[1] << "," << 
									b -> sourceCorner[1] -> position[2] << ")向第" << b -> constructedHole[1] << "面建出末梢" << std::endl;
	}
	return true;
}

bool ZomeGenerator::AddToLink( BranchEnd* branchEnd )
{
	if( firstBranchEnd == NULL )
	{
		firstBranchEnd = branchEnd;
		branchEnd -> firstBranchEnd = firstBranchEnd;
	}
	else
	{
		for( BranchEnd* b = firstBranchEnd; b != NULL; b = b -> link )
		{
			if( b -> link == NULL )
			{
				b -> link = branchEnd;
				break;
			}
		}
	}
	return true;
}

//================其他//================
//================其他//================
//================其他//================

// 站時不寫這個了
bool ZomeGenerator::DebugSocket()
{
	AnalyzeDisplay = new SocketConstructor();
	//PointList* socketPointList = new PointList();
	//這邊是外佈三角錐

	std::ifstream infile("DebugPos.txt");

	FILE* debugPos = fopen( "DebugPos.txt" ,"r");

	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		//GLfloat test[3] = { -22.342091 , -80.901695 , 34.948772 };
		//if( MathHelper::VectorDistance( i -> nearestPointOnSurface-> position , test ) < 1.0 )
		//{
		//	std::cout << "debug";
		//}

		if( i -> type != 0 )
		{
			if( i -> distanceToSurface[ i -> type ] > SOCKET_RADIUS )//超過12mm才會生乘
			{
				socketConstructor -> NewSocket( i );
				Point* tempSurfacePoint = new Point( i -> nearestPointOnSurface -> position );

				if( nearestPointOnSurface == NULL )
				{
					nearestPointOnSurface = tempSurfacePoint;
					nearestPointOnSurface -> firstPoint = tempSurfacePoint;
				}
				else
				{
					nearestPointOnSurface -> AddToLink( tempSurfacePoint );
				}
			}
			else
			{
				//socketConstructor -> NewSocket( i );//在這見一顆半圓型
				socketConstructor -> NewOriginalSocket( i );
				Point* tempSurfacePoint = new Point( i -> nearestPointOnSurface -> position );

				if( nearestPointOnSurface == NULL )
				{
					nearestPointOnSurface = tempSurfacePoint;
					nearestPointOnSurface -> firstPoint = tempSurfacePoint;
				}
				else
				{
					nearestPointOnSurface -> AddToLink( tempSurfacePoint );
				}
			}
		}
	}

	return true;
}

Cone* ZomeGenerator::GrowCone( Cone* c , int f )
{
	Cone* cP = new Cone( c , f );
	std::cout << "在(" << cP -> position[0] << "," <<
						  cP -> position[1] << "," <<
						  cP -> position[2] << ")建立新的三角錐" << std::endl;
	if( cP -> dublicated )
	{
		DeleteCone( cP );
		return NULL;
	}

	return cP;
}

bool ZomeGenerator::GrowOuterCone( Cone* c , int f , int t )
{
	if( firstOuterCone == NULL )
	{
		firstOuterCone = new OuterCone( c , f , t );
	}
	else
	{
		OuterCone* temp = new OuterCone( c , f , t , firstOuterCone );
		temp -> AddToLink();
	}
	return true;
}

bool ZomeGenerator::ConstructCusStruc()
{
	GLfloat initPos[3] = {0,0,0};
	constructor = new Constructor( initPos );
	int b0,b1,b2,b3;
	b0 = 0;
	b1 = constructor -> Construct( b0 , 38 , 0 , TYPE_PIN );
	b2 = constructor -> Construct( b1 , 41 , 0 , TYPE_PIN );
	constructor -> Construct( b2 , 1 , 0 , TYPE_TRUSS );

	b3 = constructor -> Construct( b0 , 14 , 0 , TYPE_PIN );
	constructor -> Construct( b1 , 35 , 0 , TYPE_TRUSS );
	constructor -> Construct( b2 , 15 , 0 , TYPE_TRUSS );


	return true;
}

bool ZomeGenerator::ActivateAllCone()
{
	for( Cone* c = firstCone; c != NULL; c = c -> link )
	{
		c -> activated = true;
	}
	return true;
}

bool ZomeGenerator::ConstructStructure()
{
	

	GenerateConeStructure();
	GenerateOuterConeStructure();

	//ZomeStructureAnalyze();
	return true;
}

bool ZomeGenerator::CustomizedAnalysis()
{
	structureAnalyzer = new StructureAnalyzer( constructor );

	structureAnalyzer -> SDAnalyze();
	//structureAnalyzer
	//structureAnalyzer -> Analyze();
	return true;
}

