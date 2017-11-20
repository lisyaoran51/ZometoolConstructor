#ifndef MYPOINT
#define MYPOINT



#include "stdafx.h"



class Point
{
public:
	Point();
	Point( GLfloat x , GLfloat y , GLfloat z );
	Point( GLfloat* pos );
	Point( char* FilePath );
	~Point();

	void SetPosition( GLfloat x , GLfloat y , GLfloat z );
	bool AddToLink( Point* p );//還沒寫
	bool Contain( GLfloat* pos );//還沒寫
	int Size();

	void Vector( Point* pointInit , Point* pointEnd );
	void Vector( GLfloat* positionInit , GLfloat* positionEnd );


	void Copy( Point* point );
	void Copy( GLfloat* pos );
	void Normalize();
	void Reverse();
	GLfloat Absolution();
	GLfloat DistanceToPoint( GLfloat* position );
	GLfloat Area( GLfloat* position1 , GLfloat* position2 );
	
	void Plus( Point* point );
	void Plus( GLfloat* position );
	void Minus( Point* point );
	void Multiply( GLfloat multiplier );
	GLfloat Dot( Point* point );
	GLfloat Dot( GLfloat* pos );
	void Cross( Point* vec1 , Point* vec2 );

	bool Compare( Point* point );
	bool CloselyCompare( Point* point );
	bool CloselyCompare( GLfloat* pos );
	void Mirror( Point* point , GLfloat a , GLfloat b , GLfloat c , GLfloat d );


	GLfloat* position;
	GLfloat normal[3];
	int index;

	Point* firstPoint;
	Point* link;



};

class PointList
{
public:
	PointList();
	PointList( Point* point );
	~PointList();
	void Add( Point* point );
	void Add( Point* point , int i );
	void Add( GLfloat* pos );
	void Remove( Point* point );
	bool Contain( Point* point );
	bool CloselyContain( Point* point );
	bool Contain( GLfloat* pos );
	void Get( GLfloat* pos , int index );

	int index;
	GLfloat* position;
	PointList* firstPointList;
	PointList* link;
};

#endif