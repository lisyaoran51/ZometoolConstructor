#ifndef ZOME_DIRECTION
#define ZOME_DIRECTION

#include "stdafx.h"



#define SIZE_S 0
#define SIZE_M 1
#define SIZE_L 2

#define COLOR_BLUE 0
#define COLOR_RED 1
#define COLOR_YELLOW 2
#define COLOR_WHITE 3

#define NODE_DIAMETER 17.7
#define LATCH 4.0
#define SCALE 47.3//原本是46.3改47.3
#define GOLDEN ( 1. + sqrt( 5. ) ) * 0.5

#define WEIGHT_BLUE_S 1.7
#define WEIGHT_BLUE_M 3.0
#define WEIGHT_BLUE_L 4.9
#define WEIGHT_RED_S 1.8
#define WEIGHT_RED_M 3.1
#define WEIGHT_RED_L 5.1
#define WEIGHT_YELLOW_S 1.7
#define WEIGHT_YELLOW_M 2.9
#define WEIGHT_YELLOW_L 4.8
#define WEIGHT_WHITE 3.0


enum Color
{
	Blue,
	Red,
	Yellow
};

const double golden( (1. +sqrt(5.))*0.5);
/*  
  //blues
  //0-5 -- axis aligned
  dirs3d_.push_back( Vec3(  1., 0., 0.));
  dirs3d_.push_back( Vec3( -1., 0., 0.));
  dirs3d_.push_back( Vec3( 0.,  1., 0.));
  dirs3d_.push_back( Vec3( 0., -1., 0.));
  dirs3d_.push_back( Vec3( 0., 0.,  1.));
  dirs3d_.push_back( Vec3( 0., 0., -1.));

  // diagonals
  // around x axis (6-13)
  dirs3d_.push_back( Vec3( 0.5*golden, 0.5/golden, 0.5));
  dirs3d_.push_back( Vec3( 0.5*golden, -0.5/golden, 0.5));
  dirs3d_.push_back( Vec3( -0.5*golden, 0.5/golden, 0.5));
  dirs3d_.push_back( Vec3( -0.5*golden, -0.5/golden, 0.5));

  dirs3d_.push_back( Vec3( -0.5*golden, -0.5/golden, -0.5));
  dirs3d_.push_back( Vec3( -0.5*golden, 0.5/golden, -0.5));
  dirs3d_.push_back( Vec3( 0.5*golden, -0.5/golden, -0.5));
  dirs3d_.push_back( Vec3( 0.5*golden, 0.5/golden, -0.5));

  // around y axis (14-21)
  dirs3d_.push_back( Vec3( 0.5, 0.5*golden, 0.5/golden));
  dirs3d_.push_back( Vec3( -0.5, 0.5*golden, 0.5/golden));
  dirs3d_.push_back( Vec3( 0.5, -0.5*golden, 0.5/golden));
  dirs3d_.push_back( Vec3( -0.5, -0.5*golden, 0.5/golden));

  dirs3d_.push_back( Vec3( -0.5, -0.5*golden, -0.5/golden));
  dirs3d_.push_back( Vec3( 0.5, -0.5*golden, -0.5/golden));
  dirs3d_.push_back( Vec3( -0.5, 0.5*golden, -0.5/golden));
  dirs3d_.push_back( Vec3( 0.5, 0.5*golden, -0.5/golden));

  // around z axis (22-29)
  dirs3d_.push_back( Vec3( 0.5/golden, 0.5, 0.5*golden));
  dirs3d_.push_back( Vec3( -0.5/golden, 0.5, 0.5*golden));
  dirs3d_.push_back( Vec3( 0.5/golden, -0.5, 0.5*golden));
  dirs3d_.push_back( Vec3( -0.5/golden, -0.5, 0.5*golden));

  dirs3d_.push_back( Vec3( -0.5/golden, -0.5, -0.5*golden));
  dirs3d_.push_back( Vec3( 0.5/golden, -0.5, -0.5*golden));
  dirs3d_.push_back( Vec3( -0.5/golden, 0.5, -0.5*golden));
  dirs3d_.push_back( Vec3( 0.5/golden, 0.5, -0.5*golden));

  // reds (diagonals in planes)
  // around x axis (30-33)
  dirs3d_.push_back( Vec3(  golden*0.5, 0.5, 0.));
  dirs3d_.push_back( Vec3(  golden*0.5, -0.5, 0.));
  dirs3d_.push_back( Vec3(  -golden*0.5, -0.5, 0.));
  dirs3d_.push_back( Vec3(  -golden*0.5, 0.5, 0.));
  
  // around y axis (34-37)
  dirs3d_.push_back( Vec3(  0., golden*0.5, 0.5));
  dirs3d_.push_back( Vec3(  0., golden*0.5, -0.5));
  dirs3d_.push_back( Vec3(  0., -golden*0.5, -0.5));
  dirs3d_.push_back( Vec3(  0., -golden*0.5, 0.5));

  // around z axis (38-41)
  dirs3d_.push_back( Vec3( 0.5, 0., golden*0.5));
  dirs3d_.push_back( Vec3( -0.5, 0., golden*0.5));
  dirs3d_.push_back( Vec3( -0.5, 0., -golden*0.5));
  dirs3d_.push_back( Vec3( 0.5, 0., -golden*0.5));

  // yellows (diagonals in planes)
  // around x axis (42-35)
  dirs3d_.push_back( Vec3( golden*0.5, 0.,  0.5/golden));
  dirs3d_.push_back( Vec3( golden*0.5, 0., -0.5/golden));
  dirs3d_.push_back( Vec3( -golden*0.5, 0., -0.5/golden));
  dirs3d_.push_back( Vec3( -golden*0.5, 0., 0.5/golden));

  // around y axis (46-49)
  dirs3d_.push_back( Vec3( 0.5/golden, 0.5*golden, 0.));
  dirs3d_.push_back( Vec3( -0.5/golden, 0.5*golden, 0.));
  dirs3d_.push_back( Vec3( -0.5/golden, -0.5*golden, 0.));
  dirs3d_.push_back( Vec3( 0.5/golden, -0.5*golden, 0.));

  // around z axis (50-53)
  dirs3d_.push_back( Vec3( 0., 0.5/golden, golden*0.5));
  dirs3d_.push_back( Vec3( 0., -0.5/golden, golden*0.5));
  dirs3d_.push_back( Vec3( 0., -0.5/golden, -golden*0.5));
  dirs3d_.push_back( Vec3( 0., 0.5/golden, -golden*0.5));

  // 54-61 (diagonals)
  dirs3d_.push_back( Vec3( 0.5, 0.5, 0.5));
  dirs3d_.push_back( Vec3( -0.5, 0.5, 0.5));
  dirs3d_.push_back( Vec3( 0.5, -0.5, 0.5));
  dirs3d_.push_back( Vec3( 0.5, 0.5, -0.5));
  dirs3d_.push_back( Vec3( 0.5, -0.5, -0.5));
  dirs3d_.push_back( Vec3( -0.5, 0.5, -0.5));
  dirs3d_.push_back( Vec3( -0.5, -0.5, 0.5));
  dirs3d_.push_back( Vec3( -0.5, -0.5, -0.5));
  */

class ZomeDirection
{
public:
	ZomeDirection();
	~ZomeDirection();
	void PushFaceDirection(GLfloat x , GLfloat y , GLfloat z );
	GLfloat Theta(int index);
	GLfloat Phi(int index);
	GLfloat Yaw(int index);


	GLfloat* faceDirection;
	int count;

	GLfloat GetFaceLength( int index , int size );
	GLfloat GetColorLength( int index , int size );
	int GetFaceColor( int index );
	int GetOppositeFace( int index );

	int GetDirectionFace( GLfloat* direction );
	int GetDirectionFace( GLfloat* direction , int size );
	int GetDirectionFace( GLfloat* vecInitial , GLfloat* vecEnd , int size );
	bool GetFaceDirection( int f , GLfloat* direction );

	float GetDirectionParameter( int i , int d );//拿第i面的第d個維度
	float GetWeight( int color , int size );//拿某個顏色某個長度的重量
private:
};


#endif