#ifndef OUTER_CONE
#define OUTER_CONE

#include "Cone.h"

#define TYPE_AMOUNT 10

/*
�o���x�s�̥~�h�P�ҫ��s�����l �����@�����K�Ӥ�V
���ӬO24�Ӥ�V


type:
1:
�u���u���u��

2.�u���u���u�� ���n
�u���������� 

3.
���E���椤��

4.
��01�� �����u���u�� �~ ��23�� �����u�ŵu��
//��ӬO��23��?�ﱼ
5.
�����u�ŵu��

6.
��01�� �������ŵu�� �~ ��23�� ���椤�ŵu�� ��23��0����
//���ӬO��23��?�ﱼ
7.
//�令�u�����ŵu�� �u���@�� �ﱼ���椤�ŵu��

8.
��01�� �����u�椤�� �~ ��23�� ����u�椤�� ��23��0����
�ﱼ

9.
�令�u���u�椤�� �u���@�� �ﱼ����u�椤��
*/


class OuterCone
{
private:
	bool CheckConeDirection(); //��cone���L�ө񪺦�m
	bool SelectConstructedHole();//�ھ�cone��direction�A�إߥL����l

	bool CheckNeighborOuterCone();//�d�ݬ۾F��outercone
	bool CheckDublicatedCone(); //�ݬݳo��cone���S���M��L��cone���|
	bool CheckDublicatedConeEdge( Cone* cone );//�ݬݳo��cone��edge���S�����|
	bool CheckDublicatedOuterCone(); //�ݬݳo��cone���S���M��L���~�@���|
	bool CheckDublicatedOuterConeEdge( OuterCone* outerCone );
	bool CheckIntersectWithConnector();//�d�ݦ��S���M��L���l���
	bool CheckIntersectWithConeFace();
	GLfloat CheckTwoLineDistance( Point* vec1Initial , Point* vec1End , Point* vec2Initial , Point* vec2End );

	bool CheckEngaged(); //�ݬݦ��S���P�~���ۥ�
	

public:
	OuterCone( Cone* cone , int coneFace , int t );
	OuterCone( Cone* cone , int coneFace , int t , OuterCone* firstOuterCone );
	~OuterCone();


	bool AddToLink();
	bool CheckIntersectOuterConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckIntersectOuterConeOnConeFace( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckDistanceToSurface(); //�ݬݥL�����ݶZ���h��
	bool TryDifferentType( int differentType );//���ؤ@����V�A�M�᭫�O�@��engaged�~�Z���~dublicated
	bool RefreshDistanceToSurface();//0116�A��~���@rearrange�ɡA�ª�distance to surface�٥��R���A�ҥH�n���s�אּ-1

	int index;
	int type;
	int coneDirection;//24�Ӥ�V

	Cone* sourceCone;
	int sourceConeFace;

	Point** sourceCorner;//3���I �C���I�U�Ѥ@�Ӹ��X���}���J��l ��l���t�@�ݵ�end ���O�O�W �� �k
	int* constructedHole;
	int* constructedSize;
	Point** cornerEnd;
	int singleConnector;

	OuterCone** neighborOuterCone;

	bool engaged;
	bool* connectorEngaged;
	bool dublicatedWithCone;//�M�Y��cone
	int dublicatedConeIndex;
	
	bool dublicatedWithOuterCone;//�M�Y��cone
	int dublicatedOuterConeIndex;
	bool* cornerDublicated;

	bool intersected;//���S�����

	GLfloat distanceToSurface[ TYPE_AMOUNT ];
	GLfloat weightedDistance[ TYPE_AMOUNT ];//�[�v�Z���A�P���Z��-�P��L���I�Z��+100
	Point* nearestPointOnSurface;

	OuterCone* firstOuterCone;
	OuterCone* link;

	ZomeDirection* zomeDirection;
	/*
	Cone( Point* centerPoint );
	Cone( Cone* cone , int facePropagateNumber );
	~Cone();

	void SetCorner( Point* centerPoint );
	void SetCorner( Cone* cone , int facePropagateNumber ); 
	
	bool CheckIntersectConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 );
	bool CheckDuplicated();
	bool CheckNeighbor();
	bool CheckDistance( int face );
	void AddToLink();

	bool Compare( Cone* cone );
	bool CloselyCompare( Cone* cone );
	int type;//���`:0 ���L
	int index;
	int scale;
	GLfloat* position;
	GLfloat* b0Direction;
	GLfloat* b5Direction;
	bool engaged;
	bool dublicated;
	bool* engagedFace;
	bool* facePropagated;
	bool* faceApproached;//??
	int engagedDistance;//4�� ??? �Z��edgaged���T���@���h��
	Cone** neighborCone;//4��
	Point** corner;//4���I
	Point** localCorner;//4���I �������Шt �H�I0�����I
	ZomeDirection* zomeDirection;

	Cone* firstCone;
	Cone* link;
	*/


};



#endif