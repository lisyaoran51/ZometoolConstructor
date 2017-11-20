#include "Socket.h"


Socket::Socket()
{
	position = new GLfloat[3];
	rotation = new GLfloat[3];
	direction = new GLfloat[3];
}

Socket::~Socket()
{
	delete[] position ;
	delete[] rotation ;
	delete[] direction;
}

SocketConstructor::SocketConstructor()
{
	model = new Model[10];
	//model[0] = Model("SocketR7B0309.obj");//socket1，半徑7
	model[0] = Model("SocketR12A0417.obj");//socket1，半徑12
	//memcpy( model , tempModel , sizeof( Model ) * 4 );

	//zomeDirection = new ZomeDirection();

	GLfloat sca[3] = {1.0f,1.0f,1.0f};
	GLfloat tra[3] = {0.0f,0.0f,0.0f};
	GLfloat rot[3] = { 0.0f , 0.0f , 0.0f };

	//MathHelper::CopyVector( tra , translate );

	//originalModel = new ModelObject( &model[0] , sca , tra , rot , 0 , 0 );

	//tempModelLink = originalModel;
	//tempSocketLink = originalSocket;
}

SocketConstructor::~SocketConstructor()
{
	delete[] model;

	for( Socket* s = originalSocket; s != NULL; )
	{
		Socket* temp = s -> link;
		delete s;
		s = temp;
	}

	for( ModelObject* m = originalModel; m != NULL; )
	{
		ModelObject* temp = m -> link;
		delete m;
		m = temp;
	}
}

int SocketConstructor::NewSocket( OuterCone* sOuterCone )
{
	//鍵一個心的接口
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();
	//tempSocket -> length = length;//設定長度
	tempSocket -> sourceOuterCone = sOuterCone;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================算接口的座標、長度、角度
	//外支架末端做標
	GLfloat endCenterPoint[] = { 0 , 0 , 0 }; 
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[1] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[2] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 1.0 / 3.0 );

	//外支架末端到接口的方向
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , sOuterCone -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	if( sOuterCone -> type != sOuterCone -> nearestPointOnSurface -> index )
	{
		std::cout<<"socket出錯";
	}


	//接頭的座標
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::VectorPlus( position , endCenterPoint );
	MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS / 2; i++ )MathHelper::VectorMinus( position , directionToSurface );//往外支架方向後退4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//接頭的長度
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , sOuterCone -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS試12.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================件模型
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來


	std::cout << "從第" << sOuterCone -> index << "號外支架末端建立第" << tempSocket -> index << "號接頭。" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewOriginalSocket( OuterCone* sOuterCone )
{
	//鍵一個心的接口
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();
	//tempSocket -> length = length;//設定長度
	tempSocket -> sourceOuterCone = sOuterCone;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================算接口的座標、長度、角度
	//外支架末端做標
	GLfloat endCenterPoint[] = { 0 , 0 , 0 }; 
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[1] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[2] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 1.0 / 3.0 );

	//外支架末端到接口的方向
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , sOuterCone -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	if( sOuterCone -> type != sOuterCone -> nearestPointOnSurface -> index )
	{
		std::cout<<"socket出錯";
	}


	//接頭的座標
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::CopyVector( position , sOuterCone -> nearestPointOnSurface -> position );//值接把建球的位置放在模型邊緣上
	//MathHelper::VectorPlus( position , endCenterPoint );
	//MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	//MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS; i++ )MathHelper::VectorMinus( position , directionToSurface );//往外支架方向後退4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//接頭的長度
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , sOuterCone -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS試8.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================件模型
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	//tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來


	std::cout << "從第" << sOuterCone -> index << "號外支架末端建立第" << tempSocket -> index << "號原狀接頭。" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewSocket( Branch* branch )
{
	//鍵一個心的接口
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();
	//tempSocket -> length = length;//設定長度
	tempSocket -> sourceBranch = branch;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================算接口的座標、長度、角度
	//外支架末端做標
	GLfloat endCenterPoint[3];
	MathHelper::CopyVector( endCenterPoint , &( branch -> zomeDirection -> faceDirection[ branch -> connectFace[0] * 3 ] ) );
	MathHelper::VectorNormalize( endCenterPoint );
	MathHelper::VectorMutiply( endCenterPoint , branch -> zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - NODE_DIAMETER / 2.0 + LATCH );
	MathHelper::VectorPlus( endCenterPoint , branch -> sourceCorner[0] -> position );


	//外支架末端到接口的方向
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branch -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//接頭的座標
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::VectorPlus( position , endCenterPoint );
	MathHelper::VectorPlus( position , branch -> nearestPointOnSurface -> position );
	MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS / 2; i++ )MathHelper::VectorMinus( position , directionToSurface );//往外支架方向後退4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//接頭的長度
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branch -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS試12.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================件模型
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來


	std::cout << "從(" << branch -> sourceCorner[1] -> position[0] << "," << 
						  branch -> sourceCorner[1] -> position[1] << "," << 
						  branch -> sourceCorner[1] -> position[2] << ")樹枝建立第" << tempSocket -> index << "號接頭。" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewOriginalSocket( Branch* branch )
{
	//鍵一個心的接口
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();
	//tempSocket -> length = length;//設定長度
	tempSocket -> sourceBranch = branch;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================算接口的座標、長度、角度
	//外支架末端做標
	GLfloat endCenterPoint[3];
	MathHelper::CopyVector( endCenterPoint , &( branch -> zomeDirection -> faceDirection[ branch -> connectFace[0] * 3 ] ) );
	MathHelper::VectorNormalize( endCenterPoint );
	MathHelper::VectorMutiply( endCenterPoint , branch -> zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - NODE_DIAMETER / 2.0 + LATCH );
	MathHelper::VectorPlus( endCenterPoint , branch -> sourceCorner[0] -> position );

	//外支架末端到接口的方向
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branch -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//接頭的座標
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::CopyVector( position , branch -> nearestPointOnSurface -> position );//值接把建球的位置放在模型邊緣上
	//MathHelper::VectorPlus( position , endCenterPoint );
	//MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	//MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS; i++ )MathHelper::VectorMinus( position , directionToSurface );//往外支架方向後退4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//接頭的長度
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branch -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS試8.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================件模型
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	//tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來

	std::cout << "從(" << branch -> sourceCorner[1] -> position[0] << "," << 
						  branch -> sourceCorner[1] -> position[1] << "," << 
						  branch -> sourceCorner[1] -> position[2] << ")樹枝建立第" << tempSocket -> index << "號原狀接頭。" << std::endl;
	
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewSocket( BranchEnd* branchEnd )
{
	//鍵一個心的接口
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();
	//tempSocket -> length = length;//設定長度
	tempSocket -> sourceBranchEnd = branchEnd;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================算接口的座標、長度、角度
	//外支架末端做標
	GLfloat endCenterPoint[3] = { 0 , 0 , 0 };
	MathHelper::CopyVector( endCenterPoint , branchEnd -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , branchEnd -> cornerEnd[1] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 0.5 );
	


	//外支架末端到接口的方向
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branchEnd -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//接頭的座標
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::VectorPlus( position , endCenterPoint );
	MathHelper::VectorPlus( position , branchEnd -> nearestPointOnSurface -> position );
	MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS / 2; i++ )MathHelper::VectorMinus( position , directionToSurface );//往外支架方向後退4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//接頭的長度
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branchEnd -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS試12.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================件模型
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來


	std::cout << "從(" << branchEnd -> sourceCorner[0] -> position[0] << "," << 
						  branchEnd -> sourceCorner[0] -> position[1] << "," << 
						  branchEnd -> sourceCorner[0] -> position[2] << ")到(" <<
						  branchEnd -> sourceCorner[1] -> position[0] << "," << 
						  branchEnd -> sourceCorner[1] -> position[1] << "," << 
						  branchEnd -> sourceCorner[1] -> position[2] << ")的樹枝末梢建立第" << tempSocket -> index << "號接頭。" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewOriginalSocket( BranchEnd* branchEnd )
{
	//鍵一個心的接口
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();
	//tempSocket -> length = length;//設定長度
	tempSocket -> sourceBranchEnd = branchEnd;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================算接口的座標、長度、角度
	//外支架末端做標
	GLfloat endCenterPoint[3] = { 0 , 0 , 0 };
	MathHelper::CopyVector( endCenterPoint , branchEnd -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , branchEnd -> cornerEnd[1] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 0.5 );

	//外支架末端到接口的方向
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branchEnd -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//接頭的座標
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::CopyVector( position , branchEnd -> nearestPointOnSurface -> position );//值接把建球的位置放在模型邊緣上
	//MathHelper::VectorPlus( position , endCenterPoint );
	//MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	//MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS; i++ )MathHelper::VectorMinus( position , directionToSurface );//往外支架方向後退4mm
	MathHelper::CopyVector( tempSocket -> position , position );

	//接頭的長度
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branchEnd -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS試8.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================件模型
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   tempSocket -> rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	//tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來

	std::cout << "從(" << branchEnd -> sourceCorner[0] -> position[0] << "," << 
						  branchEnd -> sourceCorner[0] -> position[1] << "," << 
						  branchEnd -> sourceCorner[0] -> position[2] << ")到(" <<
						  branchEnd -> sourceCorner[1] -> position[0] << "," << 
						  branchEnd -> sourceCorner[1] -> position[1] << "," << 
						  branchEnd -> sourceCorner[1] -> position[2] << ")的樹枝末梢建立第" << tempSocket -> index << "號原狀接頭。" << std::endl;
	
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewSocket( GLfloat* position )
{
	Socket *lastSocket = NULL , *tempSocket = NULL;
	//lastBall -> faceConnection[ faceIndex ] = new ZomeConnection();
	for ( Socket* i = originalSocket; i != NULL; i = i -> link )
	{
		if ( i -> link == NULL )
		{
			lastSocket = i;
		}
	}
	tempSocket = new Socket();

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//有建過就當下一個
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;
	//========================================算接口的座標、長度、角度
	//接頭的座標
	MathHelper::CopyVector( tempSocket -> position , position );

	//=========================================件模型
	GLfloat rotation[] = { 0 , 0 , 0 };
	GLfloat scale[] = { 1.0 , 1.0 , 1.0 };
	if( originalModel == NULL )
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   rotation , 
												   0 ,
												   0 );
	}
	else
	{
		tempSocket -> thisModel = new ModelObject( &model[0] , 
												   scale , 
												   tempSocket -> position , 
												   rotation , 
												   tempModelLink -> GetNextVertexOffset() ,
												   tempModelLink -> GetNextIndexOffset() );
	}
	
	//tempSocket -> thisModel -> elongY = tempSocket -> length;


	//=========================================把街頭的模型加入串列
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//把所有接頭用鏈結串列連起來
	}
	tempModelLink = tempSocket -> thisModel;		  //把所有求用鏈結串列連起來


	std::cout << "位於(" << tempSocket -> position[0] << "," << tempSocket -> position[1] << "," <<  tempSocket -> position[2] << ")的竿子受拉力過大" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

