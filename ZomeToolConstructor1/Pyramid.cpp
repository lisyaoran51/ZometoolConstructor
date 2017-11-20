#include "Pyramid.h"


#pragma region PYRAMID

Pyramid::Pyramid( int t , PyramidGroup* pyramidGroup )
{
	position = new GLfloat[3];
	MathHelper::CopyVector( position , pyramidGroup -> position );

	type = t;

	firstPyramid = this;
	engagedFace = new bool[4];
	for(int i = 0; i < 4; i++ )engagedFace[i] = false;
	facePropagated = new bool[4];
	for(int i = 0; i < 4; i++ )facePropagated[i] = false;
	faceApproached = new bool[4];
	for(int i = 0; i < 4; i++ )faceApproached[i] = false;

	engaged = false;
	//engagedInDistance = false;
	neighborPyramid = new Pyramid*[4];

	zomeDirection = new ZomeDirection();

	parent = pyramidGroup;

	SetCorner();
}

Pyramid::Pyramid( Pyramid* pyramid , int facePropagatedFrom , PyramidGroup* pyramidGroup )
{
	position = new GLfloat[3];
	MathHelper::CopyVector( position , pyramidGroup -> position );

	
	firstPyramid = pyramid -> firstPyramid;
	engagedFace = new bool[4];
	for(int i = 0; i < 4; i++ )engagedFace[i] = false;
	facePropagated = new bool[4];
	for(int i = 0; i < 4; i++ )facePropagated[i] = false;
	faceApproached = new bool[4];
	for(int i = 0; i < 4; i++ )faceApproached[i] = false;

	//�M��o�ӤT���Ϊ�type�A�μW�����T���������M�W�������ӧP�_
	//�t�~�T�{�L�ҳQ�W������facePropagated�A�L���y��
	switch( pyramid -> type )
	{
		case 0:
			switch( facePropagatedFrom )
			{
				case 0:
					type = 3;
					facePropagated[1] = true;
					break;
				case 1:
					type = 1;
					facePropagated[1] = true;
					break;
				case 2:
					type = 2;
					facePropagated[3] = true;
					break;
				case 3:
					type = 2;
					facePropagated[2] = true;
					break;
			}
			break;
		case 1:
			switch( facePropagatedFrom )
			{
				case 0:
					type = 2;
					facePropagated[1] = true;
					break;
				case 1:
					type = 0;
					facePropagated[1] = true;
					break;
				case 2:
					type = 3;
					facePropagated[3] = true;
					break;
				case 3:
					type = 3;
					facePropagated[2] = true;
					break;
			}
			break;
		case 2:
			switch( facePropagatedFrom )
			{
				case 0:
					type = 3;
					facePropagated[0] = true;
					break;
				case 1:
					type = 1;
					facePropagated[0] = true;
					break;
				case 2:
					type = 0;
					facePropagated[3] = true;
					break;
				case 3:
					type = 0;
					facePropagated[2] = true;
					break;
			}
			break;
		case 3:
			switch( facePropagatedFrom )
			{
				case 0:
					type = 2;
					facePropagated[0] = true;
					break;
				case 1:
					type = 0;
					facePropagated[0] = true;
					break;
				case 2:
					type = 1;
					facePropagated[3] = true;
					break;
				case 3:
					type = 1;
					facePropagated[2] = true;
					break;
			}
			break;
	}


	engaged = false;
	//engagedInDistance = false;
	neighborPyramid = new Pyramid*[4];

	zomeDirection = firstPyramid -> zomeDirection;
	
	parent = pyramidGroup;

	SetCorner();
}

Pyramid::~Pyramid()
{
	delete position;

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
	delete[] neighborPyramid;
	//delete firstGrid;
	//delete link;
	if( corner )
	{
		delete corner[0] ,
			   corner[1] , 
			   corner[2] , 
			   corner[3] , 
			   corner[4] , 
			   corner[5];
		delete corner;
	}

	delete zomeDirection;
}

void Pyramid::SetCorner()
{
	//���U���O�Ĥ@���I
	corner = new Point*[4];
	//�M�䩹�U�@�Ө������
	GLfloat* translate = new GLfloat[3];
	GLfloat* translate2 = new GLfloat[3];
	GLfloat length = 0;
	GLfloat length2 = 0;

	switch ( type )
	{
		case 0:
			//�I0�G��0�� �Ŧ�
			length = zomeDirection -> GetFaceLength ( 0 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[0*3] * length , 
											   zomeDirection->faceDirection[0*3+1] * length ,
											   zomeDirection->faceDirection[0*3+2] * length );
			corner[0] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			//�I1�G���I
			corner[1] = new Point( position[0] , position[1] , position[2] );
			//�I2�G��54�� ����
			length = zomeDirection -> GetFaceLength ( 54 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[54*3] * length , 
											   zomeDirection->faceDirection[54*3+1] * length ,
											   zomeDirection->faceDirection[54*3+2] * length );
			corner[2] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			//�I3�G��57�� ����
			length = zomeDirection -> GetFaceLength ( 57 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[57*3] * length , 
											   zomeDirection->faceDirection[57*3+1] * length ,
											   zomeDirection->faceDirection[57*3+2] * length );
			corner[3] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			break;
		case 1:
			//�I0�G��2�� �Ŧ�
			length = zomeDirection -> GetFaceLength ( 2 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[2*3] * length , 
											   zomeDirection->faceDirection[2*3+1] * length ,
											   zomeDirection->faceDirection[2*3+2] * length );
			corner[0] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			//�I1�G���I
			corner[1] = new Point( position[0] , position[1] , position[2] );
			//�I2�G��54�� ����
			length = zomeDirection -> GetFaceLength ( 54 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[54*3] * length , 
											   zomeDirection->faceDirection[54*3+1] * length ,
											   zomeDirection->faceDirection[54*3+2] * length );
			corner[2] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			//�I3�G��57�� ����
			length = zomeDirection -> GetFaceLength ( 57 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[57*3] * length , 
											   zomeDirection->faceDirection[57*3+1] * length ,
											   zomeDirection->faceDirection[57*3+2] * length );
			corner[3] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			break;
		case 2:
			//�I0�G��54�� ���� //��57�� ����
			length = zomeDirection -> GetFaceLength ( 54 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[54*3] * length , 
											   zomeDirection->faceDirection[54*3+1] * length ,
											   zomeDirection->faceDirection[54*3+2] * length );
			length2 = zomeDirection -> GetFaceLength ( 57 , SIZE_S );
			MathHelper::SetVector( translate2 , zomeDirection->faceDirection[57*3] * length2 , 
											   zomeDirection->faceDirection[57*3+1] * length2 ,
											   zomeDirection->faceDirection[57*3+2] * length2 );
			corner[0] = new Point( position[0] + translate[0] + translate2[0] , 
								   position[1] + translate[1] + translate2[0] , 
								   position[2] + translate[2] + translate2[0] );
			//�I1�G��2�� �Ŧ�
			length = zomeDirection -> GetFaceLength ( 2 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[2*3] * length , 
											   zomeDirection->faceDirection[2*3+1] * length ,
											   zomeDirection->faceDirection[2*3+2] * length );
			corner[1] = new Point( position[0] + translate[0] , 
								   position[1] + translate[1] , 
								   position[2] + translate[2] );
			//�I2�G��54�� ����
			length = zomeDirection -> GetFaceLength ( 54 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[54*3] * length , 
											   zomeDirection->faceDirection[54*3+1] * length ,
											   zomeDirection->faceDirection[54*3+2] * length );
			corner[2] = new Point( position[0] + translate[0] , 
								   position[1] + translate[1] , 
								   position[2] + translate[2] );
			//�I3�G��57�� ����
			length = zomeDirection -> GetFaceLength ( 57 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[57*3] * length , 
											   zomeDirection->faceDirection[57*3+1] * length ,
											   zomeDirection->faceDirection[57*3+2] * length );
			corner[3] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			break;
		case 3:
			//�I0�G��54�� ���� //��57�� ����
			length = zomeDirection -> GetFaceLength ( 54 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[54*3] * length , 
											   zomeDirection->faceDirection[54*3+1] * length ,
											   zomeDirection->faceDirection[54*3+2] * length );
			length2 = zomeDirection -> GetFaceLength ( 57 , SIZE_S );
			MathHelper::SetVector( translate2 , zomeDirection->faceDirection[57*3] * length2 , 
											   zomeDirection->faceDirection[57*3+1] * length2 ,
											   zomeDirection->faceDirection[57*3+2] * length2 );
			corner[0] = new Point( position[0] + translate[0] + translate2[0] , 
								   position[1] + translate[1] + translate2[0] , 
								   position[2] + translate[2] + translate2[0] );
			//�I1�G��0�� �Ŧ�
			length = zomeDirection -> GetFaceLength ( 0 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[0*3] * length , 
											   zomeDirection->faceDirection[0*3+1] * length ,
											   zomeDirection->faceDirection[0*3+2] * length );
			corner[1] = new Point( position[0] + translate[0] , 
								   position[1] + translate[1] , 
								   position[2] + translate[2] );
			//�I2�G��54�� ����
			length = zomeDirection -> GetFaceLength ( 54 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[54*3] * length , 
											   zomeDirection->faceDirection[54*3+1] * length ,
											   zomeDirection->faceDirection[54*3+2] * length );
			corner[2] = new Point( position[0] + translate[0] , 
								   position[1] + translate[1] , 
								   position[2] + translate[2] );
			//�I3�G��57�� ����
			length = zomeDirection -> GetFaceLength ( 57 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[57*3] * length , 
											   zomeDirection->faceDirection[57*3+1] * length ,
											   zomeDirection->faceDirection[57*3+2] * length );
			corner[3] = new Point( position[0] + translate[0] , position[1] + translate[1] , position[2] + translate[2] );
			break;
	}
	delete translate;
	delete translate2;
}

bool Pyramid::CheckIntersectPyramidOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	
	//std::cout << "�Ĥ@���I�O�G" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;


	
	Point* d = new Point();

	bool egdeEngaged[6] = { false , false , false , false , false , false };

	bool isIntersected = false;

	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//�o��O? ��X�O�����I�M�����I���s�u
	for( int i = 0 ; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			int engagedEdge = -1;
			//��X�o�O���@�� ��4��type 4���I 6����
			switch( type )
			{
				case 0:
					switch( i )
					{
						case 0:
							if( j == 1 ) engagedEdge = 0;
							if( j == 2 ) engagedEdge = 2;
							if( j == 3 ) engagedEdge = 3;
							break;
						case 1:
							if( j == 2 ) engagedEdge = 1;
							if( j == 3 ) engagedEdge = 4;
							break;
						case 2:
							if( j == 3 ) engagedEdge = 2;
							break;
					}
					break;
				case 1:
					switch( i )
					{
						case 0:
							if( j == 1 ) engagedEdge = 0;
							if( j == 2 ) engagedEdge = 4;
							if( j == 3 ) engagedEdge = 5;
							break;
						case 1:
							if( j == 2 ) engagedEdge = 1;
							if( j == 3 ) engagedEdge = 2;
							break;
						case 2:
							if( j == 3 ) engagedEdge = 3;
							break;
					}
					break;
				case 2:
					switch( i )
					{
						case 0:
							if( j == 1 ) engagedEdge = 5;
							if( j == 2 ) engagedEdge = 2;
							if( j == 3 ) engagedEdge = 3;
							break;
						case 1:
							if( j == 2 ) engagedEdge = 1;
							if( j == 3 ) engagedEdge = 4;
							break;
						case 2:
							if( j == 3 ) engagedEdge = 0;
							break;
					}
					break;
				case 3:
					switch( i )
					{
						case 0:
							if( j == 1 ) engagedEdge = 1;
							if( j == 2 ) engagedEdge = 4;
							if( j == 3 ) engagedEdge = 5;
							break;
						case 1:
							if( j == 2 ) engagedEdge = 0;
							if( j == 3 ) engagedEdge = 2;
							break;
						case 2:
							if( j == 3 ) engagedEdge = 3;
							break;
					}
					break;
			}
			//�p�G�o�ӬO�� �Z���|����1 �N�}�l�⭻��
			//if( MathHelper::VectorDistance( corner[i] -> position , corner[j] -> position ) == zomeDirection.GetColorLength( COLOR_BLUE , SIZE_S ) && 
			//	!egdeEngaged[ engagedEdge ] )
			if( engagedEdge >= 0 && !egdeEngaged[ engagedEdge ] )
				//�o��O�� �p�Gi��j�O�@���� �L��engagedEdge�N�|�j�󵥩�0
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
	for( int i = 0; i < 6; i++ )
	{
		if( egdeEngaged[ i ] )
		{
			engaged = true;
			switch( type )
			{
				case 0:
					switch( i )
					{
						case 0:
							engagedFace[2] = true;
							engagedFace[3] = true;
							break;
						case 1:
							engagedFace[1] = true;
							engagedFace[2] = true;
							break;
						case 2:
							engagedFace[0] = true;
							engagedFace[2] = true;
							break;
						case 3:
							engagedFace[0] = true;
							engagedFace[3] = true;
							break;
						case 4:
							engagedFace[1] = true;
							engagedFace[3] = true;
							break;
						case 5:
							engagedFace[2] = true;
							engagedFace[3] = true;
							break;
					}
					break;
				case 1:
					switch( i )
					{
						case 0:
							engagedFace[2] = true;
							engagedFace[3] = true;
							break;
						case 1:
							engagedFace[1] = true;
							engagedFace[2] = true;
							break;
						case 2:
							engagedFace[1] = true;
							engagedFace[3] = true;
							break;
						case 3:
							engagedFace[0] = true;
							engagedFace[1] = true;
							break;
						case 4:
							engagedFace[0] = true;
							engagedFace[2] = true;
							break;
						case 5:
							engagedFace[0] = true;
							engagedFace[3] = true;
							break;
					}
					break;
				case 2:
					switch( i )
					{
						case 0:
							engagedFace[0] = true;
							engagedFace[1] = true;
							break;
						case 1:
							engagedFace[1] = true;
							engagedFace[2] = true;
							break;
						case 2:
							engagedFace[0] = true;
							engagedFace[2] = true;
							break;
						case 3:
							engagedFace[0] = true;
							engagedFace[3] = true;
							break;
						case 4:
							engagedFace[1] = true;
							engagedFace[3] = true;
							break;
						case 5:
							engagedFace[2] = true;
							engagedFace[3] = true;
							break;
					}
					break;
				case 3:
					switch( i )
					{
						case 0:
							engagedFace[1] = true;
							engagedFace[2] = true;
							break;
						case 1:
							engagedFace[2] = true;
							engagedFace[3] = true;
							break;
						case 2:
							engagedFace[1] = true;
							engagedFace[3] = true;
							break;
						case 3:
							engagedFace[0] = true;
							engagedFace[1] = true;
							break;
						case 4:
							engagedFace[0] = true;
							engagedFace[2] = true;
							break;
						case 5:
							engagedFace[0] = true;
							engagedFace[3] = true;
							break;
					}
					break;
			}
		}
	}

	delete d;
	delete e1;
	delete e2;

	return isIntersected;
}

bool Pyramid::CheckDuplicated()
{
	bool isDuplicated = false;
	for( Pyramid* i = firstPyramid; i -> link != NULL; i = i -> link )//�q�Ĥ@�Ө�˼ƲĤG�ӳ��S������ �̫�@�ӭ��I�S���Y�]���̫�@�ӴN�O�o�Ӥ���ۤv
	{
		if( Compare( i ) )
		{
			isDuplicated = true;
			break;
		}
	}
	return isDuplicated;
}

bool Pyramid::Compare( Pyramid* pyramid )
{
	if( position[0] == pyramid -> position[0] &&
		position[1] == pyramid -> position[1] &&
		position[2] == pyramid -> position[2] )
	{
		return true;
	}
	return false;
}

#pragma endregion

//============================================PYRAMID============================================

PyramidGroup::PyramidGroup( Point* centerPoint )
{
	position = new GLfloat[3];
	position[0] = centerPoint -> position[0];
	position[1] = centerPoint -> position[1];
	position[2] = centerPoint -> position[2];

	pyramidInserted = new bool[4];
	facePropagated = new bool[8];
	childrenPyramid = new Pyramid*[4];
	index = -1;

	firstPyramidGroup = this;
	zomeDirection = new ZomeDirection();

}

PyramidGroup::PyramidGroup( PyramidGroup* pyramidGroup , int facePropagatedFrom )
{
	position = new GLfloat[3];
	position[0] = pyramidGroup -> position[0];
	position[1] = pyramidGroup -> position[1];
	position[2] = pyramidGroup -> position[2];

	pyramidInserted = new bool[4];
	facePropagated = new bool[8];
	childrenPyramid = new Pyramid*[4];
	index = -1;

	zomeDirection = firstPyramidGroup -> zomeDirection;

	GLfloat length;
	GLfloat* translate = new GLfloat[3];

	//�ݬO�q���ӭ��W���L�Ӫ� �ӽվ�y��
	switch( facePropagatedFrom )
	{
		case 0:
			length = zomeDirection -> GetFaceLength ( 0 , SIZE_S );
			MathHelper::SetVector( translate , zomeDirection->faceDirection[0*3] * length , 
											   zomeDirection->faceDirection[0*3+1] * length ,
											   zomeDirection->faceDirection[0*3+2] * length );
	}


	//��̫�@�Ӥ���M�o���s�_��
	for( PyramidGroup* i = firstPyramidGroup; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = this;
			break;
		}
	}

}

void PyramidGroup::SetPyramid( int childrenPyramidIndex )
{
	pyramidInserted[ childrenPyramidIndex ] = true;
	childrenPyramid[ childrenPyramidIndex ] = new Pyramid( childrenPyramidIndex , this );

	//�]�w�̭����r�𪺭��O�_�Q�W��
	switch( childrenPyramidIndex )
	{
		case 0:
			if( facePropagated[0] )
			{
				childrenPyramid[0] -> facePropagated[2] = true;
			}
			if( facePropagated[4] )
			{
				childrenPyramid[0] -> facePropagated[3] = true;
			}
			break;
		case 1:
			if( facePropagated[1] )
			{
				childrenPyramid[0] -> facePropagated[2] = true;
			}
			if( facePropagated[5] )
			{
				childrenPyramid[0] -> facePropagated[3] = true;
			}
			break;
		case 2:
			if( facePropagated[2] )
			{
				childrenPyramid[0] -> facePropagated[2] = true;
			}
			if( facePropagated[6] )
			{
				childrenPyramid[0] -> facePropagated[3] = true;
			}
			break;
		case 3:
			if( facePropagated[3] )
			{
				childrenPyramid[0] -> facePropagated[2] = true;
			}
			if( facePropagated[7] )
			{
				childrenPyramid[0] -> facePropagated[3] = true;
			}
			break;
	}


}

void PyramidGroup::CheckInnerFacePropagate()
{
	if( pyramidInserted[0] && pyramidInserted[1] )
	{
		
	}
}