#include "MyPolygon.h"
#include <vector>



MyPolygon::MyPolygon( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 , Plane* plane , bool isInside )
{
	cornerTooClose = false;
	//std::cout << "Debug:�bMyPolygon::MyPolygon" << std::endl;
	Point* point[3];
	point[0] = new Point( point1 );
	point[1] = new Point( point2 );
	point[2] = new Point( point3 );
	//�����ǧ�C�@����省���y�O�_�ۥ���աA�����H����ۥ檺�I��L�̬����_�ӡA�ðO���ۥ檺�I��ݪ��I���s���A
	Point* intersectPoint[2];
	std::vector<int> endPoint;

	for( int i = 0; i < 3; i++ )
	{
		if( plane -> CheckIntersection( point[i] -> position , point[ i==2 ? 0:i+1 ] -> position ) )
		{
			if( endPoint.size() / 2 == 2 )
			{
				std::cout << "MyPolygon::MyPolygon�X���G����W�L��ӻP�P�@�������۱��I" << std::endl;
				if( intersectPoint[0] -> DistanceToPoint( intersectPoint[1] -> position ) < SMALL_NUMBER )
				{
					std::cout << "MyPolygon::MyPolygon�X���G����W�L��ӻP�P�@�������۱��I�A�B���I�Ӿa��A�����ק令��L�b�����W���I" << std::endl;
					plane -> GetIntersectPoint( point[i] -> position , point[ i==2 ? 0:i+1 ] -> position , intersectPoint[1] -> position );
				}
				continue;
			}
			intersectPoint[ endPoint.size() / 2 ] = new Point();
			plane -> GetIntersectPoint( point[i] -> position , point[ i==2 ? 0:i+1 ] -> position , intersectPoint[ endPoint.size()/2 ] -> position );
			endPoint.push_back(i);
			endPoint.push_back( i==2 ? 0:i+1 );
		}
	}

	if( endPoint.size() == 2 )
	{
		std::cout << "MyPolygon::MyPolygon�X���G�u����ӭ����I(�q�`�n��4��)" << std::endl;//�q�`�n��4���I�i�H�����A�N��@�ӤT���γQ������b�A���@�b�ӳQ���X��Ӻ��I
		cornerTooClose = true;
		delete point[0];
		delete point[1];
		delete point[2];
		return;
	}

	Point* polygonCorner; polygonCorner = NULL;

	//���إ�inside���I����C�A�γo�Ӧ�C�ӫإ߷s���h�䫬
	for( int i = 0; i < 3; i++ )
	{
		if( plane -> IsInside( point[i] -> position ) == isInside )
		{
			if( polygonCorner == NULL )
			{
				polygonCorner = new Point( point[i] -> position );
				polygonCorner -> firstPoint = polygonCorner;
				if( plane -> IsInside( point[ i==2 ? 0:i+1 ] -> position ) != isInside )//�o��O��᭱���I�[�i�h
				{
					if( endPoint[0] == i )//�p�G�Ĥ@�Ӻ��I�N�Oi�A�N���s�Ĥ@�Ӻ��I�b�s�ĤG�Ӻ��I
					{
						polygonCorner -> AddToLink( intersectPoint[0] );
						polygonCorner -> AddToLink( intersectPoint[1] );
					}
					else//���M�N�ĤG���I���A�A�ӬO�Ĥ@���I
					{
						polygonCorner -> AddToLink( intersectPoint[1] );
						polygonCorner -> AddToLink( intersectPoint[0] );
					}
				}
			}
			else
			{
				if( polygonCorner -> Contain( point[i] -> position ) )
				{
					break;
				}
				else
				{
					Point* tempCorner = new Point( point[i] -> position );
					polygonCorner -> AddToLink( tempCorner );
					if( plane -> IsInside( point[ i==2 ? 0:i+1 ] -> position ) != isInside )
					{
						if( endPoint[0] == i )//�p�G�Ĥ@�Ӻ��I�N�Oi�A�N���s�Ĥ@�Ӻ��I�b�s�ĤG�Ӻ��I
						{
							polygonCorner -> AddToLink( intersectPoint[0] );
							polygonCorner -> AddToLink( intersectPoint[1] );
						}
						else//���M�N�ĤG���I���A�A�ӬO�Ĥ@���I
						{
							polygonCorner -> AddToLink( intersectPoint[1] );
							polygonCorner -> AddToLink( intersectPoint[0] );
						}
					}
					
				}
			}
		}
	}
	if( polygonCorner == NULL )
	{
		std::cout << "MyPolygon::MyPolygon�X���G�S����J����" << std::endl;
		cornerTooClose = true;
		return;
	}
	if( polygonCorner -> Size() <= 2 )
	{
		std::cout << "MyPolygon::MyPolygon�X���G���I�Ƥp��T��" << std::endl;
	}
	cornerAmount = polygonCorner -> Size();
	corner = new Point*[ cornerAmount ];
	int cornerCount = 0;
	for( Point* i = polygonCorner -> firstPoint; i != NULL; i = i -> link )
	{
		if( i -> link == NULL)
		{
			if( i -> DistanceToPoint( i -> firstPoint -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon�X���G�⳻�I�����Z������" << std::endl;
				cornerTooClose = true;
			}
		}
		else
		{
			if( i -> DistanceToPoint( i -> link -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon�X���G�⳻�I�����Z������" << std::endl;
				cornerTooClose = true;
			}
		}
		
		corner[ cornerCount ] = i;
		cornerCount++;
	}
	delete point[0];
	delete point[1];
	delete point[2];
}

MyPolygon::MyPolygon( MyPolygon* polygon , Plane* plane , bool isInside )
{
	cornerTooClose = false;
	int tempCornerAmount = polygon -> cornerAmount;
	Point** point = new Point*[ tempCornerAmount ];
	for( int i = 0; i < tempCornerAmount; i++ ) 
	{
		point[i] = new Point( polygon -> corner[i] -> position );
	}
	//�����ǧ�C�@����省���y�O�_�ۥ���աA�����H����ۥ檺�I��L�̬����_�ӡA�ðO���ۥ檺�I��ݪ��I���s���A
	Point* intersectPoint[2];
	std::vector<int> endPoint;

	
	for( int i = 0; i < tempCornerAmount; i++ )
	{
		if( plane -> CheckIntersection( point[i] -> position , point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position ) )//��i�Ƴ̤j��A�L���U�@���I�N�O0
		{
			if( endPoint.size() / 2 == 2)
			{
				std::cout << "MyPolygon::MyPolygon�X���G����W�L��ӻP�P�@�������۱��I" << std::endl;
				if( intersectPoint[0] -> DistanceToPoint( intersectPoint[1] -> position ) < SMALL_NUMBER )
				{
					std::cout << "MyPolygon::MyPolygon�X���G����W�L��ӻP�P�@�������۱��I�A�B���I�Ӿa��A�����ק令��L�b�����W���I" << std::endl;
					plane -> GetIntersectPoint( point[i] -> position , point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position , intersectPoint[1] -> position );
				}
				continue;
			}
			intersectPoint[ endPoint.size() / 2 ] = new Point();//�u�|��|���I�A�O����ۥ�u���U��ݡA�ҥH�s����size���H2
			plane -> GetIntersectPoint( point[i] -> position , point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position , intersectPoint[ endPoint.size()/2 ] -> position );
			endPoint.push_back(i);
			endPoint.push_back( i==tempCornerAmount-1 ? 0:i+1 );
		}
	}

	if( endPoint.size() == 2 )
	{
		std::cout << "MyPolygon::MyPolygon�X���G�u����ӭ����I(�q�`�n��4��)" << std::endl;//�q�`�n��4���I�i�H�����A�N��@�ӤT���γQ������b�A���@�b�ӳQ���X��Ӻ��I
		cornerTooClose = true;
		for( int i = 0; i < tempCornerAmount; i++ ) delete point[i];
		delete point;
		return;
	}

	Point* polygonCorner; polygonCorner = NULL;

	//���إ�inside���I����C�A�γo�Ӧ�C�ӫإ߷s���h�䫬
	for( int i = 0; i < tempCornerAmount; i++ )
	{
		if( plane -> IsInside( point[i] -> position ) == isInside )
		{
			if( polygonCorner == NULL )
			{
				polygonCorner = new Point( point[i] -> position );
				polygonCorner -> firstPoint = polygonCorner;
				if( plane -> IsInside( point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position ) != isInside )
				{
					if( endPoint[0] == i )//�p�G�Ĥ@�Ӻ��I�N�Oi�A�N���s�Ĥ@�Ӻ��I�b�s�ĤG�Ӻ��I
					{
						polygonCorner -> AddToLink( intersectPoint[0] );
						polygonCorner -> AddToLink( intersectPoint[1] );
					}
					else//���M�N�ĤG���I���A�A�ӬO�Ĥ@���I
					{
						polygonCorner -> AddToLink( intersectPoint[1] );
						polygonCorner -> AddToLink( intersectPoint[0] );
					}
				}
			}
			else
			{
				if( polygonCorner -> Contain( point[i] -> position ) )
				{
					break;
				}
				else
				{
					Point* tempCorner = new Point( point[i] -> position );
					polygonCorner -> AddToLink( tempCorner );
					if( plane -> IsInside( point[ i==tempCornerAmount-1 ? 0:i+1 ] -> position ) != isInside )
					{
						if( endPoint[0] == i )//�p�G�Ĥ@�Ӻ��I�N�Oi�A�N���s�Ĥ@�Ӻ��I�b�s�ĤG�Ӻ��I
						{
							polygonCorner -> AddToLink( intersectPoint[0] );
							polygonCorner -> AddToLink( intersectPoint[1] );
						}
						else//���M�N�ĤG���I���A�A�ӬO�Ĥ@���I
						{
							polygonCorner -> AddToLink( intersectPoint[1] );
							polygonCorner -> AddToLink( intersectPoint[0] );
						}
					}
				}
			}
		}
	}
	if( polygonCorner == NULL )
	{
		std::cout << "MyPolygon::MyPolygon�X���G�S����J����" << std::endl;
		cornerTooClose = true;
		return;
	}
	if( polygonCorner -> Size() <= 2 )
	{
		std::cout << "MyPolygon::MyPolygon�X���G���I�Ƥp��T��" << std::endl;
	}


	cornerAmount = polygonCorner -> Size();
	corner = new Point*[ cornerAmount ];
	int cornerCount = 0;
	for( Point* i = polygonCorner -> firstPoint; i != NULL; i = i -> link )
	{
		if( i -> link == NULL)
		{
			if( i -> DistanceToPoint( i -> firstPoint -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon�X���G�⳻�I�����Z������" << std::endl;
				cornerTooClose = true;
			}
		}
		else
		{
			if( i -> DistanceToPoint( i -> link -> position ) < SMALL_NUMBER )
			{
				std::cout << "MyPolygon::MyPolygon�X���G�⳻�I�����Z������" << std::endl;
				cornerTooClose = true;
			}
		}
		corner[ cornerCount ] = i;
		cornerCount++;
	}

	for( int i = 0; i < tempCornerAmount; i++ ) delete point[i];
	delete point;
}

MyPolygon::~MyPolygon()
{
	if( cornerAmount < 0 )
	{
		std::cout << "MyPolygon::~MyPolygon�X���G���I�Ƥp��s" << std::endl;
		return;
	}
	for( int i = 0; i < cornerAmount; i++ )
	{
		delete corner[i];
	}
	delete corner;
}

//�U��O�_�]�t�o���u
bool MyPolygon::Contain( Line* line )
{
	//std::cout << "Debug:�bMyPolygon::Contain" << std::endl;
	Line** edge = new Line*[ cornerAmount ];
	for( int i = 0; i < cornerAmount; i++ )
	{
		if( i != cornerAmount -1 )
		{
			edge[i] = new Line( corner[i] -> position , corner[i+1] -> position );
		}
		else
		{
			edge[i] = new Line( corner[i] -> position , corner[0] -> position );
		}
	}
	bool isContained = false;
	//���զ��S���]�t
	for( int i = 0; i < cornerAmount; i++ )
	{
		if( edge[i] -> Containing( line ) )
		{
			isContained = true;
		}
	}
	//�s�O����
	for( int i = 0; i < cornerAmount; i++ )
	{
		delete edge[i];
	}
	delete[] edge;
	return isContained;
}

GLfloat MyPolygon::DistanceToPoint( Point* point )//��쥻�����Z���A�令�N�h�䫬�����ƭӤT���ΡA�@�Ӥ@�ӯ��Z��
{
	Point* endPointCenter = new Point( point -> position );

	//Point* tempNearstPoint = new Point();

	//�����̤p�Z��
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < cornerAmount - 2; i++ ) //�X��
	{
		Point* vector1 = new Point(); vector1 -> Vector( corner[0] , corner[i+2] );//0>2
		Point* vector2 = new Point(); vector2 -> Vector( corner[i+1] , corner[i+2] );//1>2
		//��normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		
		
		
		//�q���ݨ�� �Pnormal�P��V
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position ,  corner[0] -> position );
		GLfloat sb = normal -> Dot( distancePtoV0 );//�O���� �p�G�I�b�ҫ��̡A�]�N�O�Z��
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//�O���� �p�G�I�b�ҫ��̡A�]�N�O�Z��

		if( abs(sb) < minDistance && sb > -0.1 )
		{
			//�p�G�o�ӭ��P�I���Z���p��ثe���̤p�Z���A�h�Ӭ�
			//1.�o���I��v�쭱�W�ɡA�|�b�T�����T���I�����A�h�o���Z���N�ջP�������Z��
			//2.�o���I��v�쭱�W�ɡA�b�T�����T���I���~�A�h�o���Z���N�O�P�T���I���̤p�Z��(��P�������Z���j)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//�o����v���I
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			if( abs(sb) > SMALL_NUMBER )
			{
				normal -> Multiply( sb );
				pointProjectOnPlane -> Plus( normal );
				normal -> Multiply( 1.0 / sb );
			}

			//��Ĥ@�Ӱ��� �Ĥ@�ӬO�T�����`���nx2 �ĤG�ӬO�T�����I0���v�I���Z�� �ĤT�ӬO��v�I�M�s1�ҥ]�����n
			Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector(  corner[ 0 ] -> position , pointProjectOnPlane -> position );
			Point* vertex1ToPoint = new Point(); vertex1ToPoint -> Vector(  corner[ i + 1 ] -> position , pointProjectOnPlane -> position );
			Point* crossArea1 = new Point(); crossArea1 -> Cross( vector1 , vertex0ToPoint );
			Point* crossArea2 = new Point(); crossArea2 -> Cross( vertex1ToPoint , vector2 );
			
			//��Ĥ@�Ӱ��к~�ĤG�Ӱ���
			GLfloat TotalArea = crossTotalArea -> Dot( normal );
			GLfloat Area1 = crossArea1 -> Dot( normal );
			GLfloat Area2 = crossArea2 -> Dot( normal );
			if( ( Area1 >= 0 && Area2 >= 0 ) || ( Area1 <= 0 && Area2 <= 0 ) )//�o��N��@�ӥ��@�ӭt�A�N�O�o���I���b�T���Τ�
			{
				if( abs( TotalArea ) > abs( Area1 ) + abs( Area2 ) )
				{
					//���b�T���ΤW
					insideTriangle = true;
				}
			}

			if( insideTriangle )
			{
				minDistance = abs(sb);
				//tempNearstPoint -> Copy( pointProjectOnPlane );
			}
			else
			{
				GLfloat* distance = new GLfloat[3];
				distance[0] = endPointCenter -> DistanceToPoint(  corner[ 0 ] -> position );
				distance[1] = endPointCenter -> DistanceToPoint(  corner[ i + 1 ] -> position );
				distance[2] = endPointCenter -> DistanceToPoint(  corner[ i + 2 ] -> position );
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
					//tempNearstPoint -> Copy( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + min ] * 3 );
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
	
	
	//delete tempNearstPoint;
	return minDistance;
}

bool MyPolygon::CheckIntersectionPolygonOnPlane( Plane* plane )
{
	bool isIntersected = false;
	for( int i = 0; i < cornerAmount; i++ )
	{
		if( plane -> CheckIntersection( corner[i] -> position , corner[ i==cornerAmount-1 ? 0 : i+1 ] -> position ) )
		{
			isIntersected = true;
			break;
		}
	}

	return isIntersected;
}