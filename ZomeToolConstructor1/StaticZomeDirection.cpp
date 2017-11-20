#include "StaticZomeDirection.h"

#ifndef GOLDEN
#define GOLDEN ( 1. + sqrt( 5. ) ) * 0.5
#endif

const double golden( (1. +sqrt(5.))*0.5);

bool StaticZomeDirection::GetFaceDirection( int faceIndex , GLfloat* result )
{
	GLfloat direction[3];

	switch( faceIndex )
	{
		//blue(0~5)
		case 0:
			MathHelper::SetVector( direction ,  1 , 0 , 0 );break;
		case 1:
			MathHelper::SetVector( direction , -1., 0., 0.);break;
		case 2:
			MathHelper::SetVector( direction , 0.,  1., 0.);break;
		case 3:
			MathHelper::SetVector( direction , 0., -1., 0.);break;
		case 4:
			MathHelper::SetVector( direction , 0., 0.,  1.);break;
		case 5:
			MathHelper::SetVector( direction , 0., 0., -1.);break;


		// diagonals
		// around x axis (6-13)
		case 6:
			MathHelper::SetVector( direction , 0.5*golden, 0.5/golden, 0.5);break;
		case 7:
			MathHelper::SetVector( direction , 0.5*golden, -0.5/golden, 0.5);break;
		case 8:
			MathHelper::SetVector( direction , -0.5*golden, 0.5/golden, 0.5);break;
		case 9:
			MathHelper::SetVector( direction , -0.5*golden, -0.5/golden, 0.5);break;
		case 10:

			MathHelper::SetVector( direction , -0.5*golden, -0.5/golden, -0.5);break;
		case 11:
			MathHelper::SetVector( direction , -0.5*golden, 0.5/golden, -0.5);break;
		case 12:
			MathHelper::SetVector( direction , 0.5*golden, -0.5/golden, -0.5);break;
		case 13:
			MathHelper::SetVector( direction , 0.5*golden, 0.5/golden, -0.5);break;
		


		// around y axis (14-21)
		case 14:
			MathHelper::SetVector( direction , 0.5, 0.5*golden, 0.5/golden);break;
		case 15:
			MathHelper::SetVector( direction , -0.5, 0.5*golden, 0.5/golden);break;
		case 16:
			MathHelper::SetVector( direction , 0.5, -0.5*golden, 0.5/golden);break;
		case 17:
			MathHelper::SetVector( direction , -0.5, -0.5*golden, 0.5/golden);break;
		
		case 18:
			MathHelper::SetVector( direction , -0.5, -0.5*golden, -0.5/golden);break;
		case 19:
			MathHelper::SetVector( direction , 0.5, -0.5*golden, -0.5/golden);break;
		case 20:
			MathHelper::SetVector( direction , -0.5, 0.5*golden, -0.5/golden);break;
		case 21:
			MathHelper::SetVector( direction , 0.5, 0.5*golden, -0.5/golden);break;
		


		// around z axis (22-29)
		case 22:
			MathHelper::SetVector( direction , 0.5/golden, 0.5, 0.5*golden);break;
		case 23:
			MathHelper::SetVector( direction , -0.5/golden, 0.5, 0.5*golden);break;
		case 24:
			MathHelper::SetVector( direction , 0.5/golden, -0.5, 0.5*golden);break;
		case 25:
			MathHelper::SetVector( direction , -0.5/golden, -0.5, 0.5*golden);break;
		
		case 26:	
			MathHelper::SetVector( direction , -0.5/golden, -0.5, -0.5*golden);break;
		case 27:
			MathHelper::SetVector( direction , 0.5/golden, -0.5, -0.5*golden);break;
		case 28:
			MathHelper::SetVector( direction , -0.5/golden, 0.5, -0.5*golden);break;
		case 29:
			MathHelper::SetVector( direction , 0.5/golden, 0.5, -0.5*golden);break;
		


		// reds (diagonals in planes)
		// around x axis (30-33)
		case 30:
			MathHelper::SetVector( direction , golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10), 0.);break;//除5+根號5除2 就是成5簡根號5除10
		case 31:
			MathHelper::SetVector( direction , golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10), 0.);break;
		case 32:
			MathHelper::SetVector( direction , -golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10), 0.);break;
		case 33:
			MathHelper::SetVector( direction , -golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10), 0.);break;
		
		

		// around y axis (34-37)
		case 34:
			MathHelper::SetVector( direction , 0., golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10));break;
		case 35:
			MathHelper::SetVector( direction , 0., golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10));break;
		case 36:
			MathHelper::SetVector( direction , 0., -golden*sqrt((5-sqrt(5))/10), -sqrt((5-sqrt(5))/10));break;
		case 37:
			MathHelper::SetVector( direction , 0., -golden*sqrt((5-sqrt(5))/10), sqrt((5-sqrt(5))/10));break;
	


		// around z axis (38-41)
		case 38:
			MathHelper::SetVector( direction , sqrt((5-sqrt(5))/10), 0., golden*sqrt((5-sqrt(5))/10));break;
		case 39:
			MathHelper::SetVector( direction , -sqrt((5-sqrt(5))/10), 0., golden*sqrt((5-sqrt(5))/10));break;
		case 40:
			MathHelper::SetVector( direction , -sqrt((5-sqrt(5))/10), 0., -golden*sqrt((5-sqrt(5))/10));break;
		case 41:
			MathHelper::SetVector( direction , sqrt((5-sqrt(5))/10), 0., -golden*sqrt((5-sqrt(5))/10));break;
		
		

		// yellows (diagonals in planes)
		// around x axis (42-35)
		case 42:
			MathHelper::SetVector( direction , golden / sqrt(3), 0.,  1.0/golden/ sqrt(3));break;
		case 43:
			MathHelper::SetVector( direction , golden/ sqrt(3), 0., -1.0/golden/ sqrt(3));break;
		case 44:
			MathHelper::SetVector( direction , -golden/ sqrt(3), 0., -1.0/golden/ sqrt(3));break;
		case 45:
			MathHelper::SetVector( direction , -golden/ sqrt(3), 0., 1.0/golden/ sqrt(3));break;
		


		// around y axis (46-49)
		case 46:
			MathHelper::SetVector( direction , 1.0/golden/ sqrt(3), golden/ sqrt(3), 0.);break;
		case 47:
			MathHelper::SetVector( direction , -1.0/golden/ sqrt(3), golden/ sqrt(3), 0.);break;
		case 48:
			MathHelper::SetVector( direction , -1.0/golden/ sqrt(3), -golden/ sqrt(3), 0.);break;
		case 49:
			MathHelper::SetVector( direction , 1.0/golden/ sqrt(3), -golden/ sqrt(3), 0.);break;
		
		

		// around z axis (50-53)
		case 50:
			MathHelper::SetVector( direction , 0., 1.0/golden/ sqrt(3), golden/ sqrt(3));break;
		case 51:
			MathHelper::SetVector( direction , 0., -1.0/golden/ sqrt(3), golden/ sqrt(3));break;
		case 52:
			MathHelper::SetVector( direction , 0., -1.0/golden/ sqrt(3), -golden/ sqrt(3));break;
		case 53:
			MathHelper::SetVector( direction , 0., 1.0/golden/ sqrt(3), -golden/ sqrt(3));break;


		// 54-61 (diagonals)
		case 54:
			MathHelper::SetVector( direction , sqrt(3) / 3 , sqrt(3) / 3 , sqrt(3) / 3 );break;
		case 55:
			MathHelper::SetVector( direction , -sqrt(3) / 3 , sqrt(3) / 3 , sqrt(3) / 3 );break;
		case 56:
			MathHelper::SetVector( direction , sqrt(3) / 3 , -sqrt(3) / 3 , sqrt(3) / 3 );break;
		case 57:
			MathHelper::SetVector( direction , sqrt(3) / 3 , sqrt(3) / 3 , -sqrt(3) / 3 );break;
		
		case 58:
			MathHelper::SetVector( direction , sqrt(3) / 3 , -sqrt(3) / 3 , -sqrt(3) / 3 );break;
		case 59:
			MathHelper::SetVector( direction , -sqrt(3) / 3, sqrt(3) / 3, -sqrt(3) / 3);break;
		case 60:
			MathHelper::SetVector( direction , -sqrt(3) / 3, -sqrt(3) / 3, sqrt(3) / 3);break;
		case 61:
			MathHelper::SetVector( direction , -sqrt(3) / 3, -sqrt(3) / 3, -sqrt(3) / 3);break;
	}

	MathHelper::VectorNormalize( direction );

	MathHelper::CopyVector( result , direction );

	return true;
	/*
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
  */
}