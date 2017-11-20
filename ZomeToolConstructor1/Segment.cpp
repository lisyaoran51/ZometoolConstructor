#include "Segment.h"


//==========================================SegmentFace//==========================================
bool SegmentFace::Delete( Point* pointList )
{
	for( Point* p = pointList -> firstPoint; p != NULL; )
	{
		Point* tempPoint = p -> link;
		delete p;
		p = tempPoint;
	}
	return true;
}

SegmentFace::SegmentFace( PolyhedronPoint* pFace )
{
	index = pFace -> index;
	isPolygon = false;
	polyhedronFace = pFace;
	intersected = false;

	isFirst = true;
	isTooBig = false;
}

SegmentFace::SegmentFace( PolyhedronPoint* pFace , Line* sLine )
{
	index = pFace -> index;
	isPolygon = false;
	polyhedronFace = pFace;
	intersected = false;

	sourceLine = sLine;
	isFirst = false;
	isTooBig = false;
}

SegmentFace::~SegmentFace()
{
	if( !isFirst ) delete sourceLine;
	if( isPolygon )
	{
		delete polygon;
	}
}

bool SegmentFace::AddToLink( SegmentFace* segmentFace )
{
	if( firstSegmentFace == NULL )
	{
		std::cout << "SegmentFace::AddToLink�X���GfirstSegmentFace�S������" << std::endl;
	}
	segmentFace -> firstSegmentFace = firstSegmentFace;
	for( SegmentFace* i = firstSegmentFace; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = segmentFace;
			break;
		}
	}
	return true;
}
//�o�ӸH�����O�_���M�o�ӥ����۳s
bool SegmentFace::IsConnectedWith( Plane* plane )//�o�ӸH�����O�_���M�o�ӥ����۳s
{
	bool isConnected = false;
	for( SegmentFace* s = firstSegmentFace; s != NULL; s = s -> link )
	{
		if( s -> isPolygon )
		{
			for( int i = 0; i < s -> polygon -> cornerAmount; i++ )
			{
				if( plane -> Contain( s -> polygon -> corner[i] -> position ) )
				{
					isConnected = true;
				}
			}
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				if( plane -> Contain( s -> polyhedronFace -> connectedPoint[i] -> position ) )
				{
					isConnected = true;
				}
			}
		}
	}
	

	return isConnected;
}
//�o�ӸH��������C�O�_�]�t�o���I
bool SegmentFace::ListContainPoint( int i )
{
	bool isContained = false;
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( sf -> ContainPoint( i ) )
		{
			isContained = true;
		}
	}
	return isContained;
}
//�o�ӸH���O�_�]�t�o���I
bool SegmentFace::ContainPoint( int i )
{
	for( int j = 0; j < 3; j++ )
	{
		if( polyhedronFace -> connectedPoint[j] -> index == i )
		{
			return true;
		}
	}
	return false;
}
//�o�ӸH��������C�O�_�]�t�o�ӭ�
bool SegmentFace::ListContainFace( int i )
{
	bool isContained = false;
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( sf -> ContainFace( i ) )
		{
			isContained = true;
		}
	}
	return isContained;
}
//�o�ӸH���O�_�]�t�o�ӭ�
bool SegmentFace::ContainFace( int i )
{
	for( int j = 0; j < 3; j++ )
	{
		if( polyhedronFace -> index == i )
		{
			return true;
		}
	}
	return false;


}

bool SegmentFace::Contain( Line* line )
{
	if( polyhedronFace -> Contain( line ) )
	{
		return true;
	}
	else return false;
}
//�M��o�@�ӸH���b�o�ӭ��W����Ӻ��I
bool SegmentFace::FindEndPointOnPlane( Plane* plane , GLfloat* endPoint0 , GLfloat* endPoint1 )
{
	std::cout << "�i��ﭱ���I�M����" << std::endl;
	Point* connectedPointList; connectedPointList = NULL;
	//����Ҧ������쥭���W���I�����U��
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( sf -> isPolygon )
		{
			for( int i = 0; i < sf -> polygon -> cornerAmount; i++ )
			{
				if( plane -> Contain( sf -> polygon -> corner[i] -> position ) )
				{
					if( !connectedPointList ) 
					{
						Point* tempPoint = new Point( sf -> polygon -> corner[i] -> position );
						connectedPointList = tempPoint;
						connectedPointList -> firstPoint = connectedPointList;
					}
					else
					{
						Point* tempPoint = new Point( sf -> polygon -> corner[i] -> position );
						connectedPointList -> AddToLink( tempPoint );
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				if( plane -> Contain( sf -> polyhedronFace -> connectedPoint[i] -> position ) )
				{
					if( connectedPointList == NULL ) 
					{
						Point* tempPoint = new Point( sf -> polyhedronFace -> connectedPoint[i] -> position );
						connectedPointList = tempPoint;
						connectedPointList -> firstPoint = connectedPointList;
					}
					else
					{
						Point* tempPoint = new Point( sf -> polyhedronFace -> connectedPoint[i] -> position );
						connectedPointList -> AddToLink( tempPoint );
					}
				}
			}
		}
	}
	//�A�⦳���ƪ��I�h���A���X�{�@�����ܴN�O���䤸���I
	int endPointIndex = 0;
	for( Point* cpl = connectedPointList -> firstPoint; cpl != NULL; cpl = cpl -> link )
	{
		int emergeCount = 0;
		for( Point* cpl2 = connectedPointList -> firstPoint; cpl2 != NULL; cpl2 = cpl2 -> link )
		{
			if( cpl == cpl2 ) continue;
			if( cpl -> CloselyCompare( cpl2 ) )
			{
				emergeCount++;
			}
		}
		//std::cout << "�I(" << cpl -> position[0] << "," << cpl -> position[1] << "," << cpl -> position[2] << ")�X�{" << emergeCount << "��" << std::endl;

		if( emergeCount == 0 )
		{
			if( endPointIndex == 0 )
			{
				if( cpl -> position == NULL )
				{
					std::cout << "SegmentFace::FindEndPointOnPlane�X���G�̻��䪺�I�S����m" << std::endl; 
				}
				MathHelper::CopyVector( endPoint0 , cpl -> position );
				endPointIndex++;
			}
			else if( endPointIndex == 1 )
			{
				if( cpl -> position == NULL )
				{
					std::cout << "SegmentFace::FindEndPointOnPlane�X���G�̻��䪺�I�S����m" << std::endl; 
				}
				MathHelper::CopyVector( endPoint1 , cpl -> position );
				endPointIndex++;
			}
			else
			{
				std::cout << "�H���b�����W�M���Ӻ��I�X���A����h���I" << std::endl;
			}
		}
	}

	Delete( connectedPointList );//
	return true;
}
//���o�H���P�����۳s�����u�q
bool SegmentFace::GetConnectingLine( Plane* plane , Line*& connectingLine )
{
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		GLfloat linePoint[2][3] = { {0,0,0} , {0,0,0} };
		bool connected = false;
		if( sf -> isPolygon )
		{
			
			//std::cout << "debug:" << sf << std::endl;
			
			int connectedAmount = 0;
			for( int i = 0; i < sf -> polygon -> cornerAmount; i++ )
			{
				if( plane -> Contain( sf -> polygon -> corner[i] -> position ) )
				{
					if( sf -> polygon -> corner[i] -> position == NULL )
					{
						std::cout << "SegmentFace::GetConnectingLine�X���G�P�����۱����I�S����m" << std::endl;
					}
					if( connectedAmount > 1 )
					{
						std::cout << "SegmentFace::GetConnectingLine�X���G���W�L��ӻP�����۱����I" << std::endl;
						if( MathHelper::VectorApproximation( linePoint[0] , linePoint[1] ) )
						{
							std::cout << "SegmentFace::GetConnectingLine�X���G����Ӧb�����W���I��m�L�󱵪�A�����אּ��L�I" << std::endl;
							MathHelper::CopyVector( linePoint[ 1 ] , sf -> polygon -> corner[i] -> position );
							continue;
						}
						continue;
					}
					
					MathHelper::CopyVector( linePoint[ connectedAmount ] , sf -> polygon -> corner[i] -> position );
					connectedAmount++;

				}
			}
			if( connectedAmount == 2 ) connected = true;
			
		}
		else
		{
			int connectedAmount = 0;
			for( int i = 0; i < 3; i++ )
			{
				if( plane -> Contain( sf -> polyhedronFace -> connectedPoint[i] -> position ) )
				{
					if( sf -> polyhedronFace -> connectedPoint[i] -> position == NULL )
					{
						std::cout << "SegmentFace::GetConnectingLine�X���G�P�����۱����I�S����m" << std::endl;
					}
					if( connectedAmount > 1 )
					{
						std::cout << "SegmentFace::GetConnectingLine�X���G���W�L��ӻP�����۱����I" << std::endl;
						if( MathHelper::VectorApproximation( linePoint[0] , linePoint[1] ) )
						{
							std::cout << "SegmentFace::GetConnectingLine�X���G����Ӧb�����W���I��m�L�󱵪�A�����אּ��L�I" << std::endl;
							MathHelper::CopyVector( linePoint[ 1 ] , sf -> polygon -> corner[i] -> position );
							continue;
						}
						continue;
					}
					MathHelper::CopyVector( linePoint[ connectedAmount ] , sf -> polyhedronFace -> connectedPoint[i] -> position );
					connectedAmount++;
				}
			}

			if( connectedAmount == 2 ) connected = true;
			
		}

		if( connected )
		{
			Line* tempLine = new Line( linePoint[0] , linePoint[1] ); 
			if( connectingLine == NULL )
			{
				connectingLine = tempLine;
				connectingLine -> firstLine = tempLine;
			}
			else
			{
				connectingLine -> AddToLink( tempLine );
			}
		}
	}
	return true;
}

GLfloat SegmentFace::GetArea()
{
	GLfloat area = 0;
	if( isPolygon )
	{
		for( int i = 0; i < polygon -> cornerAmount - 2; i++ )
		{
			area += polygon -> corner[0] -> Area( polygon -> corner[ i + 1 ] -> position , polygon -> corner[ i + 2 ] -> position );

		}
	}
	else
	{
		area += polyhedronFace -> GetArea();
	}
	return area;
}

GLfloat SegmentFace::GetTotalArea()
{
	GLfloat area = 0;
	for( SegmentFace* sf = firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		area += sf -> GetArea();
	}
	return area;
}
//�լ�this���S���ݩ�o�@�ssegmentFace
bool SegmentFace::BelongToSegment( SegmentFace* segmentFace )
{
	bool isBelonging = false;
	for( SegmentFace* sf = segmentFace -> firstSegmentFace; sf != NULL; sf = sf -> link )
	{
		if( MathHelper::VectorDistance( polyhedronFace -> connectedPoint[0] ->position , 
										sf -> polyhedronFace -> connectedPoint[0] -> position ) > 100.0 )
			continue;

		for( int i = 0; i < 3; i++ )//��this���T�Ө��A�h��o�ǸH�������I�����
		{
			if( sf -> isPolygon )//�h���
			{
				for( int j = 0; j < sf -> polygon -> cornerAmount; j++ )
				{
					if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] ->position ,
												   sf -> polygon -> corner[j] -> position ) )
					{
						isBelonging = true;
						return true;
					}
				}
			}
			else//��h���
			{
				for( int j = 0; j < 3; j++ )
				{
					if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] ->position ,
												   sf -> polyhedronFace -> connectedPoint[j] ->position ) )
					{
						isBelonging = true;
						return true;
					}
				}
			}
		}
	}
	return false;
}




//==========================================Segment//==========================================


Segment::Segment( SegmentFace* s , Polyhedron* p )
{
	polyhedron = p;
	segmentFace = s;
}

Segment::~Segment()
{
}
//�[�J��C
bool Segment::AddToLink( Segment* segment )
{
	segment -> firstSegment = firstSegment;
	for( Segment* s = firstSegment; s != NULL; s = s -> link )
	{
		if( s -> link == NULL )
		{
			s -> link = segment;
			break;
		}
	}
	return true;
}
//��X�Z�����I�̻����I
Point* Segment::GetFarPoint( Point* sPoint )
{
	farPoint = new Point();
	GLfloat maxDistance = 0;
	for( SegmentFace* s = segmentFace -> firstSegmentFace; s != NULL; s = s -> link )
	{
		if( s -> isPolygon )
		{
			for( int i = 0; i < s -> polygon -> cornerAmount; i++ )
			{
				GLfloat tempDistance = s -> polygon -> corner[i] -> DistanceToPoint( surfacePoint -> position );
				if( tempDistance > maxDistance )
				{
					maxDistance = tempDistance;
					farPoint -> Copy( s -> polygon -> corner[i] );
				}
			}
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				GLfloat tempDistance = MathHelper::VectorDistance( s -> polyhedronFace -> connectedPoint[i] -> position , 
																   surfacePoint -> position );
				if( tempDistance > maxDistance )
				{
					maxDistance = tempDistance;
					farPoint -> Copy( s -> polyhedronFace -> connectedPoint[i] -> position );
				}
			}
		}
	}
	return farPoint;
}
//���o����H��������
GLfloat Segment::GetArea()
{
	return segmentFace -> GetTotalArea();
}