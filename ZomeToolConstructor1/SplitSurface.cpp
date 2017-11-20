#include "SplitSurface.h"


SplitSurface::SplitSurface( Plane* p )
{
	//plane = p;
	plane = new Plane( p );
}

//加入串列
bool SplitSurface::AddToLink( SplitSurface* sSurface )
{
	sSurface -> firstSplitSurface = firstSplitSurface;
	for( SplitSurface* ss = firstSplitSurface; ss != NULL; ss = ss -> link )
	{
		if( ss -> link == NULL )
		{
			ss -> link = sSurface;
			break;
		}
	}
	return true;
}

//建立四個角，利用所有的邊線和兩條分界線
bool SplitSurface::SetCorner( Line* connectingLine , Line* line0 , Line* line1 )
{
	if( connectingLine == NULL )
	{
		std::cout << "SplitSurface::SetCorner：輸入的connectingLine線段組是空的" << std::endl;
		return false;
	}
	Point* tempVector = new Point( line0 -> endPosition );
	tempVector -> Plus( line1 -> endPosition );
	tempVector -> Normalize();
	Plane* minPlane = new Plane();
	MathHelper::CopyVector( minPlane -> vector , tempVector -> position );
	Plane* maxPlane = new Plane();
	MathHelper::CopyVector( maxPlane -> vector , tempVector -> position );

	GLfloat minConst = 999999.9;
	GLfloat maxConst = -999999.9;
	for( Line* cl = connectingLine -> firstLine; cl != NULL; cl = cl -> link )
	{
		//線段起點的值
		if( tempVector -> Dot( cl -> initPosition ) > maxConst )
		{
			maxConst = tempVector -> Dot( cl -> initPosition );
			MathHelper::CopyVector( maxPlane -> position , cl -> initPosition );
		}
		if( tempVector -> Dot( cl -> initPosition ) < minConst )
		{
			minConst = tempVector -> Dot( cl -> initPosition );
			MathHelper::CopyVector( minPlane -> position , cl -> initPosition );
		}
		//線段中點的值
		if( tempVector -> Dot( cl -> endPosition ) > maxConst )
		{
			maxConst = tempVector -> Dot( cl -> endPosition );
			MathHelper::CopyVector( maxPlane -> position , cl -> endPosition );
		}
		if( tempVector -> Dot( cl -> endPosition ) < minConst )
		{
			minConst = tempVector -> Dot( cl -> endPosition );
			MathHelper::CopyVector( minPlane -> position , cl -> endPosition );
		}
	}

	std::cout << "移動前的兩平面位置是上平面" << std::endl << "原點：(" << maxPlane -> position[0] << "," << maxPlane -> position[1] << "," << maxPlane -> position[2] << ")" << std::endl;
	std::cout << "方向：(" << maxPlane -> vector[0] << "," << maxPlane -> vector[1] << "," << maxPlane -> vector[2] << ")" << std::endl;
	std::cout << "下平面" << std::endl << "原點：(" << minPlane -> position[0] << "," << minPlane -> position[1] << "," << minPlane -> position[2] << ")" << std::endl;
	std::cout << "方向：(" << minPlane -> vector[0] << "," << minPlane -> vector[1] << "," << minPlane -> vector[2] << ")" << std::endl;


	for( int i = 0; i < 4; i++ ) corner[i] = new Point();

	for( int i = 0; i < 16; i++ ) MathHelper::VectorPlus( maxPlane -> position , maxPlane -> vector );//這個寫法不知道對不對
	for( int i = 0; i < 16; i++ ) MathHelper::VectorMinus( minPlane -> position , minPlane -> vector );
	/*
	if( maxPlane -> vector[0] + maxPlane -> vector[1] + maxPlane -> vector[2] > 0 )
	{
		for( int i = 0; i < 8; i++ ) MathHelper::VectorPlus( maxPlane -> position , maxPlane -> vector );//這個寫法不知道對不對
		for( int i = 0; i < 8; i++ ) MathHelper::VectorMinus( minPlane -> position , minPlane -> vector );
	}
	else
	{
		for( int i = 0; i < 8; i++ ) MathHelper::VectorMinus( maxPlane -> position , maxPlane -> vector );//這個寫法不知道對不對
		for( int i = 0; i < 8; i++ ) MathHelper::VectorPlus( minPlane -> position , minPlane -> vector );
	}
	*/
	std::cout << "移動後的兩平面位置是上平面" << std::endl << "原點：(" << maxPlane -> position[0] << "," << maxPlane -> position[1] << "," << maxPlane -> position[2] << ")" << std::endl;
	std::cout << "方向：(" << maxPlane -> vector[0] << "," << maxPlane -> vector[1] << "," << maxPlane -> vector[2] << ")" << std::endl;
	std::cout << "下平面" << std::endl << "原點：(" << minPlane -> position[0] << "," << minPlane -> position[1] << "," << minPlane -> position[2] << ")" << std::endl;
	std::cout << "方向：(" << minPlane -> vector[0] << "," << minPlane -> vector[1] << "," << minPlane -> vector[2] << ")" << std::endl;


	//左大->左小->右小->右大

	maxPlane -> GetIntersectPointOnRay( line0 , corner[0] -> position );//0     3
	minPlane -> GetIntersectPointOnRay( line0 , corner[1] -> position );//|     ^
	minPlane -> GetIntersectPointOnRay( line1 , corner[2] -> position );//v     |
	maxPlane -> GetIntersectPointOnRay( line1 , corner[3] -> position );//1 - > 2


	//測試分割面的兩邊線是否交差，交差的話就交換corner



	delete tempVector;
	delete minPlane;
	delete maxPlane;

	return true;
}

//=====================================SplitSurfaceConstructor//======================================

SplitSurfaceConstructor::SplitSurfaceConstructor()
{

}

SplitSurfaceConstructor::SplitSurfaceConstructor( int sSurfaceAmount )
{
	splitSurfaceAmount = sSurfaceAmount;
	tempSplitSurfaceCount = 0;
	model = new Model[ splitSurfaceAmount ];
}

int SplitSurfaceConstructor::NewSplitSurface(  SplitSurface* splitSurface )
{

	//GLfloat normalOfSplitSurface[3];
	//delete &model[ tempSplitSurfaceCount ];
	model[ tempSplitSurfaceCount ] = Model("CubeA0325Std.obj");

	//這邊出錯
	SetCube( &model[ tempSplitSurfaceCount ] , splitSurface );
	GLfloat position[] = { 0 , 0 , 0 };
	
	if( splitSurface -> corner[0] == NULL ||
		splitSurface -> corner[1] == NULL ||
		splitSurface -> corner[2] == NULL ||
		splitSurface -> corner[3] == NULL )
	{
		std::cout << "SplitSurfaceConstructor::NewSplitSurface出錯：有角是空得" << std::endl;
		int returnIndex = tempSplitSurfaceCount - 1;
		return returnIndex;
	}

	//如果點是0，代表這個點建措
	if( !MathHelper::CompareVector( splitSurface -> corner[0] -> position , position ) )
	{
		for( int i = 0; i < 6; i++ )
		{
			std::cout << "方塊" << tempSplitSurfaceCount << "點" << i << "(" << *( *( model[ tempSplitSurfaceCount ].vertices ) + i * 3 ) << ","
																			 << *( *( model[ tempSplitSurfaceCount ].vertices ) + i * 3 + 1 ) << ","
																			 << *( *( model[ tempSplitSurfaceCount ].vertices ) + i * 3 + 2 ) << ")" << std::endl;
		}
		

		//接頭的座標
	
	
		GLfloat rotation[] = { 0 , 0 , 0 };

		ModelObject* tempModel;
		//=========================================件模型
		GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
		if( originalModel == NULL )
		{
			tempModel = new ModelObject( &model[tempSplitSurfaceCount] , 
										 scale , 
										 position , 
										 rotation , 
										 0 ,
										 0 );
		}
		else
		{
			tempModel = new ModelObject( &model[tempSplitSurfaceCount] , 
										 scale , 
										 position , 
										 rotation , 
										 tempModelLink -> GetNextVertexOffset() ,
										 tempModelLink -> GetNextIndexOffset() );
		}
	


		//=========================================把街頭的模型加入串列
		if( originalModel == NULL )
		{
			originalModel = tempModel;
		}
		else
		{
			tempModelLink -> link = tempModel;//把所有接頭用鏈結串列連起來
		}
		tempModelLink = tempModel;		  //把所有求用鏈結串列連起來


		std::cout << "建立第" << tempSplitSurfaceCount << "個分割面" << std::endl;
		tempSplitSurfaceCount++;
	}
	else
	{
		std::cout << "SplitSurfaceConstructor::NewSplitSurface出錯：有角的位置是000" << std::endl;
	}
	int returnIndex = tempSplitSurfaceCount - 1;
	return returnIndex;
}

bool SplitSurfaceConstructor::SetCube( Model* m , SplitSurface* splitSurface )
{
	//先看一下分割面的四個角繞的方向
	//bool reverseNormal = false;
	Point* tempVector0 = new Point();
	Point* tempVector1 = new Point();
	if( splitSurface -> corner[0] == NULL || 
		splitSurface -> corner[1] == NULL )
	{
		std::cout << "SplitSurfaceConstructor::SetCube出錯：有角是空的" << std::endl;
	}
	else
	{
		tempVector0 -> Vector( splitSurface -> corner[0] -> position , splitSurface -> corner[1] -> position );
	}
	if( splitSurface -> corner[1] == NULL || 
		splitSurface -> corner[2] == NULL )
	{
		std::cout << "SplitSurfaceConstructor::SetCube出錯：有角是空的" << std::endl;
	}
	else
	{
		tempVector1 -> Vector( splitSurface -> corner[1] -> position , splitSurface -> corner[2] -> position );
	}
	
	Point* normalOfSplitSurface = new Point();
	normalOfSplitSurface -> Cross( tempVector0 , tempVector1 );
	normalOfSplitSurface -> Normalize();

	bool direction = false;

	if( MathHelper::VectorApproximation( normalOfSplitSurface -> position , splitSurface -> plane -> vector ) )
	{
		//同方向，四個角就照正常方向排列
		direction = SAME_DIRECTION;
	}
	else
	{
		//反方向，叫個角就照反方向排列
		direction = OPPOSITE_DIRECTION;
	}

	if( splitSurface -> corner[0] != NULL && 
		splitSurface -> corner[1] != NULL &&
		splitSurface -> corner[2] != NULL && 
		splitSurface -> corner[3] != NULL )
	{
		//左上 左下 右下 右上
		//2 4  0 6  1 7  3 5
		if( direction == SAME_DIRECTION )
		{
			//GLfloat tempCorner[3];
			//左上2 4
			MathHelper::CopyVector( *( m -> vertices ) + 2 * 3 , splitSurface -> corner[0] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 2 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 4 * 3 , splitSurface -> corner[0] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 4 * 3 , splitSurface -> plane -> vector );
			//左下0 6 
			MathHelper::CopyVector( *( m -> vertices ) + 0 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 0 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 6 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 6 * 3 , splitSurface -> plane -> vector );
			//右下1 7
			MathHelper::CopyVector( *( m -> vertices ) + 1 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 1 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 7 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 7 * 3 , splitSurface -> plane -> vector );
			//右上3 5
			MathHelper::CopyVector( *( m -> vertices ) + 3 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 3 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 5 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 5 * 3 , splitSurface -> plane -> vector );
		}
		else
		{
			//左上2 4
			MathHelper::CopyVector( *( m -> vertices ) + 2 * 3 , splitSurface -> corner[0] -> position );
			/*
			std::cout << "分割面點0:(" << splitSurface -> corner[0] -> position[0] << ","
				<< splitSurface -> corner[0] -> position[1] << ","
				<< splitSurface -> corner[0] -> position[2] << ")" << std::endl;*/
			MathHelper::VectorMinus( *( m -> vertices ) + 2 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 4 * 3 , splitSurface -> corner[0] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 4 * 3 , splitSurface -> plane -> vector );
			//左下0 6 
			MathHelper::CopyVector( *( m -> vertices ) + 0 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 0 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 6 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 6 * 3 , splitSurface -> plane -> vector );
			//右下1 7
			MathHelper::CopyVector( *( m -> vertices ) + 1 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 1 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 7 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 7 * 3 , splitSurface -> plane -> vector );
			//右上3 5
			MathHelper::CopyVector( *( m -> vertices ) + 3 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 3 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 5 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 5 * 3 , splitSurface -> plane -> vector );
		}
	}
	else
	{
		std::cout << "SplitSurfaceConstructor::SetCube出錯：有角是空的" << std::endl;
	}

	
	SetCubeNormal( m );

	delete tempVector0;
	delete tempVector1;
	delete normalOfSplitSurface;

	return true;
}

bool SplitSurfaceConstructor::SetCubeNormal( Model* m )
{
	Point** point = new Point*[ m -> verticeSize / 3 ];
	Point** faceNormal = new Point*[ m -> indiceSize / 3 ];

	Point* centerPoint = new Point( 0 , 0 , 0 );
	for( int i = 0; i < m -> verticeSize / 3; i++ )
	{
		point[i] = new Point( *( m -> vertices ) + i * 3 );
		centerPoint -> Plus( point[i] );
	}
	centerPoint -> Multiply( 1.0 / ( m -> verticeSize / 3 ) );//除以點的數量，得到中新點

	for( int i = 0; i < m -> indiceSize / 3; i++ )
	{
		//面的法向量，轉向向外
		faceNormal[i] = new Point();
		Point* tempVector0 = new Point();
		tempVector0 -> Vector( point[ *(*( m -> indices ) + i * 3) ] , 
							   point[ *(*( m -> indices ) + i * 3 + 1) ] );
		Point* tempVector1 = new Point();
		tempVector1 -> Vector( point[ *(*( m -> indices ) + i * 3) ] , 
							   point[ *(*( m -> indices ) + i * 3 + 2) ] );
		faceNormal[i] -> Cross( tempVector0 , tempVector1 );
		faceNormal[i] -> Normalize();

		Point* centerToV0 = new Point();
		centerToV0 -> Vector( centerPoint , point[ *(*( m -> indices ) + i * 3) ] );

		if( centerToV0 -> Dot( faceNormal[i] ) < 0 )
		{
			faceNormal[i] -> Reverse();
		}
	}

	//把面的法向量分給各個角
	for( int i = 0; i < m -> verticeSize / 3; i++ )
	{
		int faceCount = 0;
		GLfloat tempNormal[3] = { 0 , 0 , 0 };
		for( int j = 0; j < m -> indiceSize / 3; j++ )
		{
			if( *(*( m -> indices ) + j * 3 ) == i || 
				*(*( m -> indices ) + j * 3 + 1 ) == i ||
				*(*( m -> indices ) + j * 3 + 2 ) == i )
			{
				MathHelper::VectorPlus( tempNormal , faceNormal[j] -> position );
				faceCount++;
			}
		}
		if( faceCount == 0 )
		{
			std::cout << "SetCubeNormal時沒有找到點，出錯" << std::endl;
			return false;
		}
		MathHelper::VectorMutiply( tempNormal , 1.0 / (float)faceCount );

		MathHelper::CopyVector( point[i] -> normal , tempNormal );
	}
	return true;
}