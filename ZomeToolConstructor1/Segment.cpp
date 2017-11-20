#include "Segment.h"


//==========================================SegmentFace//==========================================
bool SegmentFace::Delete( Point* pointList )
{
	for( Point* p = pointList -> firstPoint; p != NULL; )
	{
		Point* tempPoint = p -> link;
		delete p;
		p = tempPoint;
	}
	return true;
}

SegmentFace::SegmentFace( PolyhedronPoint* pFace )
{
	index = pFace -> index;
	isPolygon = false;
	polyhedronFace = pFace;
	intersected = false;

	isFirst = true;
	isTooBig = false;
}

SegmentFace::SegmentFace( PolyhedronPoint* pFace , Line* sLine )
{
	index = pFace -> index;
	isPolygon = false;
	polyhedronFace = pFace;
	intersected = false;

	sourceLine = sLine;
	isFirst = false;
	isTooBig = false;
}

SegmentFace::~SegmentFace()
{
	if( !isFirst ) delete sourceLine;
	if( isPolygon )
	{
		delete polygon;
	}
}

bool SegmentFace::AddToLink( SegmentFace* segmentFace )
{
	if( firstSegmentFace == NULL )
	{
		std::cout << "SegmentFace::AddToLink出錯：firstSegmentFace沒有指標" << std::endl;
	}
	segmentFace -> firstSegmentFace = firstSegmentFace;
	for( SegmentFace* i = firstSegmentFace; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = segmentFace;
			break;
		}
	}
	return true;
}
//這個碎片面是否有和這個平面相連
bool SegmentFace::IsConnectedWith( Plane* plane )//這個碎片面是否有和這個平面相連
{
	bool isConnected = false;
	for( SegmentFace* s = firstSegmentFace; s != NULL; s = s -> link )
	{
		if( s -> isPolygon )
		{
			for( int i = 0; i < s -> polygon -> cornerAmount; i++ )
			{
				if( plane -> Contain( s -> polygon -> corner[i] -> position ) )
				{
					isConnected = true;
				}
			}
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				if( plane -> Contain( s -> polyhedronFace -> connectedPoint[i] -> position ) )
				{
					isConnected = true;
				}
			}
		}
	}
	

	return isConnected;
}
//這個碎片面的串列是否包含這個點
bool SegmentFace::ListContainPoint( int i )
{
	bool isContained = false;
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( sf -> ContainPoint( i ) )
		{
			isContained = true;
		}
	}
	return isContained;
}
//這個碎片是否包含這個點
bool SegmentFace::ContainPoint( int i )
{
	for( int j = 0; j < 3; j++ )
	{
		if( polyhedronFace -> connectedPoint[j] -> index == i )
		{
			return true;
		}
	}
	return false;
}
//這個碎片面的串列是否包含這個面
bool SegmentFace::ListContainFace( int i )
{
	bool isContained = false;
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( sf -> ContainFace( i ) )
		{
			isContained = true;
		}
	}
	return isContained;
}
//這個碎片是否包含這個面
bool SegmentFace::ContainFace( int i )
{
	for( int j = 0; j < 3; j++ )
	{
		if( polyhedronFace -> index == i )
		{
			return true;
		}
	}
	return false;


}

bool SegmentFace::Contain( Line* line )
{
	if( polyhedronFace -> Contain( line ) )
	{
		return true;
	}
	else return false;
}
//尋找這一個碎片在這個面上的兩個端點
bool SegmentFace::FindEndPointOnPlane( Plane* plane , GLfloat* endPoint0 , GLfloat* endPoint1 )
{
	std::cout << "進行對面交點尋找兩端" << std::endl;
	Point* connectedPointList; connectedPointList = NULL;
	//先把所有有接到平面上的點紀錄下來
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( sf -> isPolygon )
		{
			for( int i = 0; i < sf -> polygon -> cornerAmount; i++ )
			{
				if( plane -> Contain( sf -> polygon -> corner[i] -> position ) )
				{
					if( !connectedPointList ) 
					{
						Point* tempPoint = new Point( sf -> polygon -> corner[i] -> position );
						connectedPointList = tempPoint;
						connectedPointList -> firstPoint = connectedPointList;
					}
					else
					{
						Point* tempPoint = new Point( sf -> polygon -> corner[i] -> position );
						connectedPointList -> AddToLink( tempPoint );
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				if( plane -> Contain( sf -> polyhedronFace -> connectedPoint[i] -> position ) )
				{
					if( connectedPointList == NULL ) 
					{
						Point* tempPoint = new Point( sf -> polyhedronFace -> connectedPoint[i] -> position );
						connectedPointList = tempPoint;
						connectedPointList -> firstPoint = connectedPointList;
					}
					else
					{
						Point* tempPoint = new Point( sf -> polyhedronFace -> connectedPoint[i] -> position );
						connectedPointList -> AddToLink( tempPoint );
					}
				}
			}
		}
	}
	//再把有重複的點去掉，指出現一次的話就是最邊元的點
	int endPointIndex = 0;
	for( Point* cpl = connectedPointList -> firstPoint; cpl != NULL; cpl = cpl -> link )
	{
		int emergeCount = 0;
		for( Point* cpl2 = connectedPointList -> firstPoint; cpl2 != NULL; cpl2 = cpl2 -> link )
		{
			if( cpl == cpl2 ) continue;
			if( cpl -> CloselyCompare( cpl2 ) )
			{
				emergeCount++;
			}
		}
		//std::cout << "點(" << cpl -> position[0] << "," << cpl -> position[1] << "," << cpl -> position[2] << ")出現" << emergeCount << "次" << std::endl;

		if( emergeCount == 0 )
		{
			if( endPointIndex == 0 )
			{
				if( cpl -> position == NULL )
				{
					std::cout << "SegmentFace::FindEndPointOnPlane出錯：最遠邊的點沒有位置" << std::endl; 
				}
				MathHelper::CopyVector( endPoint0 , cpl -> position );
				endPointIndex++;
			}
			else if( endPointIndex == 1 )
			{
				if( cpl -> position == NULL )
				{
					std::cout << "SegmentFace::FindEndPointOnPlane出錯：最遠邊的點沒有位置" << std::endl; 
				}
				MathHelper::CopyVector( endPoint1 , cpl -> position );
				endPointIndex++;
			}
			else
			{
				std::cout << "碎片在平面上尋找兩個端點出錯，找到更多端點" << std::endl;
			}
		}
	}

	Delete( connectedPointList );//
	return true;
}
//取得碎片與平面相連成的線段
bool SegmentFace::GetConnectingLine( Plane* plane , Line*& connectingLine )
{
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		GLfloat linePoint[2][3] = { {0,0,0} , {0,0,0} };
		bool connected = false;
		if( sf -> isPolygon )
		{
			
			//std::cout << "debug:" << sf << std::endl;
			
			int connectedAmount = 0;
			for( int i = 0; i < sf -> polygon -> cornerAmount; i++ )
			{
				if( plane -> Contain( sf -> polygon -> corner[i] -> position ) )
				{
					if( sf -> polygon -> corner[i] -> position == NULL )
					{
						std::cout << "SegmentFace::GetConnectingLine出錯：與平面相接的點沒有位置" << std::endl;
					}
					if( connectedAmount > 1 )
					{
						std::cout << "SegmentFace::GetConnectingLine出錯：有超過兩個與平面相接的點" << std::endl;
						if( MathHelper::VectorApproximation( linePoint[0] , linePoint[1] ) )
						{
							std::cout << "SegmentFace::GetConnectingLine出錯：有兩個在平面上的點位置過於接近，直接改為其他點" << std::endl;
							MathHelper::CopyVector( linePoint[ 1 ] , sf -> polygon -> corner[i] -> position );
							continue;
						}
						continue;
					}
					
					MathHelper::CopyVector( linePoint[ connectedAmount ] , sf -> polygon -> corner[i] -> position );
					connectedAmount++;

				}
			}
			if( connectedAmount == 2 ) connected = true;
			
		}
		else
		{
			int connectedAmount = 0;
			for( int i = 0; i < 3; i++ )
			{
				if( plane -> Contain( sf -> polyhedronFace -> connectedPoint[i] -> position ) )
				{
					if( sf -> polyhedronFace -> connectedPoint[i] -> position == NULL )
					{
						std::cout << "SegmentFace::GetConnectingLine出錯：與平面相接的點沒有位置" << std::endl;
					}
					if( connectedAmount > 1 )
					{
						std::cout << "SegmentFace::GetConnectingLine出錯：有超過兩個與平面相接的點" << std::endl;
						if( MathHelper::VectorApproximation( linePoint[0] , linePoint[1] ) )
						{
							std::cout << "SegmentFace::GetConnectingLine出錯：有兩個在平面上的點位置過於接近，直接改為其他點" << std::endl;
							MathHelper::CopyVector( linePoint[ 1 ] , sf -> polygon -> corner[i] -> position );
							continue;
						}
						continue;
					}
					MathHelper::CopyVector( linePoint[ connectedAmount ] , sf -> polyhedronFace -> connectedPoint[i] -> position );
					connectedAmount++;
				}
			}

			if( connectedAmount == 2 ) connected = true;
			
		}

		if( connected )
		{
			Line* tempLine = new Line( linePoint[0] , linePoint[1] ); 
			if( connectingLine == NULL )
			{
				connectingLine = tempLine;
				connectingLine -> firstLine = tempLine;
			}
			else
			{
				connectingLine -> AddToLink( tempLine );
			}
		}
	}
	return true;
}

GLfloat SegmentFace::GetArea()
{
	GLfloat area = 0;
	if( isPolygon )
	{
		for( int i = 0; i < polygon -> cornerAmount - 2; i++ )
		{
			area += polygon -> corner[0] -> Area( polygon -> corner[ i + 1 ] -> position , polygon -> corner[ i + 2 ] -> position );

		}
	}
	else
	{
		area += polyhedronFace -> GetArea();
	}
	return area;
}

GLfloat SegmentFace::GetTotalArea()
{
	GLfloat area = 0;
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		area += sf -> GetArea();
	}
	return area;
}
//試看this有沒有屬於這一群segmentFace
bool SegmentFace::BelongToSegment( SegmentFace* segmentFace )
{
	bool isBelonging = false;
	for( SegmentFace* sf = segmentFace -> firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( MathHelper::VectorDistance( polyhedronFace -> connectedPoint[0] ->position , 
										sf -> polyhedronFace -> connectedPoint[0] -> position ) > 100.0 )
			continue;

		for( int i = 0; i < 3; i++ )//對this的三個角，去跟這些碎片的頂點做比對
		{
			if( sf -> isPolygon )//多邊行
			{
				for( int j = 0; j < sf -> polygon -> cornerAmount; j++ )
				{
					if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] ->position ,
												   sf -> polygon -> corner[j] -> position ) )
					{
						isBelonging = true;
						return true;
					}
				}
			}
			else//菲多邊行
			{
				for( int j = 0; j < 3; j++ )
				{
					if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] ->position ,
												   sf -> polyhedronFace -> connectedPoint[j] ->position ) )
					{
						isBelonging = true;
						return true;
					}
				}
			}
		}
	}
	return false;
}




//==========================================Segment//==========================================


Segment::Segment( SegmentFace* s , Polyhedron* p )
{
	polyhedron = p;
	segmentFace = s;
}

Segment::~Segment()
{
}
//加入串列
bool Segment::AddToLink( Segment* segment )
{
	segment -> firstSegment = firstSegment;
	for( Segment* s = firstSegment; s != NULL; s = s -> link )
	{
		if( s -> link == NULL )
		{
			s -> link = segment;
			break;
		}
	}
	return true;
}
//找出距離表面點最遠的點
Point* Segment::GetFarPoint( Point* sPoint )
{
	farPoint = new Point();
	GLfloat maxDistance = 0;
	for( SegmentFace* s = segmentFace -> firstSegmentFace; s != NULL; s = s -> link )
	{
		if( s -> isPolygon )
		{
			for( int i = 0; i < s -> polygon -> cornerAmount; i++ )
			{
				GLfloat tempDistance = s -> polygon -> corner[i] -> DistanceToPoint( surfacePoint -> position );
				if( tempDistance > maxDistance )
				{
					maxDistance = tempDistance;
					farPoint -> Copy( s -> polygon -> corner[i] );
				}
			}
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				GLfloat tempDistance = MathHelper::VectorDistance( s -> polyhedronFace -> connectedPoint[i] -> position , 
																   surfacePoint -> position );
				if( tempDistance > maxDistance )
				{
					maxDistance = tempDistance;
					farPoint -> Copy( s -> polyhedronFace -> connectedPoint[i] -> position );
				}
			}
		}
	}
	return farPoint;
}
//取得整片碎片的面機
GLfloat Segment::GetArea()
{
	return segmentFace -> GetTotalArea();
}