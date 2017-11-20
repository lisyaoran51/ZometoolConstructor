#include "OuterCone.h"


OuterCone::OuterCone( Cone* cone , int coneFace , int t )
{
	sourceCone = cone;
	sourceConeFace = coneFace;

	sourceCorner = new Point*[3];
	constructedHole = new int[3];
	constructedSize = new int[3];
	cornerEnd = new Point*[3];
	singleConnector = -1;

	neighborOuterCone = new OuterCone*[3];

	engaged = false;
	connectorEngaged = new bool[3];
	connectorEngaged[0] = false;
	connectorEngaged[1] = false;
	connectorEngaged[2] = false;

	dublicatedWithCone = false;
	dublicatedConeIndex = -1;

	dublicatedWithOuterCone = false;
	dublicatedOuterConeIndex = -1;
	cornerDublicated = new bool[3];
	cornerDublicated[0] = false;
	cornerDublicated[1] = false;
	cornerDublicated[2] = false;

	intersected = false;

	type = t;
	//firstOuterCone = firstOCone;
	firstOuterCone = this;
	for( int i = 0; i < TYPE_AMOUNT; i ++ ) 
	{
		distanceToSurface[ i ] = -1.0;
		weightedDistance[ i ] = -1.0;
	}
	nearestPointOnSurface = new Point();

	zomeDirection = sourceCone -> zomeDirection;

	CheckConeDirection();
	SelectConstructedHole();


	CheckDublicatedCone();
	//CheckDublicatedOuterCone();//檢查伸出來的方向有沒有和別人重複

}

OuterCone::OuterCone( Cone* cone , int coneFace , int t , OuterCone* firstOCone )
{

	sourceCone = cone;
	sourceConeFace = coneFace;

	sourceCorner = new Point*[3];
	constructedHole = new int[3];
	constructedSize = new int[3];
	cornerEnd = new Point*[3];
	singleConnector = -1;

	neighborOuterCone = new OuterCone*[3];

	engaged = false;
	connectorEngaged = new bool[3];
	connectorEngaged[0] = false;
	connectorEngaged[1] = false;
	connectorEngaged[2] = false;

	dublicatedWithCone = false;
	dublicatedConeIndex = -1;

	dublicatedWithOuterCone = false;
	dublicatedOuterConeIndex = -1;
	cornerDublicated = new bool[3];
	cornerDublicated[0] = false;
	cornerDublicated[1] = false;
	cornerDublicated[2] = false;

	intersected = false;

	type = t;
	firstOuterCone = firstOCone;
	for( int i = 0; i < TYPE_AMOUNT; i ++ ) 
	{
		distanceToSurface[ i ] = -1.0;
		weightedDistance[ i ] = -1.0;
	}
	nearestPointOnSurface = new Point();

	zomeDirection = sourceCone -> zomeDirection;

	CheckConeDirection();
	SelectConstructedHole();

	CheckNeighborOuterCone();//找出與她相鄰的outercone
	CheckDublicatedCone();
	CheckDublicatedOuterCone();//檢查伸出來的方向有沒有和別人重複

	//檢查和其他cone漢outercone的交錯情形
	if( !engaged && !dublicatedWithCone && !dublicatedWithOuterCone )
	{
		CheckIntersectWithConnector();
	}

	//CheckEngaged();
	/*
	if( !engaged )
	{
		CheckDistanceToSurface();
	}
	*/
}

OuterCone::~OuterCone()
{
	delete[] sourceCorner;
	delete[] constructedHole;
	delete[] constructedSize;
	
	delete[] neighborOuterCone;
	delete[] connectorEngaged;
	delete[] cornerDublicated;
	delete nearestPointOnSurface;
	for( int i = 0; i < 3; i++ )delete cornerEnd[i];
	delete[] cornerEnd;
}

bool OuterCone::CheckConeDirection()
{
	//Point** localCorner = new Point*[4];
	//基本的coneDirection是3，

	//分成24種方向
	//方快中心向前 三角形
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , 0 , -1 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 21;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 20;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 16;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 19;
	}
	//中心向前 看後面的面
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , 0 , 1 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 23;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 22;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 18;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 17;
	}
	//中心向左 原形
	if( MathHelper::CompareVector( sourceCone -> b5Direction , -1 , 0 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 4;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 7;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 1;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 0;
	}
	//中心向佐 看右邊的面
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 1 , 0 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 6;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 5;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 3;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 2;
	}
	//中心向上 正方形
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , -1 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 12;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 15;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 9;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 8;
	}
	//中心向上 看下面的面
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , 1 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 14;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 13;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 11;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 10;
	}


	switch( sourceConeFace )
	{
		//第一個corner都是直直接到單一根藍色的那個 然後兩測黃色
		//0是藍色那個 1是左邊的 2是右邊的
		//     0
		//    / \
		//   /   \
		//  /     \
		// /       \
		//1_________2
		case 0:
			sourceCorner[0] = sourceCone -> corner[0];
			sourceCorner[1] = sourceCone -> corner[3];
			sourceCorner[2] = sourceCone -> corner[2];
			break;
		case 1:
			sourceCorner[0] = sourceCone -> corner[1];
			sourceCorner[1] = sourceCone -> corner[2];
			sourceCorner[2] = sourceCone -> corner[3];
			break;
		case 2:
			sourceCorner[0] = sourceCone -> corner[2];
			sourceCorner[1] = sourceCone -> corner[1];
			sourceCorner[2] = sourceCone -> corner[0];
			break;
		case 3:
			sourceCorner[0] = sourceCone -> corner[3];
			sourceCorner[1] = sourceCone -> corner[0];
			sourceCorner[2] = sourceCone -> corner[1];
			break;
	}

	return true;
}

bool OuterCone::SelectConstructedHole()
{
	//先找出在coneDirection是0的方向下，他基本的接髮，下一步在旋轉他的接髮以後找出對應的洞
	Point* baseDirection = new Point[3]; //方向0的情況下的接髮
	//Point* baseDirection2 = new Point[3]; //方向0的情況下把球繞y軸旋轉90度的接髮
	Point* tempDirection = new Point[3]; //轉到該方向以後的接髮
	GLfloat* rotateXMatrix = new GLfloat[16]; MathHelper::makeIdentity( rotateXMatrix );
	GLfloat* rotateYMatrix = new GLfloat[16]; MathHelper::makeIdentity( rotateYMatrix );
	GLfloat* rotateZMatrix = new GLfloat[16]; MathHelper::makeIdentity( rotateZMatrix );
	
	bool rotateBaseDirection = false;//如果要使用baseDirection2，就把這個設成true
	//繞y軸旋轉90度以後，相當於是在B5指向(1,0,0)的cone
	//================================================================
	//這邊要根據三角錐在coneDirection是0的情況下，也就是b0Direction是(0,1,0)，b5Direction是(-1,0,0)時
	//這個三角錐的各面(sourceConeFace)的外支架建立方式，設定她的支架要往哪一個面增長
	switch( type )
	{
		case 0:
			cornerEnd[0] = new Point( sourceCorner[0] -> position );
			cornerEnd[1] = new Point( sourceCorner[1] -> position );
			cornerEnd[2] = new Point( sourceCorner[2] -> position );
			delete[] baseDirection;
			delete[] tempDirection;
			delete[] rotateXMatrix;
			delete[] rotateYMatrix;
			delete[] rotateZMatrix;
			return true;
			break;
		case 1://短黃短黃短欄
			
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 5;
					baseDirection[1].index = 58;
					baseDirection[2].index = 61;
					break;
				case 1://///4/25這邊有錯的樣子
					baseDirection[0].index = 5;
					baseDirection[1].index = 59;
					baseDirection[2].index = 57;
					break;
				case 2:
					baseDirection[0].index = 4;
					baseDirection[1].index = 56;
					baseDirection[2].index = 54;
					break;
				case 3:
					baseDirection[0].index = 4;
					baseDirection[1].index = 55;
					baseDirection[2].index = 60;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_S;
			break;

		case 2:// 短紅中黃中黃
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 36;
					baseDirection[1].index = 49;
					baseDirection[2].index = 48;
					break;
				case 1:
					baseDirection[0].index = 35;
					baseDirection[1].index = 47;
					baseDirection[2].index = 46;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 41;
					baseDirection[1].index = 52;
					baseDirection[2].index = 53;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 38;
					baseDirection[1].index = 50;
					baseDirection[2].index = 51;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_M;
			break;

		case 3://中鴻中欄中藍 //先負製過來被分
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 32;
					baseDirection[1].index = 18;
					baseDirection[2].index = 17;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 33;
					baseDirection[1].index = 15;
					baseDirection[2].index = 20;
					break;
				case 2:
					baseDirection[0].index = 38;
					baseDirection[1].index = 7;
					baseDirection[2].index = 6;
					break;
				case 3:
					baseDirection[0].index = 39;
					baseDirection[1].index = 8;
					baseDirection[2].index = 9;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_M;
			break;
		case 4://面01時 中黃短紅短紅 漢 面23時 中紅短藍短欄 //改到這邊，要把原本的baseDirection改道轉90度以後的
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 53;
					baseDirection[1].index = 41;
					baseDirection[2].index = 40;
					break;
				case 1:
					baseDirection[0].index = 52;
					baseDirection[1].index = 40;
					baseDirection[2].index = 41;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 42;
					baseDirection[1].index = 31;
					baseDirection[2].index = 30;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 43;
					baseDirection[1].index = 30;
					baseDirection[2].index = 31;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_S;
			break;
		case 5:// 中紅短藍短欄 //備份
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 33;
					baseDirection[1].index = 11;
					baseDirection[2].index = 8;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 32;
					baseDirection[1].index = 9;
					baseDirection[2].index = 10;
					break;
				case 2:
					baseDirection[0].index = 39;
					baseDirection[1].index = 25;
					baseDirection[2].index = 23;
					break;
				case 3:
					baseDirection[0].index = 38;
					baseDirection[1].index = 22;
					baseDirection[2].index = 24;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_S;
			break;
		case 6://面01時 中紅中藍短欄 漢 這是指有一邊 ok
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 41;
					baseDirection[1].index = 12;
					baseDirection[2].index = 27;
					break;
				case 1:
					baseDirection[0].index = 40;
					baseDirection[1].index = 11;
					baseDirection[2].index = 28;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 31;
					baseDirection[1].index = 19;
					baseDirection[2].index = 12;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 30;
					baseDirection[1].index = 14;
					baseDirection[2].index = 6;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_S;
			break;
		case 7://改成短黃中藍短欄 只有一邊 改掉中欄中藍短欄
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 52;
					baseDirection[1].index = 58;
					baseDirection[2].index = 19;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 50;
					baseDirection[1].index = 22;
					baseDirection[2].index = 14;
					break;
				case 2:
					baseDirection[0].index = 49;
					baseDirection[1].index = 19;
					baseDirection[2].index = 12;
					break;
				case 3:
					baseDirection[0].index = 47;
					baseDirection[1].index = 20;
					baseDirection[2].index = 11;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_S;
			break;
		case 8:// 中紅短欄中藍 
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 40;
					baseDirection[1].index = 26;
					baseDirection[2].index = 10;
					break;
				case 1:
					baseDirection[0].index = 41;//瞬時終012，從中央那個點開始
					baseDirection[1].index = 29;
					baseDirection[2].index = 13;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 30;
					baseDirection[1].index = 13;
					baseDirection[2].index = 21;
					singleConnector = 0;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 31;
					baseDirection[1].index = 7;
					baseDirection[2].index = 16;
					singleConnector = 0;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_M;
			break;
		case 9://改成短黃短欄中藍 只有一邊 改掉中欄短欄中藍
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 51;
					baseDirection[1].index = 16;
					baseDirection[2].index = 24;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 53;
					baseDirection[1].index = 21;
					baseDirection[2].index = 29;
					break;
				case 2:
					baseDirection[0].index = 46;
					baseDirection[1].index = 13;
					baseDirection[2].index = 21;
					break;
				case 3:
					baseDirection[0].index = 48;
					baseDirection[1].index = 10;
					baseDirection[2].index = 18;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_M;
			break;
	}

	
	baseDirection[0].SetPosition( zomeDirection -> faceDirection[ baseDirection[0].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[0].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[0].index * 3 + 2 ] );
	baseDirection[1].SetPosition( zomeDirection -> faceDirection[ baseDirection[1].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[1].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[1].index * 3 + 2 ] );
	baseDirection[2].SetPosition( zomeDirection -> faceDirection[ baseDirection[2].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[2].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[2].index * 3 + 2 ] );

	if( rotateBaseDirection )
	{
		/*
		std::cout << "旋轉前的方向是點1：(" << baseDirection[0].position[0] << "," <<
										  baseDirection[0].position[1] << "," <<
										  baseDirection[0].position[2] << ")" << std::endl;
		std::cout << "旋轉前的方向是點2：(" << baseDirection[1].position[0] << "," <<
										  baseDirection[1].position[1] << "," <<
										  baseDirection[1].position[2] << ")" << std::endl;
		std::cout << "旋轉前的方向是點3：(" << baseDirection[2].position[0] << "," <<
										  baseDirection[2].position[1] << "," <<
										  baseDirection[2].position[2] << ")" << std::endl;
										  */
		MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );
		MathHelper::vectorMultMatrix( baseDirection[0].position , rotateYMatrix );
		MathHelper::vectorMultMatrix( baseDirection[1].position , rotateYMatrix );
		MathHelper::vectorMultMatrix( baseDirection[2].position , rotateYMatrix );
		MathHelper::makeIdentity( rotateYMatrix );
		/*
		std::cout << "旋轉後的方向是點1：(" << baseDirection[0].position[0] << "," <<
										  baseDirection[0].position[1] << "," <<
										  baseDirection[0].position[2] << ")" << std::endl;
		std::cout << "旋轉後的方向是點2：(" << baseDirection[1].position[0] << "," <<
										  baseDirection[1].position[1] << "," <<
										  baseDirection[1].position[2] << ")" << std::endl;
		std::cout << "旋轉後的方向是點3：(" << baseDirection[2].position[0] << "," <<
										  baseDirection[2].position[1] << "," <<
										  baseDirection[2].position[2] << ")" << std::endl;
										  */
	}
	

	//這邊要根據三角錐在coneDirection是0的情況下，也就是b0Direction是(0,1,0)，b5Direction是(-1,0,0)時
	//這個三角錐的各面(sourceConeFace)的外支架建立方式，設定她的支架要往哪一個面增長

	switch( coneDirection )
	{
		case 0:
			break;
		case 1:
			MathHelper::makeRotateX( rotateXMatrix , M_PI );//繞x軸旋轉180度
			break;
		case 2:
			MathHelper::makeRotateY( rotateYMatrix , M_PI );//繞y軸旋轉180度
			break;
		case 3:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸旋轉180度
			break;
		case 4:
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//繞x軸逆向旋轉90度
			break;
		case 5:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸旋轉180度
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//繞x軸逆向旋轉90度
			break;
		case 6:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸旋轉180度
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正向旋轉90度
			break;
		case 7:
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正向旋轉90度
			break;
		case 8:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//繞y軸正向旋轉90度
			break;
		case 9:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆向旋轉90度
			break;
		case 10:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆向旋轉90度
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆向旋轉90度
			break;
		case 11:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆向旋轉90度
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//繞y軸正向旋轉90度
			break;
		case 12:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			break;
		case 13:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆向旋轉90度
			break;
		case 14:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆向旋轉90度
			MathHelper::makeRotateY( rotateYMatrix , M_PI );//繞y軸旋轉180度
			break;
		case 15:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			MathHelper::makeRotateY( rotateYMatrix , M_PI );//繞y軸旋轉180度
			break;
		case 16:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸旋轉180度
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//繞y軸正向旋轉90度
			break;
		case 17:
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//繞y軸正向旋轉90度
			break;
		case 18:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸旋轉180度
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆向旋轉90度
			break;
		case 19:
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆向旋轉90度
			break;
		case 20:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆向旋轉90度
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//繞x軸逆向旋轉90度
			break;
		case 21:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正向旋轉90度
			break;
		case 22:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆向旋轉90度
			//MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			//MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆向旋轉90度
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正向旋轉90度
			break;
		case 23:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正向旋轉90度
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//繞x軸逆向旋轉90度
			break;
	}

	//將原本的方向乘上旋轉矩陣
	MathHelper::vectorMultMatrix( tempDirection[0].position , baseDirection[0].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateXMatrix );

	MathHelper::vectorMultMatrix( tempDirection[1].position , baseDirection[1].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateXMatrix );

	MathHelper::vectorMultMatrix( tempDirection[2].position , baseDirection[2].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[2].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[2].position , rotateXMatrix );

	

	//根據旋轉以後的方向，找出他的面
	constructedHole[0] = zomeDirection -> GetDirectionFace( tempDirection[0].position );
	constructedHole[1] = zomeDirection -> GetDirectionFace( tempDirection[1].position );
	constructedHole[2] = zomeDirection -> GetDirectionFace( tempDirection[2].position );

	delete[] baseDirection;
	delete[] tempDirection;
	delete[] rotateXMatrix;
	delete[] rotateYMatrix;
	delete[] rotateZMatrix;

	//這裡根據選好的洞，把zometool竿子的另一端未至算出來
	GLfloat endX = 0 , endY = 0 , endZ = 0;

	if( constructedHole[0] == -1 || constructedHole[1] == -1 || constructedHole[2] == -1 )
	{
		std::cout << "GetFaceLength出錯(輸入-1)" << std::endl;
		return false;
	}

	endX = sourceCorner[0] -> position[0] + 
		   zomeDirection -> faceDirection[ constructedHole[0] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) - NODE_DIAMETER / 2.0 + LATCH );
	endY = sourceCorner[0] -> position[1] + 
		   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) - NODE_DIAMETER / 2.0 + LATCH );
	endZ = sourceCorner[0] -> position[2] + 
		   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) - NODE_DIAMETER / 2.0 + LATCH );
	cornerEnd[0] = new Point( endX , endY , endZ );

	endX = sourceCorner[1] -> position[0] + 
		   zomeDirection -> faceDirection[ constructedHole[1] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[1] , constructedSize[1] ) - NODE_DIAMETER / 2.0 + LATCH );
	endY = sourceCorner[1] -> position[1] + 
		   zomeDirection -> faceDirection[ constructedHole[1] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[1] , constructedSize[1] ) - NODE_DIAMETER / 2.0 + LATCH );
	endZ = sourceCorner[1] -> position[2] + 
		   zomeDirection -> faceDirection[ constructedHole[1] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[1] , constructedSize[1] ) - NODE_DIAMETER / 2.0 + LATCH );
	cornerEnd[1] = new Point( endX , endY , endZ );

	endX = sourceCorner[2] -> position[0] + 
		   zomeDirection -> faceDirection[ constructedHole[2] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[2] , constructedSize[2] ) - NODE_DIAMETER / 2.0 + LATCH );
	endY = sourceCorner[2] -> position[1] + 
		   zomeDirection -> faceDirection[ constructedHole[2] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[2] , constructedSize[2] ) - NODE_DIAMETER / 2.0 + LATCH );
	endZ = sourceCorner[2] -> position[2] + 
		   zomeDirection -> faceDirection[ constructedHole[2] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[2] , constructedSize[2] ) - NODE_DIAMETER / 2.0 + LATCH );
	cornerEnd[2] = new Point( endX , endY , endZ );

	return true;
}

bool OuterCone::CheckNeighborOuterCone()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if(1){}
	}
	return true;
}

bool OuterCone::CheckDublicatedCone()
{
	//先看尾段有沒有碰到別人的頭
	//GLfloat connEndPoint[3];
	//GLfloat endX = 0 , endY = 0 , endZ = 0;
	//endX = sourceCorner[0] -> position[0] + 
	//	   zomeDirection -> faceDirection[ constructedHole[0] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) );
	//endY = sourceCorner[0] -> position[1] + 
	//	   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) );
	//endZ = sourceCorner[0] -> position[2] + 
	//	   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) );
	//MathHelper::SetVector( connEndPoint , endX , endY , endZ );

	Point* endPointCenter = new Point( cornerEnd[0] -> position );
	endPointCenter -> Plus( cornerEnd[1] );
	endPointCenter -> Plus( cornerEnd[2] );
	endPointCenter -> Multiply( 1.0 / 3.0 );
	if( cornerEnd[0] -> DistanceToPoint( cornerEnd[1] -> position ) > NODE_DIAMETER * 3.0 )
	{
		std::cout << "ZomeGenerator::CheckDistanceToSurface出錯：三個endPoint距離太遠(" <<
			cornerEnd[0] -> position[0] << "," <<
			cornerEnd[0] -> position[1] << "," <<
			cornerEnd[0] -> position[2] << ")(" <<
			cornerEnd[1] -> position[0] << "," <<
			cornerEnd[1] -> position[1] << "," <<
			cornerEnd[1] -> position[2] << ")(" <<
			cornerEnd[2] -> position[0] << "," <<
			cornerEnd[2] -> position[1] << "," <<
			cornerEnd[2] -> position[2] << ")" << std::endl;
	}


	for( int i = 0; i < 4; i++ )
	{
		if( sourceCone -> neighborCone[i] )
		{
			Cone* tempConeI = sourceCone -> neighborCone[i];//取代號

			CheckDublicatedConeEdge( tempConeI );//偵測
			if( dublicatedWithCone == true ) return true;//一發現有碰到，就跳出

			for( int j = 0; j < 4; j++ )
			{
				if( endPointCenter -> DistanceToPoint( tempConeI -> corner[j] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 &&
					tempConeI -> activated )
				{
					if( !intersected )
					{
						std::cout << "第" << type << "種組法支架的尾端太靠近別顆球" << std::endl;
					}
					intersected = true;
					//return dublicatedWithCone;
				}

				if( sourceCone -> neighborCone[i] -> neighborCone[j] )
				{
					Cone* tempConeJ = tempConeI -> neighborCone[j];//取代號

					CheckDublicatedConeEdge( tempConeJ );//偵測
					if( dublicatedWithCone == true ) return true;//一發現有碰到，就跳出

					for( int k = 0; k < 4; k++ )
					{
						if( endPointCenter -> DistanceToPoint( tempConeJ -> corner[k] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 &&
							tempConeJ -> activated )
						{
							if( !intersected )
							{
								std::cout << "第" << type << "種組法支架的尾端太靠近別顆球" << std::endl;
							}
							intersected = true;
							//return dublicatedWithCone;
						}
						//看尾段有沒有碰到別人的頭================================================
						//if( MathHelper::VectorApproximation( connEndPoint , tempConeJ -> corner[k] -> position ) )  
						//{
						//	dublicatedWithCone = true;
						//}

						if( sourceCone -> neighborCone[i] -> neighborCone[j] -> neighborCone[k] )
						{
							Cone* tempConeK = tempConeJ -> neighborCone[k];

							CheckDublicatedConeEdge( tempConeK );
							if( dublicatedWithCone == true ) return true;

							for( int l = 0; l < 4; l++ )
							{
								if( endPointCenter -> DistanceToPoint( tempConeK -> corner[l] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 &&
									tempConeK -> activated )
								{
									if( !intersected )
									{
										std::cout << "第" << type << "種組法支架的尾端太靠近別顆球" << std::endl;
									}
									intersected = true;
									//return dublicatedWithCone;
								}
							}
						}
					}
				}
			}
		}
	}

	//看尾段有沒有碰到別人的頭=================================================
	for( Cone* c = sourceCone -> firstCone; c != NULL; c = c -> link )
	{
		if( !c -> activated ) continue;
		for( int i = 0; i < 4; i++ )
		{
			if( endPointCenter -> DistanceToPoint( c -> corner[i] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 )
			{
				if( !intersected )
				{
					std::cout << "第" << type << "種組法支架的尾端太靠近別顆球2" << std::endl;
				}
				intersected = true;
				
				//return dublicatedWithCone;
			}
		}
	}

	return dublicatedWithCone;

}

bool OuterCone::CheckDublicatedConeEdge( Cone* cone )
{
	bool connEngaged[3] = { false , false , false };
	//對每個點到點的連線看一次有沒有相同方向
	if( !cone -> activated ) 
		return dublicatedWithCone;//compute那
	for( int i = 0; i < 4; i++ )
	{
		for( int k = 0; k < 3; k++ )
		{
			if( cone -> corner[i] -> CloselyCompare( sourceCorner[k] ) )
			{
				for( int j = 0; j < 4; j++ )
				{
					if( MathHelper::CompareDirection( cone -> corner[i] -> position , 
														cone -> corner[j] -> position , 
														sourceCorner[k] -> position , 
														cornerEnd[k] -> position ) )
					{
						connEngaged[k] = true;
						//dublicatedWithCone = true;
						//dublicatedConeIndex = cone -> index;
					}
				}	
			}
		}
	}

	if( connEngaged[0] && connEngaged[1] && connEngaged[2] )
	{
		dublicatedWithCone = true;
		dublicatedConeIndex = cone -> index;
		std::cout << "第" << type << "種組法支架與其他Cone重疊" << std::endl;
	}
	return dublicatedWithCone;
}

bool OuterCone::CheckDublicatedOuterCone()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 || this == i ) continue;
		
		CheckDublicatedOuterConeEdge( i );
	}
	if( dublicatedWithOuterCone )
	{
		std::cout << "第" << type << "種組法支架與其他外部Cone重疊" << std::endl;
	}

	return dublicatedWithOuterCone;
}

bool OuterCone::CheckDublicatedOuterConeEdge( OuterCone* outerCone )
{
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			if( outerCone -> sourceCorner[i] -> CloselyCompare( sourceCorner[j] ) )
			{
				if( MathHelper::CompareDirection( outerCone -> sourceCorner[i] -> position , 
												  outerCone -> cornerEnd[i] -> position , 
												  sourceCorner[j] -> position , 
												  cornerEnd[j] -> position )  )
				{
					dublicatedWithOuterCone = true;
					dublicatedOuterConeIndex = outerCone -> index;
					cornerDublicated[j] = true;//第j個corner是重複的
				}
			}
		}
	}
	return dublicatedWithOuterCone;
}

bool OuterCone::CheckIntersectWithConnector()
{
	GLfloat tempSourceCorner[3][3];
	MathHelper::VectorMinus( tempSourceCorner[0] , cornerEnd[0] -> position , sourceCorner[0] -> position );
	MathHelper::VectorMinus( tempSourceCorner[1] , cornerEnd[1] -> position , sourceCorner[1] -> position );
	MathHelper::VectorMinus( tempSourceCorner[2] , cornerEnd[2] -> position , sourceCorner[2] -> position );
	MathHelper::VectorNormalize( tempSourceCorner[0] );
	MathHelper::VectorNormalize( tempSourceCorner[1] );
	MathHelper::VectorNormalize( tempSourceCorner[2] );
	MathHelper::VectorPlus( tempSourceCorner[0] , sourceCorner[0] -> position );
	MathHelper::VectorPlus( tempSourceCorner[1] , sourceCorner[1] -> position );
	MathHelper::VectorPlus( tempSourceCorner[2] , sourceCorner[2] -> position );
	//看他有沒有和其他的cone的桿子交錯
	for( Cone* i = sourceCone -> firstCone; i != NULL; i = i -> link )
	{
		//if( i -> engagedDistance == -1 ) continue;
		if( i -> activated )
		{
			//4個點
			for( int j = 0; j < 4 - 1; j++ )//三角錐的某一點
			{
				//4個點
				for( int k = j + 1; k < 4; k++ )//三角錐的另一點 他們連線
				{
					//三跟外支架
					for( int l = 0; l < 3; l++ )//把這個連線來和l這個支架比
					{
						if( !i -> corner[j] -> CloselyCompare( sourceCorner[l] ) && !i -> corner[k] -> CloselyCompare( sourceCorner[l] ) )
						{
							GLfloat distance = CheckTwoLineDistance( i -> corner[j] , i -> corner[k] , sourceCorner[l] , cornerEnd[l] );
							if( distance < SMALL_NUMBER )
							{
								intersected = true;
								std::cout << "第" << type << "種組法支架與其他Cone的桿子交錯" << std::endl;
								return true;
							}
						}
					}
				}
			}
		}
	}

	//看他有沒有跟其他的outercone竿子交錯
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 ) continue;
		//別的cone得3個外支架
		for( int j = 0; j < 3; j++ )
		{
			//自己的三跟外支架
			for( int k = 0; k < 3; k++ )
			{
				if( !i -> sourceCorner[j] -> CloselyCompare( sourceCorner[k] ) )
				{
					if( CheckTwoLineDistance( i -> sourceCorner[j] , i -> cornerEnd[j] , sourceCorner[k] , cornerEnd[k] ) < SMALL_NUMBER )
					{
						std::cout << "第" << type << "種組法支架與其他外部Cone的桿子交錯" << std::endl;
						intersected = true;
						return true;
					}
				}
			}
		}
	}


	return false;
}

bool OuterCone::CheckIntersectWithConeFace()
{
	if( cornerEnd[0] -> DistanceToPoint( cornerEnd[1] -> position ) > NODE_DIAMETER * 2.0 )
	{
		std::cout << "ZomeGenerator::CheckDistanceToSurface出錯：三個endPoint距離太遠(" <<
			cornerEnd[0] -> position[0] << "," <<
			cornerEnd[0] -> position[1] << "," <<
			cornerEnd[0] -> position[2] << ")(" <<
			cornerEnd[1] -> position[0] << "," <<
			cornerEnd[1] -> position[1] << "," <<
			cornerEnd[1] -> position[2] << ")(" <<
			cornerEnd[2] -> position[0] << "," <<
			cornerEnd[2] -> position[1] << "," <<
			cornerEnd[2] -> position[2] << ")" << std::endl;
	}
	//面0：032
	//面1：123
	//面2：210
	//面3：301
	int faceCorner[4][3] = { { 0 , 3 , 2 } , 
							 { 1 , 2 , 3 } ,
							 { 2 , 1 , 0 } , 
							 { 3 , 0 , 1 } };
	GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 2.0;


	//看他有沒有和其他的cone的面交錯
	for( Cone* c = sourceCone -> firstCone; c != NULL; c = c -> link )
	{
		if( c -> activated )
		{
			//如果距離太遠就跳過
			if( MathHelper::VectorDistance( c -> corner[0] -> position , sourceCorner[0] -> position ) > threshold ) 
				continue;
			//4個面
			for( int i = 0; i < 4; i++ )
			{
				if( CheckIntersectOuterConeOnTri( c -> corner[ faceCorner[i][0] ] -> position ,
												  c -> corner[ faceCorner[i][1] ] -> position ,
												  c -> corner[ faceCorner[i][2] ] -> position ) )
				{
					intersected = true;
					std::cout << "第" << type << "種組法會插入其他Cone面" << std::endl;
					return true;
				}
			}
		}
	}
	return false;
}

GLfloat OuterCone::CheckTwoLineDistance( Point* vec1Initial , Point* vec1End , Point* vec2Initial , Point* vec2End )
{
		//http://geomalgorithms.com/a07-_distance.html
	/*
	// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
	//    Input:  two 3D line segments S1 and S2
	//    Return: the shortest distance between S1 and S2
	float
	dist3D_Segment_to_Segment( Segment S1, Segment S2)
	{
		Vector   u = S1.P1 - S1.P0;
		Vector   v = S2.P1 - S2.P0;
		Vector   w = S1.P0 - S2.P0;
		float    a = dot(u,u);         // always >= 0
		float    b = dot(u,v);
		float    c = dot(v,v);         // always >= 0
		float    d = dot(u,w);
		float    e = dot(v,w);
		float    D = a*c - b*b;        // always >= 0
		float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
		float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
    
	}*/
	Point* u = new Point(); u -> Vector( vec1Initial -> position , vec1End -> position );
	Point* v = new Point(); v -> Vector( vec2Initial -> position , vec2End -> position );
	Point* w = new Point(); w -> Vector( vec2Initial -> position , vec1Initial -> position );

	GLfloat a = u -> Dot( u -> position );
	GLfloat b = u -> Dot( v -> position );
	GLfloat c = v -> Dot( v -> position );
	GLfloat d = u -> Dot( w -> position );
	GLfloat e = v -> Dot( w -> position );
	GLfloat D = a*c - b*b;
	GLfloat sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    GLfloat tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
	//GLfloat SMALL_NUM = 0.001;

	 // compute the line parameters of the two closest points
	if (D < SMALL_NUMBER) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < SMALL_NUMBER ) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < SMALL_NUMBER ) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < SMALL_NUMBER )
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < SMALL_NUMBER )
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUMBER ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUMBER ? 0.0 : tN / tD);

    // get the difference of the two closest points
   // Vector   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
	Point* dP = new Point( w -> position[0] + sc * u -> position[0] - tc * v -> position[0] ,
						   w -> position[1] + sc * u -> position[1] - tc * v -> position[1] ,
						   w -> position[2] + sc * u -> position[2] - tc * v -> position[2] );

	//return norm(dP);   // return the closest distance
	GLfloat result = dP -> Absolution();
	delete u;
	delete v;
	delete w;
	delete dP;
	return result;
}

bool OuterCone::CheckEngaged()
{
	
	return engaged;
}

bool OuterCone::CheckIntersectOuterConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	//std::cout << "第一個點是：" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
	
	

	Point* d = new Point();

	bool egdeEngaged[3] = { false , false , false };

	bool isIntersected = false;

	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//這邊是? 找出是哪個點和哪個點的連線
	for( int i = 0 ; i < 3; i++ )
	{
		GLfloat tempSourceCorner[3];
		MathHelper::VectorMinus( tempSourceCorner , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( tempSourceCorner );
		MathHelper::VectorPlus( tempSourceCorner , sourceCorner[i] -> position );
		d -> Vector( tempSourceCorner , cornerEnd[i] ->position );
		/*
		std::cout << "這個方塊的位置是" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		std::cout << "角0的位置是" << corner[0] -> position[0] << "," << corner[0] -> position[1] << "," << corner[0] -> position[2] << std::endl;
		std::cout << "角1的位置是" << corner[1] -> position[0] << "," << corner[1] -> position[1] << "," << corner[1] -> position[2] << std::endl;
		std::cout << "角2的位置是" << corner[2] -> position[0] << "," << corner[2] -> position[1] << "," << corner[2] -> position[2] << std::endl;
		std::cout << "角3的位置是" << corner[3] -> position[0] << "," << corner[3] -> position[1] << "," << corner[3] -> position[2] << std::endl;
		std::cout << "角4的位置是" << corner[4] -> position[0] << "," << corner[4] -> position[1] << "," << corner[4] -> position[2] << std::endl;
		std::cout << "角5的位置是" << corner[5] -> position[0] << "," << corner[5] -> position[1] << "," << corner[5] -> position[2] << std::endl;
		std::cout << "角6的位置是" << corner[6] -> position[0] << "," << corner[6] -> position[1] << "," << corner[6] -> position[2] << std::endl;
		std::cout << "角7的位置是" << corner[7] -> position[0] << "," << corner[7] -> position[1] << "," << corner[7] -> position[2] << std::endl;
		*/
		Point* s1 = new Point();
		s1 -> Cross( d , e2 );

		GLfloat divisor = s1 -> Dot( e1 );
   
		if ( abs(divisor) < SMALL_NUMBER )
		{
			//不相交
			delete s1;
			continue;
		}

		// Compute first barycentric coordinate

		Point* s = new Point();
		s -> Vector( point1 , tempSourceCorner );
		GLfloat b1 = s -> Dot( s1 ) / divisor;
		if( b1 < -SMALL_NUMBER || b1 > 1.0 + SMALL_NUMBER )
		{
			//不相交
			delete s1;
			delete s;
			continue;
		}


				
		// Compute second barycentric coordinate
		Point* s2 = new Point();
		s2 -> Cross( s , e1 );
		GLfloat b2 = d -> Dot( s2 ) / divisor;
		if( b2 < -SMALL_NUMBER || b1 + b2 > 1.0 + SMALL_NUMBER )
		{
			//不相交
			delete s1;
			delete s;
			delete s2;
			continue;
		}

				
		// Compute _t_ to intersection point
		GLfloat t = e2 -> Dot( s2 ) / divisor;
		if( t < -SMALL_NUMBER || t > 1.0 + SMALL_NUMBER )
		{
			delete s1;
			delete s;
			delete s2;
			//不相交
			continue;
		}
				
		egdeEngaged[ i ] = true;
		engaged = true;
		isIntersected = true;
				
		delete s1;
		delete s;
		delete s2;
	}
	delete d;
	delete e1;
	delete e2;

	return isIntersected;

}

bool OuterCone::CheckIntersectOuterConeOnConeFace( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	//std::cout << "第一個點是：" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
	
	Point* d = new Point();

	bool egdeEngaged[3] = { false , false , false };

	bool isIntersected = false;
	//這邊要把竿子的起點網外移一點，以免竿子和自己連接的面香蕉
	GLfloat tempSourceCorner[3][3];
	MathHelper::VectorMinus( tempSourceCorner[0] , cornerEnd[0] -> position , sourceCorner[0] -> position );
	MathHelper::VectorMinus( tempSourceCorner[1] , cornerEnd[1] -> position , sourceCorner[1] -> position );
	MathHelper::VectorMinus( tempSourceCorner[2] , cornerEnd[2] -> position , sourceCorner[2] -> position );
	MathHelper::VectorNormalize( tempSourceCorner[0] );
	MathHelper::VectorNormalize( tempSourceCorner[1] );
	MathHelper::VectorNormalize( tempSourceCorner[2] );
	MathHelper::VectorPlus( tempSourceCorner[0] , sourceCorner[0] -> position );
	MathHelper::VectorPlus( tempSourceCorner[1] , sourceCorner[1] -> position );
	MathHelper::VectorPlus( tempSourceCorner[2] , sourceCorner[2] -> position );
	//到這


	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//這邊是? 找出是哪個點和哪個點的連線
	for( int i = 0 ; i < 3; i++ )
	{
		
		d -> Vector( tempSourceCorner[i] , cornerEnd[i] -> position );
		/*
		std::cout << "這個方塊的位置是" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		std::cout << "角0的位置是" << corner[0] -> position[0] << "," << corner[0] -> position[1] << "," << corner[0] -> position[2] << std::endl;
		std::cout << "角1的位置是" << corner[1] -> position[0] << "," << corner[1] -> position[1] << "," << corner[1] -> position[2] << std::endl;
		std::cout << "角2的位置是" << corner[2] -> position[0] << "," << corner[2] -> position[1] << "," << corner[2] -> position[2] << std::endl;
		std::cout << "角3的位置是" << corner[3] -> position[0] << "," << corner[3] -> position[1] << "," << corner[3] -> position[2] << std::endl;
		std::cout << "角4的位置是" << corner[4] -> position[0] << "," << corner[4] -> position[1] << "," << corner[4] -> position[2] << std::endl;
		std::cout << "角5的位置是" << corner[5] -> position[0] << "," << corner[5] -> position[1] << "," << corner[5] -> position[2] << std::endl;
		std::cout << "角6的位置是" << corner[6] -> position[0] << "," << corner[6] -> position[1] << "," << corner[6] -> position[2] << std::endl;
		std::cout << "角7的位置是" << corner[7] -> position[0] << "," << corner[7] -> position[1] << "," << corner[7] -> position[2] << std::endl;
		*/
		Point* s1 = new Point();
		s1 -> Cross( d , e2 );

		GLfloat divisor = s1 -> Dot( e1 );
   
		if ( abs(divisor) < SMALL_NUMBER )
		{
			//不相交
			delete s1;
			continue;
		}

		// Compute first barycentric coordinate

		Point* s = new Point();
		s -> Vector( point1 , tempSourceCorner[i] );
		GLfloat b1 = s -> Dot( s1 ) / divisor;
		if( b1 < -SMALL_NUMBER || b1 > 1.0 + SMALL_NUMBER )
		{
			//不相交
			delete s1;
			delete s;
			continue;
		}


				
		// Compute second barycentric coordinate
		Point* s2 = new Point();
		s2 -> Cross( s , e1 );
		GLfloat b2 = d -> Dot( s2 ) / divisor;
		if( b2 < -SMALL_NUMBER || b1 + b2 > 1.0 + SMALL_NUMBER )
		{
			//不相交
			delete s1;
			delete s;
			delete s2;
			continue;
		}

				
		// Compute _t_ to intersection point
		GLfloat t = e2 -> Dot( s2 ) / divisor;
		if( t < -SMALL_NUMBER || t > 1.0 + SMALL_NUMBER )
		{
			delete s1;
			delete s;
			delete s2;
			//不相交
			continue;
		}
				
		egdeEngaged[ i ] = true;
		isIntersected = true;
				
		delete s1;
		delete s;
		delete s2;
	}
	delete d;
	delete e1;
	delete e2;

	return isIntersected;

}

bool OuterCone::AddToLink()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = this;
			break;
		}
	}
	return true;
}

bool OuterCone::TryDifferentType( int differentType )
{
	//蟲至一遍engaged漢dublicated
	engaged = false;
	connectorEngaged[0] = false;
	connectorEngaged[1] = false;
	connectorEngaged[2] = false;

	dublicatedWithCone = false;
	dublicatedConeIndex = -1;

	dublicatedWithOuterCone = false;
	dublicatedOuterConeIndex = -1;
	cornerDublicated[0] = false;
	cornerDublicated[1] = false;
	cornerDublicated[2] = false;

	intersected = false;


	type = differentType;
	//重新建一遍竿子
	if( !SelectConstructedHole() ) 
	{
		std::cout << "這個outerCone" << this << "的第" << type << "種組法無法組上去" << std::endl;
		return false;
	
	}

	if( type != 0 ) 
	{
		CheckDublicatedCone();//當type是0的時候，裡面的constructHole[0]會出錯
		CheckIntersectWithConeFace();
		//這邊也輸出
	}


	if( this != firstOuterCone )
	{
		CheckDublicatedOuterCone();//檢查伸出來的方向有沒有和別人重複
		//和有沒有插入別的cone
		//先在這書出
	}

	if( !engaged && !dublicatedWithCone && !dublicatedWithOuterCone )
	{
		CheckIntersectWithConnector();//檢查有沒有交錯
		//在在這書出
		
	}
	
	return true;
}

bool OuterCone::RefreshDistanceToSurface()
{
	for( int i = 0; i < TYPE_AMOUNT; i ++ ) 
	{
		distanceToSurface[ i ] = -1.0;
		weightedDistance[ i ] = -1.0;
	}
	type = 1;

	SelectConstructedHole();
	CheckDublicatedCone();

	return true;
}