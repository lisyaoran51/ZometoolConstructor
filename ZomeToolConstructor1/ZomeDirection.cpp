#include "ZomeDirection.h"


ZomeDirection::ZomeDirection()
{
	count = 0;
	faceDirection = new GLfloat[ 3 * 62 ];
	//blue
	//0~5
	PushFaceDirection(  1., 0., 0.);
	PushFaceDirection( -1., 0., 0.);
	PushFaceDirection( 0.,  1., 0.);
	PushFaceDirection( 0., -1., 0.);
	PushFaceDirection( 0., 0.,  1.);
	PushFaceDirection( 0., 0., -1.);
	// diagonals
  // around x axis (6-13)
	PushFaceDirection( 0.5*golden, 0.5/golden, 0.5);
	PushFaceDirection( 0.5*golden, -0.5/golden, 0.5);
	PushFaceDirection( -0.5*golden, 0.5/golden, 0.5);
	PushFaceDirection( -0.5*golden, -0.5/golden, 0.5);

	
  PushFaceDirection( -0.5*golden, -0.5/golden, -0.5);
  PushFaceDirection( -0.5*golden, 0.5/golden, -0.5);
  PushFaceDirection( 0.5*golden, -0.5/golden, -0.5);
  PushFaceDirection( 0.5*golden, 0.5/golden, -0.5);

  // around y axis (14-21)
  PushFaceDirection( 0.5, 0.5*golden, 0.5/golden);
  PushFaceDirection( -0.5, 0.5*golden, 0.5/golden);
  PushFaceDirection( 0.5, -0.5*golden, 0.5/golden);
  PushFaceDirection( -0.5, -0.5*golden, 0.5/golden);

  PushFaceDirection( -0.5, -0.5*golden, -0.5/golden);
  PushFaceDirection( 0.5, -0.5*golden, -0.5/golden);
  PushFaceDirection( -0.5, 0.5*golden, -0.5/golden);
  PushFaceDirection( 0.5, 0.5*golden, -0.5/golden);

  // around z axis (22-29)
  PushFaceDirection( 0.5/golden, 0.5, 0.5*golden);
  PushFaceDirection( -0.5/golden, 0.5, 0.5*golden);
  PushFaceDirection( 0.5/golden, -0.5, 0.5*golden);
  PushFaceDirection( -0.5/golden, -0.5, 0.5*golden);

  PushFaceDirection( -0.5/golden, -0.5, -0.5*golden);
  PushFaceDirection( 0.5/golden, -0.5, -0.5*golden);
  PushFaceDirection( -0.5/golden, 0.5, -0.5*golden);
  PushFaceDirection( 0.5/golden, 0.5, -0.5*golden);

  // reds (diagonals in planes)
  // around x axis (30-33)
  PushFaceDirection(  golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10), 0.);//除5+根號5除2 就是成5簡根號5除10
  PushFaceDirection(  golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10), 0.);
  PushFaceDirection(  -golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10), 0.);
  PushFaceDirection(  -golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10), 0.);
  
  // around y axis (34-37)
  PushFaceDirection(  0., golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10));
  PushFaceDirection(  0., golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10));
  PushFaceDirection(  0., -golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10));
  PushFaceDirection(  0., -golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10));

  // around z axis (38-41)
  PushFaceDirection( sqrt((5-sqrt(5))/10), 0., golden*sqrt((5-sqrt(5))/10));
  PushFaceDirection( -sqrt((5-sqrt(5))/10), 0., golden*sqrt((5-sqrt(5))/10));
  PushFaceDirection( -sqrt((5-sqrt(5))/10), 0., -golden*sqrt((5-sqrt(5))/10));
  PushFaceDirection( sqrt((5-sqrt(5))/10), 0., -golden*sqrt((5-sqrt(5))/10));

  // yellows (diagonals in planes)
  // around x axis (42-35)
  PushFaceDirection( golden / sqrt(3), 0.,  1.0/golden/ sqrt(3));
  PushFaceDirection( golden/ sqrt(3), 0., -1.0/golden/ sqrt(3));
  PushFaceDirection( -golden/ sqrt(3), 0., -1.0/golden/ sqrt(3));
  PushFaceDirection( -golden/ sqrt(3), 0., 1.0/golden/ sqrt(3));

  // around y axis (46-49)
  PushFaceDirection( 1.0/golden/ sqrt(3), golden/ sqrt(3), 0.);
  PushFaceDirection( -1.0/golden/ sqrt(3), golden/ sqrt(3), 0.);
  PushFaceDirection( -1.0/golden/ sqrt(3), -golden/ sqrt(3), 0.);
  PushFaceDirection( 1.0/golden/ sqrt(3), -golden/ sqrt(3), 0.);

  // around z axis (50-53)
  PushFaceDirection( 0., 1.0/golden/ sqrt(3), golden/ sqrt(3));
  PushFaceDirection( 0., -1.0/golden/ sqrt(3), golden/ sqrt(3));
  PushFaceDirection( 0., -1.0/golden/ sqrt(3), -golden/ sqrt(3));
  PushFaceDirection( 0., 1.0/golden/ sqrt(3), -golden/ sqrt(3));

  // 54-61 (diagonals)
  PushFaceDirection( sqrt(3) / 3 , sqrt(3) / 3 , sqrt(3) / 3 );
  PushFaceDirection( -sqrt(3) / 3 , sqrt(3) / 3 , sqrt(3) / 3 );
  PushFaceDirection( sqrt(3) / 3 , -sqrt(3) / 3 , sqrt(3) / 3 );
  PushFaceDirection( sqrt(3) / 3 , sqrt(3) / 3 , -sqrt(3) / 3 );
  PushFaceDirection( sqrt(3) / 3 , -sqrt(3) / 3 , -sqrt(3) / 3 );
  PushFaceDirection( -sqrt(3) / 3, sqrt(3) / 3, -sqrt(3) / 3);
  PushFaceDirection( -sqrt(3) / 3, -sqrt(3) / 3, sqrt(3) / 3);
  PushFaceDirection( -sqrt(3) / 3, -sqrt(3) / 3, -sqrt(3) / 3);

  for( int i = 0; i < 62; i++ )
  {
	  MathHelper::VectorNormalize( &( faceDirection[ i * 3 ] ) );
  }

}

ZomeDirection::~ZomeDirection()
{
	delete[] faceDirection;
}

void ZomeDirection::PushFaceDirection(GLfloat x , GLfloat y , GLfloat z )
{
	faceDirection[count] = x;
	faceDirection[count+1] = y;
	faceDirection[count+2] = z;
	count += 3;
}

GLfloat ZomeDirection::Theta(int index)
{
	GLfloat sinTheta = faceDirection[index*3+2] / sqrt( pow( faceDirection[index*3] , 2 ) + pow( faceDirection[index*3+2] , 2 ) );
	GLfloat theta = asin( sinTheta );
	if ( faceDirection[index*3] < 0 )
	{
		theta = M_PI - theta;
	}
	if ( sqrt( pow( faceDirection[index*3] , 2 ) + pow( faceDirection[index*3+2] , 2 ) ) < SMALL_NUMBER )
	{
		theta = 0;
	}
	return theta;
}

GLfloat ZomeDirection::Phi(int index)
{

	GLfloat sinPhi = faceDirection[index*3+1] / sqrt( pow( faceDirection[index*3] , 2 ) + pow( faceDirection[index*3+1] , 2 ) + pow( faceDirection[index*3+2] , 2 ) );
	GLfloat phi = acos( sinPhi );
	return phi;
}

GLfloat ZomeDirection::Yaw(int index)
{
	GLfloat yaw;
	if ( index < 30 )
	{
		GLfloat sinYaw = faceDirection[index*3+2] / sqrt( pow( faceDirection[index*3] , 2 ) + pow( faceDirection[index*3+2] , 2 ) );
		yaw = asin( sinYaw );
		if ( index == 0 )yaw = M_PI * 0.5;
		if ( index == 1 )yaw = M_PI * 0.5;
		if ( index == 2 )yaw = 0.0;
		if ( index == 3 )yaw = 0.0;
		if ( index == 4 )yaw = 0.0;
		if ( index == 5 )yaw = 0.0;

		if ( index == 6 )yaw = M_PI - yaw;

		if ( index == 9 )yaw = M_PI - yaw;
		if ( index == 10 )yaw = M_PI - yaw;
		if ( index == 11 );
		if ( index == 12 )yaw = -(M_PI - yaw);
		if ( index == 13 )yaw = M_PI - yaw;
		if ( index == 14 );
		if ( index == 15 )yaw = M_PI - yaw;
		if ( index == 16 )yaw = -yaw;
		if ( index == 17 )yaw = -(M_PI - yaw);
		if ( index == 18 );
		if ( index == 19 )yaw = -yaw;
		if ( index == 20 )yaw = M_PI - yaw;
		if ( index == 22 );
		if ( index == 23 )yaw = M_PI - yaw;
		if ( index == 24 )yaw = M_PI - yaw;
		if ( index == 25 )yaw = -(M_PI - yaw);
		if ( index == 26 );
		if ( index == 27 )yaw = M_PI - yaw;
		if ( index == 28 )yaw = M_PI - yaw;


		if ( 0 && sqrt( pow( faceDirection[index*3] , 2 ) + pow( faceDirection[index*3+2] , 2 ) ) < SMALL_NUMBER )
		{
			yaw = 0;
		}
	}
	else if ( index < 42 )
	{
		if ( index == 30 ) yaw = 0;
		if ( index == 31 ) yaw = M_PI;
		if ( index == 32 ) yaw = M_PI;
		if ( index == 33 ) yaw = 0;
		if ( index == 34 ) yaw = M_PI;
		if ( index == 35 ) yaw = M_PI;
		if ( index == 36 ) yaw = 0;
		if ( index == 37 ) yaw = 0;
		if ( index == 38 ) yaw = M_PI * 0.5;
		if ( index == 39 ) yaw = M_PI * 1.5;
		if ( index == 40 ) yaw = M_PI * 0.5;
		if ( index == 41 ) yaw = M_PI * 1.5;
	}
	else
	{
		if ( index == 42 ) yaw = M_PI * 0.5;
		if ( index == 43 ) yaw = M_PI * 1.5;
		if ( index == 44 ) yaw = M_PI * 0.5;
		if ( index == 45 ) yaw = M_PI * 1.5;
		if ( index == 46 ) yaw = 0;
		if ( index == 47 ) yaw = 0;
		if ( index == 48 ) yaw = M_PI;
		if ( index == 49 ) yaw = M_PI;
		if ( index == 50 ) yaw = M_PI;
		if ( index == 51 ) yaw = 0;
		if ( index == 52 ) yaw = 0;
		if ( index == 53 ) yaw = M_PI;

		if ( index == 54 ) yaw = 22.2369 / 180.0 * M_PI;
		if ( index == 55 ) yaw = -22.2369 / 180.0 * M_PI;
		if ( index == 56 ) yaw = -22.2369 / 180.0 * M_PI + M_PI;
		if ( index == 57 ) yaw = -22.2369 / 180.0 * M_PI;
		if ( index == 58 ) yaw = 22.2369 / 180.0 * M_PI + M_PI;
		if ( index == 59 ) yaw = 22.2369 / 180.0 * M_PI;
		if ( index == 60 ) yaw = 22.2369 / 180.0 * M_PI + M_PI;
		if ( index == 61 ) yaw = -22.2369 / 180.0 * M_PI + M_PI;
		
	}
	return yaw;
}


GLfloat ZomeDirection::GetFaceLength( int index , int size )
{
	//比例設成1:46.3mm
	GLfloat length;
	if ( index < 30 )
	{
		if ( size == SIZE_S ) length = 1 * SCALE;//46.3
		if ( size == SIZE_M ) length = GOLDEN * SCALE;//74.9
		if ( size == SIZE_L ) length = GOLDEN * GOLDEN * SCALE;//121.2
	}
	else if ( index < 42 )
	{
		if ( size == SIZE_S ) length = sqrt( 2 + GOLDEN ) / 2 * SCALE;//43.8
		if ( size == SIZE_M ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * SCALE;//71
		if ( size == SIZE_L ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * GOLDEN * SCALE;//115
	}
	else
	{
		if ( size == SIZE_S ) length = sqrt(3) / 2 * SCALE;//40.1
		if ( size == SIZE_M ) length = sqrt(3) / 2 * GOLDEN * SCALE;//64.8
		if ( size == SIZE_L ) length = sqrt(3) / 2 * GOLDEN * GOLDEN * SCALE;//104.9
	}
	return length;
}

GLfloat ZomeDirection::GetColorLength( int index , int size )
{
	//比例設成1:46.3mm
	GLfloat length;
	if ( index == COLOR_BLUE )
	{
		if ( size == SIZE_S ) length = 1 * SCALE;//46.3
		if ( size == SIZE_M ) length = GOLDEN * SCALE;//74.9
		if ( size == SIZE_L ) length = GOLDEN * GOLDEN * SCALE;//121.2
	}
	else if ( index == COLOR_RED )
	{
		if ( size == SIZE_S ) length = sqrt( 2 + GOLDEN ) / 2 * SCALE;//43.8
		if ( size == SIZE_M ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * SCALE;//71
		if ( size == SIZE_L ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * GOLDEN * SCALE;//115
	}
	else if ( index == COLOR_YELLOW )
	{
		if ( size == SIZE_S ) length = sqrt(3) / 2 * SCALE;//40.1
		if ( size == SIZE_M ) length = sqrt(3) / 2 * GOLDEN * SCALE;//64.8
		if ( size == SIZE_L ) length = sqrt(3) / 2 * GOLDEN * GOLDEN * SCALE;//104.9
	}
	else
	{}
	return length;
}

int ZomeDirection::GetFaceColor( int index )
{
	int color;
	if ( index < 30 )
	{
		color = COLOR_BLUE;
	}
	else if ( index < 42 )
	{
		color = COLOR_RED;
	}
	else
	{
		color = COLOR_YELLOW;
	}
	return color;
}

int ZomeDirection::GetOppositeFace( int index )
{
	if( index < 0 )
	{
		std::cout << "ZomeDirection::GetOppositeFace出錯：面的編號是-1" << std::endl;
	}
	GLfloat fDirection[3];
	MathHelper::CopyVector( fDirection , &faceDirection[ index * 3 ] );
	MathHelper::VectorReverse( fDirection );
	MathHelper::VectorNormalize( fDirection );
	return GetDirectionFace( fDirection );
}

int ZomeDirection::GetDirectionFace( GLfloat* direction )
{
	//GLfloat distance = MathHelper::VectorDistance( vecInitial , vecEnd );

	for( int i = 0; i < 62; i++ )
	{
		if( MathHelper::CompareVector( direction , &( faceDirection[ i * 3 ] ) ) )
		{
			return i;
		}

		if( MathHelper::VectorApproximation( direction , &( faceDirection[ i * 3 ] ) ) )
		{
			
			return i;
		}

	}
	return -1;
}

int ZomeDirection::GetDirectionFace( GLfloat* vecInitial , GLfloat* vecEnd , int size )
{
	GLfloat* direction = new GLfloat[3];
	MathHelper::VectorMinus( direction , vecEnd , vecInitial );
	MathHelper::vectorNormalize( direction );

	GLfloat distance = MathHelper::VectorDistance( vecInitial , vecEnd );

	for( int i = 0; i < 62; i++ )
	{
		if( MathHelper::CompareVector( direction , &( faceDirection[ i * 3 ] ) ) && 
			distance == GetFaceLength( i , size ) )
		{
			delete direction;
			return i;
		}

		if( MathHelper::VectorApproximation( direction , &( faceDirection[ i * 3 ] ) ) )
		{
			if( abs( distance - GetFaceLength( i , size ) ) < SMALL_NUMBER )
			{
				delete direction;
				return i;
			}
		}

	}
	return -1;
}

bool ZomeDirection::GetFaceDirection( int f , GLfloat* direction )
{
	if( f < 0 || f > 62 )
	{
		return false;
	}
	direction[0] = faceDirection[ f * 3 ];
	direction[1] = faceDirection[ f * 3 + 1 ];
	direction[2] = faceDirection[ f * 3 + 2 ];
	return true;
}

float ZomeDirection::GetDirectionParameter( int i , int d )
{
	return -faceDirection[ i * 3 + d ];
}

float ZomeDirection::GetWeight( int color , int size )
{
	GLfloat weight[3][3] = 
	{ { WEIGHT_BLUE_S , WEIGHT_BLUE_M , WEIGHT_BLUE_L } , 
	  { WEIGHT_RED_S , WEIGHT_RED_M , WEIGHT_RED_L } ,
	  { WEIGHT_YELLOW_S , WEIGHT_YELLOW_M , WEIGHT_YELLOW_L } };
	if( color < 3 )
	{
		return weight[ color ][ size ];
	}
	else if ( color == 3 )
	{
		return WEIGHT_WHITE;
	}
	else
	{
		std::cout << "ZomeDirection::GetWeight出錯:顏色輸入值錯誤" << std::endl;
		return 0;
	}
}