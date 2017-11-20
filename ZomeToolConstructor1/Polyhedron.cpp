#include "Polyhedron.h"


//===========================================PolyhedronPoint//===========================================
PolyhedronPoint::PolyhedronPoint( int i , GLfloat* pos , GLfloat* nor , int t )
{
	if( t != POLYHEDRON_POINT )
	{
		std::cout << "PolyhedronPoint建構子出錯" << std::endl;
	}
	index = i;
	type = t;
	MathHelper::CopyVector( position , pos );
	MathHelper::CopyVector( normal , nor );

	connectedPointsAmount = 0;
	connectedEdgesAmount = 0;
	connectedFacesAmount = 0;
}

//===========================================PolyhedronFace//===========================================

PolyhedronPoint::PolyhedronPoint( int i , PolyhedronPoint* cPoint0 , PolyhedronPoint* cPoint1 , PolyhedronPoint* cPoint2 , int t )
{
	index = i;
	type = t;
	if( t != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint建構子出錯，應該建face" << std::endl;
	}
	connectedPoint = new PolyhedronPoint*[3];
	connectedPoint[0] = cPoint0;
	connectedPoint[1] = cPoint1;
	connectedPoint[2] = cPoint2;
	connectedFace = new PolyhedronPoint*[3];;
}

PolyhedronPoint::~PolyhedronPoint()
{
	if( type == POLYHEDRON_POINT )
	{
		if( connectedPointsAmount > 0 )
		{
			delete connectedPoint;
		}
		if( connectedFacesAmount > 0 )
		{
			delete connectedFace;
		}
		if( connectedEdgesAmount > 0 )
		{
			delete connectedEdge;
		}
	}
	if( type == POLYHEDRON_FACE )
	{
		delete connectedFace;
	}
	if( type == POLYHEDRON_EDGE )
	{
	}
}

bool PolyhedronPoint::Contain( int pointNumber )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的Contain出錯，應該type要是face" << std::endl;
	}
	if( connectedPoint[0] -> index == pointNumber || 
		connectedPoint[1] -> index == pointNumber ||
		connectedPoint[2] -> index == pointNumber )
	{
		return true;
	}
	return false;
}

bool PolyhedronPoint::Contain( PolyhedronPoint* point )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的Contain出錯，應該type要是face" << std::endl;
	}
	if( connectedPoint[0] == point ||
		connectedPoint[1] == point ||
		connectedPoint[2] == point )
		return true;
	return false;
}

bool PolyhedronPoint::Contain( Line* line )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的Contain出錯，應該type要是face" << std::endl;
	}
	if( MathHelper::CompareVector( line -> endPosition , line -> initPosition ) )
	{
		std::cout << "查詢多邊型面是否包和一條線時，該線段起點和中點一樣" << std::endl;
	}
	Line* edge[3];
	edge[0] = new Line( connectedPoint[0] -> position , connectedPoint[1] -> position );
	edge[1] = new Line( connectedPoint[0] -> position , connectedPoint[2] -> position );
	edge[2] = new Line( connectedPoint[1] -> position , connectedPoint[2] -> position );
	
	if( edge[0] -> Containing( line ) || 
		edge[1] -> Containing( line ) || 
		edge[2] -> Containing( line ) )
	{
		delete edge[0];
		delete edge[1];
		delete edge[2];
		return true;
	}
	delete edge[0];
	delete edge[1];
	delete edge[2];
	return false;

}

PolyhedronPoint* PolyhedronPoint::GetPolyhedronFace( Line* propagatingLine )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的GetPolyhedronFace出錯，應該type要是face" << std::endl;
	}
	for( int i = 0; i < 3; i++ )
	{
		if( connectedFace[i] -> Contain( propagatingLine ) )
		{
			return connectedFace[i];
		}
	}
	std::cout << "取得多邊行面出錯，沒辦法利用邊線取得林邊的面" << std::endl;
}

int PolyhedronPoint::GetPolyhedronFace( int connectedPoint0 , int connectedPoint1  )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的GetPolyhedronFace出錯，應該type要是face" << std::endl;
	}
	for( int i = 0; i < 3; i++ )
	{
		if( connectedFace[i] -> Contain( connectedPoint0 ) && 
			connectedFace[i] -> Contain( connectedPoint1 ) )
		{
			return connectedFace[i] -> index;
		}
	}
	std::cout << "取得多邊行面出錯，沒辦法利用邊線取得林邊的面" << std::endl;
}

bool PolyhedronPoint::GetPropagatingLine( PolyhedronPoint* propagatingPolyhedronFace , 
										  PolyhedronPoint*& connectedPoint0 , PolyhedronPoint*& connectedPoint1 )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的GetPropagatingLine出錯，應該type要是face" << std::endl;
	}
	if( propagatingPolyhedronFace -> Contain( connectedPoint[0] ) && propagatingPolyhedronFace -> Contain( connectedPoint[1] ) )
	{
		connectedPoint0 = connectedPoint[0];
		connectedPoint1 = connectedPoint[1];
	}
	else if( propagatingPolyhedronFace -> Contain( connectedPoint[0] ) && propagatingPolyhedronFace -> Contain( connectedPoint[2] ) )
	{
		connectedPoint0 = connectedPoint[0];
		connectedPoint1 = connectedPoint[2];
	}
	else if( propagatingPolyhedronFace -> Contain( connectedPoint[1] ) && propagatingPolyhedronFace -> Contain( connectedPoint[2] ) )
	{
		connectedPoint0 = connectedPoint[1];
		connectedPoint1 = connectedPoint[2];
	}
	else
	{
		std::cout << "取得多邊型面的相連點時，出錯" << std::endl;
		return false;
	}
	return true;
}

GLfloat PolyhedronPoint::GetArea()
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint的Contain出錯，應該type要是face" << std::endl;
		return 0;
	}
	GLfloat vector1[] = {0,0,0};
	GLfloat vector2[] = {0,0,0};
	GLfloat normal[] = {0,0,0};

	MathHelper::VectorMinus( vector1 , connectedPoint[0] -> position , connectedPoint[1] -> position );
	MathHelper::VectorMinus( vector2 , connectedPoint[0] -> position , connectedPoint[2] -> position );
	MathHelper::VectorCorss( normal , vector1 , vector2 );

	return MathHelper::VectorAbsolution( normal ) / 2;
}

/*這邊是舊程式
//===========================================PolyhedronFace//===========================================

PolyhedronFace::PolyhedronFace( int i , PolyhedronPoint* cPoint0 , PolyhedronPoint* cPoint1 , PolyhedronPoint* cPoint2 )
{
	index = i;
	containedPoint[0] = cPoint0;
	containedPoint[1] = cPoint1;
	containedPoint[2] = cPoint2;
}


bool PolyhedronFace::Contain( int pointNumber )
{
	if( containedPoint[0] -> index == pointNumber || 
		containedPoint[1] -> index == pointNumber ||
		containedPoint[2] -> index == pointNumber )
	{
		return true;
	}
	return false;
}

bool PolyhedronFace::Contain( PolyhedronPoint* point )
{
	if( containedPoint[0] == point ||
		containedPoint[1] == point ||
		containedPoint[2] == point )
		return true;
	return false;
}

bool PolyhedronFace::Contain( Line* line )
{
	if( MathHelper::CompareVector( line -> endPosition , line -> initPosition ) )
	{
		std::cout << "查詢多邊型面是否包和一條線時，該線段起點和中點一樣" << std::endl;
	}
	Line* edge[3];
	edge[0] = new Line( containedPoint[0] -> position , containedPoint[1] -> position );
	edge[1] = new Line( containedPoint[0] -> position , containedPoint[2] -> position );
	edge[2] = new Line( containedPoint[1] -> position , containedPoint[2] -> position );
	
	if( edge[0] -> Containing( line ) || 
		edge[1] -> Containing( line ) || 
		edge[2] -> Containing( line ) )
	{
		delete[] edge;
		return true;
	}
	delete[] edge;
	return false;

}

PolyhedronFace* PolyhedronFace::GetPolyhedronFace( Line* propagatingLine )
{
	for( int i = 0; i < 3; i++ )
	{
		if( connectedFace[i] -> Contain( propagatingLine ) )
		{
			return connectedFace[i];
		}
	}
	std::cout << "取得多邊行面出錯，沒辦法利用邊線取得林邊的面" << std::endl;
}

bool PolyhedronFace::GetPropagatingLine( PolyhedronFace* propagatingPolyhedronFace , 
										 PolyhedronPoint* connectedPoint0 , PolyhedronPoint* connectedPoint1 )
{
	if( propagatingPolyhedronFace -> Contain( containedPoint[0] ) && propagatingPolyhedronFace -> Contain( containedPoint[1] ) )
	{
		connectedPoint0 = containedPoint[0];
		connectedPoint1 = containedPoint[1];
	}
	else if( propagatingPolyhedronFace -> Contain( containedPoint[0] ) && propagatingPolyhedronFace -> Contain( containedPoint[2] ) )
	{
		connectedPoint0 = containedPoint[0];
		connectedPoint1 = containedPoint[2];
	}
	else if( propagatingPolyhedronFace -> Contain( containedPoint[1] ) && propagatingPolyhedronFace -> Contain( containedPoint[2] ) )
	{
		connectedPoint0 = containedPoint[1];
		connectedPoint1 = containedPoint[2];
	}
	else
	{
		std::cout << "取得多邊型面的相連點時，出錯" << std::endl;
		return false;
	}
	return true;
}
這邊是舊程式
*/

//===========================================Polyhedron//===========================================

Polyhedron::Polyhedron()
{

}

Polyhedron::Polyhedron( Model* m )
{
	model = m;

	pointAmount = model -> verticeSize / 3;
	faceAmount = model -> indiceSize / 3;


	//1.開始把模型輸入進去
	point = new PolyhedronPoint*[ pointAmount ];
	for( int i = 0; i < pointAmount; i++ )
	{
		point[i] = new PolyhedronPoint( i , *( model -> vertices ) + i * 3 , *( model -> normals ) + i * 3 , POLYHEDRON_POINT );
	}
	face = new PolyhedronPoint*[ faceAmount ];
	for( int i = 0; i < faceAmount; i++ )
	{
		face[i] = new PolyhedronPoint( i , point[ *( *( model -> indices ) + i * 3 ) ] ,
										   point[ *( *( model -> indices ) + i * 3 + 1 ) ] ,
										   point[ *( *( model -> indices ) + i * 3 + 2 ) ] , POLYHEDRON_FACE );
	}


	Connect();
	
}

PolyhedronPoint* Polyhedron::GetPolyhedronFace( int i )
{
	if( face[i] == NULL )
	{
		std::cout << "出錯" << std::endl;
	}
	return face[i];
}

bool Polyhedron::Connect()//connect是用來把所有的點和面相連
{
	std::cout << "Polyhedron::Connect開始連結" << std::endl;
	//2.將每個面和點相連起，還有紀錄相連點
	for( int i = 0; i < pointAmount; i++ )//i是現在要查的點
	{
		
		std::vector<int> connectedFaceNumber;
		std::vector<int> connectedPointNumber;
		//std::cout << "Polyhedron::Connect將點" << i << "開始連結" << std::endl;
		//2.1.先計算每個點和幾個面相連
		for( int j = 0; j < faceAmount; j++ )//j試與i點相連的面
		{
			if( i == face[j] -> connectedPoint[0] -> index || 
				i == face[j] -> connectedPoint[1] -> index ||
				i == face[j] -> connectedPoint[2] -> index )
			{
				///connectedFaceAmount++;
				connectedFaceNumber.push_back( j );
				//相與點相連的點計起來
				for( int k = 0; k < 3; k++ )//k試j面上面的0~2個點
				{
					int tempIndex = face[j] -> connectedPoint[k] -> index;//j面k點的編號
					if( tempIndex != i && //下面這行是說，與點相連的點編號這個list不包含現在搜到的這個編號
						std::find( connectedPointNumber.begin(), connectedPointNumber.end(), tempIndex ) == connectedPointNumber.end() )
					{
						connectedPointNumber.push_back( tempIndex );
					}
				}

			}
		}

		//2.2.在把這些麵加進去點裡
		point[i] -> connectedFacesAmount = connectedFaceNumber.size();
		point[i] -> connectedFace = new PolyhedronPoint*[ connectedFaceNumber.size() ];
		for( int j = 0; j < connectedFaceNumber.size(); j++ )
		{
			//將與點相接的面計起來
			point[i] -> connectedFace[j] = face[ connectedFaceNumber.at(j) ];
			//std::cout << "Polyhedron::Connect將點" << i << "與面" << point[i] -> connectedFace[j] -> index << "連起來" << std::endl;
		}
		
		//2.3把相連的點加到i點裡面去
		point[i] -> connectedPointsAmount = connectedPointNumber.size();
		point[i] -> connectedPoint = new PolyhedronPoint*[ connectedPointNumber.size() ];
		for( int j = 0; j < connectedPointNumber.size(); j++ )
		{
			//連起來
			point[i] -> connectedPoint[j] = point[ connectedPointNumber.at(j) ];
			//std::cout << "Polyhedron::Connect將點" << i << "與點" << point[i] -> connectedPoint[j] -> index << "連起來" << std::endl;
		}
	}


	//3.把相連的面紀錄起來
	for( int i = 0; i < faceAmount; i++ )
	{
		//std::cout << "Polyhedron::Connect將面" << i << "開始相連" << std::endl;
		std::vector<int> connectedFaceNumber;
		for( int j = 0; j < faceAmount; j++ )
		{
			if( i == j ) continue;
			if( ( face[i] -> Contain( face[j] -> connectedPoint[0]->index ) && face[i] -> Contain( face[j] -> connectedPoint[1]->index ) ) ||
				( face[i] -> Contain( face[j] -> connectedPoint[0]->index ) && face[i] -> Contain( face[j] -> connectedPoint[2]->index ) ) ||
				( face[i] -> Contain( face[j] -> connectedPoint[1]->index ) && face[i] -> Contain( face[j] -> connectedPoint[2]->index ) ) )
			{
				//當有兩個點相同時
				connectedFaceNumber.push_back( j );
				std::cout << face[j] -> connectedPoint[0]->index << "," << face[j] -> connectedPoint[1]->index << "," << face[j] -> connectedPoint[2]->index << "," << std::endl;
			}
		}

		//把最後結果輸進去
		
		if( connectedFaceNumber.size() != 3 )
		{
			std::cout << "Polyhedron(多面體)在connect時face的相連出錯：只有"  << connectedFaceNumber.size() << "面" << std::endl;
			std::cout << face[i] -> connectedPoint[0]->index << "," << face[i] -> connectedPoint[1]->index << "," << face[i] -> connectedPoint[2]->index << "," << std::endl;
			std::cout << "(" << face[i] -> connectedPoint[0]-> position[0] << "," <<
							    face[i] -> connectedPoint[0]-> position[1] << "," <<
								face[i] -> connectedPoint[0]-> position[2] << "),(" <<
								face[i] -> connectedPoint[1]-> position[0] << "," <<
								face[i] -> connectedPoint[1]-> position[1] << "," <<
								face[i] -> connectedPoint[1]-> position[2] << "),(" <<
								face[i] -> connectedPoint[2]-> position[0] << "," <<
								face[i] -> connectedPoint[2]-> position[1] << "," <<
								face[i] -> connectedPoint[2]-> position[2] << ")" << std::endl;
		}
		for( int j = 0; j < 3; j++ )
		{
			//std::cout << connectedFaceNumber.at(j) << std::endl;
			face[i] -> connectedFace[ j ] = face[ connectedFaceNumber.at(j) ];
			//std::cout << "Polyhedron::Connect將面" << i << "與面" << face[i] -> connectedFace[j] -> index << "連起來" << std::endl;
		}
	}
	return true;
}

int Polyhedron::GetClosestFace( GLfloat* pos )//從outerCone那邊拿過來的code
{
	
	Point* endPointCenter = new Point( pos );
	//Point* tempNearstPoint = new Point();
	int nearestFaceNumber = -1;

	//紀錄最小距離
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < faceAmount; i++ ) //出錯
	{
		Point* vector1 = new Point(); vector1 -> Vector( face[i] -> connectedPoint[0] -> position , face[i] -> connectedPoint[2] -> position );
		Point* vector2 = new Point(); vector2 -> Vector( face[i] -> connectedPoint[1] -> position , face[i] -> connectedPoint[2] -> position );
		//算normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		/*
		GLfloat* vertexNormalAverage = new GLfloat[3];//vertex normal平均值
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		MathHelper::VectorMutiply( vertexNormalAverage , 1.0 / 3.0 );*/
		//( Normal 1 + Normal 2 + Normal 3 ) / 3
		Point* vertexNormalAverage = new Point( face[i] -> connectedPoint[0] -> normal );
		vertexNormalAverage -> Plus( face[i] -> connectedPoint[1] -> normal );
		vertexNormalAverage -> Plus( face[i] -> connectedPoint[2] -> normal );
		vertexNormalAverage -> Multiply( 1.0 / 3.0 );
		//如果和點法向量相反方向，就把平面normal反轉
		if( vertexNormalAverage -> Dot( normal -> position ) < 0 ) normal -> Reverse();
		
		//從末端到表面 與normal同方向
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position , face[i] -> connectedPoint[0] -> position );
		GLfloat sb = normal -> Dot( distancePtoV0 );//是正的 如果點在模型裡，也就是距離
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//是正的 如果點在模型裡，也就是距離

		if( abs( sb ) < 1 )
		{
			std::cout << "取到最近面" << i << "，包含" << std::endl;
			std::cout << "點(" << face[i] -> connectedPoint[0] -> position[0] << "," << face[i] -> connectedPoint[0] -> position[1] << "," << face[i] -> connectedPoint[0] -> position[2] << ")和" << std::endl;
			std::cout << "點(" << face[i] -> connectedPoint[1] -> position[0] << "," << face[i] -> connectedPoint[1] -> position[1] << "," << face[i] -> connectedPoint[1] -> position[2] << ")和" << std::endl;
			std::cout << "點(" << face[i] -> connectedPoint[2] -> position[0] << "," << face[i] -> connectedPoint[2] -> position[1] << "," << face[i] -> connectedPoint[2] -> position[2] << ")" << std::endl;
		}

		if( sb < minDistance && sb > -0.1 )
		{
			//如果這個面與點的距離小於目前的最小距離，則來看
			//1.這個點投影到面上時，會在三角型三個點之內，則她的距離就試與平面的距離
			//2.這個點投影到面上時，在三角型三個點之外，則她的距離就是與三個點的最小距離(比與平面的距離大)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//這邊算投影的點
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			if( abs( sb ) > SMALL_NUMBER )//如果sb泰小，就不用在算投影點
			{
				normal -> Multiply( sb );
				pointProjectOnPlane -> Plus( normal );
				normal -> Multiply( 1.0 / sb );
			}

			//算第一個做標 第一個是三角型總面積x2 第二個是三角型點0到投影點的距離 第三個是投影點和編1所包的面積
			Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector( face[i] -> connectedPoint[0] -> position , pointProjectOnPlane -> position );
			Point* vertex1ToPoint = new Point(); vertex1ToPoint -> Vector( face[i] -> connectedPoint[1] -> position , pointProjectOnPlane -> position );
			Point* crossArea1 = new Point(); crossArea1 -> Cross( vector1 , vertex0ToPoint );
			Point* crossArea2 = new Point(); crossArea2 -> Cross( vertex1ToPoint , vector2 );
			
			//算第一個做標漢第二個做標
			GLfloat TotalArea = crossTotalArea -> Dot( normal );
			GLfloat Area1 = crossArea1 -> Dot( normal );
			GLfloat Area2 = crossArea2 -> Dot( normal );
			if( ( Area1 >= 0 && Area2 >= 0 ) || 
				( Area1 <= 0 && Area2 <= 0 ) )//這行代表一個正一個負，就是這個點未在三角形中
			{
				if( abs( TotalArea ) > abs( Area1 ) + abs( Area2 ) )
				{
					//有在三角形上
					insideTriangle = true;
				}
			}

			if( insideTriangle )
			{
				if( sb > -1.0 )
				{
					minDistance = sb;
					//tempNearstPoint -> Copy( pointProjectOnPlane );
					nearestFaceNumber = i;
				}
			}
			else
			{
				GLfloat* distance = new GLfloat[3];
				distance[0] = endPointCenter -> DistanceToPoint( face[i] -> connectedPoint[0] -> position );
				distance[1] = endPointCenter -> DistanceToPoint( face[i] -> connectedPoint[1] -> position );
				distance[2] = endPointCenter -> DistanceToPoint( face[i] -> connectedPoint[2] -> position );
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
					//tempNearstPoint -> Copy( face[i] -> containedPoint[min] -> position );
					nearestFaceNumber = i;
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
	if( minDistance > SMALL_NUMBER )
	{
		std::cout << "Polyhedron::GetClosestFace出錯：沒有取到就在面上的點" << std::endl;
		nearestFaceNumber = -2;
	}
	
	if( nearestFaceNumber == -1 )
	{
		std::cout << "Polyhedron多面體計算離點最近面出錯" << std::endl;
	}

	//delete tempNearstPoint;
	return nearestFaceNumber;
}

