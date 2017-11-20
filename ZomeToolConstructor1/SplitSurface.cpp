#include "SplitSurface.h"


SplitSurface::SplitSurface( Plane* p )
{
	//plane = p;
	plane = new Plane( p );
}

//�[�J��C
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

//�إߥ|�Ө��A�Q�ΩҦ�����u�M������ɽu
bool SplitSurface::SetCorner( Line* connectingLine , Line* line0 , Line* line1 )
{
	if( connectingLine == NULL )
	{
		std::cout << "SplitSurface::SetCorner�G��J��connectingLine�u�q�լO�Ū�" << std::endl;
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
		//�u�q�_�I����
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
		//�u�q���I����
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

	std::cout << "���ʫe���⥭����m�O�W����" << std::endl << "���I�G(" << maxPlane -> position[0] << "," << maxPlane -> position[1] << "," << maxPlane -> position[2] << ")" << std::endl;
	std::cout << "��V�G(" << maxPlane -> vector[0] << "," << maxPlane -> vector[1] << "," << maxPlane -> vector[2] << ")" << std::endl;
	std::cout << "�U����" << std::endl << "���I�G(" << minPlane -> position[0] << "," << minPlane -> position[1] << "," << minPlane -> position[2] << ")" << std::endl;
	std::cout << "��V�G(" << minPlane -> vector[0] << "," << minPlane -> vector[1] << "," << minPlane -> vector[2] << ")" << std::endl;


	for( int i = 0; i < 4; i++ ) corner[i] = new Point();

	for( int i = 0; i < 16; i++ ) MathHelper::VectorPlus( maxPlane -> position , maxPlane -> vector );//�o�Ӽg�k�����D�藍��
	for( int i = 0; i < 16; i++ ) MathHelper::VectorMinus( minPlane -> position , minPlane -> vector );
	/*
	if( maxPlane -> vector[0] + maxPlane -> vector[1] + maxPlane -> vector[2] > 0 )
	{
		for( int i = 0; i < 8; i++ ) MathHelper::VectorPlus( maxPlane -> position , maxPlane -> vector );//�o�Ӽg�k�����D�藍��
		for( int i = 0; i < 8; i++ ) MathHelper::VectorMinus( minPlane -> position , minPlane -> vector );
	}
	else
	{
		for( int i = 0; i < 8; i++ ) MathHelper::VectorMinus( maxPlane -> position , maxPlane -> vector );//�o�Ӽg�k�����D�藍��
		for( int i = 0; i < 8; i++ ) MathHelper::VectorPlus( minPlane -> position , minPlane -> vector );
	}
	*/
	std::cout << "���ʫ᪺�⥭����m�O�W����" << std::endl << "���I�G(" << maxPlane -> position[0] << "," << maxPlane -> position[1] << "," << maxPlane -> position[2] << ")" << std::endl;
	std::cout << "��V�G(" << maxPlane -> vector[0] << "," << maxPlane -> vector[1] << "," << maxPlane -> vector[2] << ")" << std::endl;
	std::cout << "�U����" << std::endl << "���I�G(" << minPlane -> position[0] << "," << minPlane -> position[1] << "," << minPlane -> position[2] << ")" << std::endl;
	std::cout << "��V�G(" << minPlane -> vector[0] << "," << minPlane -> vector[1] << "," << minPlane -> vector[2] << ")" << std::endl;


	//���j->���p->�k�p->�k�j

	maxPlane -> GetIntersectPointOnRay( line0 , corner[0] -> position );//0     3
	minPlane -> GetIntersectPointOnRay( line0 , corner[1] -> position );//|     ^
	minPlane -> GetIntersectPointOnRay( line1 , corner[2] -> position );//v     |
	maxPlane -> GetIntersectPointOnRay( line1 , corner[3] -> position );//1 - > 2


	//���դ��έ�������u�O�_��t�A��t���ܴN�洫corner



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

	//�o��X��
	SetCube( &model[ tempSplitSurfaceCount ] , splitSurface );
	GLfloat position[] = { 0 , 0 , 0 };
	
	if( splitSurface -> corner[0] == NULL ||
		splitSurface -> corner[1] == NULL ||
		splitSurface -> corner[2] == NULL ||
		splitSurface -> corner[3] == NULL )
	{
		std::cout << "SplitSurfaceConstructor::NewSplitSurface�X���G�����O�űo" << std::endl;
		int returnIndex = tempSplitSurfaceCount - 1;
		return returnIndex;
	}

	//�p�G�I�O0�A�N��o���I�ر�
	if( !MathHelper::CompareVector( splitSurface -> corner[0] -> position , position ) )
	{
		for( int i = 0; i < 6; i++ )
		{
			std::cout << "���" << tempSplitSurfaceCount << "�I" << i << "(" << *( *( model[ tempSplitSurfaceCount ].vertices ) + i * 3 ) << ","
																			 << *( *( model[ tempSplitSurfaceCount ].vertices ) + i * 3 + 1 ) << ","
																			 << *( *( model[ tempSplitSurfaceCount ].vertices ) + i * 3 + 2 ) << ")" << std::endl;
		}
		

		//���Y���y��
	
	
		GLfloat rotation[] = { 0 , 0 , 0 };

		ModelObject* tempModel;
		//=========================================��ҫ�
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
	


		//=========================================����Y���ҫ��[�J��C
		if( originalModel == NULL )
		{
			originalModel = tempModel;
		}
		else
		{
			tempModelLink -> link = tempModel;//��Ҧ����Y���쵲��C�s�_��
		}
		tempModelLink = tempModel;		  //��Ҧ��D���쵲��C�s�_��


		std::cout << "�إ߲�" << tempSplitSurfaceCount << "�Ӥ��έ�" << std::endl;
		tempSplitSurfaceCount++;
	}
	else
	{
		std::cout << "SplitSurfaceConstructor::NewSplitSurface�X���G��������m�O000" << std::endl;
	}
	int returnIndex = tempSplitSurfaceCount - 1;
	return returnIndex;
}

bool SplitSurfaceConstructor::SetCube( Model* m , SplitSurface* splitSurface )
{
	//���ݤ@�U���έ����|�Ө�¶����V
	//bool reverseNormal = false;
	Point* tempVector0 = new Point();
	Point* tempVector1 = new Point();
	if( splitSurface -> corner[0] == NULL || 
		splitSurface -> corner[1] == NULL )
	{
		std::cout << "SplitSurfaceConstructor::SetCube�X���G�����O�Ū�" << std::endl;
	}
	else
	{
		tempVector0 -> Vector( splitSurface -> corner[0] -> position , splitSurface -> corner[1] -> position );
	}
	if( splitSurface -> corner[1] == NULL || 
		splitSurface -> corner[2] == NULL )
	{
		std::cout << "SplitSurfaceConstructor::SetCube�X���G�����O�Ū�" << std::endl;
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
		//�P��V�A�|�Ө��N�ӥ��`��V�ƦC
		direction = SAME_DIRECTION;
	}
	else
	{
		//�Ϥ�V�A�s�Ө��N�ӤϤ�V�ƦC
		direction = OPPOSITE_DIRECTION;
	}

	if( splitSurface -> corner[0] != NULL && 
		splitSurface -> corner[1] != NULL &&
		splitSurface -> corner[2] != NULL && 
		splitSurface -> corner[3] != NULL )
	{
		//���W ���U �k�U �k�W
		//2 4  0 6  1 7  3 5
		if( direction == SAME_DIRECTION )
		{
			//GLfloat tempCorner[3];
			//���W2 4
			MathHelper::CopyVector( *( m -> vertices ) + 2 * 3 , splitSurface -> corner[0] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 2 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 4 * 3 , splitSurface -> corner[0] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 4 * 3 , splitSurface -> plane -> vector );
			//���U0 6 
			MathHelper::CopyVector( *( m -> vertices ) + 0 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 0 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 6 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 6 * 3 , splitSurface -> plane -> vector );
			//�k�U1 7
			MathHelper::CopyVector( *( m -> vertices ) + 1 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 1 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 7 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 7 * 3 , splitSurface -> plane -> vector );
			//�k�W3 5
			MathHelper::CopyVector( *( m -> vertices ) + 3 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 3 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 5 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 5 * 3 , splitSurface -> plane -> vector );
		}
		else
		{
			//���W2 4
			MathHelper::CopyVector( *( m -> vertices ) + 2 * 3 , splitSurface -> corner[0] -> position );
			/*
			std::cout << "���έ��I0:(" << splitSurface -> corner[0] -> position[0] << ","
				<< splitSurface -> corner[0] -> position[1] << ","
				<< splitSurface -> corner[0] -> position[2] << ")" << std::endl;*/
			MathHelper::VectorMinus( *( m -> vertices ) + 2 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 4 * 3 , splitSurface -> corner[0] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 4 * 3 , splitSurface -> plane -> vector );
			//���U0 6 
			MathHelper::CopyVector( *( m -> vertices ) + 0 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 0 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 6 * 3 , splitSurface -> corner[1] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 6 * 3 , splitSurface -> plane -> vector );
			//�k�U1 7
			MathHelper::CopyVector( *( m -> vertices ) + 1 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 1 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 7 * 3 , splitSurface -> corner[2] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 7 * 3 , splitSurface -> plane -> vector );
			//�k�W3 5
			MathHelper::CopyVector( *( m -> vertices ) + 3 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorMinus( *( m -> vertices ) + 3 * 3 , splitSurface -> plane -> vector );

			MathHelper::CopyVector( *( m -> vertices ) + 5 * 3 , splitSurface -> corner[3] -> position );
			MathHelper::VectorPlus( *( m -> vertices ) + 5 * 3 , splitSurface -> plane -> vector );
		}
	}
	else
	{
		std::cout << "SplitSurfaceConstructor::SetCube�X���G�����O�Ū�" << std::endl;
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
	centerPoint -> Multiply( 1.0 / ( m -> verticeSize / 3 ) );//���H�I���ƶq�A�o�줤�s�I

	for( int i = 0; i < m -> indiceSize / 3; i++ )
	{
		//�����k�V�q�A��V�V�~
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

	//�⭱���k�V�q�����U�Ө�
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
			std::cout << "SetCubeNormal�ɨS������I�A�X��" << std::endl;
			return false;
		}
		MathHelper::VectorMutiply( tempNormal , 1.0 / (float)faceCount );

		MathHelper::CopyVector( point[i] -> normal , tempNormal );
	}
	return true;
}