#include "PolarCoordinates.h"

PolarCoor::PolarCoor( VirtualBall* tempVirtualBall , PolarCoor* oPolarCoor )
{
	if( oPolarCoor == NULL )
	{
		originalPolarCoor = this;
	}
	else
	{
		originalPolarCoor = oPolarCoor;
		for( PolarCoor* p = oPolarCoor; p != NULL; p = p -> link )
		{
			if( p -> link == NULL )
			{
				p -> link = this;
				break;
			}
		}
	}
	virtualBall = tempVirtualBall;
	radianWithPrevious = 0;
	radianWithNext = 0;
}

int PolarCoor::length()
{
	int count = 0;
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		count++;
	}
	return count;
}

void PolarCoor::SwapWithNext( PolarCoor* tempPolarCoor )
{
	if( tempPolarCoor -> link == NULL )
	{
		std::cout << "PolarCoor::SwapWithNext出錯：沒有下一個及座標" << std::endl;
		return;
	}

	PolarCoor* nextPolorCoor = tempPolarCoor -> link;
	if( tempPolarCoor == originalPolarCoor )
	{
		tempPolarCoor -> link = nextPolorCoor -> link;
		nextPolorCoor -> link = tempPolarCoor;
		for( PolarCoor* p = nextPolorCoor; p != NULL; p = p -> link )
		{
			p -> originalPolarCoor = nextPolorCoor;
		}

		return;
	}
	PolarCoor* previousPolorCoor;
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		if( p -> link == tempPolarCoor )
		{
			previousPolorCoor = p;
			tempPolarCoor -> link = nextPolorCoor -> link;
			nextPolorCoor -> link = tempPolarCoor;
			previousPolorCoor -> link = nextPolorCoor;
			return;
		}
	}
}

bool PolarCoor::CheckDone()
{
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		if( p -> area != 0 )
		{
			return true;
		}
	}
	return false;
}

void PolarCoor::Sort()
{
	//(r,theta)逆時針排序
	//bubblesort
	int bubbleCount = originalPolarCoor -> length();
	for( int i = 0; i < originalPolarCoor -> length() - 1; i++ )
	{
		PolarCoor* p = originalPolarCoor;
		bubbleCount--;
		for( int j = 0; j < bubbleCount; j++ )
		{
			if( p -> polarPosition[1] > p -> link -> polarPosition[1] )
			{
				SwapWithNext( p );
			}
			else
			{
				p = p -> link;
			}
		}
	}
}

void PolarCoor::ComputePolarPosition( GLfloat* centerPosition )
{
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		//theta = arctan(b-y/a-x) 
		//若b<y，theta+180
		//若b=y，a<x，theta+180
		//r=根號( (a-x)2 + (b-y)2 )
		p -> polarPosition[0] = sqrt( pow( p -> virtualBall -> position[0] - centerPosition[0] , 2 ) +
									  pow( p -> virtualBall -> position[2] - centerPosition[2] , 2 ) );//r
		
		p -> polarPosition[1] = atan( ( p -> virtualBall -> position[2] - centerPosition[2] ) / 
									  ( p -> virtualBall -> position[0] - centerPosition[0] ) ) * 180.0 / M_PI;

		//如果x<0，y>0，加180度

		if( p -> virtualBall -> position[0] < centerPosition[0] && p -> virtualBall -> position[2] > centerPosition[2] )
		{
			p -> polarPosition[1] += 180.0;
		}

		if( p -> virtualBall -> position[2] < centerPosition[2] )
		{
			if( p -> virtualBall -> position[0] < centerPosition[0] )
			{
				p -> polarPosition[1] += 180.0;
			}
			else
			{
				p -> polarPosition[1] += 360.0;
			}
			
		}
		else if ( p -> virtualBall -> position[2] == centerPosition[2] )
		{
			if( p -> virtualBall -> position[0] > centerPosition[0] )
			{
				p -> polarPosition[1] += 360.0;
			}
		}

		std::cout << "計算底部支撐的極座標：第" << p -> virtualBall -> index << "個底部力座標為(" 
				  <<  p -> virtualBall -> position[0] << "," << p -> virtualBall -> position[2] << ")，轉換至極座標("
				  << p -> polarPosition[0] << "," << p -> polarPosition[1] << ")" << std::endl;
		int jjj = 121;
	}
}

void PolarCoor::ComputeArea()
{
	if( length() == 1 )
	{
		std::cout << "PolarCoor::ComputeArea()出錯：紙有一個元素" << std::endl;
	}

	PolarCoor* previousPolarCoor = NULL;
	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		float averageDiameterWithPrevious;
		float averageDiameterWithNext;

		if( p == originalPolarCoor )
		{
			PolarCoor* lastPolarCoor = NULL;
			for( PolarCoor* p2 = originalPolarCoor; p2 != NULL; p2 = p2 -> link )
			{
				if( p2 -> link == NULL )
				{
					lastPolarCoor = p2;
					break;
				}
			}
			p -> radianWithPrevious = ( 360.0 - lastPolarCoor -> polarPosition[1] + p -> polarPosition[1] ) / 2.0;			 
			p -> radianWithNext = ( p -> link -> polarPosition[1] - p -> polarPosition[1] ) / 2.0;	

			averageDiameterWithPrevious = ( lastPolarCoor -> polarPosition[1] +
											p -> polarPosition[1] ) / 2.0;
			averageDiameterWithNext = ( p -> polarPosition[1] + 
										p -> link -> polarPosition[1] ) / 2.0;
			
		}
		else if( p -> link == NULL )
		{
			p -> radianWithPrevious = ( p -> polarPosition[1] - previousPolarCoor -> polarPosition[1] ) / 2.0;			 
			p -> radianWithNext = ( 360.0 - p -> polarPosition[1] + originalPolarCoor -> polarPosition[1] ) / 2.0;	

			averageDiameterWithPrevious = ( previousPolarCoor -> polarPosition[1] +
											p -> polarPosition[1] ) / 2.0;
			averageDiameterWithNext = ( p -> polarPosition[1] + 
										originalPolarCoor -> polarPosition[1] ) / 2.0;

		}
		else
		{
			p -> radianWithPrevious = ( p -> polarPosition[1] - previousPolarCoor -> polarPosition[1] ) / 2.0;	
			p -> radianWithNext = (		p -> link -> polarPosition[1] - p -> polarPosition[1] ) / 2.0;	

			averageDiameterWithPrevious = ( previousPolarCoor -> polarPosition[1] +
											p -> polarPosition[1] ) / 2.0;
			averageDiameterWithNext = ( p -> polarPosition[1] + 
										p -> link -> polarPosition[1] ) / 2.0;
		}
		//算出面積
		p -> area = pow( averageDiameterWithPrevious , 2 ) * p -> radianWithPrevious / 360.0 * M_PI +
					pow( averageDiameterWithNext , 2 )     * p -> radianWithNext     / 360.0 * M_PI;
		previousPolarCoor = p;

		std::cout << "第" << p -> virtualBall -> index << "個底部支撐的分擔面積為" << p -> area << "mm2" << std::endl;
	}
}

PolarCoor* PolarCoor::Preprocess_OriginalPolorCoor( GLfloat* centerPosition )
{
	ComputePolarPosition( centerPosition );
	Sort();
	ComputeArea();
	return originalPolarCoor;
}

float PolarCoor::ForceRate()
{
	/*
	if( !CheckDone() )
	{
		Sort();
		ComputeArea();
	}*/

	float totalWeightedSector = 0;

	for( PolarCoor* p = originalPolarCoor; p != NULL; p = p -> link )
	{
		if( p -> polarPosition[0] < 0.01 )
		{
			totalWeightedSector += p -> area / 0.01;
		}
		else
		{
			totalWeightedSector += p -> area / p -> polarPosition[0];
		}
	}

	if( totalWeightedSector == 0 )
	{
		std::cout << "出錯：總共扇面積為0" << std::endl;
	}

	return area / polarPosition[0] / totalWeightedSector;
}