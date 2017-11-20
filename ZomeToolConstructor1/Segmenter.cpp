#include "Segmenter.h"


//==========================================Segmenter//==========================================

Segmenter::Segmenter( Polyhedron* p , Point* sPointList )
{
	count = 0;
	

	polyhedron = p;
	surfacePointList = sPointList;
	segmentAmount = 0;


	GenerateSegment( GENERATOR );

 	ConstructSplitSurface();
}

Segmenter::Segmenter( Polyhedron* p , Point* sPointList , bool isGenerator )
{
	count = 0;

	polyhedron = p;
	surfacePointList = sPointList;
	segmentAmount = 0;

	GenerateSegment( isGenerator );

	if( isGenerator )
	{
		ConstructSplitSurface();
	}
}

//延伸出第一個平面
bool Segmenter::CheckIntersectionOnSplitPlane( Point* surfacePoint , Plane* splitPlane ,  SegmentFace*& segmentFace )
{
	//找出整個面被分割以後的樣子
	//對美一個平面都把模型歌乘兩半，直到留下最後一群面是沒有被任何面切到的，就是我們最終的碎片
	//要先把每一個面都跟第一個平面座checkIntersection，發現他們有沒有跟平面相切，如果有的話就停止網沒有被切到的面延伸
	//沒有切到的話就繼續延伸到下一個面做checkIntersection
	//如果有切到的面，必須把那個麵給切成更小的碎片，之後要checkIntersection時，就只判斷那個更小的碎片
	
	int initFace = polyhedron -> GetClosestFace( surfacePoint -> position );//地依點
	if( initFace == -2 ) 
	{
		std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：沒有找到該點在哪個面上" << std::endl;
		return false;//代表沒有找到點在哪個平面
	}
	/*
	std::cout << "第一個點的位置是(" << surfacePoint -> position[0] << "," << surfacePoint -> position[1] << "," << surfacePoint -> position[2] << ")" << std::endl;
	std::cout << "第一個算的面是點1:(" << polyhedron -> face[initFace] -> connectedPoint[0] -> position[0] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[0] -> position[1] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[0] -> position[2] << ")，點2:("
									   << polyhedron -> face[initFace] -> connectedPoint[1] -> position[0] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[1] -> position[1] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[1] -> position[2] << ")，點3:("
									   << polyhedron -> face[initFace] -> connectedPoint[2] -> position[0] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[2] -> position[1] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[2] -> position[2] << ")" << std::endl;
									   */

	segmentFace = new SegmentFace( polyhedron -> GetPolyhedronFace( initFace ) );
	segmentFace -> firstSegmentFace = segmentFace;


	//1.把所有平面都切一次，看切出來的多邊行長怎樣
	for( Plane* i = splitPlane; i != NULL; i = i -> link )
	{		
		if( !segmentFace -> isPolygon )
		{

			if( i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[1] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[1] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) )//還沒寫
			{
#pragma region 1.1:利用平面分割出新多邊形
				//int intersectedFaceNumber = segmentFace -> polyhedronFace -> GetPolyhedronFace( 0 , 1 );//還沒寫
				//edgeIntesrected[ intersectedNumber ] = true;
				segmentFace -> isPolygon = true;
				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
														  segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
														  segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
														  i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
														   segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
														   segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
														   i , IS_OUTSIDE );//還沒寫//------------------------------0323
				//這行是當某一個polygon她貼著平面，但卻沒有真的被平面切成兩塊時，舊用這邊判斷，要變成沒有任何型狀，還是留下整塊三角形
#pragma endregion
#pragma region 1.2:檢查這個形狀是不是剛好被平面割到邊邊，是的話就直接取原本的型狀
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					if( splitPlane -> IsInside( surfacePoint -> position ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在inside，分出來的形狀不存在(沒分割)
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//圓點在inside，分出來的形狀接近原形
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在OUTSIDE，分出來的形狀不存在(沒分割)
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//圓點在OUTSIDE，分出來的形狀接近原形
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
#pragma endregion
#pragma region 1.3:選擇哪一個多邊型比較靠近原點
				GLfloat distanceToPolygonInside = polygonInside -> DistanceToPoint( surfacePoint );//還沒寫
				GLfloat distanceToPolygonOutside = polygonOutside -> DistanceToPoint( surfacePoint );
				if( distanceToPolygonInside < distanceToPolygonOutside )
				{
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
				}
				else
				{
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
				}
				//segmentFace -> intersected = true;
#pragma endregion
			}
		}
		else
		{
			if( segmentFace -> polygon -> CheckIntersectionPolygonOnPlane( i ) )//3=0323
			{
#pragma region 1.1:利用平面分割出新多邊形
				if( segmentFace -> polygon -> cornerAmount <= 3 )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：要建心多邊形時舊多邊型只有兩個頂點。" << std::endl;
				}

				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polygon , i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polygon , i , IS_OUTSIDE );//還沒寫
#pragma endregion
#pragma region 1.2:檢查這個形狀是不是剛好被平面割到邊邊
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					if( splitPlane -> IsInside( surfacePoint -> position ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在inside，分出來的形狀不存在(沒分割)
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//圓點在inside，分出來的形狀接近原形
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在OUTSIDE，分出來的形狀不存在(沒分割)
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//圓點在OUTSIDE，分出來的形狀接近原形
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
#pragma endregion
#pragma region 1.3:選擇哪一個多邊型比較靠近原點
				GLfloat distanceToPolygonInside = polygonInside -> DistanceToPoint( surfacePoint );//還沒寫
				GLfloat distanceToPolygonOutside = polygonOutside -> DistanceToPoint( surfacePoint );

				MyPolygon *oldPolygon = segmentFace -> polygon;//取離員點進的點來用
				if( distanceToPolygonInside < distanceToPolygonOutside )
				{
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
				}
				else
				{
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
				}
				delete oldPolygon;
#pragma endregion
			}
		}
	}
	if(  segmentFace -> polygon != NULL )
	{
		if( segmentFace -> polygon -> cornerAmount == 2 )
		{
			std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯，新多邊型只有兩個頂點" << std::endl;
		}
	}
	

	//2.切完如果都沒切到，就可以直接向下延伸，如果有切到，就要根據切到以後可以延伸的面延伸
	if( !segmentFace -> isPolygon )
	{
		for( int i = 0; i < 3; i++ )
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			if( !segmentFace -> ListContainFace( segmentFace -> polyhedronFace -> connectedFace[i] -> index ) )
			{
				PolyhedronPoint *connectedPoint0 , *connectedPoint1;
				connectedPoint0 = NULL;
				connectedPoint1 = NULL;
				segmentFace -> polyhedronFace -> GetPropagatingLine( segmentFace -> polyhedronFace -> connectedFace[i] , 
																	 connectedPoint0 , connectedPoint1 );
				Line* propagatingLine = new Line( connectedPoint0 -> position , connectedPoint1 -> position );
				
				SegmentFace* propagatedSegmentFace = new SegmentFace( segmentFace -> polyhedronFace -> connectedFace[i] , propagatingLine );
				segmentFace -> AddToLink( propagatedSegmentFace );
				if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：碎片面積過大" << std::endl;
					segmentFace -> firstSegmentFace -> isTooBig = true;
					return false;
				}
				//開始下一次地回，查看下一個面有沒有相交
				CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
			}
		}
	}
	else
	{
		std::cout << std::endl << "多邊形包含";
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )
		{
			std::cout << "點(" << segmentFace -> polygon -> corner[i] -> position[0] << ","
							   << segmentFace -> polygon -> corner[i] -> position[1] << ","
							   << segmentFace -> polygon -> corner[i] -> position[2] << ")、" << std::endl;
		}

		
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )//對每個corner跟他的下一個點的連線做一個line，看看是不是可以延伸到另一個面的line
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			Line* propagatingLine;
			if( i == segmentFace -> polygon -> cornerAmount - 1 ) //取第i個點和第i+1個點的連線
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[0] -> position );
			else
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[i+1] -> position );
			
			if( segmentFace -> polyhedronFace -> Contain( propagatingLine ) )
			{
				PolyhedronPoint* propagatingPolyhedronFace = segmentFace -> polyhedronFace -> GetPolyhedronFace( propagatingLine );//還沒寫
				
				
				if( !segmentFace -> ListContainFace( propagatingPolyhedronFace -> index ) )//如果這個面還沒有被處理過，才要處理
				{
					SegmentFace* propagatedSegmentFace = new SegmentFace( propagatingPolyhedronFace , propagatingLine );
					segmentFace -> AddToLink( propagatedSegmentFace );
					if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
					{
						std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：碎片面積過大" << std::endl;
						segmentFace -> firstSegmentFace -> isTooBig = true;
						return false;
					}

					CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
				}
			}
		}
	}



	return true;


}
//延伸出沒有被切掉的平面
bool Segmenter::CheckIntersectionOnSplitPlane( Line* sLine , Plane* splitPlane , SegmentFace* segmentFace )
{
	std::cout << count << ":新的segmentFace" << segmentFace << "要查看是否被分割面切割，目前面積是" << segmentFace -> GetTotalArea() << std::endl;
	count++;
	
	//1.把所有平面都切一次，看切出來的多邊行長怎樣
	for( Plane* i = splitPlane; i != NULL; i = i -> link )
	{
		//部事多邊形就是三角形，所以只要檢查三個邊
		if( !segmentFace -> isPolygon )
		{
			if( i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[1] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[1] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) )//還沒寫
			{
#pragma region 1.1:利用平面分割出新多邊形
				
				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
													 i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
													 i , IS_OUTSIDE );//還沒寫
#pragma endregion
#pragma region 1.2:檢查這個形狀是不是剛好被平面割到邊邊
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					GLfloat middlePointOfSourceLine[] = { 0 , 0 , 0 };
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> initPosition );
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> endPosition );
					/**/MathHelper::VectorMutiply( middlePointOfSourceLine , 0.5 );//取來源線段的中點

					if( splitPlane -> IsInside( middlePointOfSourceLine ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在inside，分出來的形狀不存在(沒分割)
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//圓點在inside，分出來的形狀接近原形
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在OUTSIDE，分出來的形狀不存在(沒分割)
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//圓點在OUTSIDE，分出來的形狀接近原形
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
				segmentFace -> isPolygon = true;
#pragma endregion
#pragma region 1.3:選擇哪一個多邊型比較靠近原點
				//如果三角形有相交，就要分割出多邊形，然後根據這個多邊形有沒有包含來源邊，來選擇要用哪一個多邊形
				if( polygonInside -> Contain( sLine ) )
				{
					std::cout << "建立新多邊行polygonInside" << std::endl;
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
				}
				else if( polygonOutside -> Contain( sLine ) )
				{
					std::cout << "建立新多邊行polygonOutside" << std::endl;
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
				}
				else
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：分割出來的多邊形不能和延伸來源邊相符" << std::endl;
					segmentFace -> isPolygon = false;
					delete polygonInside;
					delete polygonOutside;

				}
#pragma endregion
			}
		}
		else
		{
			if( segmentFace -> polygon -> CheckIntersectionPolygonOnPlane( i ) )
			{
#pragma region 1.1:利用平面分割出新多邊形
				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polygon , i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polygon , i , IS_OUTSIDE );//還沒寫
#pragma endregion
#pragma region 1.2:檢查這個形狀是不是剛好被平面割到邊邊
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					GLfloat middlePointOfSourceLine[] = { 0 , 0 , 0 };
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> initPosition );
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> endPosition );
					/**/MathHelper::VectorMutiply( middlePointOfSourceLine , 0.5 );//取來源線段的中點

					if( splitPlane -> IsInside( middlePointOfSourceLine ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在inside，分出來的形狀不存在(沒分割)
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//圓點在inside，分出來的形狀接近原形
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//圓點在OUTSIDE，分出來的形狀不存在(沒分割)
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//圓點在OUTSIDE，分出來的形狀接近原形
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
#pragma endregion
#pragma region 1.3:選擇哪一個多邊型比較靠近原點
				MyPolygon *oldPolygon = segmentFace -> polygon;//取離員點進的點來用
				if( polygonInside -> Contain( sLine ) )
				{
					std::cout << "將員多邊行分割出新多邊行polygonInside" << std::endl;
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
					delete oldPolygon;
				}
				else if( polygonOutside -> Contain( sLine ) )
				{
					std::cout << "將員多邊行分割出新多邊行polygonOutside" << std::endl;
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
					delete oldPolygon;
				}
				else
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：分割出來的多邊形不能和延伸來源邊相符" << std::endl;
					delete polygonInside;
					delete polygonOutside;
				}
				
#pragma endregion
			}
		}
	}

	//2.切完如果都沒切到，就可以直接向下延伸，如果有切到，就要根據切到以後可以延伸的面延伸
	if( !segmentFace -> isPolygon )
	{
		for( int i = 0; i < 3; i++ )
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			//下面這行事說 整個segmentface的串列全部都不包含這個index，就代表整個串列裡面都沒有這個面，就可以來建
			if( !segmentFace -> ListContainFace( segmentFace -> polyhedronFace -> connectedFace[i] -> index ) )
			{
				if( segmentFace -> polyhedronFace -> connectedFace[i] -> type != POLYHEDRON_FACE )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：延伸到的下一個面佈世POLYHEDRON_FACE" << std::endl;
					continue;
				}


				//先建出一條線，用來連到下一個面
				PolyhedronPoint *connectedPoint0 , *connectedPoint1; 
				connectedPoint0 = NULL;
				connectedPoint1 = NULL;
				segmentFace -> polyhedronFace -> GetPropagatingLine( segmentFace -> polyhedronFace -> connectedFace[i] , 
																	 connectedPoint0 , connectedPoint1 );//癌沒寫
				Line* propagatingLine = new Line( connectedPoint0 -> position , connectedPoint1 -> position );//!!!!!!這裡出錯
				if( segmentFace -> polyhedronFace -> connectedFace[i] -> index > 999999 )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：臨邊的面取到不存在的" << std::endl;
				}
				//鍵力下一個面
				
				SegmentFace* propagatedSegmentFace = new SegmentFace( segmentFace -> polyhedronFace -> connectedFace[i] , propagatingLine );//還沒寫
				segmentFace -> AddToLink( propagatedSegmentFace );
				if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：碎片面積過大" << std::endl;
					segmentFace -> firstSegmentFace -> isTooBig = true;
					return false;
				}
				//開始下一次地回，查看下一個面有沒有相交
				CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
			}
		}
	}
	else
	{
		std::cout << std::endl << "多邊形包含";
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )
		{
			std::cout << "點(" << segmentFace -> polygon -> corner[i] -> position[0] << ","
							   << segmentFace -> polygon -> corner[i] -> position[1] << ","
							   << segmentFace -> polygon -> corner[i] -> position[2] << ")、" << std::endl;
		}
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )//對每個corner跟他的下一個點的連線做一個line，看看是不是可以延伸到另一個面的line
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			Line* propagatingLine;
			if( i == segmentFace -> polygon -> cornerAmount - 1 ) //取第i個點和第i+1個點的連線
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[0] -> position );
			else
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[i+1] -> position );
			
			if( segmentFace -> polyhedronFace -> Contain( propagatingLine ) )//線在這片碎片的邊包含這條線，也就是這條線在邊上
			{
				PolyhedronPoint* propagatingPolyhedronFace = segmentFace -> polyhedronFace -> GetPolyhedronFace( propagatingLine );//延伸到劇有這個邊的面
				
				if( !segmentFace -> ListContainFace( propagatingPolyhedronFace -> index ) )//如果這個面還沒有被處理過，才要處理
				{
					if( propagatingPolyhedronFace -> type != POLYHEDRON_FACE )//檢查措物質
					{
						std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：臨邊的面取到不存在的" << std::endl;
						continue;
					}
					SegmentFace* propagatedSegmentFace = new SegmentFace( propagatingPolyhedronFace , propagatingLine );
					segmentFace -> AddToLink( propagatedSegmentFace );
					//如果碎片面積過大，舊世有出錯
					if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
					{
						std::cout << "Segmenter::CheckIntersectionOnSplitPlane出錯：碎片面積過大" << std::endl;
						segmentFace -> firstSegmentFace -> isTooBig = true;
						return false;
					}
					CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
				}
			}
		}
	}

	return true;
}
//鍵力碎片
bool Segmenter::GenerateSegment( bool isGenerator )
{
	int count = 0;
	//把每一個surfacePointList都輪過一遍，美一個都產生一個碎片
	for( Point* i = surfacePointList; i != NULL; i = i -> link )
	{
		std::cout << "===開始計算第" << count << "個碎片，socket是(" << i -> position[0] << "," << i -> position[1] << "," << i -> position[2] << ")" << std::endl;
		//std::vector<int> faceNumber;//包含的三角形號碼
		std::vector<int> splitPlaneNumber;
		Plane* splitPlane; splitPlane = NULL;
		SegmentFace* segmentFace; segmentFace = NULL;

		//對每個點建立中分面
		for( Point* j = surfacePointList; j != NULL; j = j -> link )
		{
			if( i == j )continue;
			//std::cout << "兩個邊借點的距離世" << i -> DistanceToPoint( j -> position ) << std::endl;
			if( i -> DistanceToPoint( j -> position ) > SPLIT_SURFACE_LIMIT ) continue;
			Plane* tempSplitPlane = new Plane( splitPlaneNumber.size() );//將分格面的編號+1
			splitPlaneNumber.push_back( splitPlaneNumber.size() );
			tempSplitPlane -> SetSplitSurface( i -> position , j -> position );
			if( splitPlane == NULL )//將這個分格面加進去練節串列中
			{
				splitPlane = tempSplitPlane;
				splitPlane -> firstPlane = splitPlane;
			}
			else
			{
				splitPlane -> AddToLink( tempSplitPlane );
			}
		}
		//std::cout << "總共建立" << splitPlaneNumber.size() << "個平面" << std::endl;
		//int initFace = polyhedron -> GetClosestFace( i -> position );//地依點
		//faceNumber.push_back( initFace );

		//找出整個面被分割以後的樣子
		//對美一個平面都把模型歌乘兩半，直到留下最後一群面是沒有被任何面切到的，就是我們最終的碎片
		//要先把每一個面都跟第一個平面座checkIntersection，發現他們有沒有跟平面相切，如果有的話就停止網沒有被切到的面延伸
		//沒有切到的話就繼續延伸到下一個面做checkIntersection
		//如果有切到的面，必須把那個麵給切成更小的碎片，之後要checkIntersection時，就只判斷那個更小的碎片
		

		CheckIntersectionOnSplitPlane( i , splitPlane , segmentFace );

		std::cout << count << std::endl;
		//然後要建出分割面
		if( segmentFace!= NULL )
		{
			if( !segmentFace -> firstSegmentFace -> isTooBig )
			{
				GenerateSplitSurface( splitPlane , segmentFace , i );
			}
		}
		
		//Delete( segmentFace );
		Delete( splitPlane );//還沒寫
	}

	//如果是用來生成分割面的，就不要客意去合併不屬於任何碎片的面，不然會出錯
	if( isGenerator == GENERATOR ) return true;

	//接下來要尋找，有沒有模型的面不屬於任何一個segment，如果有的話代表這個面應該是屬於鄰近的segment，但是被錯誤的分割開來
	//所以就要把相鄰的segment與這些面合併在一起
	for( int i = 0; i < polyhedron -> faceAmount; i++ )
	{
		SegmentFace *tempModelFace = new SegmentFace( polyhedron -> GetPolyhedronFace( i ) );
		bool isBelongToSegment = false;
		for( Segment* s = segment; s != NULL; s = s -> link )
		{
			if( tempModelFace -> BelongToSegment( s -> segmentFace ) )//這個要去segmentface裡面寫
			{
				isBelongToSegment = true;
			}
		}
		Segment* segmentToCombine;
		if( !isBelongToSegment )
		{
			std::cout << "找到三角形不屬於任何一碎片：(" << 
						 tempModelFace -> polyhedronFace -> connectedPoint[0] -> position[0] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[0] -> position[1] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[0] -> position[2] << "),(" <<
						 tempModelFace -> polyhedronFace -> connectedPoint[1] -> position[0] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[1] -> position[1] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[1] -> position[2] << "),(" <<
						 tempModelFace -> polyhedronFace -> connectedPoint[2] -> position[0] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[2] -> position[1] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[2] -> position[2] << ")" << std::endl;
			//先找出相鄰的碎片，然後把這些不屬於任何碎片的面加入這個碎片鐘
			segmentToCombine = FindAdjacentSegment( tempModelFace -> polyhedronFace );//這兩個寫在segmenter裡面
			std::cout << "把三角形加入這個碎片：(" << segmentToCombine -> surfacePoint -> position[0] << "," <<
													  segmentToCombine -> surfacePoint -> position[1] << "," <<
													  segmentToCombine -> surfacePoint -> position[2] << ")" << std::endl;
			CombineSegment( tempModelFace , segmentToCombine );
		}
	}

	return true;
}
//鍵力分割面
bool Segmenter::GenerateSplitSurface( Plane* splitPlane , SegmentFace* segmentFace , Point* sPoint )
{

	if( segmentFace -> firstSegmentFace -> isTooBig )
	{
		std::cout << " Segmenter::GenerateSplitSurface出錯：該碎片面積太大" << std::endl;
	}
	Segment* tempSegment = new Segment( segmentFace , polyhedron );
	tempSegment -> surfacePoint = sPoint;
	//debug
	//GLfloat test[3] = { 125 , 287 , -18 };
	//if( sPoint ->DistanceToPoint( test ) < 10.0 )
	//{
	//	PrintSegment( tempSegment );
	//}

	for( Plane* j = splitPlane -> firstPlane; j != NULL; j = j -> link )
	{
		//判斷j是不是有用到的面		
		if( segmentFace -> IsConnectedWith( j ) )//i碎片是否有和j平面相連
		{
			//有的話，開始找面的最小分割
			Point* endPoint0 = new Point();
			Point* endPoint1 = new Point();
			//先找最兩端的點，再把那兩個點對到期他的平面，看哪兩個平面有用到這兩個點
			segmentFace -> FindEndPointOnPlane( j , endPoint0 -> position , endPoint1 -> position );//還沒寫//i碎片去尋找和j平面相連的線斷的最邊員
			//最後就用這三個平面智做分割面

			std::cout << "取(" << endPoint0 -> position[0] << "," << endPoint0 -> position[1] << "," << endPoint0 -> position[2] << ")" << std::endl;
			std::cout << "和(" << endPoint1 -> position[0] << "," << endPoint1 -> position[1] << "," << endPoint1 -> position[2] << ")當作分割面兩側" << std::endl;

			//做兩條邊線
			Line* line0 = new Line();
			Line* line1 = new Line();


			for( Plane* k = splitPlane -> firstPlane; k != NULL; k = k -> link )
			{
				if( j == k ) continue;
				if( segmentFace -> IsConnectedWith( k ) )//當k平面也與i相連
				{
					if( k -> Contain( endPoint0 -> position ) )//邊點在k平面上，代表k是邊面
					{
						j -> Get2PlaneIntersectLine_2( k , line0 );//還沒寫
					}

					if( k -> Contain( endPoint1 -> position ) )
					{
						j -> Get2PlaneIntersectLine_2( k , line1 );
					}
				}
			}

			if( MathHelper::VectorAbsolution( line0 -> endPosition) == 0 )
			{
				std::cout << "Segmenter::GenerateSplitSurface出錯：第一條線沒有向量" << std::endl;
				return false;
			}

			std::cout << "移動前邊線Line0原點：(" << line0 -> initPosition[0] << "," << line0 -> initPosition[1] << "," << line0 -> initPosition[2] 
					  << ")，方向：("  << line0 -> endPosition[0] << "," << line0 -> endPosition[1] << "," << line0 -> endPosition[2] << ")" << std::endl;
			std::cout << "移動前邊線Line1原點：(" << line1 -> initPosition[0] << "," << line1 -> initPosition[1] << "," << line1 -> initPosition[2] 
					  << ")，方向：("  << line1 -> endPosition[0] << "," << line1 -> endPosition[1] << "," << line1 -> endPosition[2] << ")" << std::endl;


			//把兩個現的員點移到相聚最近的位置
			line0 -> TranslateToNearest_2( line1 );
			//line0 -> TranslateToNearest( line1 );

			//讓兩條線的向量都只向兩個端點 endPoint0 , endPoint1
			GLfloat line0ToEndPoint0[3];
			GLfloat line1ToEndPoint1[3];
			MathHelper::VectorMinus( line0ToEndPoint0 , endPoint0 -> position , line0 -> initPosition );//端點0減line0員點
			MathHelper::VectorMinus( line1ToEndPoint1 , endPoint1 -> position , line1 -> initPosition );//端點1減line1員點
			if( !( MathHelper::VectorDot( line0 -> endPosition , line0ToEndPoint0 ) > 0 ) ) 
			{
				std::cout << "線段0將方向反轉" << std::endl;
				MathHelper::VectorReverse( line0 -> endPosition );
			}
				
			if( !( MathHelper::VectorDot( line1 -> endPosition , line1ToEndPoint1 ) > 0 ) ) 
			{
				std::cout << "線段1將方向反轉" << std::endl;
				MathHelper::VectorReverse( line1 -> endPosition );
			}
				
			///讓兩條線的向量都只向兩個端點 endPoint0 , endPoint1
			GLfloat aa = MathHelper::VectorDot( line1 -> endPosition , line1ToEndPoint1 );

			std::cout << "邊線Line0原點：(" << line0 -> initPosition[0] << "," << line0 -> initPosition[1] << "," << line0 -> initPosition[2] 
					  << ")，方向：("  << line0 -> endPosition[0] << "," << line0 -> endPosition[1] << "," << line0 -> endPosition[2] << ")" << std::endl;
			std::cout << "邊線Line1原點：(" << line1 -> initPosition[0] << "," << line1 -> initPosition[1] << "," << line1 -> initPosition[2] 
					  << ")，方向：("  << line1 -> endPosition[0] << "," << line1 -> endPosition[1] << "," << line1 -> endPosition[2] << ")" << std::endl;

			Line* connectingLine; connectingLine = NULL;
			segmentFace -> GetConnectingLine( j , connectingLine );//還沒寫//

			SplitSurface* tempSplitSurface = new SplitSurface( j );//還沒寫
			tempSplitSurface -> SetCorner( connectingLine , line0 , line1 );

			//把這一個分割面加到碎片i的分割面串列中
			if( tempSegment -> splitSurface == NULL )
			{
				tempSegment -> splitSurface = tempSplitSurface;
				tempSegment -> splitSurface -> firstSplitSurface = tempSplitSurface;
			}
			else
			{
				tempSegment -> splitSurface -> AddToLink( tempSplitSurface );//還沒寫
			}

			delete endPoint0;
			delete endPoint1;
			delete line0;
			delete line1;
			//Delete( connectingLine );//還沒寫
		}
	}
	if( segment == NULL )
	{
		segment = tempSegment;
		segment -> firstSegment = tempSegment;
	}
	else
	{
		segment -> AddToLink( tempSegment );//還沒寫
	}
	//}
	return true;
}

bool Segmenter::ConstructSplitSurface()
{
	//splitSurfaceConstructor = new SplitSurfaceConstructor();
	//splitSurfaceConstructor -> segment = segment;
	//splitSurfaceConstructor -> originalSplitSurface = segment -> splitSurface;//先試試看只用一片碎片
	
	//1.先見整個分割面建立器陣列
	int segmentCount = 0;
	int splitSurfaceAmount = 0;
	for( Segment* s = segment -> firstSegment; s != NULL; s = s -> link )
	{
		segmentCount++;
		//splitSurfaceConstructor -> originalSplitSurface = s -> splitSurface;
		for( SplitSurface* ss = s -> splitSurface -> firstSplitSurface; ss != NULL; ss = ss -> link )
		{
			//NewSplitSurface( ss );
			splitSurfaceAmount++;
		}
	}
	splitSurfaceConstructor = new SplitSurfaceConstructor*[ segmentCount ];

	//2.再建各別建立器的物建
	segmentCount = 0;
	splitSurfaceAmount = 0;
	for( Segment* s = segment -> firstSegment; s != NULL; s = s -> link )
	{
		
		//splitSurfaceConstructor -> originalSplitSurface = s -> splitSurface;
		for( SplitSurface* ss = s -> splitSurface -> firstSplitSurface; ss != NULL; ss = ss -> link )
		{
			//NewSplitSurface( ss );
			splitSurfaceAmount++;
		}
		splitSurfaceConstructor[ segmentCount ] = new SplitSurfaceConstructor( splitSurfaceAmount );
		segmentCount++;
		splitSurfaceAmount = 0;
	}
	segmentAmount = segmentCount;


	//3.再把每個建立氣得分割面給加進去
	segmentCount = 0;
	splitSurfaceAmount = 0;
	for( Segment* s = segment -> firstSegment; s != NULL; s = s -> link )
	{
		//splitSurfaceConstructor -> originalSplitSurface = s -> splitSurface;
		for( SplitSurface* ss = s -> splitSurface -> firstSplitSurface; ss != NULL; ss = ss -> link )
		{
			//這邊出錯
			//int tempCount = splitSurfaceConstructor -> NewSplitSurface( ss );
			splitSurfaceConstructor[ segmentCount ] -> NewSplitSurface( ss );
		}
		segmentCount++;
	}
	return true;
}

bool Segmenter::Delete( Plane* plane )
{
	if( plane == NULL )
	{
		std::cout << "Segmenter::Delete出錯：該碎片沒有任何平面" << std::endl;
		return false;
	}

	for( Plane* p = plane -> firstPlane; p != NULL; )
	{
		Plane* temp = p -> link;
		delete p;
		p = temp;
	}
	return true;
}

bool Segmenter::Delete( SegmentFace* segmentFace )
{
	if( segmentFace == NULL )
	{
		std::cout << "Segmenter::Delete出錯：該碎片沒有任何碎片" << std::endl;
		return false;
	}

	for( SegmentFace* p = segmentFace -> firstSegmentFace; p != NULL; )
	{
		SegmentFace* temp = p -> link;
		delete p;
		p = temp;
	}
	return true;
}

bool Segmenter::DeleteFromLink( SegmentFace* segmentFace )
{
	if( segmentFace == NULL )
	{
		std::cout << "Segmenter::DeleteFromLink出錯：該碎片沒有任何碎片" << std::endl;
		return false;
	}
	for( SegmentFace* p = segmentFace -> firstSegmentFace; p != NULL; p = p -> link )
	{
		if( p -> link == segmentFace )
		{
			delete p -> link;
			p -> link = NULL;
			return true;
		}
	}
	std::cout << "Segmenter::DeleteFromLink出錯：沒有找到要刪除的碎片" << std::endl;
	return false;
}
//找出與segmentFace相鄰的segment，
Segment* Segmenter::FindAdjacentSegment( PolyhedronPoint* polyhedronFace )
{
	std::vector< int >* faceSearched = new std::vector< int >();
	faceSearched -> push_back( polyhedronFace -> index );
	//還沒有改segmentFace的first是誰
	for( int i = 0; i < 3; i++ )
	{
		Segment* segmentBelongedTo = FindAdjacentSegment( polyhedronFace -> connectedFace[i] , faceSearched );
		if( segmentBelongedTo != NULL )
		{
			delete faceSearched;
			return segmentBelongedTo;
		}
	}
	delete faceSearched;
	return NULL;
}

Segment* Segmenter::FindAdjacentSegment( PolyhedronPoint* polyhedronFace , std::vector<int>* faceSearched )
{
	//先看這個面有沒有找過
	for( int i = 0; i < faceSearched -> size(); i++  )
	{
		if( faceSearched -> at(i) == polyhedronFace -> index )
		{
			return NULL;
		}
	}
	faceSearched -> push_back( polyhedronFace -> index );

	for( int i = 0; i < 3; i++ )
	{
		Segment* segmentBelongedTo = FindBelongToSegment( polyhedronFace -> connectedFace[i] );
		if( segmentBelongedTo != NULL )
		{
			return segmentBelongedTo;
		}
		else
		{
			//找沒有重複過的面，看有沒有跟哪個碎片重疊
			segmentBelongedTo = FindAdjacentSegment( polyhedronFace -> connectedFace[i] , faceSearched );
			if( segmentBelongedTo != NULL )
			{
				return segmentBelongedTo;
			}
		}
	}
	return NULL;
}
//找出有哪個碎片與這個三角形重疊
Segment* Segmenter::FindBelongToSegment( PolyhedronPoint* polyhedronFace )
{
	for( Segment* s = segment; s != NULL; s = s -> link )
	{
		for( SegmentFace* sf = s -> segmentFace; sf != NULL; sf = sf -> link )
		{
			if( sf -> polyhedronFace -> index == polyhedronFace -> index )
			{
				return s;
			}
			/*
			for( int i = 0; i < 3; i++ )
			{
				if( sf -> isPolygon )
				{
					for( int j = 0; j < sf -> polygon -> cornerAmount; j++ )
					{
						if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] -> position ,
													   sf -> polygon -> corner[j] -> position ) )
						{
							return s;
						}
					}
				}
				else
				{
					for( int j = 0; j < 3; j++ )
					{
						if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] -> position ,
													   sf -> polyhedronFace -> connectedPoint[j] -> position ) )
						{
							return s;
						}
					}
				}
			}
			*/

		}
	}
	return NULL;
}
//然後把這些不屬於任何碎片的面加入這個碎片鐘
bool Segmenter::CombineSegment( SegmentFace* segmentFace , Segment* segmentToCombine )
{
	//segmentFace -> firstSegmentFace = segmentToCombine -> segmentFace -> firstSegmentFace;
	segmentToCombine -> segmentFace -> AddToLink( segmentFace );

	for( int i = 0; i < 3; i++ )
	{
		//如果這個平面不屬於任何碎片，就把這個平面加入欲合併的碎片中
		Segment* tempSegment = FindBelongToSegment( segmentFace -> polyhedronFace -> connectedFace[i] );
		if( tempSegment == NULL )
		{
			SegmentFace* tempSegmentFace = new SegmentFace( segmentFace -> polyhedronFace -> connectedFace[i] );
			CombineSegment( tempSegmentFace , segmentToCombine );
		}
		
	}
	return true;
}

void Segmenter::PrintSegment( Segment* s )
{
	for( SegmentFace* sf = s -> segmentFace; sf != NULL; sf = sf -> link )
	{
		if( !sf -> isPolygon )
		{
			std::cout << "這片碎片中的一面是三角形，他的三個點分別是：" << std::endl << "(";
			std::cout << sf -> polyhedronFace -> connectedPoint[0] -> position[0] << "," <<
						 sf -> polyhedronFace -> connectedPoint[0] -> position[1] << "," <<
						 sf -> polyhedronFace -> connectedPoint[0] -> position[2] << ")" << std::endl << "(";
			std::cout << sf -> polyhedronFace -> connectedPoint[1] -> position[0] << "," <<
						 sf -> polyhedronFace -> connectedPoint[1] -> position[1] << "," <<
						 sf -> polyhedronFace -> connectedPoint[1] -> position[2] << ")" << std::endl << "(";
			std::cout << sf -> polyhedronFace -> connectedPoint[2] -> position[0] << "," <<
						 sf -> polyhedronFace -> connectedPoint[2] -> position[1] << "," <<
						 sf -> polyhedronFace -> connectedPoint[2] -> position[2] << ")" << std::endl;
		}
		else
		{
			std::cout << "這片碎片中的一面是多邊行，他的" << sf -> polygon -> cornerAmount << "個點分別是：" << std::endl;
			for( int i = 0; i < sf -> polygon -> cornerAmount; i++ )
			{
				std::cout << "(";
				std::cout << sf -> polygon -> corner[i] -> position[0] << "," <<
							 sf -> polygon -> corner[i] -> position[1] << "," <<
							 sf -> polygon -> corner[i] -> position[2] << ")" << std::endl;
			}
		}
		std::cout << "========================================" << std::endl;
	}
}
