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
		std::cout << "PolarCoor::SwapWithNext�X���G�S���U�@�Ӥήy��" << std::endl;
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
	//(r,theta)�f�ɰw�Ƨ�
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
		//�Yb<y�Atheta+180
		//�Yb=y�Aa<x�Atheta+180
		//r=�ڸ�( (a-x)2 + (b-y)2 )
		p -> polarPosition[0] = sqrt( pow( p -> virtualBall -> position[0] - centerPosition[0] , 2 ) +
									  pow( p -> virtualBall -> position[2] - centerPosition[2] , 2 ) );//r
		
		p -> polarPosition[1] = atan( ( p -> virtualBall -> position[2] - centerPosition[2] ) / 
									  ( p -> virtualBall -> position[0] - centerPosition[0] ) ) * 180.0 / M_PI;

		//�p�Gx<0�Ay>0�A�[180��

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

		std::cout << "�p�⩳���伵�����y�СG��" << p -> virtualBall -> index << "�ө����O�y�Ь�(" 
				  <<  p -> virtualBall -> position[0] << "," << p -> virtualBall -> position[2] << ")�A�ഫ�ܷ��y��("
				  << p -> polarPosition[0] << "," << p -> polarPosition[1] << ")" << std::endl;
		int jjj = 121;
	}
}

void PolarCoor::ComputeArea()
{
	if( length() == 1 )
	{
		std::cout << "PolarCoor::ComputeArea()�X���G�Ȧ��@�Ӥ���" << std::endl;
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
		//��X���n
		p -> area = pow( averageDiameterWithPrevious , 2 ) * p -> radianWithPrevious / 360.0 * M_PI +
					pow( averageDiameterWithNext , 2 )     * p -> radianWithNext     / 360.0 * M_PI;
		previousPolarCoor = p;

		std::cout << "��" << p -> virtualBall -> index << "�ө����伵�����᭱�n��" << p -> area << "mm2" << std::endl;
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
		std::cout << "�X���G�`�@�����n��0" << std::endl;
	}

	return area / polarPosition[0] / totalWeightedSector;
}