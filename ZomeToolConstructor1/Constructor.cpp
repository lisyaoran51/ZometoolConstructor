#include "Constructor.h"



Constructor::Constructor()//0520把所有的長度都加1mm//0526把黃半徑+0.1mm，紅半徑+0.25mm
{
	model = new Model[10];
	model[0] = Model("ball1122Std.obj");
	model[1] = Model("BlueS0523.obj");//長邊5.5mm 短邊4.5mm 
	model[2] = Model("BlueM0523.obj");//卡準長邊3.5mm 短邊2.5mm
	model[3] = Model("BlueL0523.obj");//1208改成長邊6短邊4
	model[4] = Model("RedS0526.obj");//原本的
	model[5] = Model("RedM0526.obj");//0520把原本的外圍半徑減短0.5mm//0525發現洞太小
	model[6] = Model("RedL0526.obj");
	model[7] = Model("YellowS0526.obj");//改成??1206
	model[8] = Model("YellowM0526.obj");
	model[9] = Model("YellowL0526.obj");//456鴻789黃
	//memcpy( model , tempModel , sizeof( Model ) * 4 );

	zomeDirection = new ZomeDirection();

	GLfloat sca[3] = {1.0f,1.0f,1.0f};
	GLfloat tra[3] = {0.0f,0.0f,0.0f};
	GLfloat rot[3] = { 0.0f , 0.0f , 0.0f };

	originalBall = new ZomeStructure();
	originalBall->position = new GLfloat[3];
	MathHelper::SetVector( originalBall->position , tra[0] , tra[1] , tra[2] );
	originalBall->index = 0;
	originalBall->originalBall = originalBall;

	material = new std::map<int,int>();
	matIndex2Index = new std::map<int,int>();
	//force = new std::map<int,float>();//11/7寫到這邊
	//matForce = new std::map<int,float>();

	totalForceDown = 0;
	for(int i=0;i<3;i++)gravityPosition[i] = 0;
	originalPolarCoor = NULL;

	originalModel = new ModelObject( &model[0] , sca , tra , rot , 0 , 0 );

	tempModelLink = originalModel;
	tempBallLink = originalBall;

	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			connectorAmount[i][j] = 0;
	ballAmount = 0;
	//第零面加入白色
	material -> insert( std::pair<int, int>( 0 , COLOR_WHITE ) );
	matIndex2Index -> insert( std::pair<int, int>( 0 , -1 ) );
}

Constructor::Constructor( GLfloat* translate )//0520把所有的長度都加1mm
{//0522把紅 黃縮小 藍轉900度
	model = new Model[10];
	model[0] = Model("ball1122Std.obj");
	model[1] = Model("BlueS0523.obj");//長邊5.5mm 短邊4.5mm 
	model[2] = Model("BlueM0523.obj");//卡準長邊3.5mm 短邊2.5mm
	model[3] = Model("BlueL0523.obj");//1208改成長邊6短邊4
	model[4] = Model("RedS0526.obj");//原本的
	model[5] = Model("RedM0526.obj");//0520把原本的外圍半徑減短0.5mm
	model[6] = Model("RedL0526.obj");
	model[7] = Model("YellowS0526.obj");//改成??1206
	model[8] = Model("YellowM0526.obj");
	model[9] = Model("YellowL0526.obj");//456鴻789黃
	//memcpy( model , tempModel , sizeof( Model ) * 4 );

	zomeDirection = new ZomeDirection();

	GLfloat sca[3] = {1.0f,1.0f,1.0f};
	GLfloat tra[3] = {0.0f,0.0f,0.0f};
	GLfloat rot[3] = { 0.0f , 0.0f , 0.0f };

	MathHelper::CopyVector( tra , translate );

	originalBall = new ZomeStructure();
	originalBall->position = new GLfloat[3];
	MathHelper::SetVector( originalBall->position , tra[0] , tra[1] , tra[2] );
	originalBall->index = 0;
	originalBall->originalBall = originalBall;

	material = new std::map<int,int>();
	matIndex2Index = new std::map<int,int>();
	//force = new std::map<int,float>();//11/7寫到這邊
	//matForce = new std::map<int,float>();
	
	totalForceDown = 0;
	for(int i=0;i<3;i++)gravityPosition[i] = 0;
	originalPolarCoor = NULL;

	originalModel = new ModelObject( &model[0] , sca , tra , rot , 0 , 0 );

	tempModelLink = originalModel;
	tempBallLink = originalBall;
	//第零面加入白色
	material -> insert( std::pair<int, int>( 0 , COLOR_WHITE ) );
	matIndex2Index -> insert( std::pair<int, int>( 0 , -1 ) );
}

Constructor::~Constructor()
{
	delete[] model;
	delete zomeDirection;
	
	for( ZomeStructure* b = originalBall; b != NULL; )
	{
		ZomeStructure* temp = b -> link;
		delete b;
		b = temp;
	}

	for( ZomeConnection* c = originalConn; c != NULL; )
	{
		ZomeConnection* temp = c -> link;
		delete c;
		c = temp;
	}

	for( ModelObject* m = originalModel; m != NULL; )
	{
		ModelObject* temp = m -> link;
		delete m;
		m = temp;
	}
}

int Constructor::NewBall( GLfloat* translate )
{
	ballAmount += 1;
	//確認球的pointIndex沒有出現過
	/*
	for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
	{
		if ( i -> index == pointIndex )
		{
			return -1;
		}
	}
	*/

	//設定新的球的資料 包括未至 第一顆球鏈結
	ZomeStructure* newBall = new ZomeStructure();
	//newBall -> position = new GLfloat[3];
	MathHelper::SetVector( newBall -> position , translate[0] , translate[1] , translate[2] );
	//newBall -> index = pointIndex;
	newBall -> originalBall = originalBall;

	//把新的球和上一個鍵的球連起來
	ZomeStructure* lastBall = NULL;
	newBall -> AddToLink( originalBall );
	/*for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL ) 
		{
			lastBall = i;
		}
	}
	lastBall -> link = newBall;
	newBall -> index = lastBall -> index + 1;*///設定球的index
	//加入這一顆球的顏色
	material -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , COLOR_WHITE ) );
	matIndex2Index -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , -1 ) );
	//建newBall的模型
	//tempConn -> towardIndex = tempBall -> index;
	//MathHelper::SetVector( tempBall -> position , sourceBall -> position[0] + translate[0] , 
	//												sourceBall -> position[1] + translate[1] , 
	//												sourceBall -> position[2] + translate[2] );
	GLfloat rotation[] = {0,0,0};
	GLfloat scale[] = {1.0f,1.0f,1.0f};
	newBall -> thisModel = new ModelObject( &model[ 0 ] , 
											scale , 
											newBall -> position , 
											rotation ,
											tempModelLink -> GetNextVertexOffset() ,
											tempModelLink -> GetNextIndexOffset() );
	//std::cout << "第" << tempBall -> index << "號球的點有" << tempBall -> thisModel -> verticeSize / 3 << "個，面有" 
	//<<  tempBall -> thisModel -> indiceSize / 3 << "個" << std::endl;
	tempModelLink -> link = newBall -> thisModel;//把所有求用鏈結串列連起來
	tempModelLink = newBall -> thisModel;		  //把所有求用鏈結串列連起來

	std::cout << "重新在新的位置建立第" << newBall -> index << "號球。" << std::endl;
	int returnIndex = newBall -> index;
	return returnIndex;

}

int Constructor::Construct( int pointIndex , int faceIndex , int size , bool type )
{
	//記數
	connectorAmount[ zomeDirection -> GetFaceColor( faceIndex ) ][ size ] += 1;
	if( type == TYPE_PIN ) ballAmount ++;

	//找到要新增的球
	ZomeStructure *sourceBall = NULL , *tempBall = NULL , *lastBall = NULL; 
	for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
	{
		if ( i -> index == pointIndex )
		{
			sourceBall = i;
			break;
		}
	}
	//如果沒有找到source ball 代表pointIndex輸入錯了
	lastBall = originalBall;
	if ( type == TYPE_PIN )
	{
		tempBall = new ZomeStructure();
		originalBall = tempBall -> AddToLink( originalBall );
		/*for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
		{
			if ( i -> link == NULL ) 
			{
				lastBall = i;
			}
		}
		tempBall -> index = lastBall -> index + 1;//設定球的index
		lastBall -> link = tempBall;//把最後一顆球和這顆球連起來*/
		sourceBall -> nextBall[ faceIndex ] = tempBall;//把藥新增的球和這顆球連起來
		int oppositeIndex = zomeDirection -> GetOppositeFace( faceIndex );
		tempBall -> nextBall[ oppositeIndex ] = sourceBall;//新的球的來源面連到舊的球
	}
	

	//找到面前建立的最新的桿子
	ZomeConnection *lastConn = NULL , *tempConn = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();//??
	for ( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastConn = i;
		}
	}
	tempConn = new ZomeConnection();
	tempConn -> color = zomeDirection -> GetFaceColor ( faceIndex );//設定顏色和長度
	tempConn -> size = size;
	sourceBall -> faceConnection[ faceIndex ] = tempConn;//在現在這顆球上的那個面 連接現在要建的這個竿子
	tempConn -> fromIndex = sourceBall -> index;
	tempConn -> fromFace = faceIndex;
	if( type == TYPE_PIN )//如果有目標球，就把目標球編號輸入近去
	{
		tempConn -> towardIndex = tempBall -> index;
		tempConn -> towardFace = zomeDirection -> GetOppositeFace( faceIndex );
		tempBall -> faceConnection[ tempConn -> towardFace ] = tempConn;//終於找到錯這在 0105
	}
	else//如果沒有目標球，就從所有球中找出對硬的球，把那顆球的編號輸入近去
	{
		GLfloat towardPosition[3];
		GLfloat length = zomeDirection -> GetFaceLength ( faceIndex , size );
		MathHelper::SetVector( towardPosition , zomeDirection->faceDirection[faceIndex*3] * length , 
											    zomeDirection->faceDirection[faceIndex*3+1] * length ,
											    zomeDirection->faceDirection[faceIndex*3+2] * length );
		MathHelper::VectorPlus( towardPosition , sourceBall -> position );
		for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
		{
			if ( MathHelper::VectorApproximation( towardPosition , i -> position ) )
			{
				tempConn -> towardIndex = i -> index;
				tempConn -> towardFace = zomeDirection -> GetOppositeFace( faceIndex );
				int oppositeIndex = zomeDirection -> GetOppositeFace( faceIndex );
				i -> faceConnection[ oppositeIndex ] = tempConn;//找到連接的球然後把編號打進去
				sourceBall -> nextBall[ oppositeIndex ] = i;//把連接到的球的編號書進去被連接的球的編號
				break;
			}
		}
	}
	//如果之前沒有建過竿子，就把現在的當第一個
	//寫個AddToLink

	originalConn = tempConn -> AddToLink( originalConn );
	/*
	if ( lastConn == NULL )
	{
		tempConn -> index = 0;
		originalConn = tempConn;
	}
	else//有建過就當下一個
	{
		lastConn -> link = tempConn;
		tempConn -> index = lastConn -> index + 1;
	}*/
	
	//========================================算竿子的位子
	GLfloat* translate = new GLfloat[3];
	if( faceIndex == -1 )
	{
		std::cout << "出錯" << std::endl;
	}
	GLfloat length = zomeDirection -> GetFaceLength ( faceIndex , size );
	MathHelper::SetVector( translate , zomeDirection->faceDirection[faceIndex*3] * length , 
									   zomeDirection->faceDirection[faceIndex*3+1] * length ,
									   zomeDirection->faceDirection[faceIndex*3+2] * length );

	MathHelper::SetVector( tempConn -> position , sourceBall -> position[0] + translate[0] / 2 , 
												  sourceBall -> position[1] + translate[1] / 2 , 
												  sourceBall -> position[2] + translate[2] / 2 );

	MathHelper::SetVector( tempConn -> rotation , zomeDirection -> Theta( faceIndex ) , 
												  zomeDirection -> Phi( faceIndex ) , 
												  zomeDirection -> Yaw( faceIndex ) );

	MathHelper::SetVector( tempConn -> endPosition , sourceBall -> position[0] + translate[0] , 
													 sourceBall -> position[1] + translate[1] , 
													 sourceBall -> position[2] + translate[2] );
	//========================================鍵竿子的模型
	//加入這一個竿子的顏色
	material -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , zomeDirection -> GetFaceColor ( faceIndex ) ) );
	//force -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 ,  );
	matIndex2Index -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , tempConn -> index ) );//紀錄位置

	int modelNumber = tempConn -> GetModelNumber();
	GLfloat scale[] = {1.0f,1.0f,1.0f};
	tempConn -> thisModel = new ModelObject( &model[ modelNumber ] , 
											 scale , 
											 tempConn -> position , 
											 tempConn -> rotation , 
											 tempModelLink -> GetNextVertexOffset() ,
											 tempModelLink -> GetNextIndexOffset() );
	//std::cout << "第" << tempConn -> index << "號竿子的點有" << tempConn -> thisModel -> verticeSize / 3 << "個，面有" 
	//	<<  tempConn -> thisModel -> indiceSize / 3 << "個" << std::endl;
	tempModelLink -> link = tempConn -> thisModel;//把所有竿子用鏈結串列連起來
	tempModelLink = tempConn -> thisModel;        //把所有竿子用鏈結串列連起來
	//========================================鍵球的模型
	if ( type == TYPE_PIN )
	{
		//加入這一顆球的顏色
		material -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , COLOR_WHITE ) );
		matIndex2Index -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , -1 ) );
	
		tempConn -> towardIndex = tempBall -> index;
		MathHelper::SetVector( tempBall -> position , sourceBall -> position[0] + translate[0] , 
													  sourceBall -> position[1] + translate[1] , 
													  sourceBall -> position[2] + translate[2] );

		GLfloat rotation[] = {0,0,0};
		tempBall -> thisModel = new ModelObject( &model[ 0 ] , 
												 scale , 
												 tempBall -> position , 
												 rotation ,
												 tempModelLink -> GetNextVertexOffset() ,
												 tempModelLink -> GetNextIndexOffset() );
		//std::cout << "第" << tempBall -> index << "號球的點有" << tempBall -> thisModel -> verticeSize / 3 << "個，面有" 
		//<<  tempBall -> thisModel -> indiceSize / 3 << "個" << std::endl;
		tempModelLink -> link = tempBall -> thisModel;//把所有求用鏈結串列連起來
		tempModelLink = tempBall -> thisModel;		  //把所有求用鏈結串列連起來
	}
	
	//把與這顆球相連的桿子都加入這個球的編號
	if ( type == TYPE_PIN )
	{
		for( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
		{
			if( MathHelper::VectorApproximation( i -> endPosition , tempBall -> position ) )
			{
				i -> towardIndex = tempBall -> index;
				i -> towardFace = zomeDirection -> GetOppositeFace( i -> fromFace );
			}
		}
	}

	
	int returnIndex;//回傳新的球的編號
	if ( type == TYPE_PIN )
	{
		std::cout << "從第" << sourceBall -> index << "號球的第" << faceIndex << "面建立桿子第" << tempConn -> index <<
			"號，連到第" << tempBall -> index << "號球。" << std::endl;
		returnIndex = tempBall -> index;
	}
	else
	{
		std::cout << "從第" << sourceBall -> index << "號球的第" << faceIndex << "面建立桿子第" << tempConn -> index <<
			"號。" << std::endl;
		returnIndex = sourceBall -> index;
	}

	return returnIndex;
}

bool Constructor::ContainConn( int pointIndex , int faceIndex )
{
	for ( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
	{
		if( i -> fromIndex == pointIndex && i -> fromFace == faceIndex )
		{
			return true;
		}
		if( i -> towardIndex == pointIndex && i -> towardFace == faceIndex )
		{
			return true;
		}
	}
	return false;
}

ZomeConnection* Constructor::GetConn( int index )
{
	for ( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
	{
		if( i -> index == index )
		{
			return i;
		}
	}
	return NULL;
}

void Constructor::PrintTotalComponents()
{
	std::cout << "=====================以下是總共花費的零件=====================" <<std::endl;
	std::cout << "共使用了球 " << ballAmount << " 顆" << std::endl;
	std::cout << "--------------------" << std::endl;
	
	std::cout << std::setw(5) << "顏色" << std::setw(5) << "S" << std::setw(5) << "M" << std::setw(5) << "L" << std::endl;
	std::cout << std::setw(5) << "藍色" << std::setw(5) << connectorAmount[0][0] << std::setw(5) << connectorAmount[0][1] << std::setw(5) << connectorAmount[0][2] << std::endl;
	std::cout << std::setw(5) << "紅色" << std::setw(5) << connectorAmount[1][0] << std::setw(5) << connectorAmount[1][1] << std::setw(5) << connectorAmount[1][2] << std::endl;
	std::cout << std::setw(5) << "黃色" << std::setw(5) << connectorAmount[2][0] << std::setw(5) << connectorAmount[2][1] << std::setw(5) << connectorAmount[2][2] << std::endl;
	std::cout << "--------------------" << std::endl;


	FILE* Record = fopen("Components.txt", "w");
	
	fprintf(Record, "=====================以下是總共花費的零件=====================\n" );
	fprintf(Record, "共使用了球 %d 顆\n" , ballAmount );

	fprintf(Record, "--------------------\n" );

	fprintf(Record, " 顏色    S    M    L\n" );
	fprintf(Record, " 藍色%5d%5d%5d\n" , connectorAmount[0][0] , connectorAmount[0][1] , connectorAmount[0][2] );
	fprintf(Record, " 紅色%5d%5d%5d\n" , connectorAmount[1][0] , connectorAmount[1][1] , connectorAmount[1][2] );
	fprintf(Record, " 黃色%5d%5d%5d\n" , connectorAmount[2][0] , connectorAmount[2][1] , connectorAmount[2][2] );

	fprintf(Record, "--------------------\n" );

	fclose(Record);
}

void Constructor::StructureAnalyze()
{
	//FindLowestHeight();
	std::vector< int >* Bottom; 
	Bottom = new std::vector< int >();
	float lowestEndHeight = 1000.0;
	for( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
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
	for( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex == -1 )
		{
			if( ( i -> endPosition )[1] < lowestEndHeight + BOTTOM_INTERVAL && i -> towardIndex == -1 )
			{
				Bottom -> push_back( i -> index );
				std::cout << "第" << i -> index << "根竿子末端為接地，座標為(" 
						  <<  ( i -> endPosition )[0] << "," 
						  <<  ( i -> endPosition )[1] << "," 
						  <<  ( i -> endPosition )[2] << ")" << std::endl;
				bool debug = 0;
			}
		}
	}

	//建立連接表面的點的量
	int virtualBallCount = -1;
	std::vector< VirtualBall* > virtualBalls;
	std::vector< int > errorBalls;//用來擺出錯的球
	//FindConnectors( Bottom , &virtualBalls );
	for( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
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
					std::cout << "多出來的桿子編號是：" << i -> index << "，面是：" << zomeDirection -> GetOppositeFace( i -> fromFace ) << std::endl;
					errorBalls.push_back( i -> index );
					tempVirtualBall -> connectionAmount--;
					std::cout << "虛擬球座標：(" << tempVirtualBall -> position[0] << "," 
												 << tempVirtualBall -> position[1] << "," 
												 << tempVirtualBall -> position[2] << ")" << std::endl;
					continue;
				} 
				int towardFace = zomeDirection -> GetOppositeFace( i -> fromFace );
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
			VirtualBall* tempVirtualBall = new VirtualBall( 0 - virtualBalls.size() - 1 , i );//沒有這個點的話就新增一個點
			int towardFace = zomeDirection -> GetOppositeFace( i -> fromFace );
			tempVirtualBall -> connectFace[ 0 ] = towardFace;
			if( i -> endPosition[1] < lowestEndHeight + BOTTOM_INTERVAL )
			{
				tempVirtualBall -> isBottom = true;
				if( originalPolarCoor == NULL )
				{
					originalPolarCoor = new PolarCoor( tempVirtualBall , originalPolarCoor );
				}
				else
				{
					PolarCoor* tempPolarCoor = new PolarCoor( tempVirtualBall , originalPolarCoor );
				}
			}
			virtualBalls.push_back( tempVirtualBall );
			std::cout << "新增第" << virtualBalls.size() << "個虛擬球(" << tempVirtualBall -> position[0] << ","
					  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")，加入第" 
					  << i -> index << "號竿子，連接面是" << towardFace << "，目前共" 
					  << tempVirtualBall -> connectionAmount << "根竿子" << std::endl;
			bool debug = 0;
		}
	}



	int debugCount = 0;//有一個式子出錯，用這個來debug
	//ListPolynomial();
	//開始列多項式，對每一科求建立一個三個式子，每個點分別網x、y、z各建一個式子
	for( int d = 0; d < 3; d++ )//三個維度xyz
	{
		//對每顆球獵一個算式
		for( ZomeStructure* b = originalBall; b != NULL; b = b -> link )
		{
			//debug
			if( b -> index == 69 )
			{
				std::cout << "debug:找第69球，座標：(" << b -> position[0] << "," << b -> position[1] << "," << b -> position[2] << std::endl;
			}

			//建立新多項式
			PolynomialList* tempPolynomial = new PolynomialList( b );
			debugCount++;
			//將多項式加入項
			for( int i = 0; i < 62; i++ )
			{
				if( b -> faceConnection[i] != NULL )
				{
					std::cout << "這根桿子的方向是第" << i << "面，我們取他第" << d << "個維度" << std::endl;
					float parameter = zomeDirection -> GetDirectionParameter( i , d );//拿第i面的第d個維度的系數
					int connIndex = b -> faceConnection[i] -> index;//取這個面式接到哪一根棒子



					if( abs(parameter) > 0.01 )
					{
						
						//debug
						if( connIndex == 1 )
						{
							std::cout << "debug:找到竿子在(";
						}

						std::pair< int , float > tempPair( connIndex , parameter );
						tempPolynomial -> polynomial.insert( tempPair );
						std::cout << "來自第" << b -> index << "顆球的第" << debugCount << "個式子的第" << tempPolynomial -> polynomial.size() 
								  << "個項為： #(" << connIndex << ")竿受力 X " << parameter << "---(" 
								  << b -> faceConnection[i] -> position[0] << "," << b -> faceConnection[i] -> position[1]
								  << "," << b -> faceConnection[i] -> position[2] << ")" << std::endl;
						bool debug = 0;
					}
					if( d == 1 )//加上這跟棒子本身的重量/2向下，加入常數項
					{
						float constant = tempPolynomial -> polynomial.at( -1 );
						constant -= zomeDirection -> GetWeight( b -> faceConnection[i] -> color , b -> faceConnection[i] -> size ) * 0.5f;
						tempPolynomial -> polynomial.find( -1 ) -> second = constant;
						
						totalForceDown -= zomeDirection -> GetWeight( b -> faceConnection[i] -> color , 
																	  b -> faceConnection[i] -> size ) * 0.5f;//家上項下力
						gravityPosition[0] -= b -> position[0] *
											  zomeDirection -> GetWeight( b -> faceConnection[i] -> color , 
																		  b -> faceConnection[i] -> size ) * 0.5f;//算重心用
						gravityPosition[2] -= b -> position[2] *
											  zomeDirection -> GetWeight( b -> faceConnection[i] -> color , 
																		  b -> faceConnection[i] -> size ) * 0.5f;//算重心用
					}
				}
			}
			//加上球本身的重量向下
			if( d == 1 )
			{
				float constant = tempPolynomial -> polynomial.at( -1 );
				constant -= zomeDirection -> GetWeight( 3 , 0 );
				tempPolynomial -> polynomial.find( -1 ) -> second = constant;

				totalForceDown -= zomeDirection -> GetWeight( 3 , 0 );//家總像下力
				gravityPosition[0] -= b -> position[0] * zomeDirection -> GetWeight( 3 , 0 );//算重心用
				gravityPosition[2] -= b -> position[2] * zomeDirection -> GetWeight( 3 , 0 );//算重心用
			}

			//，把多項式加入串列，如果這個多項式理面沒有東西就不要加
			firstPolynomial = tempPolynomial -> AddToLink( firstPolynomial );
			/*if( firstPolynomial == NULL )
			{
				firstPolynomial = tempPolynomial;
				firstPolynomial -> firstPolynomial = tempPolynomial;

				firstPolynomial -> solvedConnections = new std::vector<int>();
			}
			else
			{
				
				for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
				{
					if( p -> link == NULL )
					{
						tempPolynomial -> firstPolynomial = firstPolynomial;
						p -> link = tempPolynomial;
						break;
					}
				}
				tempPolynomial -> solvedConnections = firstPolynomial -> solvedConnections;

			}*/
		}
		//這裡放與表面連接的點的多項式
		int bottomForceCount = -2;//儲存底部力
		for(int i = 0; i < virtualBalls.size(); i++ )
		{
			
			VirtualBall* tempVirtualBall = virtualBalls.at(i);
			
			if( tempVirtualBall -> connectionAmount != 2 )//continue;//如果沒有三個支架支撐，就直接跳過，因為無法用這個算式算出結果
			{
				if( tempVirtualBall -> connectionAmount == 0 )//don't jump. save a result
				{
					errorBalls.push_back( tempVirtualBall -> connIndex[0] );
					std::cout << "ErrorBall:這顆球#" << tempVirtualBall -> index << "(" <<
						tempVirtualBall -> position[0] << "," << 
						tempVirtualBall -> position[1] << "," << 
						tempVirtualBall -> position[2] << ")只有一根竿子連接。" << std::endl;
				}
				if( tempVirtualBall -> connectionAmount == 1 )
				{
					errorBalls.push_back( tempVirtualBall -> connIndex[0] );
					errorBalls.push_back( tempVirtualBall -> connIndex[1] );
					std::cout << "ErrorBall:這顆球#" << tempVirtualBall -> index << "(" <<
						tempVirtualBall -> position[0] << "," << 
						tempVirtualBall -> position[1] << "," << 
						tempVirtualBall -> position[2] << ")只有二根竿子連接。" << std::endl;
				}
				continue;
			}
			//建立新多項式
			PolynomialList* tempPolynomial = new PolynomialList( tempVirtualBall );
			debugCount++;
			//==============================================
			if( tempVirtualBall -> isBottom && d == 1 ) //如果是底部，y方向就直接被反作用力抵銷，所以不用計算
			{
				std::pair< int , float > tempPair( bottomForceCount , 1.0 );
				tempPolynomial -> polynomial.insert( tempPair );
				tempVirtualBall -> bottomForceIndex = bottomForceCount;
				std::cout << "在第" << tempVirtualBall -> index << "顆球上加上地面給的反力#(" << bottomForceCount << ")" << std::endl;
				bottomForceCount--;
			}
			//==============================================
			for( int j = 0; j <= tempVirtualBall -> connectionAmount; j++ )
			{
				float parameter = zomeDirection -> GetDirectionParameter( tempVirtualBall -> connectFace[j] , d );//拿第i面的第d個維度的系數
				int connIndex = tempVirtualBall -> connIndex[j];//取這個面式接到哪一根棒子
				if( abs(parameter) > 0.01 )
				{
					std::pair< int , float > tempPair( connIndex , parameter );
					tempPolynomial -> polynomial.insert( tempPair );
				}
				if( d == 1 )//加上這跟棒子本身的重量/2向下，加入常數項
				{
					float constant = tempPolynomial -> polynomial.at( -1 );
					constant -= zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
															tempVirtualBall -> conn[j] -> size ) * 0.5f;
					//std::pair< int , float > tempPair( -1 , constant );
					//tempPolynomial -> polynomial.insert( tempPair );
					tempPolynomial -> polynomial.find( -1 ) -> second = constant;

					totalForceDown -= zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
																  tempVirtualBall -> conn[j] -> size ) * 0.5f;//家上項下力
					gravityPosition[0] -= tempVirtualBall -> position[0] *
										  zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
																	  tempVirtualBall -> conn[j] -> size ) * 0.5f;//算重心用
					gravityPosition[2] -= tempVirtualBall -> position[2] *
										  zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
																	  tempVirtualBall -> conn[j] -> size ) * 0.5f;///算重心用


					std::cout << "找出一個底座支撐，位置在(" << tempVirtualBall -> position[0] << ","
							  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")" << std::endl;
				}
			}
			if( d == 1 )//把碎片的重量加上去
			{
				float constant = tempPolynomial -> polynomial.at( -1 );
				constant -= SEGMENT_WEIGHT;
				//std::pair< int , float > tempPair( -1 , constant );
				//tempPolynomial -> polynomial.insert( tempPair );
				tempPolynomial -> polynomial.find( -1 ) -> second = constant;

				totalForceDown -= SEGMENT_WEIGHT;
				gravityPosition[0] -= tempVirtualBall -> position[0] * SEGMENT_WEIGHT;//算重心用
				gravityPosition[2] -= tempVirtualBall -> position[2] * SEGMENT_WEIGHT;
			}
			
			//，把多項式加入串列，如果這個多項式理面沒有東西就不要加
			firstPolynomial = tempPolynomial -> AddToLink( firstPolynomial );
			/*
			if( firstPolynomial == NULL )
			{
				firstPolynomial = tempPolynomial;
				firstPolynomial -> firstPolynomial = tempPolynomial;
			}
			else
			{
				for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
				{
					if( p -> link == NULL )
					{
						tempPolynomial -> firstPolynomial = firstPolynomial;
						p -> link = tempPolynomial;
						break;
					}
				}
			}
			*/
		}
	}

	//整段不能用
	/*
	//AnalyzeBottomForce();
	//建立底座反力的極座標，並解出反力。
	gravityPosition[0] /= totalForceDown;
	gravityPosition[2] /= totalForceDown;
	std::cout << "分析底部受力：重心座標在(" << gravityPosition[0] << "," << gravityPosition[2] << ")" << std::endl;
	//將每一個底不做標轉為極座標
	//originalPolarCoor -> ComputePolarPosition( gravityPosition );
	originalPolarCoor = originalPolarCoor -> Preprocess_OriginalPolorCoor( gravityPosition );
	//建立每一個反力的解
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		PolynomialList* tempPolynomial = new PolynomialList();
		debugCount++;
		tempPolynomial -> polynomial.find( -1 ) -> second = totalForceDown * p -> ForceRate();//將解算成總重/每個底座的比率
		//將底座反力的編號加入多項是 -X * 1 = 反力
		std::pair< int , float > tempPair( p -> virtualBall -> bottomForceIndex , 1.0 );
		tempPolynomial -> polynomial.insert( tempPair );
		std::cout << "第" << p -> virtualBall -> bottomForceIndex << "個反力的比率為：" << p -> ForceRate() << std::endl;
		//加入多項式中
		tempPolynomial -> AddToLink( firstPolynomial );*/
		/*for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
		{
			if( p -> link == NULL )
			{
				tempPolynomial -> firstPolynomial = firstPolynomial;
				p -> link = tempPolynomial;
				break;
			}
		}*/ 
	/*123
	}*/ 

	//debug
	int polynomialDebugCount = 0;
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		polynomialDebugCount++;
	}



	//這邊開始寫VirtualFroceAnalysis的建構子
	int ballAmount = 0;
	for( ZomeStructure* b = originalBall; b != NULL; b = b -> link ) ballAmount++;
	ballAmount += virtualBalls.size();
	VirtualForceAnalyzer* virtualForceAnalyzer = new VirtualForceAnalyzer( firstPolynomial , ballAmount , originalConn , originalBall );



	//把出錯的那幾根桿子的姐都設成0
	for( int e = 0; e < errorBalls.size(); e++ )
	{
		PolynomialList* tempPolynomial = new PolynomialList();
		debugCount++;
		tempPolynomial -> polynomial.insert( std::pair< int , float >( errorBalls[e] , 1.0 ) );
		for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
		{
			if( p -> link == NULL )
			{
				tempPolynomial -> firstPolynomial = firstPolynomial;
				p -> link = tempPolynomial;
				break;
			}
		}
	}
	
	//輸出沒有算過的連利方程式
	FILE* formulaFile = fopen("Structure Formula Original.txt", "w");//輸出整個算式到文件檔裡
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		fprintf(formulaFile, "第%d式：", p -> index );
		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
		{
			if( i -> first == -1 )
			{
				fprintf(formulaFile, "%f", i -> second );//輸出常數項
			}
			else
			{
				fprintf(formulaFile, "%f × (%d)", i -> second , i -> first );//輸出第i根的係數
			}
			if ( ++i != p -> polynomial.end() )//看她是不是最後一項
			{
				fprintf(formulaFile, " ＋ " );
			}
			--i;//把指標變回去
		}
		fprintf(formulaFile," ＝ 0\n");
		//p -> index = polynomialCount;
		//polynomialCount++;
		p -> PrintPolynomialList();
	}
	fclose( formulaFile );

	//PrintPolynomialList();
	//int polynomialCount = 0;
	//debug
	/*
	FILE* formulaFile = fopen("StructureFormula.txt", "w");
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		fprintf(formulaFile, "%d,", p -> index );
		std::map< int , float >::iterator firstTerm = p -> polynomial.begin();
		firstTerm++;
		if( firstTerm -> first > 30 ) fprintf(formulaFile,",");
		if( firstTerm -> first > 60 ) fprintf(formulaFile,",");
		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
		{
			if( i -> first < 0 ) continue;
			fprintf(formulaFile, "%d ", i -> first );
			int tempParam = i -> first;
			i++;
			if( i == p -> polynomial.end() )
			{
				i--;
				continue;
			}
			if( ( tempParam < 31 && i -> first > 30 ) || ( tempParam < 61 && i -> first > 60 ) )
			{
				fprintf(formulaFile,",");
			}
			i--;
		}
		fprintf(formulaFile," \n");
		//p -> index = polynomialCount;
		//polynomialCount++;
		p -> PrintPolynomialList();
	}
	fclose( formulaFile );
	*/
	
	//SolvePolynomial();
	firstPolynomial -> SolvePolynomialList();
	/*
	bool solutionDone = false;
	bool noMoreSolution = false;
	int solveTimes = 0;
	std::vector< int > solvedUnknown;
	while( !solutionDone && !noMoreSolution )
	{
		noMoreSolution = true;
		std::cout << "第" << solveTimes << "次解方程式" << std::endl;
		for( PolynomialList* p1 = firstPolynomial; p1 != NULL; p1 = p1 -> link )
		{
			for( PolynomialList* p2 = firstPolynomial; p2 != NULL; p2 = p2 -> link )
			{
				if( p1 == p2 ) continue;
				if( p2 -> polynomial.size() == 1 ) continue;
				if( p2 -> IsOneUnknown() && //紙有一個未知數
					std::find( solvedUnknown.begin() , solvedUnknown.end() , p2 -> index ) == solvedUnknown.end() )
				{
					p2 -> Substitute();//如果只剩一項，就把這個解代入所有式子中
					solvedUnknown.push_back( p2 -> index );
					noMoreSolution = false;
					continue;
				}
				////用p2去消去p1，所以p2的所有項p1都要有，p1包含p2
				if( p1 -> Include( p2 ) )
				{//包含者 -> Include( 被包含者 )
					p2 -> Eliminate( p1 );//p2去消掉p1的項
					//消去者 -> Eliminate( 被消去者 )
					noMoreSolution = false;
				}
				//這段有錯，不可以紙消去重疊的項，要每一項都削掉
				/*int overlappedTerm = p1 -> Overlap( p2 );//兩個是子有項重疊，就把那個項消掉
				if( overlappedTerm != -1 )
				{
					p2 -> Eliminate( p1 , overlappedTerm );
					noMoreSolution = false;
				}*//*
				//2/15姐聯所未知數的聯立方程式
				//確認是布是有一個未知數相同，一個未知數不相同
				if( p1 -> OneSameUnknownAmongTwo( p2 ) )
				{
					if( p1 -> index == 20 && p2 -> index == 49 )
					{
						std::cout << "debug:尋找20和49式" << std::endl;
					}
					//要先檢查這個式子有沒有消剪過
					bool ifMinimalist = true;
					for( PolynomialList* p3 = firstPolynomial; p3 != NULL; p3 = p3 -> link )
					{
						if( p3 -> polynomial.size() != 3 ) continue;//只找長度一樣式3的多項式
						if( p1 -> Include( p3 ) && p3 -> Include( p1 ) && p3 != p1 )
						{//包含者 -> Include( 被包含者 )
							ifMinimalist = false;
						}
					}
					if( ifMinimalist ) //===============================接下來才開始尋找連續
					{
						std::vector< PolynomialList* >* chainPolynomial = new std::vector< PolynomialList* >();
						if( p1 -> FindChainPolynomial( chainPolynomial , p2 ) )
						{
							p1 -> ChainSubstitute( chainPolynomial , p2 );

							p2 -> Eliminate( p1 );//p2去消掉p1的項
							//消去者 -> Eliminate( 被消去者 )
							noMoreSolution = false;
						}
						delete chainPolynomial;
					}
				}
				if( p1 -> IsOneUnknown() && //紙有一個未知數
					std::find( solvedUnknown.begin() , solvedUnknown.end() , p1 -> index ) == solvedUnknown.end() )//這個式子還沒代入過
				{
					p1 -> Substitute();//如果只剩一項，就把這個解代入所有式子中
					solvedUnknown.push_back( p1 -> index );
					noMoreSolution = false;
				}
			}
		}

		for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
		{
			if( !p -> IsOneUnknown() && !p -> IsEmpty() )//如果有任何一個式子還沒解完，就繼續while迴圈
			{
				break;
			}
			if( p -> link == NULL )//每個式子都解完了就結束while迴圈
			{
				solutionDone = true;
			}
		}
		solveTimes++;
	}*/
	//std::cout << "==============================================" << std::endl;
	//std::cout << "==============================================" << std::endl;
	//std::cout << "===================輸出多項式結果====================" << std::endl;
	//███████████████████████████████████████
	formulaFile = fopen("StructureFormula.txt", "w");//輸出整個算式到文件檔裡
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		fprintf(formulaFile, "第%d式：", p -> index );
		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
		{
			if( i -> first == -1 )
			{
				fprintf(formulaFile, "%f", i -> second );//輸出常數項
			}
			else
			{
				fprintf(formulaFile, "%f × (%d)", i -> second , i -> first );//輸出第i根的係數
			}
			if ( ++i != p -> polynomial.end() )//看她是不是最後一項
			{
				fprintf(formulaFile, " ＋ " );
			}
			--i;//把指標變回去
		}
		fprintf(formulaFile," ＝ 0\n");
		//p -> index = polynomialCount;
		//polynomialCount++;
		p -> PrintPolynomialList();
	}
	fclose( formulaFile );
	//███████████████████████████████████████
	//std::cout << "==============================================" << std::endl;
	//std::cout << "==============================================" << std::endl;
	virtualForceAnalyzer -> Analyze();

	std::cout << "==============================================" << std::endl;
	std::cout << "==============================================" << std::endl;
	std::cout << "===================虛功法分析結果為====================" << std::endl;
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		p -> PrintPolynomialList();
		for( PolynomialList* p2 = firstPolynomial; p2 != NULL; p2 = p2 -> link )
		{
			if( p -> polynomial.size() != 2 || p2 -> polynomial.size() != 2 ) continue;
			if( p -> GetOneUnknown() == p2 -> GetOneUnknown() && p != p2 )
			{
				std::cout << "此式與第" << p2 -> index << "式一樣：";
				p2 -> PrintPolynomialList();
			}
		}
	}
	std::cout << "==============================================" << std::endl;
	std::cout << "==============================================" << std::endl;

	//debug輸出結果
	//PrintPolynomialList();
	//debug
	formulaFile = fopen("StructureFormulaSolved.txt", "w");
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		//std::cout << "debug:" << p -> index << std::endl;
		fprintf(formulaFile, "%d,", p -> index );
		std::map< int , float >::iterator firstTerm = p -> polynomial.begin();
		firstTerm++;
		if( firstTerm -> first > 30 ) fprintf(formulaFile,",");
		if( firstTerm -> first > 60 ) fprintf(formulaFile,",");
		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
		{
			if( i -> first < 0 ) continue;
			fprintf(formulaFile, "%d ", i -> first );
			int tempParam = i -> first;
			i++;
			if( i == p -> polynomial.end() )
			{
				i--;
				continue;
			}
			if( ( tempParam < 31 && i -> first > 30 ) || ( tempParam < 61 && i -> first > 60 ) )
			{
				fprintf(formulaFile,",");
			}
			i--;
		}
		fprintf(formulaFile," \n");
	}
	fclose( formulaFile );



	//PrintSolution();
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p -> polynomial.size() == 2 )
		{
			std::map< int , float >::iterator iter = p -> polynomial.begin();//第一項式<-1,常數>，所以先跳過
			if( iter -> first == -1 )
			{
				float solution = iter -> second;
				iter++;
				std::cout << "第" << iter -> first << "根支架的力量是" << solution / iter -> second << "g" << std::endl;
			}
			else//第一項式<反力編號,1>，第二像是<-1,常數>
			{
				std::cout << "第" << -iter -> first << "個反力的力量是";
				iter++;
				std::cout << iter -> second << "g" << std::endl;
			}
		}
		else
		{
			std::cout << "(此式尚未解出)";
			p -> PrintPolynomialList();
		}
	}

	

}