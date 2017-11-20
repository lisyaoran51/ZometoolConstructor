#ifndef SPLIT_SURFACE
#define SPLIT_SURFACE

#include "Plane.h"
#include "Polyhedron.h"
#include "Model.h"

#define SAME_DIRECTION true
#define OPPOSITE_DIRECTION false

class SplitSurface
{


public:
	SplitSurface( Plane* p );
	~SplitSurface();
	bool AddToLink( SplitSurface* sSurface );

	//建立四個角，利用所有的邊線和兩條分界線
	bool SetCorner( Line* connectingLine , Line* line0 , Line* line1 );

	int index;
	Point* corner[4];
	Plane* plane;

	SplitSurface* firstSplitSurface;
	SplitSurface* link;
};

class SplitSurfaceConstructor
{
	bool SetCubeNormal( Model* m );
	bool SetCube( Model* m , SplitSurface* splitSurface );
public:
	SplitSurfaceConstructor();
	SplitSurfaceConstructor( int sSurfaceAmount );

	Model* model;
	Polyhedron* polyhedron;

	//Segment* segment;
	SplitSurface* originalSplitSurface;

	ModelObject* originalModel;
	ModelObject* tempModelLink;

	int tempSplitSurfaceCount;
	int splitSurfaceAmount;

	int NewSplitSurface(  SplitSurface* splitSurface );
	//int Construct( SplitSurface* sOuterCone , PointList* socketPoint , GLfloat length );
};


#endif