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
	//model[0] = Model("SocketR7B0309.obj");//socket1�A�b�|7
	model[0] = Model("SocketR12A0417.obj");//socket1�A�b�|12
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
	//��@�Ӥߪ����f
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
	//tempSocket -> length = length;//�]�w����
	tempSocket -> sourceOuterCone = sOuterCone;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================�Ⱶ�f���y�СB���סB����
	//�~��[���ݰ���
	GLfloat endCenterPoint[] = { 0 , 0 , 0 }; 
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[1] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[2] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 1.0 / 3.0 );

	//�~��[���ݨ챵�f����V
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , sOuterCone -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	if( sOuterCone -> type != sOuterCone -> nearestPointOnSurface -> index )
	{
		std::cout<<"socket�X��";
	}


	//���Y���y��
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::VectorPlus( position , endCenterPoint );
	MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS / 2; i++ )MathHelper::VectorMinus( position , directionToSurface );//���~��[��V��h4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//���Y������
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , sOuterCone -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS��12.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��


	std::cout << "�q��" << sOuterCone -> index << "���~��[���ݫإ߲�" << tempSocket -> index << "�����Y�C" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewOriginalSocket( OuterCone* sOuterCone )
{
	//��@�Ӥߪ����f
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
	//tempSocket -> length = length;//�]�w����
	tempSocket -> sourceOuterCone = sOuterCone;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================�Ⱶ�f���y�СB���סB����
	//�~��[���ݰ���
	GLfloat endCenterPoint[] = { 0 , 0 , 0 }; 
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[1] -> position );
	MathHelper::VectorPlus( endCenterPoint , sOuterCone -> cornerEnd[2] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 1.0 / 3.0 );

	//�~��[���ݨ챵�f����V
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , sOuterCone -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	if( sOuterCone -> type != sOuterCone -> nearestPointOnSurface -> index )
	{
		std::cout<<"socket�X��";
	}


	//���Y���y��
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::CopyVector( position , sOuterCone -> nearestPointOnSurface -> position );//�ȱ���زy����m��b�ҫ���t�W
	//MathHelper::VectorPlus( position , endCenterPoint );
	//MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	//MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS; i++ )MathHelper::VectorMinus( position , directionToSurface );//���~��[��V��h4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//���Y������
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , sOuterCone -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS��8.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��


	std::cout << "�q��" << sOuterCone -> index << "���~��[���ݫإ߲�" << tempSocket -> index << "���쪬���Y�C" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewSocket( Branch* branch )
{
	//��@�Ӥߪ����f
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
	//tempSocket -> length = length;//�]�w����
	tempSocket -> sourceBranch = branch;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================�Ⱶ�f���y�СB���סB����
	//�~��[���ݰ���
	GLfloat endCenterPoint[3];
	MathHelper::CopyVector( endCenterPoint , &( branch -> zomeDirection -> faceDirection[ branch -> connectFace[0] * 3 ] ) );
	MathHelper::VectorNormalize( endCenterPoint );
	MathHelper::VectorMutiply( endCenterPoint , branch -> zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - NODE_DIAMETER / 2.0 + LATCH );
	MathHelper::VectorPlus( endCenterPoint , branch -> sourceCorner[0] -> position );


	//�~��[���ݨ챵�f����V
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branch -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//���Y���y��
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::VectorPlus( position , endCenterPoint );
	MathHelper::VectorPlus( position , branch -> nearestPointOnSurface -> position );
	MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS / 2; i++ )MathHelper::VectorMinus( position , directionToSurface );//���~��[��V��h4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//���Y������
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branch -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS��12.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��


	std::cout << "�q(" << branch -> sourceCorner[1] -> position[0] << "," << 
						  branch -> sourceCorner[1] -> position[1] << "," << 
						  branch -> sourceCorner[1] -> position[2] << ")��K�إ߲�" << tempSocket -> index << "�����Y�C" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewOriginalSocket( Branch* branch )
{
	//��@�Ӥߪ����f
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
	//tempSocket -> length = length;//�]�w����
	tempSocket -> sourceBranch = branch;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================�Ⱶ�f���y�СB���סB����
	//�~��[���ݰ���
	GLfloat endCenterPoint[3];
	MathHelper::CopyVector( endCenterPoint , &( branch -> zomeDirection -> faceDirection[ branch -> connectFace[0] * 3 ] ) );
	MathHelper::VectorNormalize( endCenterPoint );
	MathHelper::VectorMutiply( endCenterPoint , branch -> zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - NODE_DIAMETER / 2.0 + LATCH );
	MathHelper::VectorPlus( endCenterPoint , branch -> sourceCorner[0] -> position );

	//�~��[���ݨ챵�f����V
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branch -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//���Y���y��
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::CopyVector( position , branch -> nearestPointOnSurface -> position );//�ȱ���زy����m��b�ҫ���t�W
	//MathHelper::VectorPlus( position , endCenterPoint );
	//MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	//MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS; i++ )MathHelper::VectorMinus( position , directionToSurface );//���~��[��V��h4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//���Y������
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branch -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS��8.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��

	std::cout << "�q(" << branch -> sourceCorner[1] -> position[0] << "," << 
						  branch -> sourceCorner[1] -> position[1] << "," << 
						  branch -> sourceCorner[1] -> position[2] << ")��K�إ߲�" << tempSocket -> index << "���쪬���Y�C" << std::endl;
	
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewSocket( BranchEnd* branchEnd )
{
	//��@�Ӥߪ����f
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
	//tempSocket -> length = length;//�]�w����
	tempSocket -> sourceBranchEnd = branchEnd;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================�Ⱶ�f���y�СB���סB����
	//�~��[���ݰ���
	GLfloat endCenterPoint[3] = { 0 , 0 , 0 };
	MathHelper::CopyVector( endCenterPoint , branchEnd -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , branchEnd -> cornerEnd[1] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 0.5 );
	


	//�~��[���ݨ챵�f����V
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branchEnd -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//���Y���y��
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::VectorPlus( position , endCenterPoint );
	MathHelper::VectorPlus( position , branchEnd -> nearestPointOnSurface -> position );
	MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS / 2; i++ )MathHelper::VectorMinus( position , directionToSurface );//���~��[��V��h4mm
	MathHelper::CopyVector( tempSocket -> position , position );


	//���Y������
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branchEnd -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS��12.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��


	std::cout << "�q(" << branchEnd -> sourceCorner[0] -> position[0] << "," << 
						  branchEnd -> sourceCorner[0] -> position[1] << "," << 
						  branchEnd -> sourceCorner[0] -> position[2] << ")��(" <<
						  branchEnd -> sourceCorner[1] -> position[0] << "," << 
						  branchEnd -> sourceCorner[1] -> position[1] << "," << 
						  branchEnd -> sourceCorner[1] -> position[2] << ")����K����إ߲�" << tempSocket -> index << "�����Y�C" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

int SocketConstructor::NewOriginalSocket( BranchEnd* branchEnd )
{
	//��@�Ӥߪ����f
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
	//tempSocket -> length = length;//�]�w����
	tempSocket -> sourceBranchEnd = branchEnd;

	if ( lastSocket == NULL )
	{
		tempSocket -> index = 0;
		originalSocket = tempSocket;
	}
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;

	//========================================�Ⱶ�f���y�СB���סB����
	//�~��[���ݰ���
	GLfloat endCenterPoint[3] = { 0 , 0 , 0 };
	MathHelper::CopyVector( endCenterPoint , branchEnd -> cornerEnd[0] -> position );
	MathHelper::VectorPlus( endCenterPoint , branchEnd -> cornerEnd[1] -> position );
	MathHelper::VectorMutiply( endCenterPoint , 0.5 );

	//�~��[���ݨ챵�f����V
	GLfloat* directionToSurface = new GLfloat[3];
	MathHelper::VectorMinus( directionToSurface , branchEnd -> nearestPointOnSurface -> position , endCenterPoint );
	MathHelper::VectorNormalize( directionToSurface );

	//���Y���y��
	GLfloat position[] = { 0 , 0 , 0 };
	MathHelper::CopyVector( position , branchEnd -> nearestPointOnSurface -> position );//�ȱ���زy����m��b�ҫ���t�W
	//MathHelper::VectorPlus( position , endCenterPoint );
	//MathHelper::VectorPlus( position , sOuterCone -> nearestPointOnSurface -> position );
	//MathHelper::VectorMutiply( position , 1.0 / 2.0 );
	for( int i = 0; i < SOCKET_RADIUS; i++ )MathHelper::VectorMinus( position , directionToSurface );//���~��[��V��h4mm
	MathHelper::CopyVector( tempSocket -> position , position );

	//���Y������
	tempSocket -> length = MathHelper::VectorDistance( endCenterPoint , branchEnd -> nearestPointOnSurface -> position )
						   - SOCKET_RADIUS;//SOCKET_RADIUS��8.0

	GLfloat rotation[] = { MathHelper::GetDirectionTheta( directionToSurface ) ,
						   MathHelper::GetDirectionPhi( directionToSurface ) , 
						   MathHelper::GetDirectionYaw( directionToSurface ) };
	MathHelper::CopyVector( tempSocket -> rotation , rotation );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��

	std::cout << "�q(" << branchEnd -> sourceCorner[0] -> position[0] << "," << 
						  branchEnd -> sourceCorner[0] -> position[1] << "," << 
						  branchEnd -> sourceCorner[0] -> position[2] << ")��(" <<
						  branchEnd -> sourceCorner[1] -> position[0] << "," << 
						  branchEnd -> sourceCorner[1] -> position[1] << "," << 
						  branchEnd -> sourceCorner[1] -> position[2] << ")����K����إ߲�" << tempSocket -> index << "���쪬���Y�C" << std::endl;
	
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
	else//���عL�N��U�@��
	{
		lastSocket -> link = tempSocket;
		tempSocket -> index = lastSocket -> index + 1;
	}
	tempSocket -> originalSocket = originalSocket;
	//========================================�Ⱶ�f���y�СB���סB����
	//���Y���y��
	MathHelper::CopyVector( tempSocket -> position , position );

	//=========================================��ҫ�
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


	//=========================================����Y���ҫ��[�J��C
	if( originalModel == NULL )
	{
		originalModel = tempSocket -> thisModel;
	}
	else
	{
		tempModelLink -> link = tempSocket -> thisModel;//��Ҧ����Y���쵲��C�s�_��
	}
	tempModelLink = tempSocket -> thisModel;		  //��Ҧ��D���쵲��C�s�_��


	std::cout << "���(" << tempSocket -> position[0] << "," << tempSocket -> position[1] << "," <<  tempSocket -> position[2] << ")����l���ԤO�L�j" << std::endl;
	int returnIndex = tempSocket -> index;
	return returnIndex;
}

