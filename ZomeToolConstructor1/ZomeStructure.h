#ifndef ZOME_STRUCTURE
#define ZOME_STRUCTURE

#include "stdafx.h"
#include "Model.h"


class ZomeConnection
{
public:
	ZomeConnection();
	~ZomeConnection();
	int GetModelNumber();

	int index;
	int color;
	GLfloat* position;
	GLfloat* rotation;
	GLfloat* endPosition;
	int type;//0�L 1�� 2��
	int fromIndex;
	int towardIndex;
	int fromFace;
	int towardFace;
	int size;// S M L
	ModelObject* thisModel;

	bool isAnalyzed;//��O
	float force;

	ZomeConnection* firstConnection;
	ZomeConnection* link;
	int GetColor( int i );
	int GetSize( int i );
	ZomeConnection* GetConn( int i );//���o��i�ӽs������l

	ZomeConnection* AddToLink( ZomeConnection* fConnection );
};

class ZomeStructure
{
public:
	ZomeStructure();
	~ZomeStructure();
	int index;
	GLfloat* position;
	ZomeStructure* originalBall;
	ZomeConnection** faceConnection;//64
	ZomeStructure** nextBall;//64

	ZomeStructure* link;
	ModelObject* thisModel;

	int CheckConnection( int faceIndex );// -1 0 1 2 
	bool CheckBallExist( int faceIndex );

	ZomeStructure* GetBall( int i );//���o��i�ӽs�����y

	ZomeStructure* AddToLink( ZomeStructure* oBall );
};

class VirtualBall
{
public:
	VirtualBall( int i , ZomeConnection* c );
	int index;
	GLfloat position[3];
	bool isBottom;
	int bottomForceIndex;
	int connectionAmount;
	int connIndex[6];
	int connectFace[6];
	ZomeConnection* conn[6];

	void AddConn( ZomeConnection* c);
};

#endif