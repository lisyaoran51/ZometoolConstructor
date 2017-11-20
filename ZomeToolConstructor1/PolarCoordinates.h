#ifndef POLAR_COORDINATES
#define POLAR_COORDINATES

#include "ZomeStructure.h"

class PolarCoor
{
public:

	VirtualBall* virtualBall;
	GLfloat polarPosition[2];//(r,theta)
	float radianWithPrevious;//弧度
	float radianWithNext;//弧度
	float area;//面積
	
	PolarCoor* originalPolarCoor;
	PolarCoor* link;

	PolarCoor( VirtualBall* tempVirtualBall , PolarCoor* oPolarCoor );
private:
	int length();
	void SwapWithNext( PolarCoor* tempPolarCoor );
	void Sort();//將座標細膩時針排序
	bool CheckDone();//是否以精算出每一個的比率


	void ComputePolarPosition( GLfloat* centerPosition );
	void ComputeArea();//算出那個底座的扇面積
	
public:
	PolarCoor* Preprocess_OriginalPolorCoor( GLfloat* centerPosition );
	float ForceRate();
	
};




#endif