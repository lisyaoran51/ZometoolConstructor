#ifndef SOCKET
#define SOCKET

#include "stdafx.h"
#include "Model.h"
#include "OuterCone.h"
#include "Branch.h"

class Socket
{
public:
	Socket();
	~Socket();
	int index;
	GLfloat length;
	OuterCone* sourceOuterCone;
	Branch* sourceBranch;
	BranchEnd* sourceBranchEnd;
	//PointList* surfacePoint;

	GLfloat* position;
	GLfloat* rotation;
	GLfloat* direction;
	Socket* originalSocket;
	Socket** nextSocket;//64

	Socket* link;
	ModelObject* thisModel;

	int CheckConnection( int faceIndex );// -1 0 1 2 
	bool CheckBallExist( int faceIndex );
};


class SocketConstructor
{
public:
	SocketConstructor();
	SocketConstructor( GLfloat* translate );
	~SocketConstructor();
	Model* model;


	Socket* originalSocket;
	ModelObject* originalModel;

	Socket* tempSocketLink;
	ModelObject* tempModelLink;

	int NewSocket( OuterCone* sOuterCone );
	int NewOriginalSocket( OuterCone* sOuterCone );

	int NewSocket( Branch* branch );
	int NewOriginalSocket( Branch* branch );

	int NewSocket( BranchEnd* branchEnd );
	int NewOriginalSocket( BranchEnd* branchEnd );

	int NewSocket( GLfloat* position );

	int Construct( OuterCone* sOuterCone , PointList* socketPoint , GLfloat length );
	void PostConstruct();
};

#endif