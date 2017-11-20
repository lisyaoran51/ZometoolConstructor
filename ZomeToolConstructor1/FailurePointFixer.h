#ifndef FAILURE_POINT_FIXER
#define FAILURE_POINT_FIXER


#include "stdafx.h"
#include "Model.h"
#include "Point.h"
#include "Cone.h"
#include <vector>


class FailurePoint
{
public:
	int index;
	std::vector< Cone* >* groupA;
	std::vector< Cone* >* groupB;

	std::vector< Cone* > coneFromA;
	std::vector< Cone* > coneFromB;

	std::vector< Point* > engagedPoint;

	bool fixed;


	FailurePoint* firstFailurePoint;
	FailurePoint* link;


	//FailurePoint( std::vector< Cone* >* groupA , std::vector< Cone* >* groupB );
	FailurePoint( std::vector< Cone* >* gA , std::vector< Cone* >* gB , Point* ePoint , FailurePoint* fFailurePoint );

private:
	bool AddToLink( FailurePoint* fFailurePoint );

};


class FailurePointFixer
{
	Cone* firstCone;
	std::vector< std::vector< Cone* >* >* totalGroup;
	
	FailurePoint* firstFailurePoint;

	bool Combine( std::vector< std::vector< Cone* >* >* tGroup , int inserted , int inserter );
	bool CheckIfNeighbor( Cone* coneA , Cone* coneB );
public:
	FailurePointFixer( Cone* fCone );
	bool Assort();
	bool FindFailurePoint();

	bool TryFillCone();
	bool ExistInTheGroup( Cone* cone , std::vector< Cone* >* group );
};



#endif