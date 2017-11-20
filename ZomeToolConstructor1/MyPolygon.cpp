#include "MyPolygon.h"
#include <vector>



MyPolygon::MyPolygon( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 , Plane* plane , bool isInside )
{
	cornerTooClose = false;
	//std::cout << "Debug:在MyPolygon::MyPolygon" << std::endl;
	Point* point[3];
	point[0] = new Point( point1 );
	point[1] = new Point( point2 );
	point[2] = new Point( point3 );
	//趙順序把每一個邊對平面座是否相交測試，測完以後找到相交的點把他們紀錄起來，並記錄相交的點兩端的點的編號，
	Point* intersectPoint[2];
	std::vector<int> endPoint;

	for( int i = 0; i < 3; i++ )
	{
		if( plane -> CheckIntersection( point[i] -> position , point[ i==2 ? 0:i+1 ] -> position ) )
		{
			if( endPoint.size() / 2 == 2 )
			{
				std::cout << "MyPolygon::MyPolygon出錯：取到超過兩個與同一平面的相接點" << std::endl;
				if( intersectPoint[0] -> DistanceToPoint( intersectPoint[1] -> position ) < SMALL_NUMBER )
				{
					std::cout << "MyPolygon::MyPolygon出錯：取到超過兩個與同一平面的相接點，且兩點太靠近，直接修改成其他在平面上的點" << std::endl;
					plane -> GetIntersectPoint( point[i] -> position , point[ i==2 ? 0:i+1 ] -> position , intersectPoint[1] -> position );
				}
				continue;
			}
			intersectPoint[ endPoint.size() / 2 ] = new Point();
			plane -> GetIntersectPoint( point[i] -> position , point[ i==2 ? 0:i+1 ] -> position , intersectPoint[ endPoint.size()/2 ] -> position );
			endPoint.push_back(i);
			endPoint.push_back( i==2 ? 0:i+1 );
		}
	}

	if( endPoint.size() == 2 )
	{
		std::cout << "MyPolygon::MyPolygon出錯：只有兩個香蕉點(通常要有4個)" << std::endl;//通常要有4個點可以香蕉，代表一個三角形被分成兩半，美一半個被切出兩個端點
		cornerTooClose = true;
		delete point[0];
		delete point[1];
		delete point[2];
		return;
	}

	Point* polygonCorner; polygonCorner = NULL;

	//先建立inside的點的串列，用這個串列來建立新的多邊型
	for( int i = 0; i < 3; i++ )
	{
		if( plane -> IsInside( point[i] -> position ) == isInside )
		{
			if( polygonCorner == NULL )
			{
				polygonCorner = new Point( point[i] -> position );
				polygonCorner -> firstPoint = polygonCorner;
				if( plane -> IsInside( point[ i==2 ? 0:i+1 ] -> position ) != isInside )//這行是把後面的點加進去
				{
					if( endPoint[0] == i )//如果第一個端點就是i，就先連第一個端點在連第二個端點
					{
						polygonCorner -> AddToLink( intersectPoint[0] );
						polygonCorner -> AddToLink( intersectPoint[1] );
					}
					else//不然就第二個點先，再來是第一個點
					{
						polygonCorner -> AddToLink( intersectPoint[1] );
						polygonCorner -> AddToLink( intersectPoint[0] );
					}
				}
			}
			else
			{
				if( polygonCorner -> Contain( point[i] -> position ) )
				{
					break;
				}
				else
				{
					Point* tempCorner = new Point( point[i] -> position );
					polygonCorner -> AddToLink( tempCorner );
					if( plane -> IsInside( point[ i==2 ? 0:i+1 ] -> position ) != isInside )
					{
						if( endPoint[0] == i )//如果第一個端點就是i，就先連第一個端點在連第二個端點
						{
							polygonCorner -> AddToLink( intersectPoint[0] );
							polygonCorner -> AddToLink( intersectPoint[1] );
						}
						else//不然就第二個點先，再來是第一個點
						{
							polygonCorner -> AddToLink( intersectPoint[1] );
							polygonCorner -> AddToLink( intersectPoint[0] );
						}
					}
					
				}
			}
		}
	}
	if( polygonCorner == NULL )
	{
		std::cout << "MyPolygon::MyPolygon出錯：沒有放入任何角" << std::endl;
		cornerTooClose = true;
		return;
	}
	if( polygonCorner -> Size() <= 2 )
	{
		std::cout << "MyPolygon::MyPolygon出錯：頂點數小於三個" << std::endl;
	}
	cornerAmount = polygonCorner -> Size();
	corner = new Point*[ cornerAmount ];
	int cornerCount = 0;
	for( Point* i = polygonCorner -> firstPoint; i != NULL; i = i -> link )
	{
		if( i -> link == NULL)
		{
			if( i -> DistanceToPoint( i -> firstPoint -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon出錯：兩頂點之間距離泰近" << std::endl;
				cornerTooClose = true;
			}
		}
		else
		{
			if( i -> DistanceToPoint( i -> link -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon出錯：兩頂點之間距離泰近" << std::endl;
				cornerTooClose = true;
			}
		}
		
		corner[ cornerCount ] = i;
		cornerCount++;
	}
	delete point[0];
	delete point[1];
	delete point[2];
}

MyPolygon::MyPolygon( MyPolygon* polygon , Plane* plane , bool isInside )
{
	cornerTooClose = false;
	int tempCornerAmount = polygon -> cornerAmount;
	Point** point = new Point*[ tempCornerAmount ];
	for( int i = 0; i < tempCornerAmount; i++ ) 
	{
		point[i] = new Point( polygon -> corner[i] -> position );
	}
	//趙順序把每一個邊對平面座是否相交測試，測完以後找到相交的點把他們紀錄起來，並記錄相交的點兩端的點的編號，
	Point* intersectPoint[2];
	std::vector<int> endPoint;

	
	for( int i = 0; i < tempCornerAmount; i++ )
	{
		if( plane -> CheckIntersection( point[i] -> position , point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position ) )//當i事最大實，他的下一個點就是0
		{
			if( endPoint.size() / 2 == 2)
			{
				std::cout << "MyPolygon::MyPolygon出錯：取到超過兩個與同一平面的相接點" << std::endl;
				if( intersectPoint[0] -> DistanceToPoint( intersectPoint[1] -> position ) < SMALL_NUMBER )
				{
					std::cout << "MyPolygon::MyPolygon出錯：取到超過兩個與同一平面的相接點，且兩點太靠近，直接修改成其他在平面上的點" << std::endl;
					plane -> GetIntersectPoint( point[i] -> position , point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position , intersectPoint[1] -> position );
				}
				continue;
			}
			intersectPoint[ endPoint.size() / 2 ] = new Point();//只會放四個點，是兩條相交線的各兩端，所以編號事size除以2
			plane -> GetIntersectPoint( point[i] -> position , point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position , intersectPoint[ endPoint.size()/2 ] -> position );
			endPoint.push_back(i);
			endPoint.push_back( i==tempCornerAmount-1 ? 0:i+1 );
		}
	}

	if( endPoint.size() == 2 )
	{
		std::cout << "MyPolygon::MyPolygon出錯：只有兩個香蕉點(通常要有4個)" << std::endl;//通常要有4個點可以香蕉，代表一個三角形被分成兩半，美一半個被切出兩個端點
		cornerTooClose = true;
		for( int i = 0; i < tempCornerAmount; i++ ) delete point[i];
		delete point;
		return;
	}

	Point* polygonCorner; polygonCorner = NULL;

	//先建立inside的點的串列，用這個串列來建立新的多邊型
	for( int i = 0; i < tempCornerAmount; i++ )
	{
		if( plane -> IsInside( point[i] -> position ) == isInside )
		{
			if( polygonCorner == NULL )
			{
				polygonCorner = new Point( point[i] -> position );
				polygonCorner -> firstPoint = polygonCorner;
				if( plane -> IsInside( point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position ) != isInside )
				{
					if( endPoint[0] == i )//如果第一個端點就是i，就先連第一個端點在連第二個端點
					{
						polygonCorner -> AddToLink( intersectPoint[0] );
						polygonCorner -> AddToLink( intersectPoint[1] );
					}
					else//不然就第二個點先，再來是第一個點
					{
						polygonCorner -> AddToLink( intersectPoint[1] );
						polygonCorner -> AddToLink( intersectPoint[0] );
					}
				}
			}
			else
			{
				if( polygonCorner -> Contain( point[i] -> position ) )
				{
					break;
				}
				else
				{
					Point* tempCorner = new Point( point[i] -> position );
					polygonCorner -> AddToLink( tempCorner );
					if( plane -> IsInside( point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position ) != isInside )
					{
						if( endPoint[0] == i )//如果第一個端點就是i，就先連第一個端點在連第二個端點
						{
							polygonCorner -> AddToLink( intersectPoint[0] );
							polygonCorner -> AddToLink( intersectPoint[1] );
						}
						else//不然就第二個點先，再來是第一個點
						{
							polygonCorner -> AddToLink( intersectPoint[1] );
							polygonCorner -> AddToLink( intersectPoint[0] );
						}
					}
				}
			}
		}
	}
	if( polygonCorner == NULL )
	{
		std::cout << "MyPolygon::MyPolygon出錯：沒有放入任何角" << std::endl;
		cornerTooClose = true;
		return;
	}
	if( polygonCorner -> Size() <= 2 )
	{
		std::cout << "MyPolygon::MyPolygon出錯：頂點數小於三個" << std::endl;
	}


	cornerAmount = polygonCorner -> Size();
	corner = new Point*[ cornerAmount ];
	int cornerCount = 0;
	for( Point* i = polygonCorner -> firstPoint; i != NULL; i = i -> link )
	{
		if( i -> link == NULL)
		{
			if( i -> DistanceToPoint( i -> firstPoint -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon出錯：兩頂點之間距離泰近" << std::endl;
				cornerTooClose = true;
			}
		}
		else
		{
			if( i -> DistanceToPoint( i -> link -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon出錯：兩頂點之間距離泰近" << std::endl;
				cornerTooClose = true;
			}
		}
		corner[ cornerCount ] = i;
		cornerCount++;
	}

	for( int i = 0; i < tempCornerAmount; i++ ) delete point[i];
	delete point;
}

MyPolygon::~MyPolygon()
{
	if( cornerAmount < 0 )
	{
		std::cout << "MyPolygon::~MyPolygon出錯：頂點數小於零" << std::endl;
		return;
	}
	for( int i = 0; i < cornerAmount; i++ )
	{
		delete corner[i];
	}
	delete corner;
}

//各邊是否包含這條線
bool MyPolygon::Contain( Line* line )
{
	//std::cout << "Debug:在MyPolygon::Contain" << std::endl;
	Line** edge = new Line*[ cornerAmount ];
	for( int i = 0; i < cornerAmount; i++ )
	{
		if( i != cornerAmount -1 )
		{
			edge[i] = new Line( corner[i] -> position , corner[i+1] -> position );
		}
		else
		{
			edge[i] = new Line( corner[i] -> position , corner[0] -> position );
		}
	}
	bool isContained = false;
	//測試有沒有包含
	for( int i = 0; i < cornerAmount; i++ )
	{
		if( edge[i] -> Containing( line ) )
		{
			isContained = true;
		}
	}
	//山記憶體
	for( int i = 0; i < cornerAmount; i++ )
	{
		delete edge[i];
	}
	delete[] edge;
	return isContained;
}

GLfloat MyPolygon::DistanceToPoint( Point* point )//把原本的茶距離，改成將多邊型分成數個三角形，一個一個茶距離
{
	Point* endPointCenter = new Point( point -> position );

	//Point* tempNearstPoint = new Point();

	//紀錄最小距離
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < cornerAmount - 2; i++ ) //出錯
	{
		Point* vector1 = new Point(); vector1 -> Vector( corner[0] , corner[i+2] );//0>2
		Point* vector2 = new Point(); vector2 -> Vector( corner[i+1] , corner[i+2] );//1>2
		//算normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		
		
		
		//從末端到表面 與normal同方向
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position ,  corner[0] -> position );
		GLfloat sb = normal -> Dot( distancePtoV0 );//是正的 如果點在模型裡，也就是距離
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//是正的 如果點在模型裡，也就是距離

		if( abs(sb) < minDistance && sb > -0.1 )
		{
			//如果這個面與點的距離小於目前的最小距離，則來看
			//1.這個點投影到面上時，會在三角型三個點之內，則她的距離就試與平面的距離
			//2.這個點投影到面上時，在三角型三個點之外，則她的距離就是與三個點的最小距離(比與平面的距離大)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//這邊算投影的點
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			if( abs(sb) > SMALL_NUMBER )
			{
				normal -> Multiply( sb );
				pointProjectOnPlane -> Plus( normal );
				normal -> Multiply( 1.0 / sb );
			}

			//算第一個做標 第一個是三角型總面積x2 第二個是三角型點0到投影點的距離 第三個是投影點和編1所包的面積
			Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector(  corner[ 0 ] -> position , pointProjectOnPlane -> position );
			Point* vertex1ToPoint = new Point(); vertex1ToPoint -> Vector(  corner[ i + 1 ] -> position , pointProjectOnPlane -> position );
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
				minDistance = abs(sb);
				//tempNearstPoint -> Copy( pointProjectOnPlane );
			}
			else
			{
				GLfloat* distance = new GLfloat[3];
				distance[0] = endPointCenter -> DistanceToPoint(  corner[ 0 ] -> position );
				distance[1] = endPointCenter -> DistanceToPoint(  corner[ i + 1 ] -> position );
				distance[2] = endPointCenter -> DistanceToPoint(  corner[ i + 2 ] -> position );
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
					//tempNearstPoint -> Copy( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + min ] * 3 );
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
	
	
	//delete tempNearstPoint;
	return minDistance;
}

bool MyPolygon::CheckIntersectionPolygonOnPlane( Plane* plane )
{
	bool isIntersected = false;
	for( int i = 0; i < cornerAmount; i++ )
	{
		if( plane -> CheckIntersection( corner[i] -> position , corner[ i==cornerAmount-1 ? 0 : i+1 ] -> position ) )
		{
			isIntersected = true;
			break;
		}
	}

	return isIntersected;
}