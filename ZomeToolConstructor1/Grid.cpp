#include "Grid.h"



//======================================================POINT=============================================

Grid::Grid()
{
	position = new GLfloat[3];


	firstGrid = this;
	engagedFace = new bool[6];
	for(int i = 0; i < 6; i++ )engagedFace[i] = false;
	facePropagated = new bool[6];
	for(int i = 0; i < 6; i++ )facePropagated[i] = false;

	engaged = false;
	engagedInDistance = false;
	neighborGrid = new Grid*[6];

	SetCorner();
}

Grid::Grid( Point* centerPoint )
{
	position = new GLfloat[3];
	MathHelper::CopyVector( position , centerPoint -> position );


	firstGrid = this;
	engagedFace = new bool[6];
	for(int i = 0; i < 6; i++ )engagedFace[i] = false;
	facePropagated = new bool[6];
	for(int i = 0; i < 6; i++ )facePropagated[i] = false;

	engaged = false;
	engagedInDistance = false;
	neighborGrid = new Grid*[6];

	SetCorner();
	
	
}

Grid::Grid( Grid* grid , int facePropogatedNumber )
{
	position = new GLfloat[3];
	MathHelper::CopyVector( position , grid -> position );
	firstGrid = grid -> firstGrid;

	engagedFace = new bool[6];
	for(int i = 0; i < 6; i++ )engagedFace[i] = false;
	facePropagated = new bool[6];
	for(int i = 0; i < 6; i++ )facePropagated[i] = false;

	engaged = false;
	engagedInDistance = false;

	neighborGrid = new Grid*[6];


	switch( facePropogatedNumber )
	{
		case 0:
			position[0] += GRID_SCALE;
			break;
		case 1:
			position[0] -= GRID_SCALE;
			break;
		case 2:
			position[1] += GRID_SCALE;
			break;
		case 3:
			position[1] -= GRID_SCALE;
			break;
		case 4:
			position[2] += GRID_SCALE;
			break;
		case 5:
			position[2] -= GRID_SCALE;
			break;
	}

	SetCorner();

	//�ݦ��S������
	bool isDuplicated = false;
	for( Grid* i = firstGrid; i != NULL; i = i -> link )
	{
		if( Compare( i ) )
		{
			isDuplicated = true;
		}
	}

	//�P�۾F������۳s
	if( !isDuplicated )
	{
		for( Grid* i = firstGrid; i != NULL; i = i-> link )
		{
			//�o�@�����k�䦳�@��
			if ( i -> position[0] == position[0] + GRID_SCALE && 
				 i -> position[1] == position[1] &&
				 i -> position[2] == position[2] )
			{
				facePropagated[0] = true;
				neighborGrid[0] = i;
				i -> facePropagated[1] = true;
				i -> neighborGrid[1] = this;
			}
			//�o�@�������䦳�@��
			if ( i -> position[0] == position[0] - GRID_SCALE && 
				 i -> position[1] == position[1] &&
				 i -> position[2] == position[2] )
			{
				facePropagated[1] = true;
				neighborGrid[1] = i; ///////////////////////////////////////////�X��
				i -> facePropagated[0] = true;
				i -> neighborGrid[0] = this;
			}
			//�o�@�����W�����@��
			if ( i -> position[0] == position[0] && 
				 i -> position[1] == position[1] + GRID_SCALE &&
				 i -> position[2] == position[2] )
			{
				facePropagated[2] = true;
				neighborGrid[2] = i;
				i -> facePropagated[3] = true;
				i -> neighborGrid[3] = this;
			}
			//�o�@�����U�����@��
			if ( i -> position[0] == position[0] && 
				 i -> position[1] == position[1] - GRID_SCALE &&
				 i -> position[2] == position[2] )
			{
				facePropagated[3] = true;
				neighborGrid[3] = i;
				i -> facePropagated[2] = true;
				i -> neighborGrid[2] = this;
			}
			//�o�@�����������@��
			if ( i -> position[0] == position[0] && 
				 i -> position[1] == position[1] &&
				 i -> position[2] == position[2] + GRID_SCALE )
			{
				facePropagated[4] = true;
				neighborGrid[4] = i;
				i -> facePropagated[5] = true;
				i -> neighborGrid[5] = this;
			}
			//�o�@�����᭱���@��
			if ( i -> position[0] == position[0] && 
				 i -> position[1] == position[1] &&
				 i -> position[2] == position[2] - GRID_SCALE )
			{
				facePropagated[5] = true;
				neighborGrid[5] = i;
				i -> facePropagated[4] = true;
				i -> neighborGrid[4] = this;
			}
		}

		//��̫�@�Ӥ���M�o���s�_��
		for( Grid* i = firstGrid; i != NULL; i = i -> link )
		{
			if( i -> link == NULL )
			{
				i -> link = this;
				break;
			}
		}

	}
}

Grid::~Grid()
{
	delete[] position;
	delete[] engagedFace;
	delete[] facePropagated;
	/*
	delete neighborGrid[0] , 
		   neighborGrid[1] ,
		   neighborGrid[2] ,
		   neighborGrid[3] ,
		   neighborGrid[4] ,
		   neighborGrid[5];
		   */
	delete[] neighborGrid;
	//delete firstGrid;
	//delete link;
	if( corner )
	{
		delete corner[0] ,
			   corner[1] , 
			   corner[2] , 
			   corner[3] , 
			   corner[4] , 
			   corner[5] , 
			   corner[6] , 
			   corner[7];
		delete corner;
	}
}

void Grid::SetCorner()
{
	corner = new Point*[8];
	corner[0] = new Point( position[0] , position[1] , position[2] );
	corner[1] = new Point( position[0] , position[1] , position[2] + GRID_SCALE );
	corner[2] = new Point( position[0] , position[1] + GRID_SCALE , position[2] );
	corner[3] = new Point( position[0] , position[1] + GRID_SCALE , position[2] + GRID_SCALE );
	corner[4] = new Point( position[0] + GRID_SCALE , position[1] , position[2] );
	corner[5] = new Point( position[0] + GRID_SCALE , position[1] , position[2] + GRID_SCALE );
	corner[6] = new Point( position[0] + GRID_SCALE , position[1] + GRID_SCALE , position[2] );
	corner[7] = new Point( position[0] + GRID_SCALE , position[1] + GRID_SCALE , position[2] + GRID_SCALE );
}

bool Grid::CheckIntersectGridOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	
	//std::cout << "�Ĥ@���I�O�G" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;


	
	Point* d = new Point();

	bool egdeEngaged[12] = { false , false , false , false , false , false , false , false , false , false , false , false };

	bool isIntersected = false;

	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//�o��O? ��X�O�����I�M�����I���s�u
	for( int i = 0 ; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			int engagedEdge = 0;
			//��X�o�O���@��
			switch( i )
			{
				case 0:
					if( j == 1 ) engagedEdge = 0;
					if( j == 4 ) engagedEdge = 1;
					if( j == 2 ) engagedEdge = 4;
					break;
				case 1:
					if( j == 0 ) engagedEdge = 0;
					if( j == 5 ) engagedEdge = 3;
					if( j == 3 ) engagedEdge = 7;
					break;
				case 2:
					if( j == 0 ) engagedEdge = 4;
					if( j == 3 ) engagedEdge = 8;
					if( j == 6 ) engagedEdge = 9;
					break;
				case 3:
					if( j == 1 ) engagedEdge = 7;
					if( j == 2 ) engagedEdge = 8;
					if( j == 7 ) engagedEdge = 11;
					break;
				case 4:
					if( j == 0 ) engagedEdge = 1;
					if( j == 5 ) engagedEdge = 2;
					if( j == 6 ) engagedEdge = 5;
					break;
				case 5:
					if( j == 3 ) engagedEdge = 2;
					if( j == 1 ) engagedEdge = 3;
					if( j == 7 ) engagedEdge = 6;
					break;
				case 6:
					if( j == 4 ) engagedEdge = 5;
					if( j == 2 ) engagedEdge = 9;
					if( j == 7 ) engagedEdge = 10;
					break;
				case 7:
					if( j == 5 ) engagedEdge = 6;
					if( j == 6 ) engagedEdge = 10;
					if( j == 3 ) engagedEdge = 11;
					break;
			}
			//�p�G�o�ӬO�� �Z���|����1 �N�}�l�⭻��
			if( MathHelper::VectorDistance( corner[i] -> position , corner[j] -> position ) == GRID_SCALE && !egdeEngaged[ engagedEdge ] )
				//�o��O�� �p�G��Ө����Z����n�O1 �N�N��o��Ө��Φ��@����C �᭱��egdeEngaged�O�����䪺�s�� 
				//�p�G�o�ӽs������w�g����L���S���ۥ�F �N���ΦA�i�Ӻ�@��
			{//�o��X�� �٨S�]�w corner
				d -> Vector( corner[i] , corner[j] );
				/*
				std::cout << "�o�Ӥ������m�O" << position[0] << "," << position[1] << "," << position[2] << std::endl;
				std::cout << "��0����m�O" << corner[0] -> position[0] << "," << corner[0] -> position[1] << "," << corner[0] -> position[2] << std::endl;
				std::cout << "��1����m�O" << corner[1] -> position[0] << "," << corner[1] -> position[1] << "," << corner[1] -> position[2] << std::endl;
				std::cout << "��2����m�O" << corner[2] -> position[0] << "," << corner[2] -> position[1] << "," << corner[2] -> position[2] << std::endl;
				std::cout << "��3����m�O" << corner[3] -> position[0] << "," << corner[3] -> position[1] << "," << corner[3] -> position[2] << std::endl;
				std::cout << "��4����m�O" << corner[4] -> position[0] << "," << corner[4] -> position[1] << "," << corner[4] -> position[2] << std::endl;
				std::cout << "��5����m�O" << corner[5] -> position[0] << "," << corner[5] -> position[1] << "," << corner[5] -> position[2] << std::endl;
				std::cout << "��6����m�O" << corner[6] -> position[0] << "," << corner[6] -> position[1] << "," << corner[6] -> position[2] << std::endl;
				std::cout << "��7����m�O" << corner[7] -> position[0] << "," << corner[7] -> position[1] << "," << corner[7] -> position[2] << std::endl;
				*/
				Point* s1 = new Point();
				s1 -> Cross( d , e2 );

				GLfloat divisor = s1 -> Dot( e1 );
   
				if (divisor == 0.)
				{
					//���ۥ�
					delete s1;
					continue;
				}

				// Compute first barycentric coordinate

				Point* s = new Point();
				s -> Vector( point1 , corner[i] -> position );
				GLfloat b1 = s -> Dot( s1 ) / divisor;
				if( b1 < 0. || b1 > 1.)
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
				if( b2 < 0. || b1 + b2 > 1.)
				{
					//���ۥ�
					delete s1;
					delete s;
					delete s2;
					continue;
				}

				
				// Compute _t_ to intersection point
				GLfloat t = e2 -> Dot( s2 ) / divisor;
				if( t < 0. || t > 1.)
				{
					delete s1;
					delete s;
					delete s2;
					//���ۥ�
					continue;
				}
				
				egdeEngaged[ engagedEdge ] = true;
				isIntersected = true;
				
				delete s1;
				delete s;
				delete s2;
				
			}
		
		}
	
	}

	//�ݭ��X�ӭ����ۥ�
	for( int i = 0; i < 12; i++ )
	{
		if( egdeEngaged[ i ] )
		{
			engaged = true;
			switch( i )
			{
				case 0:
					engagedFace[1] = true;
					engagedFace[3] = true;
					break;
				case 1:
					engagedFace[3] = true;
					engagedFace[5] = true;
					break;
				case 2:
					engagedFace[0] = true;
					engagedFace[3] = true;
					break;
				case 3:
					engagedFace[3] = true;
					engagedFace[4] = true;
					break;
				case 4:
					engagedFace[1] = true;
					engagedFace[5] = true;
					break;
				case 5:
					engagedFace[0] = true;
					engagedFace[5] = true;
					break;
				case 6:
					engagedFace[0] = true;
					engagedFace[4] = true;
					break;
				case 7:
					engagedFace[1] = true;
					engagedFace[4] = true;
					break;
				case 8:
					engagedFace[1] = true;
					engagedFace[2] = true;
					break;
				case 9:
					engagedFace[2] = true;
					engagedFace[5] = true;
					break;
				case 10:
					engagedFace[0] = true;
					engagedFace[2] = true;
					break;
				case 11:
					engagedFace[2] = true;
					engagedFace[4] = true;
					break;
			}
		
		}
	}
	//�ݨƤ��O���������L�h
	if( engagedFace[2] && engagedFace[3] && engagedFace[4] && engagedFace[5] )//¶x�b
	{
		if( facePropagated[0] )
		{
			engagedFace[1] = true;
		}
		else
		{
			engagedFace[0] = true;
		}
		//std::cout << "������:" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		//std::cout << "�T�����I1�b:" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
		//std::cout << "�T�����I2�b:" << point2[0] << "," << point2[1] << "," << point2[2] << std::endl;
		//std::cout << "�T�����I3�b:" << point3[0] << "," << point3[1] << "," << point3[2] << std::endl;
	}
	if( engagedFace[0] && engagedFace[1] && engagedFace[4] && engagedFace[5] )//¶y�b
	{
		if( facePropagated[2] )
		{
			engagedFace[3] = true;
		}
		else
		{
			engagedFace[2] = true;
		}
		//std::cout << "������:" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		//std::cout << "�T�����I1�b:" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
		//std::cout << "�T�����I2�b:" << point2[0] << "," << point2[1] << "," << point2[2] << std::endl;
		//std::cout << "�T�����I3�b:" << point3[0] << "," << point3[1] << "," << point3[2] << std::endl;
	}
	if( engagedFace[0] && engagedFace[1] && engagedFace[2] && engagedFace[3] )//¶z�b
	{
		if( facePropagated[4] )
		{
			engagedFace[5] = true;
		}
		else
		{
			engagedFace[4] = true;
		}
		//std::cout << "������:" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		//std::cout << "�T�����I1�b:" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
		//std::cout << "�T�����I2�b:" << point2[0] << "," << point2[1] << "," << point2[2] << std::endl;
		//std::cout << "�T�����I3�b:" << point3[0] << "," << point3[1] << "," << point3[2] << std::endl;
	}

	if( engagedFace[0] && engagedFace[1] && engagedFace[2] && engagedFace[3] && engagedFace[4] && engagedFace[5] )//¶z�b
	{
		std::cout << "�P����";
		/*
		std::cout << "������:" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		std::cout << "�T�����I1�b:" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
		std::cout << "�T�����I2�b:" << point2[0] << "," << point2[1] << "," << point2[2] << std::endl;
		std::cout << "�T�����I3�b:" << point3[0] << "," << point3[1] << "," << point3[2] << std::endl;
		*/
	}

	delete d;
	delete e1;
	delete e2;

	return isIntersected;
}

bool Grid::CheckIntersectTriOnGrid( GLfloat* point1 , GLfloat* point2 )
{
	bool isIntersected = false;

	Point* p1 = new Point();
	Point* p2 = new Point();
	Point* p3 = new Point();
	Point* p4 = new Point();

	for( int i = 0; i < 6; i++ )
	{
		switch( i )
		{
			case 0:
				p1 -> Copy( corner[4] );
				p2 -> Copy( corner[5] );
				p3 -> Copy( corner[6] );
				p4 -> Copy( corner[7] );
				break;
			case 1:
				p1 -> Copy( corner[0] );
				p2 -> Copy( corner[1] );
				p3 -> Copy( corner[2] );
				p4 -> Copy( corner[3] );
				break;
			case 2:
				p1 -> Copy( corner[2] );
				p2 -> Copy( corner[3] );
				p3 -> Copy( corner[6] );
				p4 -> Copy( corner[7] );
				break;
			case 3:
				p1 -> Copy( corner[0] );
				p2 -> Copy( corner[1] );
				p3 -> Copy( corner[4] );
				p4 -> Copy( corner[5] );
				break;
			case 4:
				p1 -> Copy( corner[1] );
				p2 -> Copy( corner[3] );
				p3 -> Copy( corner[5] );
				p4 -> Copy( corner[7] );
				break;
			case 5:
				p1 -> Copy( corner[0] );
				p2 -> Copy( corner[2] );
				p3 -> Copy( corner[4] );
				p4 -> Copy( corner[6] );
				break;
		}

		bool InstersectTriangle1 = false , InstersectTriangle2 = false ;
		//�Ĥ@�ӤT����~~~~~~~~~~~~~~~~~~~~~~~~~���S������
		Point* e1 = new Point();e1 -> Vector( p1 , p2 );
		Point* e2 = new Point();e2 -> Vector( p1 , p3 );
		Point* d = new Point(); d -> Vector( point1 , point2 );

		Point* s1 = new Point();s1 -> Cross( d , e2 );

		GLfloat divisor = s1 -> Dot( e1 );
		if (divisor == 0.)
		{
			//���ۥ�
		}
		else
		{
			// Compute first barycentric coordinate
			Point* s = new Point(); s -> Vector( p1 -> position , point1 );
			GLfloat b1 = s -> Dot( s1 ) / divisor;
			if( b1 < 0. || b1 > 1.)
			{
				//���ۥ�
			}
			else
			{
				// Compute second barycentric coordinate
				Point* s2 = new Point(); s2 -> Cross( s , e1 );
				GLfloat b2 = d -> Dot( s2 ) / divisor;
				if( b2 < 0. || b1 + b2 > 1.)
				{
					//���ۥ�
				}
				else
				{
					// Compute _t_ to intersection point
					GLfloat t = e2 -> Dot( s2 ) / divisor;
					if( t < 0. || t > 1.)
					{
						//���ۥ�
					}
					else
					{
						//�ۥ�
						InstersectTriangle1 = true;
					}
				}
			}
		}

		
		//�ĤG�ӤT����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���S������
		p1 -> Copy( p4 );//��p1�令p4 �A��Ҧ��ʧ@�����@��

		e1 -> Vector( p1 , p2 );
		e2 -> Vector( p1 , p3 );
		d -> Vector( point1 , point2 );

		s1 -> Cross( d , e2 );

		divisor = s1 -> Dot( e1 );
		if (divisor == 0.)
		{
			//���ۥ�
		}
		else
		{
			// Compute first barycentric coordinate
			Point* s = new Point(); s -> Vector( p1 -> position , point1 );
			GLfloat b1 = s -> Dot( s1 ) / divisor;
			if( b1 < 0. || b1 > 1.)
			{
				//���ۥ�
			}
			else
			{
				// Compute second barycentric coordinate
				Point* s2 = new Point(); s2 -> Cross( s , e1 );
				GLfloat b2 = d -> Dot( s2 ) / divisor;
				if( b2 < 0. || b1 + b2 > 1.)
				{
					//���ۥ�
				}
				else
				{
					// Compute _t_ to intersection point
					GLfloat t = e2 -> Dot( s2 ) / divisor;
					if( t < 0. || t > 1.)
					{
						//���ۥ�
					}
					else
					{
						//�ۥ�
						InstersectTriangle2 = true;
					}
				}
			}
		}
		engagedFace[ i ] |= InstersectTriangle1 || InstersectTriangle2;

		engaged |= InstersectTriangle1 || InstersectTriangle2;
		isIntersected |= engagedFace[ i ];
		//debug
		/*
		if( engagedFace[ i ] ) 
			std::cout<<".";
			*/


	}

	//�ݨƤ��O���������L�h
	if( engagedFace[2] && engagedFace[3] && engagedFace[4] && engagedFace[5] )//¶x�b
	{
		if( facePropagated[0] )
		{
			engagedFace[1] = true;
		}
		else
		{
			engagedFace[0] = true;
		}
	}
	if( engagedFace[0] && engagedFace[1] && engagedFace[4] && engagedFace[5] )//¶y�b
	{
		if( facePropagated[2] )
		{
			engagedFace[3] = true;
		}
		else
		{
			engagedFace[2] = true;
		}
	}
	if( engagedFace[0] && engagedFace[1] && engagedFace[2] && engagedFace[3] )//¶z�b
	{
		if( facePropagated[4] )
		{
			engagedFace[5] = true;
		}
		else
		{
			engagedFace[4] = true;
		}
	}

	return isIntersected;
}

bool Grid::CheckDuplicated()
{
	bool isDuplicated = false;
	for( Grid* i = firstGrid; i -> link != NULL; i = i -> link )//�q�Ĥ@�Ө�˼ƲĤG�ӳ��S������ �̫�@�ӭ��I�S���Y�]���̫�@�ӴN�O�o�Ӥ���ۤv
	{
		if( Compare( i ) )
		{
			isDuplicated = true;
			break;
		}
	}
	return isDuplicated;
}

bool Grid::CheckEngagedInDistance( int distance )
{
	bool isEngaged = false;
	if( engaged ) 
	{
		isEngaged = true;
		return isEngaged;
	}
	if( distance > 0 )
	{
	
		for( int i = 0; i < 6; i++ )
		{
			//�p�G�o��������I����
			if( neighborGrid[i] )
			{
				if ( neighborGrid[i] -> engaged )
				{
					isEngaged = true;
					return isEngaged;
				}

				if( neighborGrid[i] -> CheckEngagedInDistance( distance - 1 ) )
				{
					isEngaged = true;
					return isEngaged;
				}
			}
		}
	}

	return isEngaged;
}

bool Grid::Compare( Grid* grid )
{
	if( position[0] == grid -> position[0] &&
		position[1] == grid -> position[1] &&
		position[2] == grid -> position[2] )
	{
		return true;
	}
	return false;
}