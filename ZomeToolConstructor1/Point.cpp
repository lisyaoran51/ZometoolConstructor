

#include "Point.h"

#pragma region POINT

Point::Point( )
{
	position = new GLfloat[3];
	position[0] = 0;
	position[1] = 0;
	position[2] = 0;
	//normal = new GLfloat[3];
	index = -1;
	firstPoint = NULL;
}

Point::Point( GLfloat x , GLfloat y , GLfloat z )
{
	position = new GLfloat[3];
	position[0] = x;
	position[1] = y;
	position[2] = z;
	//normal = new GLfloat[3];

	index = -1;
	firstPoint = NULL;
}

Point::Point( GLfloat* pos )
{
	position = new GLfloat[3];
	position[0] = pos[0];
	position[1] = pos[1];
	position[2] = pos[2];
	//normal = new GLfloat[3];

	index = -1;
	firstPoint = NULL;
}

Point::Point( char* FilePath )
{
	position = new GLfloat[3];
	firstPoint = this;

	FILE* pointFile = fopen(FilePath, "r");
	if (!pointFile){
		std::cout << "無法讀取 \"" << FilePath << "\" !" << std::endl;
	}
	
	char* TempLine = new char[1000];
	bool firstPoint = true;
	GLfloat tempPosition[] = {0,0,0};
	while (fscanf(pointFile, "%f %f %f\n", &tempPosition[0] , &tempPosition[1] , &tempPosition[2] ) != EOF)
	{
		
		//fscanf(pointFile, "%f %f %f\n", &tempPosition[0] , &tempPosition[1] , &tempPosition[2] );
		std::cout << "讀取表面上點(" << tempPosition[0] << " , " << tempPosition[1] << " , " << tempPosition[2] << ")" << std::endl;
		if( firstPoint )
		{
			Copy( tempPosition );
			firstPoint = false;
		}
		else
		{
			Point* tempPoint = new Point( tempPosition );
			AddToLink( tempPoint );
		}
	}
	delete[] TempLine;
	fclose(pointFile);
}

Point::~Point()
{
	//delete[] normal;
	delete[] position;
	//std::cout<<"姐";
}


void Point::SetPosition( GLfloat x , GLfloat y , GLfloat z )
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

bool Point::AddToLink( Point* p )
{
	p -> firstPoint = firstPoint;
	if( firstPoint == NULL )
	{
		return false;
	}
	for( Point* i = firstPoint; i != NULL; i = i -> link )
	{
		if( p == i )
		{
			return false;
		}
		if( i -> link == NULL )
		{
			i -> link = p;
			break;
		}
	}
	return true;
}

bool Point::Contain( GLfloat* pos )
{
	for( Point* i = firstPoint; i != NULL; i = i -> link )
	{
		if( MathHelper::CompareVector( pos , i -> position ) )
		{
			return true;
		}
	}
	return false;
}

int Point::Size()
{
	int size = 0;
	for( Point* i = firstPoint; i != NULL; i = i -> link )
	{
		size++;
	}
	return size;
}

void Point::Vector( Point* pointInit , Point* pointEnd )
{
	position[0] = pointEnd -> position[0] - pointInit -> position[0];
	position[1] = pointEnd -> position[1] - pointInit -> position[1];
	position[2] = pointEnd -> position[2] - pointInit -> position[2];
}

void Point::Vector( GLfloat* positionInit , GLfloat* positionEnd )
{
	position[0] = positionEnd[0] - positionInit[0];
	position[1] = positionEnd[1] - positionInit[1];
	position[2] = positionEnd[2] - positionInit[2];
}

void Point::Copy( Point* point )
{
	position[0] = point -> position[0];
	position[1] = point -> position[1];
	position[2] = point -> position[2];
}

void Point::Copy( GLfloat* pos )
{
	position[0] = pos[0];
	position[1] = pos[1];
	position[2] = pos[2];
}

void Point::Normalize()
{
	GLfloat absolute = sqrt( pow( position[0] , 2 ) + pow( position[1] , 2 ) + pow( position[2] , 2 ) );
	if( absolute == 0 ) 
	{
		std::cout << "Normalize失敗" << std::endl;
	}
	else
	{
		position[0] = position[0] / absolute;
		position[1] = position[1] / absolute;
		position[2] = position[2] / absolute;
	}
}

void Point::Reverse()
{
	position[0] = -position[0];
	position[1] = -position[1];
	position[2] = -position[2];
}

//絕對值，長度
GLfloat Point::Absolution()
{
	GLfloat absolute = sqrt( pow( position[0] , 2 ) + pow( position[1] , 2 ) + pow( position[2] , 2 ) );
	return absolute;
}

GLfloat Point::DistanceToPoint( GLfloat* pos )
{
	return MathHelper::VectorDistance( position , pos );
}

GLfloat Point::Area( GLfloat* position1 , GLfloat* position2 )
{
	GLfloat vector1[] = {0,0,0};
	GLfloat vector2[] = {0,0,0};
	GLfloat normal[] = {0,0,0};

	MathHelper::VectorMinus( vector1 , position1 , position );
	MathHelper::VectorMinus( vector2 , position2 , position );
	MathHelper::VectorCorss( normal , vector1 , vector2 );

	return MathHelper::VectorAbsolution( normal ) / 2;
}
void Point::Plus( Point* point )
{
	position[0] += point -> position[0];
	position[1] += point -> position[1];
	position[2] += point -> position[2];
}

void Point::Plus( GLfloat* pos )
{
	position[0] += pos[0];
	position[1] += pos[1];
	position[2] += pos[2];
}

void Point::Minus( Point* point )
{
	position[0] -= point -> position[0];
	position[1] -= point -> position[1];
	position[2] -= point -> position[2];
}

void Point::Multiply( GLfloat multiplier )
{
	position[0] *= multiplier;
	position[1] *= multiplier;
	position[2] *= multiplier;
}

GLfloat Point::Dot( Point* point )
{
	GLfloat result = position[0] * point -> position[0] + 
					 position[1] * point -> position[1] + 
					 position[2] * point -> position[2];
	return result;
}

GLfloat Point::Dot( GLfloat* pos )
{
	GLfloat result = position[0] * pos[0] + 
					 position[1] * pos[1] + 
					 position[2] * pos[2];
	return result;
}

void Point::Cross( Point* vec1 , Point* vec2 )
{//A x B = (a2b3 ' a3b2, a3b1 ' a1b3, a1b2 ' a2b1)
	position[0] = vec1 -> position[1] * vec2 -> position[2] - vec1 -> position[2] * vec2 -> position[1];
	position[1] = vec1 -> position[2] * vec2 -> position[0] - vec1 -> position[0] * vec2 -> position[2];
	position[2] = vec1 -> position[0] * vec2 -> position[1] - vec1 -> position[1] * vec2 -> position[0];
}

bool Point::Compare( Point* point )
{
	if( position[0] == point -> position[0] &&
		position[1] == point -> position[1] &&
		position[2] == point -> position[2] )
		return true;
	else
		return false;
}

bool Point::CloselyCompare( Point* point )
{
	if( MathHelper::VectorApproximation( point -> position , position ) )
		return true;
	else
		return false;
}

bool Point::CloselyCompare( GLfloat* pos )
{
	if( MathHelper::VectorApproximation( pos , position ) )
		return true;
	else
		return false;
}
#pragma endregion

//================================================================================


PointList::PointList()
{
	position = new GLfloat[3];

	//firstPointList = this;
	index = -1;
}

PointList::PointList( Point* point )
{
	position = new GLfloat[3];
	MathHelper::CopyVector( position , point -> position );

	firstPointList = this;
	index = 0;
}

PointList::~PointList()
{
	delete[] position;
}

void PointList::Add( Point* point )
{
	PointList* tempPointList = new PointList();
	
	MathHelper::CopyVector( tempPointList -> position , point -> position );

	if( link == NULL )
	{
		firstPointList = tempPointList;
		tempPointList -> firstPointList = firstPointList;
	}
	else
	{
		tempPointList -> firstPointList = firstPointList;
		for( PointList* i = firstPointList; i != NULL; i = i -> link )
		{
			if( i -> link == NULL )
			{
				i -> link = tempPointList;
				//tempPointList -> index = i -> index + 1;
				break;
			}
		}
	}
}

void PointList::Add( Point* point , int ind )
{
	PointList* tempPointList = new PointList();
	tempPointList -> index = ind;
	
	MathHelper::CopyVector( tempPointList -> position , point -> position );

	if( link == NULL )
	{
		firstPointList = tempPointList;
		link = tempPointList;
		tempPointList -> firstPointList = firstPointList;
	}
	else
	{
		tempPointList -> firstPointList = firstPointList;
		for( PointList* i = firstPointList; i != NULL; i = i -> link )
		{
			if( i -> link == NULL )
			{
				i -> link = tempPointList;
				//tempPointList -> index = i -> index + 1;
				break;
			}
		}
	}
	

	
}

void PointList::Add( GLfloat* pos )
{
	PointList* tempPointList = new PointList();
	
	MathHelper::CopyVector( tempPointList -> position , pos );

	if( link == NULL )
	{
		firstPointList = tempPointList;
	}
	tempPointList -> firstPointList = firstPointList;

	for( PointList* i = firstPointList; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = tempPointList;
			//tempPointList -> index = i -> index + 1;
			break;
		}
	}
}

bool PointList::Contain( Point* point )
{
	for( PointList* i = firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::CompareVector( point -> position , i -> position ) )
		{
			return true;
		}
	}
	return false;
}

bool PointList::CloselyContain( Point* point )
{
	for( PointList* i = firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::VectorApproximation( point -> position , i -> position ) )
		{
			return true;
		}
	}
	return false;
}

bool PointList::Contain( GLfloat* pos )
{
	for( PointList* i = firstPointList; i != NULL; i = i -> link )
	{
		if( MathHelper::CompareVector( pos , i -> position ) )
		{
			return true;
		}
	}
	return false;
}