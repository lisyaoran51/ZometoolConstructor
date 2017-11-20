#include "ObjFunction.h"


#pragma region ObjHollower
ObjHollower::ObjHollower( Model* m )
{
	/*
	bool pattern[256][8];
	GLfloat normal[256][3];

	InitializeNormalPattern( pattern , normal );
	*/
	model = m;
	/*//debug��
	Point* tp = new Point( -45 , -4 , -43 );
	Grid* test = new Grid( tp );
	CheckEngaged( test );
	*/
	count = 0;

	int sampleNumber = GetSampleNumber( model );

	std::cout << "�ҫ������I�Ʀ@��" << sampleNumber << "�I" << std::endl;

	for( int i = 0; i < 10; i++ )
	{
		std::cout << "�ثe���˲�" << i << "�I" << std::endl;

		Point* centerPoint = new Point();
		RandomInnerPoint( model , centerPoint );//�H���ͦ��@�Ӹ̭����I
		Grid* grid = new Grid( centerPoint );
		if ( !IfGridCollide( grid ) )
		{
			std::cout << "�ͦ��s��grid��" << std::endl;
			//�Y�٨S���ͦ��I �o���I�N��Ĥ@���I
			if ( firstGrid == NULL )
			{
				firstGrid = grid;
			}
			else//�����ܡA�N��̫�@�Ӥ���s��o�ӲĤ@�Ӥ��
			{
				tempGrid -> link = grid;
			}

			//��̫�@�Ӥ��tempgrid��s
			Grid* lastGrid;
			for ( Grid* j = grid; j != NULL; j = j -> link )
			{
				j -> firstGrid = firstGrid;
				if ( j -> link == NULL )
					tempGrid = j;
			}

		}
		else
		{//����grid���R��
			std::cout << "�o�����P���e����" << std::endl;
			DeleteGrid( grid );
		}
	}

	for( Grid* i = firstGrid; i != NULL; i = i -> link )
	{
		i -> engagedInDistance = i -> CheckEngagedInDistance( ENGAGE_DISTANCE );
	}



	GenerateGridSurface();
}

int ObjHollower::GetSampleNumber( Model* model )
{
	GLfloat sizeOfModel = 0.f;
	

	for ( int i = 0 ; i < model -> verticeSize; i++ )
	{
		if( abs( ( ( *( model -> vertices ) )[ i ] ) ) > abs( sizeOfModel ) )//�o��X��
		{
			sizeOfModel = ( *( model -> vertices ) )[ i ];
		}
	}
	int sampleNumber = pow( (int)sizeOfModel / 50 , 3 );//�C50mm���ˤ@���I

	return sampleNumber;

}

void ObjHollower::RandomInnerPoint_Old( Model* model , Point* randomPoint )
{
	GLfloat sizeOfModel = 0.f;
	for ( int i = 0 ; i < model -> verticeSize; i++ )
	{
		if( abs( ( ( *( model -> vertices ) )[ i ] ) ) > abs( sizeOfModel ) )
		{
			sizeOfModel = ( *( model -> vertices ) )[ i ];
		}
	}
	
	srand( ( unsigned ) time ( NULL) );

	bool isInside = false;
	Point* innerPoint = new Point();

	while( !isInside )
	{
		//std::cout << "���s���ˤ�" << std::endl;
		int x,y,z;
		x = (rand() + rand() * 32768) % ((int)sizeOfModel*1000000) * ( ( rand() % 2 == 0 ) ? -1 : 1 );
		y = (rand() + rand() * 32768) % ((int)sizeOfModel*1000000) * ( ( rand() % 2 == 0 ) ? -1 : 1 );
		z = (rand() + rand() * 32768) % ((int)sizeOfModel*1000000) * ( ( rand() % 2 == 0 ) ? -1 : 1 );
		innerPoint = new Point( (GLfloat)x * 0.000001 , (GLfloat)y * 0.000001 , (GLfloat)z * 0.000001 );
		
		GLfloat minDistance = 999999999;
		GLuint minIndex;
		for( int i = 0; i < model -> verticeSize / 3; i++ )
		{
			int tempDistance = pow( ( *( model -> vertices ) )[ i * 3 ] - innerPoint -> position[0] , 2 ) + 
							   pow( ( *( model -> vertices ) )[ i * 3 + 1 ] - innerPoint -> position[1] , 2 ) + 
							   pow( ( *( model -> vertices ) )[ i * 3 + 2 ] - innerPoint -> position[2] , 2 );
			if ( tempDistance < minDistance )
			{
				minIndex = i;
				minDistance = tempDistance;
			}
		}
		//minVector�O�q�I��ҫ��I���̤p�Z��
		Point* minVector = new Point( ( *( model -> vertices ) )[ minIndex * 3 ] - innerPoint -> position[0] , 
									  ( *( model -> vertices ) )[ minIndex * 3 + 1 ] - innerPoint -> position[1] , 
									  ( *( model -> vertices ) )[ minIndex * 3 + 2 ] - innerPoint -> position[2] );
		//�q�H���I��mesh���V�q

		//�p�G�H���I�Ӿa��mesh�A�N����@���I
		if ( minVector -> Absolution() < sqrt(3) * GRID_SCALE ) continue;

		GLfloat minCos = 0;
		GLuint minFace;
		Point* minCross = new Point();
		//std::cout << "�d�ݬO���O�ҫ������I";
		for( int i = 0; i < model -> indiceSize / 3; i++ )
		{
			if ( ( *( model -> indices ) )[ i * 3 ] == minIndex || 
				 ( *( model -> indices ) )[ i * 3 + 1 ] == minIndex || 
				 ( *( model -> indices ) )[ i * 3 + 2 ] == minIndex )
			{
				/*
				std::cout << "�����I1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
				std::cout << "�����I2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 2 ] << std::endl;
				std::cout << "�����I3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 2 ] << std::endl;
										   */
				//std::cout << ".";
				//p1p2 �M p1p3 ���� v1 �M v2
				Point* vector1 = new Point( ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 1 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 2 ] );

				Point* vector2 = new Point( ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 1 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 2 ] );
				//���T���I���k�V�q
				Point* normal1 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
				Point* normal2 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
				Point* normal3 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
				//�������@��
				Point* averageNormal = new Point( normal1 -> position[0] +  normal2 -> position[0] + normal3 -> position[0] ,
												  normal1 -> position[1] +  normal2 -> position[1] + normal3 -> position[1] ,
												  normal1 -> position[2] +  normal2 -> position[2] + normal3 -> position[2] );

				
				//�~��v1 v2 �o��ӭ��k�V�qcross
				Point* cross = new Point();
				MathHelper::VectorCorss( cross -> position , vector1 -> position , vector2 -> position );
				cross -> Normalize();

				if ( !cross -> Dot( averageNormal ) > 0 )
				{
					cross -> Reverse();
				}

				cross -> Normalize();
				minVector -> Normalize();
				GLfloat tempCos = cross -> Dot( minVector );


				if ( abs( cross -> Dot( minVector ) ) > minCos )//�p�G�k�V�q�P���I��mesh���V�q���n�̤j�A�N��ܳ̾a�񪺭�
				{												//�b��o�ӭ����k�V�q���n�ݬO���٬O�t�A�N���D�b���٦b�~
					minCos = abs( cross -> Dot( minVector ) );
					minFace = i;
					minCross -> Copy( cross );
				}
			}
		}
		//std::cout << std::endl;
		//�b��o�ӭ����k�V�q���n�ݬO���٬O�t�A�N���D�b���٦b�~
		if ( minCross -> Dot( minVector ) > 0 )//�����N�O�I�b�ҫ���
		{
			isInside = true;
		}


	}
	randomPoint -> Copy( innerPoint );
}

void ObjHollower::RandomInnerPoint( Model* model , Point* randomPoint )
{
	GLfloat sizeOfModel = 0.f;
	for ( int i = 0 ; i < model -> verticeSize; i++ )
	{
		if( abs( ( ( *( model -> vertices ) )[ i ] ) ) > abs( sizeOfModel ) )
		{
			sizeOfModel = ( *( model -> vertices ) )[ i ];
		}
	}
	
	srand( ( unsigned ) time ( NULL ) );

	bool isInside = false;
	Point* innerPoint = new Point();

	while( !isInside )
	{
		//std::cout << "���s���ˤ�" << std::endl;
		int x,y,z;
		x = rand() % ( (int)( sizeOfModel / GRID_SCALE ) ) * GRID_SCALE * ( ( rand() % 2 == 0 ) ? -1 : 1 );
		y = rand() % ( (int)( sizeOfModel / GRID_SCALE ) ) * GRID_SCALE * ( ( rand() % 2 == 0 ) ? -1 : 1 );
		z = rand() % ( (int)( sizeOfModel / GRID_SCALE ) ) * GRID_SCALE * ( ( rand() % 2 == 0 ) ? -1 : 1 );
		delete innerPoint;
		innerPoint = NULL;
		innerPoint = new Point( (GLfloat)x , (GLfloat)y , (GLfloat)z );
		

		GLfloat minDistance = 999999999;
		GLuint minIndex;
		
		for( int i = 0; i < model -> verticeSize / 3; i++ )
		{
			int tempDistance = pow( ( *( model -> vertices ) )[ i * 3 ] - innerPoint -> position[0] , 2 ) + 
							   pow( ( *( model -> vertices ) )[ i * 3 + 1 ] - innerPoint -> position[1] , 2 ) + 
							   pow( ( *( model -> vertices ) )[ i * 3 + 2 ] - innerPoint -> position[2] , 2 );
			if ( tempDistance < minDistance )
			{
				minIndex = i;
				minDistance = tempDistance;
			}
		}
		//minVector�O�q�I��ҫ��I���̤p�Z��
		
		Point* minVector = new Point( ( *( model -> vertices ) )[ minIndex * 3 ] - innerPoint -> position[0] , 
									  ( *( model -> vertices ) )[ minIndex * 3 + 1 ] - innerPoint -> position[1] , 
									  ( *( model -> vertices ) )[ minIndex * 3 + 2 ] - innerPoint -> position[2] );
		//�q�H���I��mesh���V�q
		
		//�p�G�H���I�Ӿa��mesh�A�N����@���I
		if ( minVector -> Absolution() < sqrt(3) * GRID_SCALE )
		{
			delete minVector;
			delete innerPoint;
			innerPoint = NULL;
			continue;
		}

		GLfloat minCos = 0;
		GLuint minFace;
		Point* minCross = new Point();
		GLfloat minDist2Surface = 999999999;
		
		bool isTooCloseToFace = false;
		//std::cout << "�d�ݬO���O�ҫ������I";
		for( int i = 0; i < model -> indiceSize / 3; i++ )
		{
			if ( ( *( model -> indices ) )[ i * 3 ] == minIndex || 
				 ( *( model -> indices ) )[ i * 3 + 1 ] == minIndex || 
				 ( *( model -> indices ) )[ i * 3 + 2 ] == minIndex )
			{
				
				/*
				std::cout << "�����I1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
				std::cout << "�����I2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 2 ] << std::endl;
				std::cout << "�����I3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 1 ] << "," << 
										   ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 2 ] << std::endl;
										   */
				//std::cout << ".";
				//p1p2 �M p1p3 ���� v1 �M v2
				Point* vector1 = new Point( ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 1 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 + 2 ] );

				Point* vector2 = new Point( ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 1 ] ,
											( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] - ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 + 2 ] );
				//�p���I�쥭�����Z��
				//d = N * P - N * P1
				//N = P2-P1 * P3-P1 Normalize
				Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();

				

				if( abs( normal -> Dot( innerPoint -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] ) ) < sqrt(3) * GRID_SCALE )
				{
					isTooCloseToFace = true; 
					delete vector1;
					delete vector2; 
					delete normal;
					break;
				}
				
				//���T���I���k�V�q
				Point* normal1 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
				Point* normal2 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
				Point* normal3 = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
				//�������@��
				Point* averageNormal = new Point( normal1 -> position[0] +  normal2 -> position[0] + normal3 -> position[0] ,
												  normal1 -> position[1] +  normal2 -> position[1] + normal3 -> position[1] ,
												  normal1 -> position[2] +  normal2 -> position[2] + normal3 -> position[2] );
				
				
				//�~��v1 v2 �o��ӭ��k�V�qcross
				Point* cross = new Point();
				MathHelper::VectorCorss( cross -> position , vector1 -> position , vector2 -> position );
				cross -> Normalize();

				if ( !cross -> Dot( averageNormal ) > 0 )
				{
					cross -> Reverse();
				}

				cross -> Normalize();
				minVector -> Normalize();
				GLfloat tempCos = cross -> Dot( minVector );


				/*
				if ( abs( cross -> Dot( minVector ) ) > minCos )//�p�G�k�V�q�P���I��mesh���V�q���n�̤j�A�N��ܳ̾a�񪺭�
				{												//�b��o�ӭ����k�V�q���n�ݬO���٬O�t�A�N���D�b���٦b�~
					minCos = abs( cross -> Dot( minVector ) );
					minFace = i;
					minCross -> Copy( cross );
				}*/
				//�o��~�O�諸 ���Z���̤p���� ���̱��񪺭�
				if( abs( normal -> Dot( innerPoint -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] ) ) < minDist2Surface )
				{
					minDist2Surface = abs( normal -> Dot( innerPoint -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] ) );
					minFace = i;
					minCross -> Copy( cross );
				}
				
				delete vector1;
				delete vector2;
				delete normal;
				delete normal1;
				delete normal2;
				delete normal3; 
				delete averageNormal;
				delete cross;
			}
		}
		//std::cout << std::endl;
		//�b��o�ӭ����k�V�q���n�ݬO���٬O�t�A�N���D�b���٦b�~
		if ( minCross -> Dot( minVector ) > 0 )//�����N�O�I�b�ҫ���
		{
			if( !isTooCloseToFace ) isInside = true;
		}

		delete minCross;
		delete minVector;
	}
	randomPoint -> Copy( innerPoint );
	std::cout << "random����l�I��m���G" << randomPoint -> position[0] << "," << randomPoint -> position[1] << "," << randomPoint -> position[2] << std::endl; 
	delete innerPoint;
}

bool ObjHollower::IfGridCollide( Grid* grid )
{
	for( Grid* j = grid -> firstGrid; j -> link != NULL; j = j -> link )
	{
		if( grid -> position[0] == j -> position[0] && 
			grid -> position[1] == j -> position[1] && 
			grid -> position[2] == j -> position[2] )
			std::cout << "bug";
	}


	std::cout<<"->";
	bool isCollided = false;
	for( int i = 0; i < 6; i++ )
	{
		
		//��6�Ӥ�V�s�W �p�G�s�W�L�α�Ĳ��~�� �N��
		if( !grid -> facePropagated[i] && !grid -> engagedFace[i] && !grid -> engaged )
		{
			//std::cout<<i;
			Grid* gridPropagated = new Grid( grid , i );//�o��̭� �p�G�o�Ӥ���S���P���e��������I
														//�N�|�۰ʧ�L�P�e��������۳s �åB��۾F�䳣��n
			count++;
			std::cout<<count;
			//�ˬd���S������
			if ( gridPropagated -> CheckDuplicated() )
			{
				std::cout << "X" ;
				DeleteGrid( gridPropagated );
				continue;
			}
			else
			{ 
				if( !gridPropagated )
					std::cout<<"?";
				if( !gridPropagated -> position )
					std::cout<<"?";

				std::cout << ":" << gridPropagated -> position[0] << " , " << gridPropagated -> position[1] << " , " << gridPropagated -> position[2];
				//�o�Ӥ���S������
				//�d�ݳo�Ӥ�����S���I���� �åB�⦳�S���I�����J��engaged�ȸ̭�
				CheckEngaged( gridPropagated );

				for( Grid* j = firstGrid; j != NULL; j = j -> link )
				{//�d�ݦ��S�����e�O�������I�����
					if( gridPropagated -> Compare ( j ) )
						return true;
				}
				std::cout << std::endl;
				//std::cout << "�o��grid��engaged���G" << grid -> engaged << std::endl;
				
				isCollided |= IfGridCollide( gridPropagated );
			}
		}
	}

	return isCollided;

}

bool ObjHollower::CheckEngaged_Old( Grid* grid )
{
	//std::cout<<"C";
	bool isEngaged = false;
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //�X��
	{
		//�p�G�ҫ������o�ӭ����������1mm
		//�o��g���F ���ӬO�������I���Z������ڸ�3 ���O�I���I
		//
		if( MathHelper::VectorDistance( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ]  , grid -> position ) < sqrt(3) || 
			MathHelper::VectorDistance( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] , grid -> position ) < sqrt(3) ||
			MathHelper::VectorDistance( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] , grid -> position ) < sqrt(3) )
		{
			std::cout << "�d���I�ۥ�:�I1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 2 ] << std::endl;
			std::cout << "�d���I�ۥ�:�I2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 2 ] << std::endl;
			std::cout << "�d���I�ۥ�:�I3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 2 ] << std::endl;
										   
			//�g���F
			/*
			isEngaged |= grid -> CheckIntersect ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
												  &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] ,
											 	  &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			isEngaged |= grid -> CheckIntersect ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
												  &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] );
			isEngaged |= grid -> CheckIntersect ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
												  &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			isEngaged |= grid -> CheckIntersect ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] ,
												  &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );*/

			
		}
	}
	if( isEngaged ) 
		std::cout << "C";
	grid -> engaged = isEngaged;
	return isEngaged;
	//grid -> CheckIntersect( )
}

bool ObjHollower::CheckEngaged( Grid* grid )
{
	//std::cout<<"C";
	bool isEngaged = false;
	//std::cout << "�d���I�ۥ�";
	for( int i = 0; i < model -> indiceSize / 3; i++ ) //�X��
	{
		/*
		std::cout << "�j�M��1:" << ( *( model -> indices ) )[ i * 3 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 1 ] << ","
								<< ( *( model -> indices ) )[ i * 3 + 2 ] << std::endl;
		std::cout << "�j�M�I1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 1 ] << ","
								<< ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 + 2 ] << std::endl;
		*/
		//�p�G�ҫ������o�ӭ����������1mm
		//�o��g���F ���ӬO�������I���Z������ڸ�3 ���O�I���I
		//d = N * P - N * P1
		//N = P2-P1 * P3-P1 Normalize
		Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		/*
		GLfloat dddd = abs( normal -> Dot( grid -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) );
		GLfloat vvvv = ( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 )[0];
		*/
		if( abs( normal -> Dot( grid -> position ) - normal -> Dot( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 ) ) < sqrt(3) * GRID_SCALE )
		{
			//std::cout << ".";
			/*
			std::cout << "�d���I�ۥ�:�I1:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] + 2 ] << std::endl;
			
			std::cout << "�d���I�ۥ�:�I2:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] + 2 ] << std::endl;
			std::cout << "�d���I�ۥ�:�I3:" << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 1 ] << ","
									       << ( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] + 2 ] << std::endl;
				*/						   
			//�g���F
			
			isEngaged |= grid -> CheckIntersectGridOnTri ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] ,
											 			   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] );
			/*
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] );
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			isEngaged |= grid -> CheckIntersectTriOnGrid ( &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] ] ,
														   &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] ] );
			*/
		}
		delete vector1;
		delete vector2;
		delete normal;
	}
	if( isEngaged ) 
		std::cout << "�A������";
	grid -> engaged = isEngaged;
	return isEngaged;
	//grid -> CheckIntersect( )
}

bool ObjHollower::CheckCornerDuplicated_Old( Grid* grid , int cornerNumber , int *cornerIndex )
{


	switch( cornerNumber )
	{
		case 0:
			//�Y�X
			if( grid -> neighborGrid[1] -> engagedInDistance )//����
			{
				if( grid -> neighborGrid[3] -> engagedInDistance )//�U��
				{//�L��L��
					//�p�G��0�����@�Y�X
					if( grid -> neighborGrid[5] -> engagedInDistance )//�ᦳ
					{
						grid -> corner [ 0 ] -> index = *cornerIndex;
						*cornerIndex++;
					}//�ᦳ
					else
					{//��S�� 
						if( grid -> neighborGrid[5] -> 
									neighborGrid[1] -> engagedInDistance )//����//�U��//��S��//��@���� 
						{
							//�p�G��0�M5���۾F�������1���@���W�l
							//��@
							if( grid -> neighborGrid[5] -> 
										neighborGrid[3] -> engagedInDistance )//��@�U��
							{
								int tempIndex = -1;
								if( grid -> neighborGrid[5] -> corner [ 1 ] -> index > tempIndex )
								{
									tempIndex = grid -> neighborGrid[5] -> corner [ 1 ] -> index;
								}
								if( tempIndex > -1 )
								{//�p�G�۾F�I�w�g���s���A�N��o����I���]���o�ӽs��
									grid -> corner [ 0 ] -> index = tempIndex;
									grid -> neighborGrid[5] -> corner [ 1 ] -> index = tempIndex;
								}
								else
								{//�p�G�۾F�I�S���s���A�N��o����I���]�s���s��
									grid -> corner [ 0 ] -> index = *cornerIndex;
									grid -> neighborGrid[5] -> corner [ 1 ] -> index = *cornerIndex;
									*cornerIndex++;
								}
							}//=======================��@�U��
							else if( grid -> neighborGrid[5] -> 
											 neighborGrid[3] ->
											 neighborGrid[1] -> engagedInDistance )//��U�@����~~~~~~~~~~~~~~~~~~~
							{//�ܦ��@�ӭ˭Ȩ�
								int tempIndex = -1;
								if( grid -> neighborGrid[5] -> corner [ 1 ] -> index > tempIndex )
								{
									tempIndex = grid -> neighborGrid[5] -> corner [ 1 ] -> index;
								}
								if( grid -> neighborGrid[5] -> 
											neighborGrid[3] -> corner [ 3 ] -> index > tempIndex )
								{
									tempIndex = grid -> neighborGrid[5] -> 
														neighborGrid[3] -> corner [ 3 ] -> index;
								}
								if( tempIndex > -1 )
								{//�p�G�۾F�I�w�g���s���A�N��o�T���I���]���o�ӽs��
									grid -> corner [ 0 ] -> index = tempIndex;
									grid -> neighborGrid[5] -> corner [ 1 ] -> index = tempIndex;
									grid -> neighborGrid[5] -> 
											neighborGrid[3] -> corner [ 3 ] -> index = tempIndex;
								}
								else
								{//�p�G�۾F�I�S���s���A�N��o�T���I���]�s���s��
									grid -> corner [ 0 ] -> index = *cornerIndex;
									grid -> neighborGrid[5] -> corner [ 1 ] -> index = *cornerIndex;
									grid -> neighborGrid[5] -> 
											neighborGrid[3] -> corner [ 3 ] -> index = *cornerIndex;
									*cornerIndex++;
								}

							}//��U�@����~~~~~~~~~~~~~~~~~~~

						}//��@���� 
						else//����//�U��//��S��//��@���S��
						{
							if( grid -> neighborGrid[5] -> 
										neighborGrid[1] -> engagedInDistance )//����//�U��//��S��//��@���S��//��@�U��
							{}
							
						}//����//�U��//��S��//��@���S�� 

					}//��S��
				}//�U��


			}//����

	
	}
	return false;
}

bool ObjHollower::CheckCornerDuplicated( Grid* grid , int cornerNumber , int *cornerIndex )
{
	//���٨S���s�����I�A�d�@�M�Ҧ���t�I�A���䤸�I���ЬۦP��������W�P�˪��s��
	if( grid -> corner[ cornerNumber ] -> index == -1 )
	{
		grid -> corner[ cornerNumber ] -> index = *cornerIndex;
		for( Grid* i = grid -> firstGrid; i != NULL; i = i -> link )
		{
			for ( int j = 0; j < 6; j++ )
			{
				if( !i -> engagedInDistance && i -> neighborGrid[j] -> engagedInDistance )
				{
					//surfaceFace++;

					switch( j )
					{
						case 0:
							if( i -> corner[4] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[4] -> index = *cornerIndex;
							if( i -> corner[5] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[5] -> index = *cornerIndex;
							if( i -> corner[6] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[6] -> index = *cornerIndex;
							if( i -> corner[7] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[7] -> index = *cornerIndex;
							break;
						case 1:
							if( i -> corner[0] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[0] -> index = *cornerIndex;
							if( i -> corner[1] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[1] -> index = *cornerIndex;
							if( i -> corner[2] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[2] -> index = *cornerIndex;
							if( i -> corner[3] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[3] -> index = *cornerIndex;
							break;
						case 2:
							if( i -> corner[2] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[2] -> index = *cornerIndex;
							if( i -> corner[3] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[3] -> index = *cornerIndex;
							if( i -> corner[6] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[6] -> index = *cornerIndex;
							if( i -> corner[7] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[7] -> index = *cornerIndex;
							break;
						case 3:
							if( i -> corner[0] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[0] -> index = *cornerIndex;
							if( i -> corner[1] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[1] -> index = *cornerIndex;
							if( i -> corner[4] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[4] -> index = *cornerIndex;
							if( i -> corner[5] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[5] -> index = *cornerIndex;
							break;
						case 4:
							if( i -> corner[1] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[1] -> index = *cornerIndex;
							if( i -> corner[3] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[3] -> index = *cornerIndex;
							if( i -> corner[5] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[5] -> index = *cornerIndex;
							if( i -> corner[7] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[7] -> index = *cornerIndex;
							break;
						case 5:
							if( i -> corner[0] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[0] -> index = *cornerIndex;
							if( i -> corner[2] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[2] -> index = *cornerIndex;
							if( i -> corner[4] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[4] -> index = *cornerIndex;
							if( i -> corner[6] -> Compare( grid -> corner[ cornerNumber ] ) )
								i -> corner[6] -> index = *cornerIndex;
							break;
					}
				}
			
			}
		}
		(*cornerIndex)++;
		return false;
	}
	else//���I�w�g�Q�s�L��
	{
		return true;
	}


}

bool ObjHollower::CheckCornerSource()
{
	for( Grid* i = firstGrid; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < 6; j++ )
		{
			//���C�@�ӥ~�䭱 �d�ݥL���|�ӳ��I�O�Ӧۤ����I ������pointSource�̭�
			if( !i -> engagedInDistance && i -> neighborGrid[j] -> engagedInDistance )
			{
				//surfaceFace++;

				switch( j )
				{
					case 0:
						cornerSource[ i -> corner[4] -> index ][4] = true;
						cornerSource[ i -> corner[5] -> index ][5] = true;
						cornerSource[ i -> corner[6] -> index ][6] = true;
						cornerSource[ i -> corner[7] -> index ][7] = true;
						break;
					case 1:
						cornerSource[ i -> corner[0] -> index ][0] = true;
						cornerSource[ i -> corner[1] -> index ][1] = true;
						cornerSource[ i -> corner[2] -> index ][2] = true;
						cornerSource[ i -> corner[3] -> index ][3] = true;
						break;
					case 2:
						cornerSource[ i -> corner[2] -> index ][2] = true;
						cornerSource[ i -> corner[3] -> index ][3] = true;
						cornerSource[ i -> corner[6] -> index ][6] = true;
						cornerSource[ i -> corner[7] -> index ][7] = true;
						break;
					case 3:
						cornerSource[ i -> corner[0] -> index ][0] = true;
						cornerSource[ i -> corner[1] -> index ][1] = true;
						cornerSource[ i -> corner[4] -> index ][4] = true;
						cornerSource[ i -> corner[5] -> index ][5] = true;
						break;
					case 4:
						cornerSource[ i -> corner[1] -> index ][1] = true;
						cornerSource[ i -> corner[3] -> index ][3] = true;
						cornerSource[ i -> corner[5] -> index ][5] = true;
						cornerSource[ i -> corner[7] -> index ][7] = true;
						break;
					case 5:
						cornerSource[ i -> corner[0] -> index ][0] = true;
						cornerSource[ i -> corner[2] -> index ][2] = true;
						cornerSource[ i -> corner[4] -> index ][4] = true;
						cornerSource[ i -> corner[6] -> index ][6] = true;
						break;
				}
			}
		}
	}
	return true;
}

bool ObjHollower::CompareCornerSource( bool* cornerSrc , bool* pattern )
{
	if( cornerSrc[0] == pattern[0] &&
		cornerSrc[1] == pattern[1] &&
		cornerSrc[2] == pattern[2] &&
		cornerSrc[3] == pattern[3] &&
		cornerSrc[4] == pattern[4] &&
		cornerSrc[5] == pattern[5] &&
		cornerSrc[6] == pattern[6] &&
		cornerSrc[7] == pattern[7] )
	{
		return true;
	}
	else return false;
}

void ObjHollower::InitializeNormalPattern( bool pattern[256][8] , GLfloat normal[256][3] )
{
	/*
	0:FFFFFFFF �L
	1:TFFFFFFF ��0
	2:FTFFFFFF ��1
	3:TTFFFFFF ��0 1
	4:FFTFFFFF ��2
	5:TFTFFFFF ��0 2
	6:FTTFFFFF ��1 2
	*/
	for( int i = 0; i < 256; i++ )
	{//��256�ܦ�2�i���s
		//�Ĥ@��O��0�� �ĤG��O��1�� �H������
		//�p13���G�i���O00001101 ���G�N�OTFTTFFFF
		if( int( i / pow( 2 , 0 ) ) - int( i / pow( 2 , 1 ) ) * 2 == 0 )
			pattern[i][0] = false;
		else
			pattern[i][0] = true;
		if( int( i / pow( 2 , 1 ) ) - int( i / pow( 2 , 2 ) ) * 2 == 0 )
			pattern[i][1] = false;
		else
			pattern[i][1] = true;
		if( int( i / pow( 2 , 2 ) ) - int( i / pow( 2 , 3 ) ) * 2 == 0 )
			pattern[i][2] = false;
		else
			pattern[i][2] = true;
		if( int( i / pow( 2 , 3 ) ) - int( i / pow( 2 , 4 ) ) * 2 == 0 )
			pattern[i][3] = false;
		else
			pattern[i][3] = true;
		if( int( i / pow( 2 , 4 ) ) - int( i / pow( 2 , 5 ) ) * 2 == 0 )
			pattern[i][4] = false;
		else
			pattern[i][4] = true;
		if( int( i / pow( 2 , 5 ) ) - int( i / pow( 2 , 6 ) ) * 2 == 0 )
			pattern[i][5] = false;
		else
			pattern[i][5] = true;
		if( int( i / pow( 2 , 6 ) ) - int( i / pow( 2 , 7 ) ) * 2 == 0 )
			pattern[i][6] = false;
		else
			pattern[i][6] = true;
		if( int( i / pow( 2 , 7 ) ) == 0 )
			pattern[i][7] = false;
		else
			pattern[i][7] = true;
	}
	for( int i = 0; i < 256; i++ )
	{
		MathHelper::SetVector( normal[i] , 0 , 0 , 0 );
	}
	//��W����V
	MathHelper::SetVector( normal[128] , -1 , -1 , -1 ); MathHelper::VectorNormalize( normal[128] );//��W��0
	MathHelper::SetVector( normal[64] , -1 , -1 , 1 ); MathHelper::VectorNormalize( normal[64] );//��W��1
	MathHelper::SetVector( normal[32] , -1 , 1 , -1 ); MathHelper::VectorNormalize( normal[32] );//��W��2
	MathHelper::SetVector( normal[16] , -1 , 1 , 1 ); MathHelper::VectorNormalize( normal[16] );//��W��3
	MathHelper::SetVector( normal[8] , 1 , -1 , -1 ); MathHelper::VectorNormalize( normal[8] );//��W��4
	MathHelper::SetVector( normal[4] , 1 , -1 , 1 ); MathHelper::VectorNormalize( normal[4] );//��W��5
	MathHelper::SetVector( normal[2] , 1 , 1 , -1 ); MathHelper::VectorNormalize( normal[2] );//��W��6
	MathHelper::SetVector( normal[1] , 1 , 1 , 1 ); MathHelper::VectorNormalize( normal[1] );//��W��7
	//��X����V
	for( int i = 0; i < 256; i++ )
	{
		if( int( i / pow( 2 , 0 ) ) - int( i / pow( 2 , 1 ) ) * 2 == 1 )//��1���O1 0000000"1"~1111111"1"
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[1] );
		if( int( i / pow( 2 , 1 ) ) - int( i / pow( 2 , 2 ) ) * 2 == 1 )//��2���O1 000000"1"0~111111"1"1
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[2] );
		if( int( i / pow( 2 , 2 ) ) - int( i / pow( 2 , 3 ) ) * 2 == 1 )
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[4] );
		if( int( i / pow( 2 , 3 ) ) - int( i / pow( 2 , 4 ) ) * 2 == 1 )
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[8] );
		if( int( i / pow( 2 , 4 ) ) - int( i / pow( 2 , 5 ) ) * 2 == 1 )
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[16] );
		if( int( i / pow( 2 , 5 ) ) - int( i / pow( 2 , 6 ) ) * 2 == 1 )
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[32] );
		if( int( i / pow( 2 , 6 ) ) - int( i / pow( 2 , 7 ) ) * 2 == 1 )
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[64] );
		if( int( i / pow( 2 , 7 ) ) == 1 )
			MathHelper::VectorPlus( normal[i] , normal[i] , normal[128] );

		MathHelper::VectorNormalize( normal[i] );

		std::cout << "��" << i << "�ب����k�V�q�O:" << normal[i][0] << "," << normal[i][1] << "," << normal[i][2] << std::endl; 
	}
}

void ObjHollower::GenerateCornerNormal( Point* corner , bool pattern[256][8] , GLfloat normal[256][3] )
{
	//corner -> normal = new GLfloat[3];

	for( int i = 0 ; i < 256; i++ )
	{
		if( CompareCornerSource( cornerSource [ corner -> index ] , pattern[i] ) )
		{
			MathHelper::CopyVector( corner -> normal , normal[i] );
			break;
		}
	}

}

void ObjHollower::GenerateNormal()
{
	//���s�� ����Ҧ���normal��V�M�����˦�pattern�]�w�n
	bool pattern[256][8];
	GLfloat normal[256][3];

	InitializeNormalPattern( pattern , normal );

	
	for( Grid* i = firstGrid; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < 6; j++ )
		{
			//���C�@�ӥ~�䭱 �d�ݥL���|�ӳ��I�O�Ӧۤ����I ��|���I��CornerSource�˦� �ഫ���k�V�q
			if( !i -> engagedInDistance && i -> neighborGrid[j] -> engagedInDistance )
			{
				//surfaceFace++;

				switch( j )
				{
					case 0:
						GenerateCornerNormal( i -> corner[4] , pattern , normal );
						GenerateCornerNormal( i -> corner[5] , pattern , normal );
						GenerateCornerNormal( i -> corner[6] , pattern , normal );
						GenerateCornerNormal( i -> corner[7] , pattern , normal );
						break;
					case 1:
						GenerateCornerNormal( i -> corner[0] , pattern , normal );
						GenerateCornerNormal( i -> corner[1] , pattern , normal );
						GenerateCornerNormal( i -> corner[2] , pattern , normal );
						GenerateCornerNormal( i -> corner[3] , pattern , normal );
						break;
					case 2:
						GenerateCornerNormal( i -> corner[2] , pattern , normal );
						GenerateCornerNormal( i -> corner[3] , pattern , normal );
						GenerateCornerNormal( i -> corner[6] , pattern , normal );
						GenerateCornerNormal( i -> corner[7] , pattern , normal );
						break;
					case 3:
						GenerateCornerNormal( i -> corner[0] , pattern , normal );
						GenerateCornerNormal( i -> corner[1] , pattern , normal );
						GenerateCornerNormal( i -> corner[4] , pattern , normal );
						GenerateCornerNormal( i -> corner[5] , pattern , normal );
						break;
					case 4:
						GenerateCornerNormal( i -> corner[1] , pattern , normal );
						GenerateCornerNormal( i -> corner[3] , pattern , normal );
						GenerateCornerNormal( i -> corner[5] , pattern , normal );
						GenerateCornerNormal( i -> corner[7] , pattern , normal );
						break;
					case 5:
						GenerateCornerNormal( i -> corner[0] , pattern , normal );
						GenerateCornerNormal( i -> corner[2] , pattern , normal );
						GenerateCornerNormal( i -> corner[4] , pattern , normal );
						GenerateCornerNormal( i -> corner[6] , pattern , normal );
						break;
				}
			}
		}
	}
}

void ObjHollower::GenerateGridSurface()
{
	//���N�Ҧ����I�s��
	int surfaceFace = 0;
	int cornerIndex = 0;
	for( Grid* i = firstGrid; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < 6; j++ )
		{
			if( !i -> engagedInDistance && i -> neighborGrid[j] -> engagedInDistance )
			{
				surfaceFace++;

				switch( j )
				{
					case 0:
						CheckCornerDuplicated( i , 4 , &cornerIndex );
						CheckCornerDuplicated( i , 5 , &cornerIndex );
						CheckCornerDuplicated( i , 6 , &cornerIndex );
						CheckCornerDuplicated( i , 7 , &cornerIndex );
						break;
					case 1:
						CheckCornerDuplicated( i , 0 , &cornerIndex );
						CheckCornerDuplicated( i , 1 , &cornerIndex );
						CheckCornerDuplicated( i , 2 , &cornerIndex );
						CheckCornerDuplicated( i , 3 , &cornerIndex );
						break;
					case 2:
						CheckCornerDuplicated( i , 2 , &cornerIndex );
						CheckCornerDuplicated( i , 3 , &cornerIndex );
						CheckCornerDuplicated( i , 6 , &cornerIndex );
						CheckCornerDuplicated( i , 7 , &cornerIndex );
						break;
					case 3:
						CheckCornerDuplicated( i , 0 , &cornerIndex );
						CheckCornerDuplicated( i , 1 , &cornerIndex );
						CheckCornerDuplicated( i , 4 , &cornerIndex );
						CheckCornerDuplicated( i , 5 , &cornerIndex );
						break;
					case 4:
						CheckCornerDuplicated( i , 1 , &cornerIndex );
						CheckCornerDuplicated( i , 3 , &cornerIndex );
						CheckCornerDuplicated( i , 5 , &cornerIndex );
						CheckCornerDuplicated( i , 7 , &cornerIndex );
						break;
					case 5:
						CheckCornerDuplicated( i , 0 , &cornerIndex );
						CheckCornerDuplicated( i , 2 , &cornerIndex );
						CheckCornerDuplicated( i , 4 , &cornerIndex );
						CheckCornerDuplicated( i , 6 , &cornerIndex );
						break;
				}
			}
		}
	}

	if( cornerIndex < 1 )
		std::cout << "���~" << std::endl;
	//�N�s�������I�ƶq����model
	int indiceSize = surfaceFace * 2;
	innerSurface = new Model( cornerIndex * 3 , cornerIndex * 3 , indiceSize * 3 );

	//�p��Ҧ��I���k�V�q
	cornerSource = new bool*[ cornerIndex ];
	for ( int i = 0; i < cornerIndex; i++ )
	{
		cornerSource[ i ] = new bool[8];//��l�� �����C�@��corner���˦� ���}�C
		for( int j = 0; j < 8; j++ ) ( cornerSource[ i ] )[j] = false;
	}
	//�����Ҧ��s���I�������Ǩ�
	CheckCornerSource();
	GenerateNormal();//���C�Ө��t�W�O�X���k�V�q

	//�N��ƿ�JinnerSurface��
	int faceCount = 0;
	for( Grid* i = firstGrid; i != NULL; i = i -> link )
	{
		
		for ( int j = 0; j < 6; j++ )
		{
			if( !i -> engagedInDistance && i -> neighborGrid[j] -> engagedInDistance )
			{
				//surfaceFace++;

				switch( j )
				{
					case 0:
						//���⳻�I��m�M�k�V�q��J�i�h�A�A���Ѫ���Ƽg�i�h
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[4] -> index * 3 , i -> corner[4] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[5] -> index * 3 , i -> corner[5] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[6] -> index * 3 , i -> corner[6] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[7] -> index * 3 , i -> corner[7] -> position );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[4] -> index * 3 , i -> corner[4] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[5] -> index * 3 , i -> corner[5] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[6] -> index * 3 , i -> corner[6] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[7] -> index * 3 , i -> corner[7] -> normal );
						( *innerSurface -> indices )[ faceCount * 3 ] = i -> corner[4] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 1 ] = i -> corner[5] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 2 ] = i -> corner[6] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 3 ] = i -> corner[5] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 4 ] = i -> corner[6] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 5 ] = i -> corner[7] -> index;


						break;
					case 1:
						//���⳻�I��m�M�k�V�q��J�i�h�A�A���Ѫ���Ƽg�i�h
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[0] -> index * 3 , i -> corner[0] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[1] -> index * 3 , i -> corner[1] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[2] -> index * 3 , i -> corner[2] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[3] -> index * 3 , i -> corner[3] -> position );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[0] -> index * 3 , i -> corner[0] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[1] -> index * 3 , i -> corner[1] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[2] -> index * 3 , i -> corner[2] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[3] -> index * 3 , i -> corner[3] -> normal );
						( *innerSurface -> indices )[ faceCount * 3 ] = i -> corner[0] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 1 ] = i -> corner[1] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 2 ] = i -> corner[2] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 3 ] = i -> corner[1] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 4 ] = i -> corner[2] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 5 ] = i -> corner[3] -> index;
						break;
					case 2:
						//���⳻�I��m�M�k�V�q��J�i�h�A�A���Ѫ���Ƽg�i�h
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[2] -> index * 3 , i -> corner[2] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[3] -> index * 3 , i -> corner[3] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[6] -> index * 3 , i -> corner[6] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[7] -> index * 3 , i -> corner[7] -> position );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[2] -> index * 3 , i -> corner[2] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[3] -> index * 3 , i -> corner[3] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[6] -> index * 3 , i -> corner[6] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[7] -> index * 3 , i -> corner[7] -> normal );
						( *innerSurface -> indices )[ faceCount * 3 ] = i -> corner[2] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 1 ] = i -> corner[3] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 2 ] = i -> corner[6] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 3 ] = i -> corner[3] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 4 ] = i -> corner[6] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 5 ] = i -> corner[7] -> index;
						break;
					case 3:
						//���⳻�I��m�M�k�V�q��J�i�h�A�A���Ѫ���Ƽg�i�h
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[0] -> index * 3 , i -> corner[0] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[1] -> index * 3 , i -> corner[1] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[4] -> index * 3 , i -> corner[4] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[5] -> index * 3 , i -> corner[5] -> position );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[0] -> index * 3 , i -> corner[0] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[1] -> index * 3 , i -> corner[1] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[4] -> index * 3 , i -> corner[4] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[5] -> index * 3 , i -> corner[5] -> normal );
						( *innerSurface -> indices )[ faceCount * 3 ] = i -> corner[0] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 1 ] = i -> corner[1] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 2 ] = i -> corner[4] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 3 ] = i -> corner[1] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 4 ] = i -> corner[4] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 5 ] = i -> corner[5] -> index;
						break;
					case 4:
						//���⳻�I��m�M�k�V�q��J�i�h�A�A���Ѫ���Ƽg�i�h
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[1] -> index * 3 , i -> corner[1] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[3] -> index * 3 , i -> corner[3] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[5] -> index * 3 , i -> corner[5] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[7] -> index * 3 , i -> corner[7] -> position );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[1] -> index * 3 , i -> corner[1] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[3] -> index * 3 , i -> corner[3] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[5] -> index * 3 , i -> corner[5] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[7] -> index * 3 , i -> corner[7] -> normal );
						( *innerSurface -> indices )[ faceCount * 3 ] = i -> corner[1] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 1 ] = i -> corner[3] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 2 ] = i -> corner[5] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 3 ] = i -> corner[3] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 4 ] = i -> corner[5] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 5 ] = i -> corner[7] -> index;
						break;
					case 5:
						//���⳻�I��m�M�k�V�q��J�i�h�A�A���Ѫ���Ƽg�i�h
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[0] -> index * 3 , i -> corner[0] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[2] -> index * 3 , i -> corner[2] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[4] -> index * 3 , i -> corner[4] -> position );
						MathHelper::CopyVector( ( *innerSurface -> vertices ) + i -> corner[6] -> index * 3 , i -> corner[6] -> position );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[0] -> index * 3 , i -> corner[0] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[2] -> index * 3 , i -> corner[2] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[4] -> index * 3 , i -> corner[4] -> normal );
						MathHelper::CopyVector( ( *innerSurface -> normals ) + i -> corner[6] -> index * 3 , i -> corner[6] -> normal );
						( *innerSurface -> indices )[ faceCount * 3 ] = i -> corner[0] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 1 ] = i -> corner[2] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 2 ] = i -> corner[4] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 3 ] = i -> corner[2] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 4 ] = i -> corner[4] -> index;
						( *innerSurface -> indices )[ faceCount * 3 + 5 ] = i -> corner[6] -> index;
						/*�o��n�F��??
						CheckCornerDuplicated( i , 0 , &cornerIndex );
						CheckCornerDuplicated( i , 2 , &cornerIndex );
						CheckCornerDuplicated( i , 4 , &cornerIndex );
						CheckCornerDuplicated( i , 6 , &cornerIndex );*/
						break;
				}
				faceCount += 2;
			}
		}
	}
	std::cout << "�ͫ���������" << std::endl;
}

void ObjHollower::DeleteGrid( Grid* grid )
{
	for( Grid* i = grid; i != NULL; )
	{
		Grid* j = i -> link;
		delete i;
		i = j;
	}
}

#pragma endregion

//======================================ObjHollower2=====================================

#pragma region ObjHollower2_Private

bool ObjHollower2::GetMaxSectionSize()
{
	maxSectionSize[0][0] = 9999;//min
	maxSectionSize[0][1] = -9999;//max
	maxSectionSize[1][0] = 9999;//min
	maxSectionSize[1][1] = -9999;//max
	maxSectionSize[2][0] = 9999;//min
	maxSectionSize[2][1] = -9999;//max

	for ( int i = 0 ; i < model -> verticeSize / 3; i++ )
	{
		//x
		if( ( ( *( model -> vertices ) )[ i * 3 ] ) < SECTION_SCALE * maxSectionSize[0][0] )//�o��X��
		{
			maxSectionSize[0][0] = floor( ( *( model -> vertices ) )[ i * 3 ] / SECTION_SCALE );
		}
		if( ( ( *( model -> vertices ) )[ i * 3 ] ) > SECTION_SCALE * maxSectionSize[0][1] )//�o��X��
		{
			maxSectionSize[0][1] = ceil( ( *( model -> vertices ) )[ i * 3 ] / SECTION_SCALE );//�]������]�^�骺�W�u���O<maxSectionSize[0][1]�A�ҥH��ceil
		}
		//y
		if( ( ( *( model -> vertices ) )[ i * 3 + 1 ] ) < SECTION_SCALE * maxSectionSize[1][0] )//�o��X��
		{
			maxSectionSize[1][0] = floor( ( *( model -> vertices ) )[ i * 3 + 1 ] / SECTION_SCALE );
		}
		if( ( ( *( model -> vertices ) )[ i * 3 + 1 ] ) > SECTION_SCALE * maxSectionSize[1][1] )//�o��X��
		{
			maxSectionSize[1][1] = ceil( ( *( model -> vertices ) )[ i * 3 + 1 ] / SECTION_SCALE );
		}
		//z
		if( ( ( *( model -> vertices ) )[ i * 3 + 2 ] ) < SECTION_SCALE * maxSectionSize[2][0] )//�o��X��
		{
			maxSectionSize[2][0] = floor( ( *( model -> vertices ) )[ i * 3 + 2 ] / SECTION_SCALE );
		}
		if( ( ( *( model -> vertices ) )[ i * 3 + 2 ] ) > SECTION_SCALE * maxSectionSize[2][1] )//�o��X��
		{
			maxSectionSize[2][1] = ceil( ( *( model -> vertices ) )[ i * 3 + 2 ] / SECTION_SCALE );
		}
	}

	std::cout << "�ҫ����Ϫ�" << maxSectionSize[0][0] << "~" << maxSectionSize[0][1] << 
				 ",�e" << maxSectionSize[1][0] << "~" << maxSectionSize[1][1] << 
				 ",��" << maxSectionSize[2][0] << "~" << maxSectionSize[2][1] << std::endl;

	sectionDimension[0] = abs( maxSectionSize[0][0] ) + abs( maxSectionSize[0][1] );
	sectionDimension[1] = abs( maxSectionSize[1][0] ) + abs( maxSectionSize[1][1] );
	sectionDimension[2] = abs( maxSectionSize[2][0] ) + abs( maxSectionSize[2][1] );


	return true;
}

bool ObjHollower2::GetSectionIndex( int x , int y , int z , int* sectionIndex )
{
	sectionIndex[0] = x + abs( maxSectionSize[0][0] );
	sectionIndex[1] = y + abs( maxSectionSize[1][0] );
	sectionIndex[2] = z + abs( maxSectionSize[2][0] );
	return true;
}

bool ObjHollower2::GetSectionIndex( int* sectionPosition , int* sectionIndex )
{
	sectionIndex[0] = sectionPosition[0] + abs( maxSectionSize[0][0] );
	sectionIndex[1] = sectionPosition[1] + abs( maxSectionSize[1][0] );
	sectionIndex[2] = sectionPosition[2] + abs( maxSectionSize[2][0] );
	return true;
}

bool ObjHollower2::GetSectionPosition( int x , int y , int z , int* sectionPosition )
{
	sectionPosition[0] = x - abs( maxSectionSize[0][0] );
	sectionPosition[1] = y - abs( maxSectionSize[1][0] );
	sectionPosition[2] = z - abs( maxSectionSize[2][0] );
	return true;
}
//���o�ҫ����j�p
bool ObjHollower2::GetMaxGridSize()
{
	maxGridSize[0][0] = 9999;//min
	maxGridSize[0][1] = -9999;//max
	maxGridSize[1][0] = 9999;//min
	maxGridSize[1][1] = -9999;//max
	maxGridSize[2][0] = 9999;//min
	maxGridSize[2][1] = -9999;//max
		

	for ( int i = 0 ; i < model -> verticeSize / 3; i++ )
	{
		//x
		if( ( ( *( model -> vertices ) )[ i * 3 ] ) < GRID_SCALE * maxGridSize[0][0] )//�o��X��
		{
			maxGridSize[0][0] = floor( ( *( model -> vertices ) )[ i * 3 ] / GRID_SCALE );
		}
		if( ( ( *( model -> vertices ) )[ i * 3 ] ) > GRID_SCALE * maxGridSize[0][1] )//�o��X��
		{
			maxGridSize[0][1] = ceil( ( *( model -> vertices ) )[ i * 3 ] / GRID_SCALE );
		}
		//y
		if( ( ( *( model -> vertices ) )[ i * 3 + 1 ] ) < GRID_SCALE * maxGridSize[1][0] )//�o��X��
		{
			maxGridSize[1][0] = floor( ( *( model -> vertices ) )[ i * 3 + 1 ] / GRID_SCALE );
		}
		if( ( ( *( model -> vertices ) )[ i * 3 + 1 ] ) > GRID_SCALE * maxGridSize[1][1] )//�o��X��
		{
			maxGridSize[1][1] = ceil( ( *( model -> vertices ) )[ i * 3 + 1 ] / GRID_SCALE );
		}
		//z
		if( ( ( *( model -> vertices ) )[ i * 3 + 2 ] ) < GRID_SCALE * maxGridSize[2][0] )//�o��X��
		{
			maxGridSize[2][0] = floor( ( *( model -> vertices ) )[ i * 3 + 2 ] / GRID_SCALE );
		}
		if( ( ( *( model -> vertices ) )[ i * 3 + 2 ] ) > GRID_SCALE * maxGridSize[2][1] )//�o��X��
		{
			maxGridSize[2][1] = ceil( ( *( model -> vertices ) )[ i * 3 + 2 ] / GRID_SCALE );
		}
	}
	std::cout << "�ҫ���" << maxGridSize[0][0] << "~" << maxGridSize[0][1] << 
				 ",�e" << maxGridSize[1][0] << "~" << maxGridSize[1][1] << 
				 ",��" << maxGridSize[2][0] << "~" << maxGridSize[2][1] << std::endl;
	return true;

}
//���Q�ҫ����ɶ�
bool ObjHollower2::GenerateSection()
{
	//����������j�p
	GetMaxSectionSize();

	//��O����
	section = new std::vector<int>**[ sectionDimension[0] ];
	for( int i = 0; i < sectionDimension[0] ; i++ )
	{
		section[i] = new std::vector<int>*[ sectionDimension[1] ];
		for( int j = 0; j < sectionDimension[1]; j++ )
		{
			section[i][j] = new std::vector<int>[ sectionDimension[2] ];
		}
	}

	std::cout << "�}�l����" << std::endl;
	for( int i = 0; i < model -> indiceSize / 3; i++ )
	{
		std::cout << ".";
		GLfloat trianglePosition[3][3];
		MathHelper::CopyVector( trianglePosition[0] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] );
		MathHelper::CopyVector( trianglePosition[1] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] );
		MathHelper::CopyVector( trianglePosition[2] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] );
		//std::cout << "��" << i << "�ӭ��ؤ���" << std::endl;
		for( int x = 0; x < sectionDimension[0]; x++ )
		{
			for( int y = 0; y < sectionDimension[1]; y++ )
			{
				for( int z = 0; z < sectionDimension[2]; z++ )
				{
					int sectionIndex[3];
					GetSectionPosition( x , y , z , sectionIndex );
					GLfloat sectionPosition[] = { sectionIndex[0] * SECTION_SCALE , 
												  sectionIndex[1] * SECTION_SCALE , 
												  sectionIndex[2] * SECTION_SCALE };
					//std::cout << "��" << i << "�ӭ��ؤ���(" << sectionIndex[0] << "," << sectionIndex[1] << "," << sectionIndex[2] << ")" << std::endl;

					//����@�ݴN�O�ӻ����I�h��
					GLfloat sectionCenter[] = { sectionPosition[0] + 0.5 * SECTION_SCALE ,
												sectionPosition[1] + 0.5 * SECTION_SCALE ,
												sectionPosition[2] + 0.5 * SECTION_SCALE };
					if( MathHelper::VectorDistance( trianglePosition[0] , sectionCenter ) > SECTION_SCALE * 4.0 )
					{
						if( MathHelper::VectorDistance( trianglePosition[1] , sectionCenter ) > SECTION_SCALE * 4.0 )
						{
							if( MathHelper::VectorDistance( trianglePosition[2] , sectionCenter ) > SECTION_SCALE * 4.0 )
							{
								continue;
							}
						}
					}

					//���ݦo���I�O���O�b�����
					bool inSection = false;
					
					if( CheckPointInCube( trianglePosition[0] , sectionPosition ) ||
						CheckPointInCube( trianglePosition[1] , sectionPosition ) ||
						CheckPointInCube( trianglePosition[2] , sectionPosition ) )
					{
						inSection = true;
					}
					else
					{
						
						//�b�ݦo���䦳�S����L���
						GLfloat linePosition[8][3];
						for( int i = 0; i < 8; i++ )
						{
							MathHelper::CopyVector( linePosition[i] , sectionPosition );
						}
						MathHelper::VectorPlus( linePosition[0] , 0 , 0 , 0 );//�٨S�g
						MathHelper::VectorPlus( linePosition[1] , 0 , 0 , SECTION_SCALE );
						MathHelper::VectorPlus( linePosition[2] , 0 , SECTION_SCALE , 0 );
						MathHelper::VectorPlus( linePosition[3] , 0 , SECTION_SCALE , SECTION_SCALE );
						MathHelper::VectorPlus( linePosition[4] , SECTION_SCALE , 0 , 0 );
						MathHelper::VectorPlus( linePosition[5] , SECTION_SCALE , 0 , SECTION_SCALE );
						MathHelper::VectorPlus( linePosition[6] , SECTION_SCALE , SECTION_SCALE , 0 );
						MathHelper::VectorPlus( linePosition[7] , SECTION_SCALE , SECTION_SCALE , SECTION_SCALE );
						//��1
						if( CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[4] , linePosition[5] , linePosition[6] ) ||
							CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[5] , linePosition[6] , linePosition[7] ) ||
							CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[4] , linePosition[5] , linePosition[6] ) ||
							CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[5] , linePosition[6] , linePosition[7] ) ||
							CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[4] , linePosition[5] , linePosition[6] ) ||
							CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[5] , linePosition[6] , linePosition[7] ) )
						{
							inSection = true;
						}//��2
						else if( CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[1] , linePosition[0] , linePosition[3] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[0] , linePosition[3] , linePosition[2] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[1] , linePosition[0] , linePosition[3] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[0] , linePosition[3] , linePosition[2] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[1] , linePosition[0] , linePosition[3] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[0] , linePosition[3] , linePosition[2] ) )
						{
							inSection = true;
						}//��3
						else if( CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[3] , linePosition[2] , linePosition[7] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[2] , linePosition[7] , linePosition[6] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[3] , linePosition[2] , linePosition[7] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[2] , linePosition[7] , linePosition[6] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[3] , linePosition[2] , linePosition[7] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[2] , linePosition[7] , linePosition[6] ) )
						{
							inSection = true;
						}//��4
						else if( CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[0] , linePosition[1] , linePosition[4] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[1] , linePosition[4] , linePosition[5] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[0] , linePosition[1] , linePosition[4] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[1] , linePosition[4] , linePosition[5] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[0] , linePosition[1] , linePosition[4] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[1] , linePosition[4] , linePosition[5] ) )
						{
							inSection = true;
						}//��5
						else if( CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[5] , linePosition[1] , linePosition[7] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[1] , linePosition[7] , linePosition[3] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[5] , linePosition[1] , linePosition[7] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[1] , linePosition[7] , linePosition[3] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[5] , linePosition[1] , linePosition[7] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[1] , linePosition[7] , linePosition[3] ) )
						{
							inSection = true;
						}//��6
						else if( CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[0] , linePosition[4] , linePosition[2] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[1] , linePosition[4] , linePosition[2] , linePosition[6] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[0] , linePosition[4] , linePosition[2] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[0] , trianglePosition[2] , linePosition[4] , linePosition[2] , linePosition[6] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[0] , linePosition[4] , linePosition[2] ) ||
								 CheckLineIntersectOnTriangle( trianglePosition[1] , trianglePosition[2] , linePosition[4] , linePosition[2] , linePosition[6] ) )
						{
							inSection = true;
						}
						
					}
					
					//�ݵ��G�p�G�O�b�̭��A�N��o�ӭ��[�J�o��section��
					if( inSection )
					{
						//std::cout << "��" << i << "�ӭ��[�J��(" << sectionIndex[0] << "," << sectionIndex[1] << "," << sectionIndex[2] << ")��" << std::endl;
						
						section[x][y][z].push_back(i);
					}
				}
			}
		}
		
	}
	return true;
}
//�ͦ��Ҧ��P�ҫ��������I
bool ObjHollower2::GeneratePoint()
{
	std::cout<<"->";
	for( int i = maxGridSize[0][0]; i < maxGridSize[0][1]; i++ )
	{
		for( int j = maxGridSize[1][0]; j < maxGridSize[1][1]; j++ )
		{
			for( int k = maxGridSize[2][0]; k < maxGridSize[2][1]; k++ )
			{
				Point* pointPropagated = new Point( GRID_SCALE * i ,  GRID_SCALE * j ,  GRID_SCALE * k );
				//std::cout << "�ͦ��s�I(" << i << "," << j << "," << k << ")" << std::endl;
				if( CheckEngaged( pointPropagated ) )
				{
					std::cout << "�ͦ��s�I(" << i << "," << j << "," << k << ")" << std::endl;
					if( firstPoint == NULL )
					{
						pointPropagated -> firstPoint = pointPropagated;
						firstPoint = pointPropagated;
						tempPoint = pointPropagated;
					}
					else
					{
						firstPoint -> AddToLink( pointPropagated );
						tempPoint = pointPropagated;
					}
				}
				else
				{
					delete pointPropagated;
				}
			}
		}
	}
	return true;
}

bool ObjHollower2::GeneratePointWithSection()
{
	std::cout<<"->";
	float ratio = float(GRID_SCALE) / float(SECTION_SCALE);

	for( int i = maxGridSize[0][0]; i < maxGridSize[0][1]; i++ )
	{
		std::cout << "�ثe����" << i << "�h�A�̤j" << maxGridSize[0][1] << "�h" << std::endl;
		for( int j = maxGridSize[1][0]; j < maxGridSize[1][1]; j++ )
		{
			for( int k = maxGridSize[2][0]; k < maxGridSize[2][1]; k++ )
			{
				Point* pointPropagated = new Point( GRID_SCALE * i ,  GRID_SCALE * j ,  GRID_SCALE * k );
				//std::cout << "�ͦ��s�I(" << i << "," << j << "," << k << ")" << std::endl;
				//�����������X
				int sectionPosition[3] = { floor( ratio * i ) ,
										   floor( ratio * j ) ,
										   floor( ratio * k ) }; 
				//std::cout << "��b��(" << sectionPosition[0] << "," << sectionPosition[1] << "," << sectionPosition[2] << ")�Ӥ���" << std::endl;
				int sectionIndex[3];
				GetSectionIndex( sectionPosition , sectionIndex );
				//�}�l�P�_
				if( CheckEngagedWithSection( pointPropagated , sectionIndex ) )
				{
					//std::cout << "�ͦ��s�I(" << i << "," << j << "," << k << ")��b��(" << sectionPosition[0] << "," << sectionPosition[1] << "," << sectionPosition[2] << ")�Ӥ���" << std::endl;
					if( firstPoint == NULL )
					{
						pointPropagated -> firstPoint = pointPropagated;
						firstPoint = pointPropagated;
						tempPoint = pointPropagated;
					}
					else
					{
						pointPropagated -> firstPoint = pointPropagated;
						tempPoint -> link = pointPropagated;
						tempPoint = pointPropagated;
						//firstPoint -> AddToLink( pointPropagated );
					}
				}
				else
				{
					delete pointPropagated;
				}
			}
		}
	}
	return true;
}

bool ObjHollower2::CheckPointInCube( GLfloat* point , GLfloat* cubePosition )
{
	if( point[0] >= cubePosition[0] && point[0] <= cubePosition[0] + SECTION_SCALE &&
		point[1] >= cubePosition[1] && point[1] <= cubePosition[1] + SECTION_SCALE &&
		point[2] >= cubePosition[2] && point[2] <= cubePosition[2] + SECTION_SCALE )
	{
		return true;
	}
	return false;
}
//�d�ݬY�@���I������O�_�P�ҫ�����
bool ObjHollower2::CheckEngaged( Point* point )
{
	GLfloat linePosition[8][3];
	for( int i = 0; i < 8; i++ )
	{
		MathHelper::CopyVector( linePosition[i] , point -> position );
	}
	MathHelper::VectorPlus( linePosition[0] , 0 , 0 , 0 );//�٨S�g
	MathHelper::VectorPlus( linePosition[1] , 0 , 0 , GRID_SCALE );
	MathHelper::VectorPlus( linePosition[2] , 0 , GRID_SCALE , 0 );
	MathHelper::VectorPlus( linePosition[3] , 0 , GRID_SCALE , GRID_SCALE );
	MathHelper::VectorPlus( linePosition[4] , GRID_SCALE , 0 , 0 );
	MathHelper::VectorPlus( linePosition[5] , GRID_SCALE , 0 , GRID_SCALE );
	MathHelper::VectorPlus( linePosition[6] , GRID_SCALE , GRID_SCALE , 0 );
	MathHelper::VectorPlus( linePosition[7] , GRID_SCALE , GRID_SCALE , GRID_SCALE );
	
	int edge[12][2] = { { 0 , 1 } , { 0 , 4 } , { 4 , 5 } , { 5 , 1 } , 
						{ 0 , 2 } , { 1 , 3 } , { 2 , 4 } , { 3 , 5 } , 
						{ 2 , 3 } , { 3 , 7 } , { 7 , 6 } , { 6 , 2 } };
						
	

	bool engaged = false;

	for( int i = 0; i < 12; i++ )
	{
		if( CheckLineIntersectOnModel( linePosition[ edge[i][0] ] , linePosition[ edge[i][1] ] ) )
		{
			return true;
		}
	}
	return false;
}

bool ObjHollower2::CheckEngagedWithSection( Point* point , int* sectionIndex )
{
	GLfloat linePosition[8][3];
	for( int i = 0; i < 8; i++ )
	{
		MathHelper::CopyVector( linePosition[i] , point -> position );
	}
	if(0)
	{
	MathHelper::VectorPlus( linePosition[0] , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * -GRID_THICKNESS );//�٨S�g
	MathHelper::VectorPlus( linePosition[1] , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * GRID_THICKNESS*2 );
	MathHelper::VectorPlus( linePosition[2] , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * -GRID_THICKNESS );
	MathHelper::VectorPlus( linePosition[3] , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * GRID_THICKNESS*2 );
	MathHelper::VectorPlus( linePosition[4] , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * -GRID_THICKNESS );
	MathHelper::VectorPlus( linePosition[5] , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * -GRID_THICKNESS , GRID_SCALE * GRID_THICKNESS*2 );
	MathHelper::VectorPlus( linePosition[6] , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * -GRID_THICKNESS );
	MathHelper::VectorPlus( linePosition[7] , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * GRID_THICKNESS*2 , GRID_SCALE * GRID_THICKNESS*2 );
	}
	if(1)
	{
	MathHelper::VectorPlus( linePosition[0] , 0 , 0 , 0 );//�٨S�g
	MathHelper::VectorPlus( linePosition[1] , 0 , 0 , GRID_SCALE );
	MathHelper::VectorPlus( linePosition[2] , 0 , GRID_SCALE , 0 );
	MathHelper::VectorPlus( linePosition[3] , 0 , GRID_SCALE , GRID_SCALE );
	MathHelper::VectorPlus( linePosition[4] , GRID_SCALE , 0 , 0 );
	MathHelper::VectorPlus( linePosition[5] , GRID_SCALE , 0 , GRID_SCALE );
	MathHelper::VectorPlus( linePosition[6] , GRID_SCALE , GRID_SCALE , 0 );
	MathHelper::VectorPlus( linePosition[7] , GRID_SCALE , GRID_SCALE , GRID_SCALE );
	}
	
	int edge[12][2] = { { 0 , 1 } , { 0 , 4 } , { 4 , 5 } , { 5 , 1 } , 
						{ 0 , 2 } , { 1 , 3 } , { 2 , 4 } , { 3 , 5 } , 
						{ 2 , 3 } , { 3 , 7 } , { 7 , 6 } , { 6 , 2 } };

	bool engaged = false;

	for( int i = 0; i < section[ sectionIndex[0] ][ sectionIndex[1] ][ sectionIndex[2] ].size(); i++ )
	{
		int face = section[ sectionIndex[0] ][ sectionIndex[1] ][ sectionIndex[2] ].at(i);
		GLfloat trianglePosition[3][3];
		MathHelper::CopyVector( trianglePosition[0] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ face * 3 ] * 3 ] );
		MathHelper::CopyVector( trianglePosition[1] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ face * 3 + 1 ] * 3 ] );
		MathHelper::CopyVector( trianglePosition[2] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ face * 3 + 2 ] * 3 ] );

		for( int j = 0; j < 12; j++ )
		{
			if( CheckLineIntersectOnTriangle( linePosition[ edge[j][0] ] , 
											  linePosition[ edge[j][1] ] ,
											  trianglePosition[0] , 
											  trianglePosition[1] , 
											  trianglePosition[2] ) )
			{
				return true;
			}
		}
	}
	return false;
}

bool ObjHollower2::CheckLineIntersectOnModel( GLfloat* position1 , GLfloat* position2 )
{
	Point* d = new Point();
	Point* e1 = new Point();
	Point* e2 = new Point();

	//bool intersected = false;

	for( int i = 0; i < model -> indiceSize / 3; i++ ) //�X��
	{
		GLfloat trianglePosition[3][3];
		MathHelper::CopyVector( trianglePosition[0] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 ] * 3 ] );
		MathHelper::CopyVector( trianglePosition[1] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 1 ] * 3 ] );
		MathHelper::CopyVector( trianglePosition[2] , &( *( model -> vertices ) )[ ( *( model -> indices ) )[ i * 3 + 2 ] * 3 ] );

		
		d -> Vector( position1 , position2 );

		
		e1 -> Vector( trianglePosition[0] , trianglePosition[1] );
		e2 -> Vector( trianglePosition[0] , trianglePosition[2] );

		
		Point* s1 = new Point();
		s1 -> Cross( d , e2 );

		GLfloat divisor = s1 -> Dot( e1 );
   
		if ( abs(divisor) <= -SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			continue;
		}

		// Compute first barycentric coordinate

		Point* s = new Point();
		s -> Vector( trianglePosition[0] , position1 );
		GLfloat b1 = s -> Dot( s1 ) / divisor;
		if( b1 < -SMALL_NUMBER || b1 > 1 + SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			delete s;
			continue;
		}


				
		// Compute second barycentric coordinate
		Point* s2 = new Point();
		s2 -> Cross( s , e1 );
		GLfloat b2 = d -> Dot( s2 ) / divisor;
		if( b2 < -SMALL_NUMBER || b1 + b2 > 1 + SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			delete s;
			delete s2;
			continue;
		}

				
		// Compute _t_ to intersection point
		GLfloat t = e2 -> Dot( s2 ) / divisor;
		if( t < -SMALL_NUMBER || t > 1 + SMALL_NUMBER )
		{
			delete s1;
			delete s;
			delete s2;
			//���ۥ�
			continue;
		}
				
				
		delete s1;
		delete s;
		delete s2;

		delete d;
		delete e1;
		delete e2;

		return true;
	}
	delete d;
	delete e1;
	delete e2;

	return false;
}

bool ObjHollower2::CheckLineIntersectOnTriangle( GLfloat* linePosition1 , 
												 GLfloat* linePosition2 , 
												 GLfloat* trianglePosition1 , 
												 GLfloat* trianglePosition2 , 
												 GLfloat* trianglePosition3 )
{
	GLfloat d[3];
	GLfloat e1[3];
	GLfloat e2[3];
	//Point* d = new Point();
	//Point* e1 = new Point();
	//Point* e2 = new Point();

	MathHelper::SetVector( d , linePosition1 , linePosition2 );
	//d -> Vector( linePosition1 , linePosition2 );

	MathHelper::SetVector( e1 , linePosition1 , linePosition2 );
	MathHelper::SetVector( e2 , linePosition1 , linePosition2 );
	//e1 -> Vector( trianglePosition1 , trianglePosition2 );
	//e2 -> Vector( trianglePosition1 , trianglePosition3 );

	GLfloat s1[3];
	//Point* s1 = new Point();
	MathHelper::VectorCorss( s1 , d , e2 );
	//s1 -> Cross( d , e2 );

	GLfloat divisor = MathHelper::VectorDot( s1 , e1 );
	//s1 -> Dot( e1 );
	GLfloat indivisor = 1.0 / divisor;
   
	if ( abs(divisor) <= -SMALL_NUMBER )
	{
		//���ۥ�
		//delete s1;
		//
		//delete d;
		//delete e1;
		//delete e2;
		return false;
	}

	// Compute first barycentric coordinate
	GLfloat s[3];
	//Point* s = new Point();
	MathHelper::SetVector( s , trianglePosition1 , linePosition1 );
	//s -> Vector( trianglePosition1 , linePosition1 );
	GLfloat b1 = MathHelper::VectorDot( s , s1 ) * indivisor;
	//GLfloat b1 = s -> Dot( s1 ) * indivisor;
	if( b1 < -SMALL_NUMBER || b1 > 1 + SMALL_NUMBER )
	{
		//���ۥ�
		//delete s1;
		//delete s;
		//
		//delete d;
		//delete e1;
		//delete e2;
		return false;
	}


				
	// Compute second barycentric coordinate
	GLfloat s2[3];
	//Point* s2 = new Point();
	MathHelper::VectorCorss( s2 , s , e1 );
	//s2 -> Cross( s , e1 );
	GLfloat b2 = MathHelper::VectorDot( d , s2 ) * indivisor;
	//GLfloat b2 = d -> Dot( s2 ) * indivisor;
	if( b2 < -SMALL_NUMBER || b1 + b2 > 1 + SMALL_NUMBER )
	{
		//���ۥ�
		//delete s1;
		//delete s;
		//delete s2;
		//
		//delete d;
		//delete e1;
		//delete e2;
		return false;
	}

				
	// Compute _t_ to intersection point
	GLfloat t = MathHelper::VectorDot( e2 , s2 ) * indivisor;
	//GLfloat t = e2 -> Dot( s2 ) * indivisor;
	if( t < -SMALL_NUMBER || t > 1 + SMALL_NUMBER )
	{
		//delete s1;
		//delete s;
		//delete s2;
		//
		//delete d;
		//delete e1;
		//delete e2;
		//���ۥ�
		return false;
	}
				
				
	//delete s1;
	//delete s;
	//delete s2;
	//
	//delete d;
	//delete e1;
	//delete e2;

	return true;
}


#pragma endregion

#pragma region ObjHollower2_Public

ObjHollower2::ObjHollower2( Model* m )
{
	model = m;
	count = 0;
	faceCount = 0;
	
	GenerateSection();
	GetMaxGridSize();
	GeneratePointWithSection();
}
//�N�ͦ��n���I�ѥX
bool ObjHollower2::GenerateSurface( char* filePath )
{
	
	
	//����Ҧ��I����10�s
	Point* entryPoint[ GRID_GROUP + 1 ];
	entryPoint[ GRID_GROUP ] = NULL;
	int count = 0;
	for( Point* p = firstPoint; p != NULL; p = p -> link )
	{
		count++;
	}
	count /= GRID_GROUP;
	int tempCount = 0;
	for( Point* p = firstPoint; p != NULL; p = p -> link )
	{
		for( int i = 0; i < GRID_GROUP; i++ )
		{
			if( tempCount == count * i ) entryPoint[i] = p;
		}
		tempCount++;
	}

	//������ɸs���I���O�@�ѥX
	for( int i = 0; i < GRID_GROUP; i++ )
	{
		int vectexCount = 1;
		char fileName[1000];
		strcpy( fileName , filePath );

		char segmentNumber[4];
		sprintf( segmentNumber , "%i" , i );
		strcat( fileName , segmentNumber );
		strcat( fileName , ".obj" );

		FILE* ObjFile = fopen( fileName ,"w");
		std::cout << "�ͦ���" << i << "�s" << std::endl;
		for( Point* p = entryPoint[i]; p != entryPoint[i+1]; p = p -> link )
		{
				//�ݦo�P���Ӥ���۾F�A�N�G�n�󨺭ӭ�
			GLfloat direction[6][3];
			for( int j = 0; j < 6; j++ )
			{
				MathHelper::CopyVector( direction[j] , p -> position );
			}
			MathHelper::VectorPlus( direction[0] , GRID_SCALE , 0 , 0 );//�٨S�g
			MathHelper::VectorPlus( direction[1] , -GRID_SCALE , 0 , 0 );
			MathHelper::VectorPlus( direction[2] , 0 , GRID_SCALE , 0 );
			MathHelper::VectorPlus( direction[3] , 0 , -GRID_SCALE , 0 );
			MathHelper::VectorPlus( direction[4] , 0 , 0 , GRID_SCALE );
			MathHelper::VectorPlus( direction[5] , 0 , 0 , -GRID_SCALE );

			bool endSearch = false;
			bool faceOut[6] = { true , true , true , true , true , true };
			for( Point* p2 = entryPoint[i]; p2 != entryPoint[i+1]; p2 = p2 -> link )
			{
				if( p == p2 ) continue;
				if( MathHelper::VectorDistance( p -> position , p2 -> position ) > GRID_SCALE * 2 ) continue;
				for( int j = 0; j < 6; j++ )
				{
					if( MathHelper::CompareVector( p2 -> position , direction[j] ) )
					{
						faceOut[j] = false;
						if( j == 0 ) endSearch = true;
					}
				}
				if( endSearch ) break;
			}

			//�}�l�ح�
			GLfloat linePosition[8][3];
			for( int j = 0; j < 8; j++ )
			{
				MathHelper::CopyVector( linePosition[j] , p -> position );
			}
			MathHelper::VectorPlus( linePosition[0] , 0 , 0 , 0 );//�٨S�g
			MathHelper::VectorPlus( linePosition[1] , 0 , 0 , GRID_SCALE );
			MathHelper::VectorPlus( linePosition[2] , 0 , GRID_SCALE , 0 );
			MathHelper::VectorPlus( linePosition[3] , 0 , GRID_SCALE , GRID_SCALE );
			MathHelper::VectorPlus( linePosition[4] , GRID_SCALE , 0 , 0 );
			MathHelper::VectorPlus( linePosition[5] , GRID_SCALE , 0 , GRID_SCALE );
			MathHelper::VectorPlus( linePosition[6] , GRID_SCALE , GRID_SCALE , 0 );
			MathHelper::VectorPlus( linePosition[7] , GRID_SCALE , GRID_SCALE , GRID_SCALE );


			for( int j = 0; j < 6; j++ )
			{
				if( !faceOut[j] ) continue;
				switch(j)
				{
					case 0:
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
						vectexCount += 6;
						break;
					case 1:
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
						vectexCount += 6;
						break;
					case 2:
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
						vectexCount += 6;
						break;
					case 3:
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
						vectexCount += 6;
						break;
					case 4:
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
						vectexCount += 6;
						break;
					case 5:
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
						fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
						fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
						vectexCount += 6;
						break;
		
				}
			}
		}
		fclose(ObjFile);
	}
	std::cout << "������X" << std::endl;
	/*
	for( Point* p = firstPoint; p != NULL; p = p -> link )
	{
		
		//�ݦo�P���Ӥ���۾F�A�N�G�n�󨺭ӭ�
		GLfloat direction[6][3];
		for( int i = 0; i < 6; i++ )
		{
			MathHelper::CopyVector( direction[i] , p -> position );
		}
		MathHelper::VectorPlus( direction[0] , GRID_SCALE , 0 , 0 );//�٨S�g
		MathHelper::VectorPlus( direction[1] , -GRID_SCALE , 0 , 0 );
		MathHelper::VectorPlus( direction[2] , 0 , GRID_SCALE , 0 );
		MathHelper::VectorPlus( direction[3] , 0 , -GRID_SCALE , 0 );
		MathHelper::VectorPlus( direction[4] , 0 , 0 , GRID_SCALE );
		MathHelper::VectorPlus( direction[5] , 0 , 0 , -GRID_SCALE );

		bool endSearch = false;
		bool faceOut[6] = { true , true , true , true , true , true };
		for( Point* p2 = firstPoint; p2 != NULL; p2 = p2 -> link )
		{
			if( p == p2 ) continue;
			for( int i = 0; i < 6; i++ )
			{
				if( MathHelper::CompareVector( p2 -> position , direction[i] ) )
				{
					faceOut[i] = false;
					if( i == 0 ) endSearch = true;
				}
			}
			if( endSearch ) break;
		}

		//�}�l�ح�
		GLfloat linePosition[8][3];
		for( int i = 0; i < 8; i++ )
		{
			MathHelper::CopyVector( linePosition[i] , p -> position );
		}
		MathHelper::VectorPlus( linePosition[0] , 0 , 0 , 0 );//�٨S�g
		MathHelper::VectorPlus( linePosition[1] , 0 , 0 , GRID_SCALE );
		MathHelper::VectorPlus( linePosition[2] , 0 , GRID_SCALE , 0 );
		MathHelper::VectorPlus( linePosition[3] , 0 , GRID_SCALE , GRID_SCALE );
		MathHelper::VectorPlus( linePosition[4] , GRID_SCALE , 0 , 0 );
		MathHelper::VectorPlus( linePosition[5] , GRID_SCALE , 0 , GRID_SCALE );
		MathHelper::VectorPlus( linePosition[6] , GRID_SCALE , GRID_SCALE , 0 );
		MathHelper::VectorPlus( linePosition[7] , GRID_SCALE , GRID_SCALE , GRID_SCALE );


		for( int i = 0; i < 6; i++ )
		{
			if( !faceOut[i] ) continue;
			switch(i)
			{
				case 0:
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
					vectexCount += 6;
					break;
				case 1:
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
					vectexCount += 6;
					break;
				case 2:
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
					vectexCount += 6;
					break;
				case 3:
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
					vectexCount += 6;
					break;
				case 4:
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[5][0] , (float)linePosition[5][1] , (float)linePosition[5][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[1][0] , (float)linePosition[1][1] , (float)linePosition[1][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[7][0] , (float)linePosition[7][1] , (float)linePosition[7][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[3][0] , (float)linePosition[3][1] , (float)linePosition[3][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
					vectexCount += 6;
					break;
				case 5:
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[0][0] , (float)linePosition[0][1] , (float)linePosition[0][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount , (int) vectexCount + 1 , (int) vectexCount + 2 );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[4][0] , (float)linePosition[4][1] , (float)linePosition[4][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[2][0] , (float)linePosition[2][1] , (float)linePosition[2][2] );
					fprintf( ObjFile , "v %f %f %f \n" , (float)linePosition[6][0] , (float)linePosition[6][1] , (float)linePosition[6][2] );
					fprintf(ObjFile, "f %d %d %d \n", (int) vectexCount + 3 , (int) vectexCount + 4 , (int) vectexCount + 5 );
					vectexCount += 6;
					break;
		
			}
		}

		//�[�J�s��

	}
	*/
	return true;
}
#pragma region
