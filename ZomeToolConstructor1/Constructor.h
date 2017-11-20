#ifndef CONSTRUCTOR
#define CONSTRUCTOR

#include "stdafx.h"
#include "ZomeDirection.h"
#include "Model.h"
#include "ZomeStructure.h"
#include "PolarCoordinates.h"
#include "PolynomialList.h"
#include "VirtualForce.h"
#include <map>
#include <iomanip>
#include <vector>

#define TYPE_TRUSS 0
#define TYPE_PIN 1

#define BOTTOM_INTERVAL 50.0








class Constructor
{
public:
	Constructor();
	Constructor( GLfloat* translate );
	~Constructor();
	Model* model;
	ZomeDirection* zomeDirection;

	int connectorAmount[3][3];//什麼顏色
	int ballAmount;

	float totalForceDown;
	float gravityPosition[3];
	PolarCoor* originalPolarCoor;//用來存極座標

	ZomeConnection* originalConn;
	ZomeStructure* originalBall;
	ModelObject* originalModel;

	ZomeConnection* tempConnLink;
	ZomeStructure* tempBallLink;
	ModelObject* tempModelLink;

	std::map<int,int>* material;
	std::map<int,int>* matIndex2Index;
	//std::map<int,float>* force;
	//std::map<int,float>* matForce;
	PolynomialList* firstPolynomial;

	int NewBall( GLfloat* translate );
	int Construct( int pointIndex , int faceIndex , int size , bool type );
	bool ContainConn( int pointIndex , int faceIndex );
	void PostConstruct();
	void PrintTotalComponents();

	void StructureAnalyze();


	ZomeStructure* GetBall( int index );//還沒寫
	ZomeConnection* GetConn( int index );
};


#endif