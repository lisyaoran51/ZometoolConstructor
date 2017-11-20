#ifndef OBJ_FUNCTION
#define OBJ_FUNCTION

#include "stdafx.h"
#include "Model.h"
#include "Grid.h"
#include <vector>

#define ENGAGE_DISTANCE 0

#define SECTION_SCALE 25//要是grid的倍數
#define GRID_GROUP 20

/*
http://cg2010studio.com/2012/04/09/cc-%E8%A8%88%E7%AE%97%E8%A8%98%E6%86%B6%E9%AB%94%E4%BD%BF%E7%94%A8%E7%8B%80%E6%B3%81-compute-the-usability-of-memory/
*/
class ObjHollower
{
public:
	Point* firstCorner;
	Point* tempCorner;

	bool** cornerSource;

	int count;

	ObjHollower( Model* m );

	void RandomInnerPoint_Old( Model* model , Point* randomPoint );
	void RandomInnerPoint( Model* model , Point* randomPoint );

	int GetSampleNumber( Model* model );

	bool IfGridCollide( Grid* grid );

	bool CheckEngaged( Grid* grid );
	bool CheckEngaged_Old( Grid* grid );
	bool CheckCornerDuplicated( Grid* grid , int cornerNumber , int *cornerIndex );
	bool CheckCornerDuplicated_Old( Grid* grid , int cornerNumber , int *cornerIndex );
	bool CheckCornerSource();

	bool CompareCornerSource( bool* cornerSrc , bool* pattern );

	void InitializeNormalPattern( bool pattern[256][8] , GLfloat normal[256][3] );
	void GenerateCornerNormal( Point* corner , bool pattern[256][8] , GLfloat normal[256][3] );
	void GenerateNormal();

	void GenerateGridSurface();

	void DeleteGrid( Grid* grid );


	Grid* firstGrid;
	Grid* tempGrid;

	Model* model;

	Model* innerSurface;


};


class ObjHollower2
{
	bool GetMaxSectionSize();
	bool GetSectionIndex( int x , int y , int z , int* sectionIndex );
	bool GetSectionIndex( int* sectionPosition , int* sectionIndex );
	bool GetSectionPosition( int x , int y , int z , int* sectionIndex );
	bool GetSectionPosition( int* sectionPosition , int* sectionIndex );
	bool GetMaxGridSize();

	bool GenerateSection();
	bool GeneratePoint();
	bool GeneratePointWithSection();

	bool CheckPointInCube( GLfloat* point , int x , int y , int z );
	bool CheckPointInCube( GLfloat* point , GLfloat* cubePosition );

	bool CheckEngaged( Point* point );
	bool CheckEngagedWithSection( Point* point , int* sectionIndex );


	bool CheckLineIntersectOnModel( GLfloat* position1 , GLfloat* position2 );
	bool CheckLineIntersectOnTriangle( GLfloat* linePosition1 , 
									   GLfloat* linePosition2 , 
									   GLfloat* trianglePosition1 , 
									   GLfloat* trianglePosition2 , 
									   GLfloat* trianglePosition3 );
public:
	ObjHollower2( Model* m );
	bool GenerateSurface( char* filePath );

	Point* firstPoint;
	Point* tempPoint;

	std::vector<int>*** section;
	int sectionDimension[3];
	int maxSectionSize[3][2];
	int maxGridSize[3][2];
	int count;
	int** sectionList;
	int faceCount;

	Model* model;

	Model* innerSurface;
};
#endif