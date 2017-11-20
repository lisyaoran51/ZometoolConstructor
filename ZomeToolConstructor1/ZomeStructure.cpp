#include "ZomeStructure.h"

//========================================ZomeConnection//========================================
ZomeConnection::ZomeConnection()
{
	position = new GLfloat[3];
	rotation = new GLfloat[3];
	endPosition = new GLfloat[3];
	fromIndex = -1;
	fromFace = -1;
	towardIndex = -1;
	towardFace = -1;
}

ZomeConnection::~ZomeConnection()
{
	delete[] position;
	delete[] rotation;
}

int ZomeConnection::GetModelNumber()
{
	int modelNumber;
	if ( color == COLOR_BLUE )
	{
		if ( size == SIZE_S ) modelNumber = 1;
		if ( size == SIZE_M ) modelNumber = 2;
		if ( size == SIZE_L ) modelNumber = 3;
	}
	else if ( color == COLOR_RED )
	{
		if ( size == SIZE_S ) modelNumber = 4;
		if ( size == SIZE_M ) modelNumber = 5;
		if ( size == SIZE_L ) modelNumber = 6;
	}
	else if ( color == COLOR_YELLOW )
	{
		if ( size == SIZE_S ) modelNumber = 7;
		if ( size == SIZE_M ) modelNumber = 8;
		if ( size == SIZE_L ) modelNumber = 9;
	}
	return modelNumber;
}

ZomeConnection* ZomeConnection::GetConn( int i )
{
	for( ZomeConnection* c = firstConnection; c != NULL; c = c -> link )
	{
		if( c -> index == i )
		{
			return c;
		}
	}
	std::cout << "ZomeConnection::GetConn出錯：沒有找到這個index" << std::endl;
}

ZomeConnection* ZomeConnection::AddToLink( ZomeConnection* fConnection )
{
	if ( fConnection == NULL )
	{
		index = 0;
		firstConnection = this;
	}
	else//有建過就當下一個
	{
		firstConnection = fConnection;
		for( ZomeConnection* c = fConnection; c != NULL; c = c -> link )
		{
			if( c -> link == NULL )
			{
				c -> link = this;
				index = c -> index + 1;
				break;
			}
		}
	}
	return firstConnection;
}
//========================================ZomeConnection//========================================
//========================================ZomeStructure//========================================

ZomeStructure::ZomeStructure()
{
	faceConnection = new ZomeConnection*[62];
	nextBall = new ZomeStructure*[62];
	for( int i = 0; i < 62; i++ )
	{
		faceConnection[i] = NULL;
		nextBall[i] = NULL;
	}
	position = new GLfloat[3];
}

ZomeStructure::~ZomeStructure()
{
	delete[] faceConnection ;
	delete[] nextBall ;
	delete[] position ;
}

ZomeStructure* ZomeStructure::AddToLink( ZomeStructure* oBall )
{
	if ( oBall == NULL )
	{
		index = 0;
		originalBall = this;
	}
	else//有建過就當下一個
	{
		originalBall = oBall;
		for( ZomeStructure* b = oBall; b != NULL; b = b -> link )
		{
			if( b -> link == NULL )
			{
				b -> link = this;
				index = b -> index + 1;
				break;
			}
		}
	}
	return originalBall;
}

ZomeStructure* ZomeStructure::GetBall( int i )
{
	for( ZomeStructure* b = originalBall; b != NULL; b = b -> link )
	{
		if( b -> index == i )
		{
			return b;
		}
	}
	std::cout << "ZomeStructure::GetBall出錯：沒有找到該編號" << std::endl;
	return NULL;
}

//========================================ZomeStructure//========================================

VirtualBall::VirtualBall( int i , ZomeConnection* c )
{
	index = i;
	MathHelper::CopyVector( position , c -> endPosition );
	isBottom = false;
	connIndex[0] = c -> index;
	//connectFace[0] = c -> towardFace;
	
	connectionAmount = 0;//??
	conn[0] = c;
}