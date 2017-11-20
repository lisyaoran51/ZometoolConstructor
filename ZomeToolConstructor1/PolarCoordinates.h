#ifndef POLAR_COORDINATES
#define POLAR_COORDINATES

#include "ZomeStructure.h"

class PolarCoor
{
public:

	VirtualBall* virtualBall;
	GLfloat polarPosition[2];//(r,theta)
	float radianWithPrevious;//����
	float radianWithNext;//����
	float area;//���n
	
	PolarCoor* originalPolarCoor;
	PolarCoor* link;

	PolarCoor( VirtualBall* tempVirtualBall , PolarCoor* oPolarCoor );
private:
	int length();
	void SwapWithNext( PolarCoor* tempPolarCoor );
	void Sort();//�N�y�вӿ��ɰw�Ƨ�
	bool CheckDone();//�O�_�H���X�C�@�Ӫ���v


	void ComputePolarPosition( GLfloat* centerPosition );
	void ComputeArea();//��X���ө��y�������n
	
public:
	PolarCoor* Preprocess_OriginalPolorCoor( GLfloat* centerPosition );
	float ForceRate();
	
};




#endif