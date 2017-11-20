#include "Constructor.h"



Constructor::Constructor()//0520��Ҧ������׳��[1mm//0526����b�|+0.1mm�A���b�|+0.25mm
{
	model = new Model[10];
	model[0] = Model("ball1122Std.obj");
	model[1] = Model("BlueS0523.obj");//����5.5mm �u��4.5mm 
	model[2] = Model("BlueM0523.obj");//�d�Ǫ���3.5mm �u��2.5mm
	model[3] = Model("BlueL0523.obj");//1208�令����6�u��4
	model[4] = Model("RedS0526.obj");//�쥻��
	model[5] = Model("RedM0526.obj");//0520��쥻���~��b�|��u0.5mm//0525�o�{�}�Ӥp
	model[6] = Model("RedL0526.obj");
	model[7] = Model("YellowS0526.obj");//�令??1206
	model[8] = Model("YellowM0526.obj");
	model[9] = Model("YellowL0526.obj");//456�E789��
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
	//force = new std::map<int,float>();//11/7�g��o��
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
	//�Ĺs���[�J�զ�
	material -> insert( std::pair<int, int>( 0 , COLOR_WHITE ) );
	matIndex2Index -> insert( std::pair<int, int>( 0 , -1 ) );
}

Constructor::Constructor( GLfloat* translate )//0520��Ҧ������׳��[1mm
{//0522��� ���Y�p ����900��
	model = new Model[10];
	model[0] = Model("ball1122Std.obj");
	model[1] = Model("BlueS0523.obj");//����5.5mm �u��4.5mm 
	model[2] = Model("BlueM0523.obj");//�d�Ǫ���3.5mm �u��2.5mm
	model[3] = Model("BlueL0523.obj");//1208�令����6�u��4
	model[4] = Model("RedS0526.obj");//�쥻��
	model[5] = Model("RedM0526.obj");//0520��쥻���~��b�|��u0.5mm
	model[6] = Model("RedL0526.obj");
	model[7] = Model("YellowS0526.obj");//�令??1206
	model[8] = Model("YellowM0526.obj");
	model[9] = Model("YellowL0526.obj");//456�E789��
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
	//force = new std::map<int,float>();//11/7�g��o��
	//matForce = new std::map<int,float>();
	
	totalForceDown = 0;
	for(int i=0;i<3;i++)gravityPosition[i] = 0;
	originalPolarCoor = NULL;

	originalModel = new ModelObject( &model[0] , sca , tra , rot , 0 , 0 );

	tempModelLink = originalModel;
	tempBallLink = originalBall;
	//�Ĺs���[�J�զ�
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
	//�T�{�y��pointIndex�S���X�{�L
	/*
	for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
	{
		if ( i -> index == pointIndex )
		{
			return -1;
		}
	}
	*/

	//�]�w�s���y����� �]�A���� �Ĥ@���y�쵲
	ZomeStructure* newBall = new ZomeStructure();
	//newBall -> position = new GLfloat[3];
	MathHelper::SetVector( newBall -> position , translate[0] , translate[1] , translate[2] );
	//newBall -> index = pointIndex;
	newBall -> originalBall = originalBall;

	//��s���y�M�W�@���䪺�y�s�_��
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
	newBall -> index = lastBall -> index + 1;*///�]�w�y��index
	//�[�J�o�@���y���C��
	material -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , COLOR_WHITE ) );
	matIndex2Index -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , -1 ) );
	//��newBall���ҫ�
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
	//std::cout << "��" << tempBall -> index << "���y���I��" << tempBall -> thisModel -> verticeSize / 3 << "�ӡA����" 
	//<<  tempBall -> thisModel -> indiceSize / 3 << "��" << std::endl;
	tempModelLink -> link = newBall -> thisModel;//��Ҧ��D���쵲��C�s�_��
	tempModelLink = newBall -> thisModel;		  //��Ҧ��D���쵲��C�s�_��

	std::cout << "���s�b�s����m�إ߲�" << newBall -> index << "���y�C" << std::endl;
	int returnIndex = newBall -> index;
	return returnIndex;

}

int Constructor::Construct( int pointIndex , int faceIndex , int size , bool type )
{
	//�O��
	connectorAmount[ zomeDirection -> GetFaceColor( faceIndex ) ][ size ] += 1;
	if( type == TYPE_PIN ) ballAmount ++;

	//���n�s�W���y
	ZomeStructure *sourceBall = NULL , *tempBall = NULL , *lastBall = NULL; 
	for ( ZomeStructure* i = originalBall; i != NULL; i = i -> link )
	{
		if ( i -> index == pointIndex )
		{
			sourceBall = i;
			break;
		}
	}
	//�p�G�S�����source ball �N��pointIndex��J���F
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
		tempBall -> index = lastBall -> index + 1;//�]�w�y��index
		lastBall -> link = tempBall;//��̫�@���y�M�o���y�s�_��*/
		sourceBall -> nextBall[ faceIndex ] = tempBall;//���ķs�W���y�M�o���y�s�_��
		int oppositeIndex = zomeDirection -> GetOppositeFace( faceIndex );
		tempBall -> nextBall[ oppositeIndex ] = sourceBall;//�s���y���ӷ����s���ª��y
	}
	

	//��쭱�e�إߪ��̷s����l
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
	tempConn -> color = zomeDirection -> GetFaceColor ( faceIndex );//�]�w�C��M����
	tempConn -> size = size;
	sourceBall -> faceConnection[ faceIndex ] = tempConn;//�b�{�b�o���y�W�����ӭ� �s���{�b�n�ت��o�Ӭ�l
	tempConn -> fromIndex = sourceBall -> index;
	tempConn -> fromFace = faceIndex;
	if( type == TYPE_PIN )//�p�G���ؼвy�A�N��ؼвy�s����J��h
	{
		tempConn -> towardIndex = tempBall -> index;
		tempConn -> towardFace = zomeDirection -> GetOppositeFace( faceIndex );
		tempBall -> faceConnection[ tempConn -> towardFace ] = tempConn;//�ש�����o�b 0105
	}
	else//�p�G�S���ؼвy�A�N�q�Ҧ��y����X��w���y�A�⨺���y���s����J��h
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
				i -> faceConnection[ oppositeIndex ] = tempConn;//���s�����y�M���s�����i�h
				sourceBall -> nextBall[ oppositeIndex ] = i;//��s���쪺�y���s���Ѷi�h�Q�s�����y���s��
				break;
			}
		}
	}
	//�p�G���e�S���عL��l�A�N��{�b����Ĥ@��
	//�g��AddToLink

	originalConn = tempConn -> AddToLink( originalConn );
	/*
	if ( lastConn == NULL )
	{
		tempConn -> index = 0;
		originalConn = tempConn;
	}
	else//���عL�N��U�@��
	{
		lastConn -> link = tempConn;
		tempConn -> index = lastConn -> index + 1;
	}*/
	
	//========================================���l����l
	GLfloat* translate = new GLfloat[3];
	if( faceIndex == -1 )
	{
		std::cout << "�X��" << std::endl;
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
	//========================================���l���ҫ�
	//�[�J�o�@�Ӭ�l���C��
	material -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , zomeDirection -> GetFaceColor ( faceIndex ) ) );
	//force -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 ,  );
	matIndex2Index -> insert( std::pair<int, int>( tempModelLink -> GetNextIndexOffset() / 3 , tempConn -> index ) );//������m

	int modelNumber = tempConn -> GetModelNumber();
	GLfloat scale[] = {1.0f,1.0f,1.0f};
	tempConn -> thisModel = new ModelObject( &model[ modelNumber ] , 
											 scale , 
											 tempConn -> position , 
											 tempConn -> rotation , 
											 tempModelLink -> GetNextVertexOffset() ,
											 tempModelLink -> GetNextIndexOffset() );
	//std::cout << "��" << tempConn -> index << "����l���I��" << tempConn -> thisModel -> verticeSize / 3 << "�ӡA����" 
	//	<<  tempConn -> thisModel -> indiceSize / 3 << "��" << std::endl;
	tempModelLink -> link = tempConn -> thisModel;//��Ҧ���l���쵲��C�s�_��
	tempModelLink = tempConn -> thisModel;        //��Ҧ���l���쵲��C�s�_��
	//========================================��y���ҫ�
	if ( type == TYPE_PIN )
	{
		//�[�J�o�@���y���C��
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
		//std::cout << "��" << tempBall -> index << "���y���I��" << tempBall -> thisModel -> verticeSize / 3 << "�ӡA����" 
		//<<  tempBall -> thisModel -> indiceSize / 3 << "��" << std::endl;
		tempModelLink -> link = tempBall -> thisModel;//��Ҧ��D���쵲��C�s�_��
		tempModelLink = tempBall -> thisModel;		  //��Ҧ��D���쵲��C�s�_��
	}
	
	//��P�o���y�۳s����l���[�J�o�Ӳy���s��
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

	
	int returnIndex;//�^�Ƿs���y���s��
	if ( type == TYPE_PIN )
	{
		std::cout << "�q��" << sourceBall -> index << "���y����" << faceIndex << "���إ߱�l��" << tempConn -> index <<
			"���A�s���" << tempBall -> index << "���y�C" << std::endl;
		returnIndex = tempBall -> index;
	}
	else
	{
		std::cout << "�q��" << sourceBall -> index << "���y����" << faceIndex << "���إ߱�l��" << tempConn -> index <<
			"���C" << std::endl;
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
	std::cout << "=====================�H�U�O�`�@��O���s��=====================" <<std::endl;
	std::cout << "�@�ϥΤF�y " << ballAmount << " ��" << std::endl;
	std::cout << "--------------------" << std::endl;
	
	std::cout << std::setw(5) << "�C��" << std::setw(5) << "S" << std::setw(5) << "M" << std::setw(5) << "L" << std::endl;
	std::cout << std::setw(5) << "�Ŧ�" << std::setw(5) << connectorAmount[0][0] << std::setw(5) << connectorAmount[0][1] << std::setw(5) << connectorAmount[0][2] << std::endl;
	std::cout << std::setw(5) << "����" << std::setw(5) << connectorAmount[1][0] << std::setw(5) << connectorAmount[1][1] << std::setw(5) << connectorAmount[1][2] << std::endl;
	std::cout << std::setw(5) << "����" << std::setw(5) << connectorAmount[2][0] << std::setw(5) << connectorAmount[2][1] << std::setw(5) << connectorAmount[2][2] << std::endl;
	std::cout << "--------------------" << std::endl;


	FILE* Record = fopen("Components.txt", "w");
	
	fprintf(Record, "=====================�H�U�O�`�@��O���s��=====================\n" );
	fprintf(Record, "�@�ϥΤF�y %d ��\n" , ballAmount );

	fprintf(Record, "--------------------\n" );

	fprintf(Record, " �C��    S    M    L\n" );
	fprintf(Record, " �Ŧ�%5d%5d%5d\n" , connectorAmount[0][0] , connectorAmount[0][1] , connectorAmount[0][2] );
	fprintf(Record, " ����%5d%5d%5d\n" , connectorAmount[1][0] , connectorAmount[1][1] , connectorAmount[1][2] );
	fprintf(Record, " ����%5d%5d%5d\n" , connectorAmount[2][0] , connectorAmount[2][1] , connectorAmount[2][2] );

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
	std::cout << "�̧C�����׳]��" << lowestEndHeight << std::endl;
	//��̧C���X�Ӧa��ХX��
	for( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex == -1 )
		{
			if( ( i -> endPosition )[1] < lowestEndHeight + BOTTOM_INTERVAL && i -> towardIndex == -1 )
			{
				Bottom -> push_back( i -> index );
				std::cout << "��" << i -> index << "�ڬ�l���ݬ����a�A�y�Ь�(" 
						  <<  ( i -> endPosition )[0] << "," 
						  <<  ( i -> endPosition )[1] << "," 
						  <<  ( i -> endPosition )[2] << ")" << std::endl;
				bool debug = 0;
			}
		}
	}

	//�إ߳s�������I���q
	int virtualBallCount = -1;
	std::vector< VirtualBall* > virtualBalls;
	std::vector< int > errorBalls;//�Ψ��\�X�����y
	//FindConnectors( Bottom , &virtualBalls );
	for( ZomeConnection* i = originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex != -1 )continue;
		bool newVirtualBall = true;
		for(int j = 0; j < virtualBalls.size(); j++ )
		{
			VirtualBall* tempVirtualBall = virtualBalls.at( j );
			if( MathHelper::VectorApproximation( tempVirtualBall -> position ,
												 i -> endPosition ) )//�p�G�o�ڴΤl��s�������I��m�c�P�A�N��o�a�i�h
			{
				newVirtualBall = false;
				//tempVirtualBall -> AddConn( i );
				tempVirtualBall -> connectionAmount++;
				if( tempVirtualBall -> connectionAmount > 5 )
				{
					
					std::cout << "Constructor::StructureAnalyze()�X��:�s�������I�ұ�����l�W�L6��" << std::endl;
					std::cout << "�h�X�Ӫ���l�s���O�G" << i -> index << "�A���O�G" << zomeDirection -> GetOppositeFace( i -> fromFace ) << std::endl;
					errorBalls.push_back( i -> index );
					tempVirtualBall -> connectionAmount--;
					std::cout << "�����y�y�СG(" << tempVirtualBall -> position[0] << "," 
												 << tempVirtualBall -> position[1] << "," 
												 << tempVirtualBall -> position[2] << ")" << std::endl;
					continue;
				} 
				int towardFace = zomeDirection -> GetOppositeFace( i -> fromFace );
				tempVirtualBall -> connectFace[ tempVirtualBall -> connectionAmount ] = towardFace;//��J�P��l�s�������s��
				tempVirtualBall -> connIndex[ tempVirtualBall -> connectionAmount ] = i -> index;//��J��l���s��
				tempVirtualBall -> conn[ tempVirtualBall -> connectionAmount ] = i;//��J��l������
				std::cout << "�b��" << j << "�ӵ����y(" << tempVirtualBall -> position[0] << "," 
						  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")���[�J��" << i -> index << "����l�A�s�����O" << towardFace << "�A�ثe�@" 
						  << tempVirtualBall -> connectionAmount << "�ڬ�l" << std::endl;
				bool debug = 0;
			}
		}
		if( newVirtualBall )
		{
			VirtualBall* tempVirtualBall = new VirtualBall( 0 - virtualBalls.size() - 1 , i );//�S���o���I���ܴN�s�W�@���I
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
			std::cout << "�s�W��" << virtualBalls.size() << "�ӵ����y(" << tempVirtualBall -> position[0] << ","
					  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")�A�[�J��" 
					  << i -> index << "����l�A�s�����O" << towardFace << "�A�ثe�@" 
					  << tempVirtualBall -> connectionAmount << "�ڬ�l" << std::endl;
			bool debug = 0;
		}
	}



	int debugCount = 0;//���@�Ӧ��l�X���A�γo�Ө�debug
	//ListPolynomial();
	//�}�l�C�h�����A��C�@��D�إߤ@�ӤT�Ӧ��l�A�C���I���O��x�By�Bz�U�ؤ@�Ӧ��l
	for( int d = 0; d < 3; d++ )//�T�Ӻ���xyz
	{
		//��C���y�y�@�Ӻ⦡
		for( ZomeStructure* b = originalBall; b != NULL; b = b -> link )
		{
			//debug
			if( b -> index == 69 )
			{
				std::cout << "debug:���69�y�A�y�СG(" << b -> position[0] << "," << b -> position[1] << "," << b -> position[2] << std::endl;
			}

			//�إ߷s�h����
			PolynomialList* tempPolynomial = new PolynomialList( b );
			debugCount++;
			//�N�h�����[�J��
			for( int i = 0; i < 62; i++ )
			{
				if( b -> faceConnection[i] != NULL )
				{
					std::cout << "�o�ڱ�l����V�O��" << i << "���A�ڭ̨��L��" << d << "�Ӻ���" << std::endl;
					float parameter = zomeDirection -> GetDirectionParameter( i , d );//����i������d�Ӻ��ת��t��
					int connIndex = b -> faceConnection[i] -> index;//���o�ӭ���������@�ڴΤl



					if( abs(parameter) > 0.01 )
					{
						
						//debug
						if( connIndex == 1 )
						{
							std::cout << "debug:����l�b(";
						}

						std::pair< int , float > tempPair( connIndex , parameter );
						tempPolynomial -> polynomial.insert( tempPair );
						std::cout << "�Ӧ۲�" << b -> index << "���y����" << debugCount << "�Ӧ��l����" << tempPolynomial -> polynomial.size() 
								  << "�Ӷ����G #(" << connIndex << ")����O X " << parameter << "---(" 
								  << b -> faceConnection[i] -> position[0] << "," << b -> faceConnection[i] -> position[1]
								  << "," << b -> faceConnection[i] -> position[2] << ")" << std::endl;
						bool debug = 0;
					}
					if( d == 1 )//�[�W�o��Τl���������q/2�V�U�A�[�J�`�ƶ�
					{
						float constant = tempPolynomial -> polynomial.at( -1 );
						constant -= zomeDirection -> GetWeight( b -> faceConnection[i] -> color , b -> faceConnection[i] -> size ) * 0.5f;
						tempPolynomial -> polynomial.find( -1 ) -> second = constant;
						
						totalForceDown -= zomeDirection -> GetWeight( b -> faceConnection[i] -> color , 
																	  b -> faceConnection[i] -> size ) * 0.5f;//�a�W���U�O
						gravityPosition[0] -= b -> position[0] *
											  zomeDirection -> GetWeight( b -> faceConnection[i] -> color , 
																		  b -> faceConnection[i] -> size ) * 0.5f;//�⭫�ߥ�
						gravityPosition[2] -= b -> position[2] *
											  zomeDirection -> GetWeight( b -> faceConnection[i] -> color , 
																		  b -> faceConnection[i] -> size ) * 0.5f;//�⭫�ߥ�
					}
				}
			}
			//�[�W�y���������q�V�U
			if( d == 1 )
			{
				float constant = tempPolynomial -> polynomial.at( -1 );
				constant -= zomeDirection -> GetWeight( 3 , 0 );
				tempPolynomial -> polynomial.find( -1 ) -> second = constant;

				totalForceDown -= zomeDirection -> GetWeight( 3 , 0 );//�a�`���U�O
				gravityPosition[0] -= b -> position[0] * zomeDirection -> GetWeight( 3 , 0 );//�⭫�ߥ�
				gravityPosition[2] -= b -> position[2] * zomeDirection -> GetWeight( 3 , 0 );//�⭫�ߥ�
			}

			//�A��h�����[�J��C�A�p�G�o�Ӧh�����z���S���F��N���n�[
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
		//�o�̩�P���s�����I���h����
		int bottomForceCount = -2;//�x�s�����O
		for(int i = 0; i < virtualBalls.size(); i++ )
		{
			
			VirtualBall* tempVirtualBall = virtualBalls.at(i);
			
			if( tempVirtualBall -> connectionAmount != 2 )//continue;//�p�G�S���T�Ӥ�[�伵�A�N�������L�A�]���L�k�γo�Ӻ⦡��X���G
			{
				if( tempVirtualBall -> connectionAmount == 0 )//don't jump. save a result
				{
					errorBalls.push_back( tempVirtualBall -> connIndex[0] );
					std::cout << "ErrorBall:�o���y#" << tempVirtualBall -> index << "(" <<
						tempVirtualBall -> position[0] << "," << 
						tempVirtualBall -> position[1] << "," << 
						tempVirtualBall -> position[2] << ")�u���@�ڬ�l�s���C" << std::endl;
				}
				if( tempVirtualBall -> connectionAmount == 1 )
				{
					errorBalls.push_back( tempVirtualBall -> connIndex[0] );
					errorBalls.push_back( tempVirtualBall -> connIndex[1] );
					std::cout << "ErrorBall:�o���y#" << tempVirtualBall -> index << "(" <<
						tempVirtualBall -> position[0] << "," << 
						tempVirtualBall -> position[1] << "," << 
						tempVirtualBall -> position[2] << ")�u���G�ڬ�l�s���C" << std::endl;
				}
				continue;
			}
			//�إ߷s�h����
			PolynomialList* tempPolynomial = new PolynomialList( tempVirtualBall );
			debugCount++;
			//==============================================
			if( tempVirtualBall -> isBottom && d == 1 ) //�p�G�O�����Ay��V�N�����Q�ϧ@�ΤO��P�A�ҥH���έp��
			{
				std::pair< int , float > tempPair( bottomForceCount , 1.0 );
				tempPolynomial -> polynomial.insert( tempPair );
				tempVirtualBall -> bottomForceIndex = bottomForceCount;
				std::cout << "�b��" << tempVirtualBall -> index << "���y�W�[�W�a�������ϤO#(" << bottomForceCount << ")" << std::endl;
				bottomForceCount--;
			}
			//==============================================
			for( int j = 0; j <= tempVirtualBall -> connectionAmount; j++ )
			{
				float parameter = zomeDirection -> GetDirectionParameter( tempVirtualBall -> connectFace[j] , d );//����i������d�Ӻ��ת��t��
				int connIndex = tempVirtualBall -> connIndex[j];//���o�ӭ���������@�ڴΤl
				if( abs(parameter) > 0.01 )
				{
					std::pair< int , float > tempPair( connIndex , parameter );
					tempPolynomial -> polynomial.insert( tempPair );
				}
				if( d == 1 )//�[�W�o��Τl���������q/2�V�U�A�[�J�`�ƶ�
				{
					float constant = tempPolynomial -> polynomial.at( -1 );
					constant -= zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
															tempVirtualBall -> conn[j] -> size ) * 0.5f;
					//std::pair< int , float > tempPair( -1 , constant );
					//tempPolynomial -> polynomial.insert( tempPair );
					tempPolynomial -> polynomial.find( -1 ) -> second = constant;

					totalForceDown -= zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
																  tempVirtualBall -> conn[j] -> size ) * 0.5f;//�a�W���U�O
					gravityPosition[0] -= tempVirtualBall -> position[0] *
										  zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
																	  tempVirtualBall -> conn[j] -> size ) * 0.5f;//�⭫�ߥ�
					gravityPosition[2] -= tempVirtualBall -> position[2] *
										  zomeDirection -> GetWeight( tempVirtualBall -> conn[j] -> color , 
																	  tempVirtualBall -> conn[j] -> size ) * 0.5f;///�⭫�ߥ�


					std::cout << "��X�@�ө��y�伵�A��m�b(" << tempVirtualBall -> position[0] << ","
							  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")" << std::endl;
				}
			}
			if( d == 1 )//��H�������q�[�W�h
			{
				float constant = tempPolynomial -> polynomial.at( -1 );
				constant -= SEGMENT_WEIGHT;
				//std::pair< int , float > tempPair( -1 , constant );
				//tempPolynomial -> polynomial.insert( tempPair );
				tempPolynomial -> polynomial.find( -1 ) -> second = constant;

				totalForceDown -= SEGMENT_WEIGHT;
				gravityPosition[0] -= tempVirtualBall -> position[0] * SEGMENT_WEIGHT;//�⭫�ߥ�
				gravityPosition[2] -= tempVirtualBall -> position[2] * SEGMENT_WEIGHT;
			}
			
			//�A��h�����[�J��C�A�p�G�o�Ӧh�����z���S���F��N���n�[
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

	//��q�����
	/*
	//AnalyzeBottomForce();
	//�إߩ��y�ϤO�����y�СA�øѥX�ϤO�C
	gravityPosition[0] /= totalForceDown;
	gravityPosition[2] /= totalForceDown;
	std::cout << "���R�������O�G���߮y�Цb(" << gravityPosition[0] << "," << gravityPosition[2] << ")" << std::endl;
	//�N�C�@�ө��������ର���y��
	//originalPolarCoor -> ComputePolarPosition( gravityPosition );
	originalPolarCoor = originalPolarCoor -> Preprocess_OriginalPolorCoor( gravityPosition );
	//�إߨC�@�ӤϤO����
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		PolynomialList* tempPolynomial = new PolynomialList();
		debugCount++;
		tempPolynomial -> polynomial.find( -1 ) -> second = totalForceDown * p -> ForceRate();//�N�Ѻ⦨�`��/�C�ө��y����v
		//�N���y�ϤO���s���[�J�h���O -X * 1 = �ϤO
		std::pair< int , float > tempPair( p -> virtualBall -> bottomForceIndex , 1.0 );
		tempPolynomial -> polynomial.insert( tempPair );
		std::cout << "��" << p -> virtualBall -> bottomForceIndex << "�ӤϤO����v���G" << p -> ForceRate() << std::endl;
		//�[�J�h������
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



	//�o��}�l�gVirtualFroceAnalysis���غc�l
	int ballAmount = 0;
	for( ZomeStructure* b = originalBall; b != NULL; b = b -> link ) ballAmount++;
	ballAmount += virtualBalls.size();
	VirtualForceAnalyzer* virtualForceAnalyzer = new VirtualForceAnalyzer( firstPolynomial , ballAmount , originalConn , originalBall );



	//��X�������X�ڱ�l���j���]��0
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
	
	//��X�S����L���s�Q��{��
	FILE* formulaFile = fopen("Structure Formula Original.txt", "w");//��X��Ӻ⦡�����ɸ�
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		fprintf(formulaFile, "��%d���G", p -> index );
		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
		{
			if( i -> first == -1 )
			{
				fprintf(formulaFile, "%f", i -> second );//��X�`�ƶ�
			}
			else
			{
				fprintf(formulaFile, "%f �� (%d)", i -> second , i -> first );//��X��i�ڪ��Y��
			}
			if ( ++i != p -> polynomial.end() )//�ݦo�O���O�̫�@��
			{
				fprintf(formulaFile, " �� " );
			}
			--i;//������ܦ^�h
		}
		fprintf(formulaFile," �� 0\n");
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
		std::cout << "��" << solveTimes << "���Ѥ�{��" << std::endl;
		for( PolynomialList* p1 = firstPolynomial; p1 != NULL; p1 = p1 -> link )
		{
			for( PolynomialList* p2 = firstPolynomial; p2 != NULL; p2 = p2 -> link )
			{
				if( p1 == p2 ) continue;
				if( p2 -> polynomial.size() == 1 ) continue;
				if( p2 -> IsOneUnknown() && //�Ȧ��@�ӥ�����
					std::find( solvedUnknown.begin() , solvedUnknown.end() , p2 -> index ) == solvedUnknown.end() )
				{
					p2 -> Substitute();//�p�G�u�Ѥ@���A�N��o�ӸѥN�J�Ҧ����l��
					solvedUnknown.push_back( p2 -> index );
					noMoreSolution = false;
					continue;
				}
				////��p2�h���hp1�A�ҥHp2���Ҧ���p1���n���Ap1�]�tp2
				if( p1 -> Include( p2 ) )
				{//�]�t�� -> Include( �Q�]�t�� )
					p2 -> Eliminate( p1 );//p2�h����p1����
					//���h�� -> Eliminate( �Q���h�� )
					noMoreSolution = false;
				}
				//�o�q�����A���i�H�Ȯ��h���|�����A�n�C�@�����d��
				/*int overlappedTerm = p1 -> Overlap( p2 );//��ӬO�l�������|�A�N�⨺�Ӷ�����
				if( overlappedTerm != -1 )
				{
					p2 -> Eliminate( p1 , overlappedTerm );
					noMoreSolution = false;
				}*//*
				//2/15�j�p�ҥ����ƪ��p�ߤ�{��
				//�T�{�O���O���@�ӥ����ƬۦP�A�@�ӥ����Ƥ��ۦP
				if( p1 -> OneSameUnknownAmongTwo( p2 ) )
				{
					if( p1 -> index == 20 && p2 -> index == 49 )
					{
						std::cout << "debug:�M��20�M49��" << std::endl;
					}
					//�n���ˬd�o�Ӧ��l���S�����ŹL
					bool ifMinimalist = true;
					for( PolynomialList* p3 = firstPolynomial; p3 != NULL; p3 = p3 -> link )
					{
						if( p3 -> polynomial.size() != 3 ) continue;//�u����פ@�˦�3���h����
						if( p1 -> Include( p3 ) && p3 -> Include( p1 ) && p3 != p1 )
						{//�]�t�� -> Include( �Q�]�t�� )
							ifMinimalist = false;
						}
					}
					if( ifMinimalist ) //===============================���U�Ӥ~�}�l�M��s��
					{
						std::vector< PolynomialList* >* chainPolynomial = new std::vector< PolynomialList* >();
						if( p1 -> FindChainPolynomial( chainPolynomial , p2 ) )
						{
							p1 -> ChainSubstitute( chainPolynomial , p2 );

							p2 -> Eliminate( p1 );//p2�h����p1����
							//���h�� -> Eliminate( �Q���h�� )
							noMoreSolution = false;
						}
						delete chainPolynomial;
					}
				}
				if( p1 -> IsOneUnknown() && //�Ȧ��@�ӥ�����
					std::find( solvedUnknown.begin() , solvedUnknown.end() , p1 -> index ) == solvedUnknown.end() )//�o�Ӧ��l�٨S�N�J�L
				{
					p1 -> Substitute();//�p�G�u�Ѥ@���A�N��o�ӸѥN�J�Ҧ����l��
					solvedUnknown.push_back( p1 -> index );
					noMoreSolution = false;
				}
			}
		}

		for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
		{
			if( !p -> IsOneUnknown() && !p -> IsEmpty() )//�p�G������@�Ӧ��l�٨S�ѧ��A�N�~��while�j��
			{
				break;
			}
			if( p -> link == NULL )//�C�Ӧ��l���ѧ��F�N����while�j��
			{
				solutionDone = true;
			}
		}
		solveTimes++;
	}*/
	//std::cout << "==============================================" << std::endl;
	//std::cout << "==============================================" << std::endl;
	//std::cout << "===================��X�h�������G====================" << std::endl;
	//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i
	formulaFile = fopen("StructureFormula.txt", "w");//��X��Ӻ⦡�����ɸ�
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		fprintf(formulaFile, "��%d���G", p -> index );
		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
		{
			if( i -> first == -1 )
			{
				fprintf(formulaFile, "%f", i -> second );//��X�`�ƶ�
			}
			else
			{
				fprintf(formulaFile, "%f �� (%d)", i -> second , i -> first );//��X��i�ڪ��Y��
			}
			if ( ++i != p -> polynomial.end() )//�ݦo�O���O�̫�@��
			{
				fprintf(formulaFile, " �� " );
			}
			--i;//������ܦ^�h
		}
		fprintf(formulaFile," �� 0\n");
		//p -> index = polynomialCount;
		//polynomialCount++;
		p -> PrintPolynomialList();
	}
	fclose( formulaFile );
	//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i
	//std::cout << "==============================================" << std::endl;
	//std::cout << "==============================================" << std::endl;
	virtualForceAnalyzer -> Analyze();

	std::cout << "==============================================" << std::endl;
	std::cout << "==============================================" << std::endl;
	std::cout << "===================��\�k���R���G��====================" << std::endl;
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		p -> PrintPolynomialList();
		for( PolynomialList* p2 = firstPolynomial; p2 != NULL; p2 = p2 -> link )
		{
			if( p -> polynomial.size() != 2 || p2 -> polynomial.size() != 2 ) continue;
			if( p -> GetOneUnknown() == p2 -> GetOneUnknown() && p != p2 )
			{
				std::cout << "�����P��" << p2 -> index << "���@�ˡG";
				p2 -> PrintPolynomialList();
			}
		}
	}
	std::cout << "==============================================" << std::endl;
	std::cout << "==============================================" << std::endl;

	//debug��X���G
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
			std::map< int , float >::iterator iter = p -> polynomial.begin();//�Ĥ@����<-1,�`��>�A�ҥH�����L
			if( iter -> first == -1 )
			{
				float solution = iter -> second;
				iter++;
				std::cout << "��" << iter -> first << "�ڤ�[���O�q�O" << solution / iter -> second << "g" << std::endl;
			}
			else//�Ĥ@����<�ϤO�s��,1>�A�ĤG���O<-1,�`��>
			{
				std::cout << "��" << -iter -> first << "�ӤϤO���O�q�O";
				iter++;
				std::cout << iter -> second << "g" << std::endl;
			}
		}
		else
		{
			std::cout << "(�����|���ѥX)";
			p -> PrintPolynomialList();
		}
	}

	

}