#include "Polyhedron.h"


//===========================================PolyhedronPoint//===========================================
PolyhedronPoint::PolyhedronPoint( int i , GLfloat* pos , GLfloat* nor , int t )
{
	if( t != POLYHEDRON_POINT )
	{
		std::cout << "PolyhedronPoint�غc�l�X��" << std::endl;
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
		std::cout << "PolyhedronPoint�غc�l�X���A���ӫ�face" << std::endl;
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
		std::cout << "PolyhedronPoint��Contain�X���A����type�n�Oface" << std::endl;
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
		std::cout << "PolyhedronPoint��Contain�X���A����type�n�Oface" << std::endl;
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
		std::cout << "PolyhedronPoint��Contain�X���A����type�n�Oface" << std::endl;
	}
	if( MathHelper::CompareVector( line -> endPosition , line -> initPosition ) )
	{
		std::cout << "�d�ߦh�䫬���O�_�]�M�@���u�ɡA�ӽu�q�_�I�M���I�@��" << std::endl;
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
		std::cout << "PolyhedronPoint��GetPolyhedronFace�X���A����type�n�Oface" << std::endl;
	}
	for( int i = 0; i < 3; i++ )
	{
		if( connectedFace[i] -> Contain( propagatingLine ) )
		{
			return connectedFace[i];
		}
	}
	std::cout << "���o�h��歱�X���A�S��k�Q����u���o�L�䪺��" << std::endl;
}

int PolyhedronPoint::GetPolyhedronFace( int connectedPoint0 , int connectedPoint1  )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint��GetPolyhedronFace�X���A����type�n�Oface" << std::endl;
	}
	for( int i = 0; i < 3; i++ )
	{
		if( connectedFace[i] -> Contain( connectedPoint0 ) && 
			connectedFace[i] -> Contain( connectedPoint1 ) )
		{
			return connectedFace[i] -> index;
		}
	}
	std::cout << "���o�h��歱�X���A�S��k�Q����u���o�L�䪺��" << std::endl;
}

bool PolyhedronPoint::GetPropagatingLine( PolyhedronPoint* propagatingPolyhedronFace , 
										  PolyhedronPoint*& connectedPoint0 , PolyhedronPoint*& connectedPoint1 )
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint��GetPropagatingLine�X���A����type�n�Oface" << std::endl;
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
		std::cout << "���o�h�䫬�����۳s�I�ɡA�X��" << std::endl;
		return false;
	}
	return true;
}

GLfloat PolyhedronPoint::GetArea()
{
	if( type != POLYHEDRON_FACE )
	{
		std::cout << "PolyhedronPoint��Contain�X���A����type�n�Oface" << std::endl;
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

/*�o��O�µ{��
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
		std::cout << "�d�ߦh�䫬���O�_�]�M�@���u�ɡA�ӽu�q�_�I�M���I�@��" << std::endl;
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
	std::cout << "���o�h��歱�X���A�S��k�Q����u���o�L�䪺��" << std::endl;
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
		std::cout << "���o�h�䫬�����۳s�I�ɡA�X��" << std::endl;
		return false;
	}
	return true;
}
�o��O�µ{��
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


	//1.�}�l��ҫ���J�i�h
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
		std::cout << "�X��" << std::endl;
	}
	return face[i];
}

bool Polyhedron::Connect()//connect�O�Ψӧ�Ҧ����I�M���۳s
{
	std::cout << "Polyhedron::Connect�}�l�s��" << std::endl;
	//2.�N�C�ӭ��M�I�۳s�_�A�٦������۳s�I
	for( int i = 0; i < pointAmount; i++ )//i�O�{�b�n�d���I
	{
		
		std::vector<int> connectedFaceNumber;
		std::vector<int> connectedPointNumber;
		//std::cout << "Polyhedron::Connect�N�I" << i << "�}�l�s��" << std::endl;
		//2.1.���p��C���I�M�X�ӭ��۳s
		for( int j = 0; j < faceAmount; j++ )//j�ջPi�I�۳s����
		{
			if( i == face[j] -> connectedPoint[0] -> index || 
				i == face[j] -> connectedPoint[1] -> index ||
				i == face[j] -> connectedPoint[2] -> index )
			{
				///connectedFaceAmount++;
				connectedFaceNumber.push_back( j );
				//�ۻP�I�۳s���I�p�_��
				for( int k = 0; k < 3; k++ )//k��j���W����0~2���I
				{
					int tempIndex = face[j] -> connectedPoint[k] -> index;//j��k�I���s��
					if( tempIndex != i && //�U���o��O���A�P�I�۳s���I�s���o��list���]�t�{�b�j�쪺�o�ӽs��
						std::find( connectedPointNumber.begin(), connectedPointNumber.end(), tempIndex ) == connectedPointNumber.end() )
					{
						connectedPointNumber.push_back( tempIndex );
					}
				}

			}
		}

		//2.2.�b��o���ѥ[�i�h�I��
		point[i] -> connectedFacesAmount = connectedFaceNumber.size();
		point[i] -> connectedFace = new PolyhedronPoint*[ connectedFaceNumber.size() ];
		for( int j = 0; j < connectedFaceNumber.size(); j++ )
		{
			//�N�P�I�۱������p�_��
			point[i] -> connectedFace[j] = face[ connectedFaceNumber.at(j) ];
			//std::cout << "Polyhedron::Connect�N�I" << i << "�P��" << point[i] -> connectedFace[j] -> index << "�s�_��" << std::endl;
		}
		
		//2.3��۳s���I�[��i�I�̭��h
		point[i] -> connectedPointsAmount = connectedPointNumber.size();
		point[i] -> connectedPoint = new PolyhedronPoint*[ connectedPointNumber.size() ];
		for( int j = 0; j < connectedPointNumber.size(); j++ )
		{
			//�s�_��
			point[i] -> connectedPoint[j] = point[ connectedPointNumber.at(j) ];
			//std::cout << "Polyhedron::Connect�N�I" << i << "�P�I" << point[i] -> connectedPoint[j] -> index << "�s�_��" << std::endl;
		}
	}


	//3.��۳s���������_��
	for( int i = 0; i < faceAmount; i++ )
	{
		//std::cout << "Polyhedron::Connect�N��" << i << "�}�l�۳s" << std::endl;
		std::vector<int> connectedFaceNumber;
		for( int j = 0; j < faceAmount; j++ )
		{
			if( i == j ) continue;
			if( ( face[i] -> Contain( face[j] -> connectedPoint[0]->index ) && face[i] -> Contain( face[j] -> connectedPoint[1]->index ) ) ||
				( face[i] -> Contain( face[j] -> connectedPoint[0]->index ) && face[i] -> Contain( face[j] -> connectedPoint[2]->index ) ) ||
				( face[i] -> Contain( face[j] -> connectedPoint[1]->index ) && face[i] -> Contain( face[j] -> connectedPoint[2]->index ) ) )
			{
				//������I�ۦP��
				connectedFaceNumber.push_back( j );
				std::cout << face[j] -> connectedPoint[0]->index << "," << face[j] -> connectedPoint[1]->index << "," << face[j] -> connectedPoint[2]->index << "," << std::endl;
			}
		}

		//��̫ᵲ�G��i�h
		
		if( connectedFaceNumber.size() != 3 )
		{
			std::cout << "Polyhedron(�h����)�bconnect��face���۳s�X���G�u��"  << connectedFaceNumber.size() << "��" << std::endl;
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
			//std::cout << "Polyhedron::Connect�N��" << i << "�P��" << face[i] -> connectedFace[j] -> index << "�s�_��" << std::endl;
		}
	}
	return true;
}

int Polyhedron::GetClosestFace( GLfloat* pos )//�qouterCone���䮳�L�Ӫ�code
{
	
	Point* endPointCenter = new Point( pos );
	//Point* tempNearstPoint = new Point();
	int nearestFaceNumber = -1;

	//�����̤p�Z��
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < faceAmount; i++ ) //�X��
	{
		Point* vector1 = new Point(); vector1 -> Vector( face[i] -> connectedPoint[0] -> position , face[i] -> connectedPoint[2] -> position );
		Point* vector2 = new Point(); vector2 -> Vector( face[i] -> connectedPoint[1] -> position , face[i] -> connectedPoint[2] -> position );
		//��normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		/*
		GLfloat* vertexNormalAverage = new GLfloat[3];//vertex normal������
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		MathHelper::VectorMutiply( vertexNormalAverage , 1.0 / 3.0 );*/
		//( Normal 1 + Normal 2 + Normal 3 ) / 3
		Point* vertexNormalAverage = new Point( face[i] -> connectedPoint[0] -> normal );
		vertexNormalAverage -> Plus( face[i] -> connectedPoint[1] -> normal );
		vertexNormalAverage -> Plus( face[i] -> connectedPoint[2] -> normal );
		vertexNormalAverage -> Multiply( 1.0 / 3.0 );
		//�p�G�M�I�k�V�q�ۤϤ�V�A�N�⥭��normal����
		if( vertexNormalAverage -> Dot( normal -> position ) < 0 ) normal -> Reverse();
		
		//�q���ݨ�� �Pnormal�P��V
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position , face[i] -> connectedPoint[0] -> position );
		GLfloat sb = normal -> Dot( distancePtoV0 );//�O���� �p�G�I�b�ҫ��̡A�]�N�O�Z��
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//�O���� �p�G�I�b�ҫ��̡A�]�N�O�Z��

		if( abs( sb ) < 1 )
		{
			std::cout << "����̪�" << i << "�A�]�t" << std::endl;
			std::cout << "�I(" << face[i] -> connectedPoint[0] -> position[0] << "," << face[i] -> connectedPoint[0] -> position[1] << "," << face[i] -> connectedPoint[0] -> position[2] << ")�M" << std::endl;
			std::cout << "�I(" << face[i] -> connectedPoint[1] -> position[0] << "," << face[i] -> connectedPoint[1] -> position[1] << "," << face[i] -> connectedPoint[1] -> position[2] << ")�M" << std::endl;
			std::cout << "�I(" << face[i] -> connectedPoint[2] -> position[0] << "," << face[i] -> connectedPoint[2] -> position[1] << "," << face[i] -> connectedPoint[2] -> position[2] << ")" << std::endl;
		}

		if( sb < minDistance && sb > -0.1 )
		{
			//�p�G�o�ӭ��P�I���Z���p��ثe���̤p�Z���A�h�Ӭ�
			//1.�o���I��v�쭱�W�ɡA�|�b�T�����T���I�����A�h�o���Z���N�ջP�������Z��
			//2.�o���I��v�쭱�W�ɡA�b�T�����T���I���~�A�h�o���Z���N�O�P�T���I���̤p�Z��(��P�������Z���j)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//�o����v���I
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			if( abs( sb ) > SMALL_NUMBER )//�p�Gsb���p�A�N���Φb���v�I
			{
				normal -> Multiply( sb );
				pointProjectOnPlane -> Plus( normal );
				normal -> Multiply( 1.0 / sb );
			}

			//��Ĥ@�Ӱ��� �Ĥ@�ӬO�T�����`���nx2 �ĤG�ӬO�T�����I0���v�I���Z�� �ĤT�ӬO��v�I�M�s1�ҥ]�����n
			Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector( face[i] -> connectedPoint[0] -> position , pointProjectOnPlane -> position );
			Point* vertex1ToPoint = new Point(); vertex1ToPoint -> Vector( face[i] -> connectedPoint[1] -> position , pointProjectOnPlane -> position );
			Point* crossArea1 = new Point(); crossArea1 -> Cross( vector1 , vertex0ToPoint );
			Point* crossArea2 = new Point(); crossArea2 -> Cross( vertex1ToPoint , vector2 );
			
			//��Ĥ@�Ӱ��к~�ĤG�Ӱ���
			GLfloat TotalArea = crossTotalArea -> Dot( normal );
			GLfloat Area1 = crossArea1 -> Dot( normal );
			GLfloat Area2 = crossArea2 -> Dot( normal );
			if( ( Area1 >= 0 && Area2 >= 0 ) || 
				( Area1 <= 0 && Area2 <= 0 ) )//�o��N��@�ӥ��@�ӭt�A�N�O�o���I���b�T���Τ�
			{
				if( abs( TotalArea ) > abs( Area1 ) + abs( Area2 ) )
				{
					//���b�T���ΤW
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

				//��X�̪��I
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

				//��o���I����m�������U�� �H�btempNearstPoint��
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
		std::cout << "Polyhedron::GetClosestFace�X���G�S������N�b���W���I" << std::endl;
		nearestFaceNumber = -2;
	}
	
	if( nearestFaceNumber == -1 )
	{
		std::cout << "Polyhedron�h����p�����I�̪񭱥X��" << std::endl;
	}

	//delete tempNearstPoint;
	return nearestFaceNumber;
}

