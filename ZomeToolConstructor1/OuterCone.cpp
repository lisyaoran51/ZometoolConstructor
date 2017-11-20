#include "OuterCone.h"


OuterCone::OuterCone( Cone* cone , int coneFace , int t )
{
	sourceCone = cone;
	sourceConeFace = coneFace;

	sourceCorner = new Point*[3];
	constructedHole = new int[3];
	constructedSize = new int[3];
	cornerEnd = new Point*[3];
	singleConnector = -1;

	neighborOuterCone = new OuterCone*[3];

	engaged = false;
	connectorEngaged = new bool[3];
	connectorEngaged[0] = false;
	connectorEngaged[1] = false;
	connectorEngaged[2] = false;

	dublicatedWithCone = false;
	dublicatedConeIndex = -1;

	dublicatedWithOuterCone = false;
	dublicatedOuterConeIndex = -1;
	cornerDublicated = new bool[3];
	cornerDublicated[0] = false;
	cornerDublicated[1] = false;
	cornerDublicated[2] = false;

	intersected = false;

	type = t;
	//firstOuterCone = firstOCone;
	firstOuterCone = this;
	for( int i = 0; i < TYPE_AMOUNT; i ++ ) 
	{
		distanceToSurface[ i ] = -1.0;
		weightedDistance[ i ] = -1.0;
	}
	nearestPointOnSurface = new Point();

	zomeDirection = sourceCone -> zomeDirection;

	CheckConeDirection();
	SelectConstructedHole();


	CheckDublicatedCone();
	//CheckDublicatedOuterCone();//�ˬd���X�Ӫ���V���S���M�O�H����

}

OuterCone::OuterCone( Cone* cone , int coneFace , int t , OuterCone* firstOCone )
{

	sourceCone = cone;
	sourceConeFace = coneFace;

	sourceCorner = new Point*[3];
	constructedHole = new int[3];
	constructedSize = new int[3];
	cornerEnd = new Point*[3];
	singleConnector = -1;

	neighborOuterCone = new OuterCone*[3];

	engaged = false;
	connectorEngaged = new bool[3];
	connectorEngaged[0] = false;
	connectorEngaged[1] = false;
	connectorEngaged[2] = false;

	dublicatedWithCone = false;
	dublicatedConeIndex = -1;

	dublicatedWithOuterCone = false;
	dublicatedOuterConeIndex = -1;
	cornerDublicated = new bool[3];
	cornerDublicated[0] = false;
	cornerDublicated[1] = false;
	cornerDublicated[2] = false;

	intersected = false;

	type = t;
	firstOuterCone = firstOCone;
	for( int i = 0; i < TYPE_AMOUNT; i ++ ) 
	{
		distanceToSurface[ i ] = -1.0;
		weightedDistance[ i ] = -1.0;
	}
	nearestPointOnSurface = new Point();

	zomeDirection = sourceCone -> zomeDirection;

	CheckConeDirection();
	SelectConstructedHole();

	CheckNeighborOuterCone();//��X�P�o�۾F��outercone
	CheckDublicatedCone();
	CheckDublicatedOuterCone();//�ˬd���X�Ӫ���V���S���M�O�H����

	//�ˬd�M��Lcone�~outercone���������
	if( !engaged && !dublicatedWithCone && !dublicatedWithOuterCone )
	{
		CheckIntersectWithConnector();
	}

	//CheckEngaged();
	/*
	if( !engaged )
	{
		CheckDistanceToSurface();
	}
	*/
}

OuterCone::~OuterCone()
{
	delete[] sourceCorner;
	delete[] constructedHole;
	delete[] constructedSize;
	
	delete[] neighborOuterCone;
	delete[] connectorEngaged;
	delete[] cornerDublicated;
	delete nearestPointOnSurface;
	for( int i = 0; i < 3; i++ )delete cornerEnd[i];
	delete[] cornerEnd;
}

bool OuterCone::CheckConeDirection()
{
	//Point** localCorner = new Point*[4];
	//�򥻪�coneDirection�O3�A

	//����24�ؤ�V
	//��֤��ߦV�e �T����
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , 0 , -1 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 21;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 20;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 16;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 19;
	}
	//���ߦV�e �ݫ᭱����
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , 0 , 1 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 23;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 22;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 18;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 17;
	}
	//���ߦV�� ���
	if( MathHelper::CompareVector( sourceCone -> b5Direction , -1 , 0 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 4;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 7;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 1;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 0;
	}
	//���ߦV�� �ݥk�䪺��
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 1 , 0 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 6;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 5;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , -1 , 0 ) ) coneDirection = 3;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 1 , 0 ) ) coneDirection = 2;
	}
	//���ߦV�W �����
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , -1 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 12;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 15;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 9;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 8;
	}
	//���ߦV�W �ݤU������
	if( MathHelper::CompareVector( sourceCone -> b5Direction , 0 , 1 , 0 ) )
	{
		if( MathHelper::CompareVector( sourceCone -> b0Direction , -1 , 0 , 0 ) ) coneDirection = 14;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 1 , 0 , 0 ) ) coneDirection = 13;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , -1 ) ) coneDirection = 11;
		if( MathHelper::CompareVector( sourceCone -> b0Direction , 0 , 0 , 1 ) ) coneDirection = 10;
	}


	switch( sourceConeFace )
	{
		//�Ĥ@��corner���O���������@���Ŧ⪺���� �M��������
		//0�O�Ŧ⨺�� 1�O���䪺 2�O�k�䪺
		//     0
		//    / \
		//   /   \
		//  /     \
		// /       \
		//1_________2
		case 0:
			sourceCorner[0] = sourceCone -> corner[0];
			sourceCorner[1] = sourceCone -> corner[3];
			sourceCorner[2] = sourceCone -> corner[2];
			break;
		case 1:
			sourceCorner[0] = sourceCone -> corner[1];
			sourceCorner[1] = sourceCone -> corner[2];
			sourceCorner[2] = sourceCone -> corner[3];
			break;
		case 2:
			sourceCorner[0] = sourceCone -> corner[2];
			sourceCorner[1] = sourceCone -> corner[1];
			sourceCorner[2] = sourceCone -> corner[0];
			break;
		case 3:
			sourceCorner[0] = sourceCone -> corner[3];
			sourceCorner[1] = sourceCone -> corner[0];
			sourceCorner[2] = sourceCone -> corner[1];
			break;
	}

	return true;
}

bool OuterCone::SelectConstructedHole()
{
	//����X�bconeDirection�O0����V�U�A�L�򥻪����v�A�U�@�B�b����L�����v�H���X�������}
	Point* baseDirection = new Point[3]; //��V0�����p�U�����v
	//Point* baseDirection2 = new Point[3]; //��V0�����p�U��y¶y�b����90�ת����v
	Point* tempDirection = new Point[3]; //���Ӥ�V�H�᪺���v
	GLfloat* rotateXMatrix = new GLfloat[16]; MathHelper::makeIdentity( rotateXMatrix );
	GLfloat* rotateYMatrix = new GLfloat[16]; MathHelper::makeIdentity( rotateYMatrix );
	GLfloat* rotateZMatrix = new GLfloat[16]; MathHelper::makeIdentity( rotateZMatrix );
	
	bool rotateBaseDirection = false;//�p�G�n�ϥ�baseDirection2�A�N��o�ӳ]��true
	//¶y�b����90�ץH��A�۷��O�bB5���V(1,0,0)��cone
	//================================================================
	//�o��n�ھڤT���@�bconeDirection�O0�����p�U�A�]�N�Ob0Direction�O(0,1,0)�Ab5Direction�O(-1,0,0)��
	//�o�ӤT���@���U��(sourceConeFace)���~��[�إߤ覡�A�]�w�o����[�n�����@�ӭ��W��
	switch( type )
	{
		case 0:
			cornerEnd[0] = new Point( sourceCorner[0] -> position );
			cornerEnd[1] = new Point( sourceCorner[1] -> position );
			cornerEnd[2] = new Point( sourceCorner[2] -> position );
			delete[] baseDirection;
			delete[] tempDirection;
			delete[] rotateXMatrix;
			delete[] rotateYMatrix;
			delete[] rotateZMatrix;
			return true;
			break;
		case 1://�u���u���u��
			
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 5;
					baseDirection[1].index = 58;
					baseDirection[2].index = 61;
					break;
				case 1://///4/25�o�䦳�����ˤl
					baseDirection[0].index = 5;
					baseDirection[1].index = 59;
					baseDirection[2].index = 57;
					break;
				case 2:
					baseDirection[0].index = 4;
					baseDirection[1].index = 56;
					baseDirection[2].index = 54;
					break;
				case 3:
					baseDirection[0].index = 4;
					baseDirection[1].index = 55;
					baseDirection[2].index = 60;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_S;
			break;

		case 2:// �u����������
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 36;
					baseDirection[1].index = 49;
					baseDirection[2].index = 48;
					break;
				case 1:
					baseDirection[0].index = 35;
					baseDirection[1].index = 47;
					baseDirection[2].index = 46;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 41;
					baseDirection[1].index = 52;
					baseDirection[2].index = 53;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 38;
					baseDirection[1].index = 50;
					baseDirection[2].index = 51;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_M;
			break;

		case 3://���E���椤�� //���t�s�L�ӳQ��
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 32;
					baseDirection[1].index = 18;
					baseDirection[2].index = 17;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 33;
					baseDirection[1].index = 15;
					baseDirection[2].index = 20;
					break;
				case 2:
					baseDirection[0].index = 38;
					baseDirection[1].index = 7;
					baseDirection[2].index = 6;
					break;
				case 3:
					baseDirection[0].index = 39;
					baseDirection[1].index = 8;
					baseDirection[2].index = 9;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_M;
			break;
		case 4://��01�� �����u���u�� �~ ��23�� �����u�ŵu�� //���o��A�n��쥻��baseDirection��D��90�ץH�᪺
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 53;
					baseDirection[1].index = 41;
					baseDirection[2].index = 40;
					break;
				case 1:
					baseDirection[0].index = 52;
					baseDirection[1].index = 40;
					baseDirection[2].index = 41;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 42;
					baseDirection[1].index = 31;
					baseDirection[2].index = 30;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 43;
					baseDirection[1].index = 30;
					baseDirection[2].index = 31;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_S;
			break;
		case 5:// �����u�ŵu�� //�ƥ�
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 33;
					baseDirection[1].index = 11;
					baseDirection[2].index = 8;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 32;
					baseDirection[1].index = 9;
					baseDirection[2].index = 10;
					break;
				case 2:
					baseDirection[0].index = 39;
					baseDirection[1].index = 25;
					baseDirection[2].index = 23;
					break;
				case 3:
					baseDirection[0].index = 38;
					baseDirection[1].index = 22;
					baseDirection[2].index = 24;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_S;
			break;
		case 6://��01�� �������ŵu�� �~ �o�O�����@�� ok
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 41;
					baseDirection[1].index = 12;
					baseDirection[2].index = 27;
					break;
				case 1:
					baseDirection[0].index = 40;
					baseDirection[1].index = 11;
					baseDirection[2].index = 28;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 31;
					baseDirection[1].index = 19;
					baseDirection[2].index = 12;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 30;
					baseDirection[1].index = 14;
					baseDirection[2].index = 6;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_S;
			break;
		case 7://�令�u�����ŵu�� �u���@�� �ﱼ���椤�ŵu��
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 52;
					baseDirection[1].index = 58;
					baseDirection[2].index = 19;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 50;
					baseDirection[1].index = 22;
					baseDirection[2].index = 14;
					break;
				case 2:
					baseDirection[0].index = 49;
					baseDirection[1].index = 19;
					baseDirection[2].index = 12;
					break;
				case 3:
					baseDirection[0].index = 47;
					baseDirection[1].index = 20;
					baseDirection[2].index = 11;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			constructedSize[2] = SIZE_S;
			break;
		case 8:// �����u�椤�� 
			switch( sourceConeFace )
			{
				case 0:
					baseDirection[0].index = 40;
					baseDirection[1].index = 26;
					baseDirection[2].index = 10;
					break;
				case 1:
					baseDirection[0].index = 41;//���ɲ�012�A�q���������I�}�l
					baseDirection[1].index = 29;
					baseDirection[2].index = 13;
					break;
				case 2:
					rotateBaseDirection = true;
					baseDirection[0].index = 30;
					baseDirection[1].index = 13;
					baseDirection[2].index = 21;
					singleConnector = 0;
					break;
				case 3:
					rotateBaseDirection = true;
					baseDirection[0].index = 31;
					baseDirection[1].index = 7;
					baseDirection[2].index = 16;
					singleConnector = 0;
					break;
			}
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_M;
			break;
		case 9://�令�u���u�椤�� �u���@�� �ﱼ����u�椤��
			switch( sourceConeFace )
			{
				case 0:
					rotateBaseDirection = true;
					baseDirection[0].index = 51;
					baseDirection[1].index = 16;
					baseDirection[2].index = 24;
					break;
				case 1:
					rotateBaseDirection = true;
					baseDirection[0].index = 53;
					baseDirection[1].index = 21;
					baseDirection[2].index = 29;
					break;
				case 2:
					baseDirection[0].index = 46;
					baseDirection[1].index = 13;
					baseDirection[2].index = 21;
					break;
				case 3:
					baseDirection[0].index = 48;
					baseDirection[1].index = 10;
					baseDirection[2].index = 18;
					break;
			}
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			constructedSize[2] = SIZE_M;
			break;
	}

	
	baseDirection[0].SetPosition( zomeDirection -> faceDirection[ baseDirection[0].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[0].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[0].index * 3 + 2 ] );
	baseDirection[1].SetPosition( zomeDirection -> faceDirection[ baseDirection[1].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[1].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[1].index * 3 + 2 ] );
	baseDirection[2].SetPosition( zomeDirection -> faceDirection[ baseDirection[2].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[2].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[2].index * 3 + 2 ] );

	if( rotateBaseDirection )
	{
		/*
		std::cout << "����e����V�O�I1�G(" << baseDirection[0].position[0] << "," <<
										  baseDirection[0].position[1] << "," <<
										  baseDirection[0].position[2] << ")" << std::endl;
		std::cout << "����e����V�O�I2�G(" << baseDirection[1].position[0] << "," <<
										  baseDirection[1].position[1] << "," <<
										  baseDirection[1].position[2] << ")" << std::endl;
		std::cout << "����e����V�O�I3�G(" << baseDirection[2].position[0] << "," <<
										  baseDirection[2].position[1] << "," <<
										  baseDirection[2].position[2] << ")" << std::endl;
										  */
		MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );
		MathHelper::vectorMultMatrix( baseDirection[0].position , rotateYMatrix );
		MathHelper::vectorMultMatrix( baseDirection[1].position , rotateYMatrix );
		MathHelper::vectorMultMatrix( baseDirection[2].position , rotateYMatrix );
		MathHelper::makeIdentity( rotateYMatrix );
		/*
		std::cout << "����᪺��V�O�I1�G(" << baseDirection[0].position[0] << "," <<
										  baseDirection[0].position[1] << "," <<
										  baseDirection[0].position[2] << ")" << std::endl;
		std::cout << "����᪺��V�O�I2�G(" << baseDirection[1].position[0] << "," <<
										  baseDirection[1].position[1] << "," <<
										  baseDirection[1].position[2] << ")" << std::endl;
		std::cout << "����᪺��V�O�I3�G(" << baseDirection[2].position[0] << "," <<
										  baseDirection[2].position[1] << "," <<
										  baseDirection[2].position[2] << ")" << std::endl;
										  */
	}
	

	//�o��n�ھڤT���@�bconeDirection�O0�����p�U�A�]�N�Ob0Direction�O(0,1,0)�Ab5Direction�O(-1,0,0)��
	//�o�ӤT���@���U��(sourceConeFace)���~��[�إߤ覡�A�]�w�o����[�n�����@�ӭ��W��

	switch( coneDirection )
	{
		case 0:
			break;
		case 1:
			MathHelper::makeRotateX( rotateXMatrix , M_PI );//¶x�b����180��
			break;
		case 2:
			MathHelper::makeRotateY( rotateYMatrix , M_PI );//¶y�b����180��
			break;
		case 3:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b����180��
			break;
		case 4:
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//¶x�b�f�V����90��
			break;
		case 5:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b����180��
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//¶x�b�f�V����90��
			break;
		case 6:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b����180��
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//¶x�b���V����90��
			break;
		case 7:
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//¶x�b���V����90��
			break;
		case 8:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//¶y�b���V����90��
			break;
		case 9:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f�V����90��
			break;
		case 10:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f�V����90��
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f�V����90��
			break;
		case 11:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f�V����90��
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//¶y�b���V����90��
			break;
		case 12:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			break;
		case 13:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f�V����90��
			break;
		case 14:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f�V����90��
			MathHelper::makeRotateY( rotateYMatrix , M_PI );//¶y�b����180��
			break;
		case 15:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			MathHelper::makeRotateY( rotateYMatrix , M_PI );//¶y�b����180��
			break;
		case 16:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b����180��
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//¶y�b���V����90��
			break;
		case 17:
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//¶y�b���V����90��
			break;
		case 18:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b����180��
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f�V����90��
			break;
		case 19:
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f�V����90��
			break;
		case 20:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f�V����90��
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//¶x�b�f�V����90��
			break;
		case 21:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//¶x�b���V����90��
			break;
		case 22:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f�V����90��
			//MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			//MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f�V����90��
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//¶x�b���V����90��
			break;
		case 23:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b���V����90��
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//¶x�b�f�V����90��
			break;
	}

	//�N�쥻����V���W����x�}
	MathHelper::vectorMultMatrix( tempDirection[0].position , baseDirection[0].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateXMatrix );

	MathHelper::vectorMultMatrix( tempDirection[1].position , baseDirection[1].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateXMatrix );

	MathHelper::vectorMultMatrix( tempDirection[2].position , baseDirection[2].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[2].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[2].position , rotateXMatrix );

	

	//�ھڱ���H�᪺��V�A��X�L����
	constructedHole[0] = zomeDirection -> GetDirectionFace( tempDirection[0].position );
	constructedHole[1] = zomeDirection -> GetDirectionFace( tempDirection[1].position );
	constructedHole[2] = zomeDirection -> GetDirectionFace( tempDirection[2].position );

	delete[] baseDirection;
	delete[] tempDirection;
	delete[] rotateXMatrix;
	delete[] rotateYMatrix;
	delete[] rotateZMatrix;

	//�o�̮ھڿ�n���}�A��zometool��l���t�@�ݥ��ܺ�X��
	GLfloat endX = 0 , endY = 0 , endZ = 0;

	if( constructedHole[0] == -1 || constructedHole[1] == -1 || constructedHole[2] == -1 )
	{
		std::cout << "GetFaceLength�X��(��J-1)" << std::endl;
		return false;
	}

	endX = sourceCorner[0] -> position[0] + 
		   zomeDirection -> faceDirection[ constructedHole[0] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) - NODE_DIAMETER / 2.0 + LATCH );
	endY = sourceCorner[0] -> position[1] + 
		   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) - NODE_DIAMETER / 2.0 + LATCH );
	endZ = sourceCorner[0] -> position[2] + 
		   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) - NODE_DIAMETER / 2.0 + LATCH );
	cornerEnd[0] = new Point( endX , endY , endZ );

	endX = sourceCorner[1] -> position[0] + 
		   zomeDirection -> faceDirection[ constructedHole[1] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[1] , constructedSize[1] ) - NODE_DIAMETER / 2.0 + LATCH );
	endY = sourceCorner[1] -> position[1] + 
		   zomeDirection -> faceDirection[ constructedHole[1] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[1] , constructedSize[1] ) - NODE_DIAMETER / 2.0 + LATCH );
	endZ = sourceCorner[1] -> position[2] + 
		   zomeDirection -> faceDirection[ constructedHole[1] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[1] , constructedSize[1] ) - NODE_DIAMETER / 2.0 + LATCH );
	cornerEnd[1] = new Point( endX , endY , endZ );

	endX = sourceCorner[2] -> position[0] + 
		   zomeDirection -> faceDirection[ constructedHole[2] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[2] , constructedSize[2] ) - NODE_DIAMETER / 2.0 + LATCH );
	endY = sourceCorner[2] -> position[1] + 
		   zomeDirection -> faceDirection[ constructedHole[2] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[2] , constructedSize[2] ) - NODE_DIAMETER / 2.0 + LATCH );
	endZ = sourceCorner[2] -> position[2] + 
		   zomeDirection -> faceDirection[ constructedHole[2] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[2] , constructedSize[2] ) - NODE_DIAMETER / 2.0 + LATCH );
	cornerEnd[2] = new Point( endX , endY , endZ );

	return true;
}

bool OuterCone::CheckNeighborOuterCone()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if(1){}
	}
	return true;
}

bool OuterCone::CheckDublicatedCone()
{
	//���ݧ��q���S���I��O�H���Y
	//GLfloat connEndPoint[3];
	//GLfloat endX = 0 , endY = 0 , endZ = 0;
	//endX = sourceCorner[0] -> position[0] + 
	//	   zomeDirection -> faceDirection[ constructedHole[0] * 3 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) );
	//endY = sourceCorner[0] -> position[1] + 
	//	   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 1 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) );
	//endZ = sourceCorner[0] -> position[2] + 
	//	   zomeDirection -> faceDirection[ constructedHole[0] * 3 + 2 ] * ( zomeDirection -> GetFaceLength( constructedHole[0] , constructedSize[0] ) );
	//MathHelper::SetVector( connEndPoint , endX , endY , endZ );

	Point* endPointCenter = new Point( cornerEnd[0] -> position );
	endPointCenter -> Plus( cornerEnd[1] );
	endPointCenter -> Plus( cornerEnd[2] );
	endPointCenter -> Multiply( 1.0 / 3.0 );
	if( cornerEnd[0] -> DistanceToPoint( cornerEnd[1] -> position ) > NODE_DIAMETER * 3.0 )
	{
		std::cout << "ZomeGenerator::CheckDistanceToSurface�X���G�T��endPoint�Z���ӻ�(" <<
			cornerEnd[0] -> position[0] << "," <<
			cornerEnd[0] -> position[1] << "," <<
			cornerEnd[0] -> position[2] << ")(" <<
			cornerEnd[1] -> position[0] << "," <<
			cornerEnd[1] -> position[1] << "," <<
			cornerEnd[1] -> position[2] << ")(" <<
			cornerEnd[2] -> position[0] << "," <<
			cornerEnd[2] -> position[1] << "," <<
			cornerEnd[2] -> position[2] << ")" << std::endl;
	}


	for( int i = 0; i < 4; i++ )
	{
		if( sourceCone -> neighborCone[i] )
		{
			Cone* tempConeI = sourceCone -> neighborCone[i];//���N��

			CheckDublicatedConeEdge( tempConeI );//����
			if( dublicatedWithCone == true ) return true;//�@�o�{���I��A�N���X

			for( int j = 0; j < 4; j++ )
			{
				if( endPointCenter -> DistanceToPoint( tempConeI -> corner[j] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 &&
					tempConeI -> activated )
				{
					if( !intersected )
					{
						std::cout << "��" << type << "�زժk��[�����ݤӾa��O���y" << std::endl;
					}
					intersected = true;
					//return dublicatedWithCone;
				}

				if( sourceCone -> neighborCone[i] -> neighborCone[j] )
				{
					Cone* tempConeJ = tempConeI -> neighborCone[j];//���N��

					CheckDublicatedConeEdge( tempConeJ );//����
					if( dublicatedWithCone == true ) return true;//�@�o�{���I��A�N���X

					for( int k = 0; k < 4; k++ )
					{
						if( endPointCenter -> DistanceToPoint( tempConeJ -> corner[k] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 &&
							tempConeJ -> activated )
						{
							if( !intersected )
							{
								std::cout << "��" << type << "�زժk��[�����ݤӾa��O���y" << std::endl;
							}
							intersected = true;
							//return dublicatedWithCone;
						}
						//�ݧ��q���S���I��O�H���Y================================================
						//if( MathHelper::VectorApproximation( connEndPoint , tempConeJ -> corner[k] -> position ) )  
						//{
						//	dublicatedWithCone = true;
						//}

						if( sourceCone -> neighborCone[i] -> neighborCone[j] -> neighborCone[k] )
						{
							Cone* tempConeK = tempConeJ -> neighborCone[k];

							CheckDublicatedConeEdge( tempConeK );
							if( dublicatedWithCone == true ) return true;

							for( int l = 0; l < 4; l++ )
							{
								if( endPointCenter -> DistanceToPoint( tempConeK -> corner[l] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 &&
									tempConeK -> activated )
								{
									if( !intersected )
									{
										std::cout << "��" << type << "�زժk��[�����ݤӾa��O���y" << std::endl;
									}
									intersected = true;
									//return dublicatedWithCone;
								}
							}
						}
					}
				}
			}
		}
	}

	//�ݧ��q���S���I��O�H���Y=================================================
	for( Cone* c = sourceCone -> firstCone; c != NULL; c = c -> link )
	{
		if( !c -> activated ) continue;
		for( int i = 0; i < 4; i++ )
		{
			if( endPointCenter -> DistanceToPoint( c -> corner[i] -> position ) < float(SOCKET_RADIUS) + NODE_DIAMETER / 2.0 )
			{
				if( !intersected )
				{
					std::cout << "��" << type << "�زժk��[�����ݤӾa��O���y2" << std::endl;
				}
				intersected = true;
				
				//return dublicatedWithCone;
			}
		}
	}

	return dublicatedWithCone;

}

bool OuterCone::CheckDublicatedConeEdge( Cone* cone )
{
	bool connEngaged[3] = { false , false , false };
	//��C���I���I���s�u�ݤ@�����S���ۦP��V
	if( !cone -> activated ) 
		return dublicatedWithCone;//compute��
	for( int i = 0; i < 4; i++ )
	{
		for( int k = 0; k < 3; k++ )
		{
			if( cone -> corner[i] -> CloselyCompare( sourceCorner[k] ) )
			{
				for( int j = 0; j < 4; j++ )
				{
					if( MathHelper::CompareDirection( cone -> corner[i] -> position , 
														cone -> corner[j] -> position , 
														sourceCorner[k] -> position , 
														cornerEnd[k] -> position ) )
					{
						connEngaged[k] = true;
						//dublicatedWithCone = true;
						//dublicatedConeIndex = cone -> index;
					}
				}	
			}
		}
	}

	if( connEngaged[0] && connEngaged[1] && connEngaged[2] )
	{
		dublicatedWithCone = true;
		dublicatedConeIndex = cone -> index;
		std::cout << "��" << type << "�زժk��[�P��LCone���|" << std::endl;
	}
	return dublicatedWithCone;
}

bool OuterCone::CheckDublicatedOuterCone()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 || this == i ) continue;
		
		CheckDublicatedOuterConeEdge( i );
	}
	if( dublicatedWithOuterCone )
	{
		std::cout << "��" << type << "�زժk��[�P��L�~��Cone���|" << std::endl;
	}

	return dublicatedWithOuterCone;
}

bool OuterCone::CheckDublicatedOuterConeEdge( OuterCone* outerCone )
{
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			if( outerCone -> sourceCorner[i] -> CloselyCompare( sourceCorner[j] ) )
			{
				if( MathHelper::CompareDirection( outerCone -> sourceCorner[i] -> position , 
												  outerCone -> cornerEnd[i] -> position , 
												  sourceCorner[j] -> position , 
												  cornerEnd[j] -> position )  )
				{
					dublicatedWithOuterCone = true;
					dublicatedOuterConeIndex = outerCone -> index;
					cornerDublicated[j] = true;//��j��corner�O���ƪ�
				}
			}
		}
	}
	return dublicatedWithOuterCone;
}

bool OuterCone::CheckIntersectWithConnector()
{
	GLfloat tempSourceCorner[3][3];
	MathHelper::VectorMinus( tempSourceCorner[0] , cornerEnd[0] -> position , sourceCorner[0] -> position );
	MathHelper::VectorMinus( tempSourceCorner[1] , cornerEnd[1] -> position , sourceCorner[1] -> position );
	MathHelper::VectorMinus( tempSourceCorner[2] , cornerEnd[2] -> position , sourceCorner[2] -> position );
	MathHelper::VectorNormalize( tempSourceCorner[0] );
	MathHelper::VectorNormalize( tempSourceCorner[1] );
	MathHelper::VectorNormalize( tempSourceCorner[2] );
	MathHelper::VectorPlus( tempSourceCorner[0] , sourceCorner[0] -> position );
	MathHelper::VectorPlus( tempSourceCorner[1] , sourceCorner[1] -> position );
	MathHelper::VectorPlus( tempSourceCorner[2] , sourceCorner[2] -> position );
	//�ݥL���S���M��L��cone����l���
	for( Cone* i = sourceCone -> firstCone; i != NULL; i = i -> link )
	{
		//if( i -> engagedDistance == -1 ) continue;
		if( i -> activated )
		{
			//4���I
			for( int j = 0; j < 4 - 1; j++ )//�T���@���Y�@�I
			{
				//4���I
				for( int k = j + 1; k < 4; k++ )//�T���@���t�@�I �L�̳s�u
				{
					//�T��~��[
					for( int l = 0; l < 3; l++ )//��o�ӳs�u�өMl�o�Ӥ�[��
					{
						if( !i -> corner[j] -> CloselyCompare( sourceCorner[l] ) && !i -> corner[k] -> CloselyCompare( sourceCorner[l] ) )
						{
							GLfloat distance = CheckTwoLineDistance( i -> corner[j] , i -> corner[k] , sourceCorner[l] , cornerEnd[l] );
							if( distance < SMALL_NUMBER )
							{
								intersected = true;
								std::cout << "��" << type << "�زժk��[�P��LCone����l���" << std::endl;
								return true;
							}
						}
					}
				}
			}
		}
	}

	//�ݥL���S�����L��outercone��l���
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 ) continue;
		//�O��cone�o3�ӥ~��[
		for( int j = 0; j < 3; j++ )
		{
			//�ۤv���T��~��[
			for( int k = 0; k < 3; k++ )
			{
				if( !i -> sourceCorner[j] -> CloselyCompare( sourceCorner[k] ) )
				{
					if( CheckTwoLineDistance( i -> sourceCorner[j] , i -> cornerEnd[j] , sourceCorner[k] , cornerEnd[k] ) < SMALL_NUMBER )
					{
						std::cout << "��" << type << "�زժk��[�P��L�~��Cone����l���" << std::endl;
						intersected = true;
						return true;
					}
				}
			}
		}
	}


	return false;
}

bool OuterCone::CheckIntersectWithConeFace()
{
	if( cornerEnd[0] -> DistanceToPoint( cornerEnd[1] -> position ) > NODE_DIAMETER * 2.0 )
	{
		std::cout << "ZomeGenerator::CheckDistanceToSurface�X���G�T��endPoint�Z���ӻ�(" <<
			cornerEnd[0] -> position[0] << "," <<
			cornerEnd[0] -> position[1] << "," <<
			cornerEnd[0] -> position[2] << ")(" <<
			cornerEnd[1] -> position[0] << "," <<
			cornerEnd[1] -> position[1] << "," <<
			cornerEnd[1] -> position[2] << ")(" <<
			cornerEnd[2] -> position[0] << "," <<
			cornerEnd[2] -> position[1] << "," <<
			cornerEnd[2] -> position[2] << ")" << std::endl;
	}
	//��0�G032
	//��1�G123
	//��2�G210
	//��3�G301
	int faceCorner[4][3] = { { 0 , 3 , 2 } , 
							 { 1 , 2 , 3 } ,
							 { 2 , 1 , 0 } , 
							 { 3 , 0 , 1 } };
	GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 2.0;


	//�ݥL���S���M��L��cone�������
	for( Cone* c = sourceCone -> firstCone; c != NULL; c = c -> link )
	{
		if( c -> activated )
		{
			//�p�G�Z���ӻ��N���L
			if( MathHelper::VectorDistance( c -> corner[0] -> position , sourceCorner[0] -> position ) > threshold ) 
				continue;
			//4�ӭ�
			for( int i = 0; i < 4; i++ )
			{
				if( CheckIntersectOuterConeOnTri( c -> corner[ faceCorner[i][0] ] -> position ,
												  c -> corner[ faceCorner[i][1] ] -> position ,
												  c -> corner[ faceCorner[i][2] ] -> position ) )
				{
					intersected = true;
					std::cout << "��" << type << "�زժk�|���J��LCone��" << std::endl;
					return true;
				}
			}
		}
	}
	return false;
}

GLfloat OuterCone::CheckTwoLineDistance( Point* vec1Initial , Point* vec1End , Point* vec2Initial , Point* vec2End )
{
		//http://geomalgorithms.com/a07-_distance.html
	/*
	// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
	//    Input:  two 3D line segments S1 and S2
	//    Return: the shortest distance between S1 and S2
	float
	dist3D_Segment_to_Segment( Segment S1, Segment S2)
	{
		Vector   u = S1.P1 - S1.P0;
		Vector   v = S2.P1 - S2.P0;
		Vector   w = S1.P0 - S2.P0;
		float    a = dot(u,u);         // always >= 0
		float    b = dot(u,v);
		float    c = dot(v,v);         // always >= 0
		float    d = dot(u,w);
		float    e = dot(v,w);
		float    D = a*c - b*b;        // always >= 0
		float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
		float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
    
	}*/
	Point* u = new Point(); u -> Vector( vec1Initial -> position , vec1End -> position );
	Point* v = new Point(); v -> Vector( vec2Initial -> position , vec2End -> position );
	Point* w = new Point(); w -> Vector( vec2Initial -> position , vec1Initial -> position );

	GLfloat a = u -> Dot( u -> position );
	GLfloat b = u -> Dot( v -> position );
	GLfloat c = v -> Dot( v -> position );
	GLfloat d = u -> Dot( w -> position );
	GLfloat e = v -> Dot( w -> position );
	GLfloat D = a*c - b*b;
	GLfloat sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    GLfloat tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
	//GLfloat SMALL_NUM = 0.001;

	 // compute the line parameters of the two closest points
	if (D < SMALL_NUMBER) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < SMALL_NUMBER ) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < SMALL_NUMBER ) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < SMALL_NUMBER )
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < SMALL_NUMBER )
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUMBER ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUMBER ? 0.0 : tN / tD);

    // get the difference of the two closest points
   // Vector   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
	Point* dP = new Point( w -> position[0] + sc * u -> position[0] - tc * v -> position[0] ,
						   w -> position[1] + sc * u -> position[1] - tc * v -> position[1] ,
						   w -> position[2] + sc * u -> position[2] - tc * v -> position[2] );

	//return norm(dP);   // return the closest distance
	GLfloat result = dP -> Absolution();
	delete u;
	delete v;
	delete w;
	delete dP;
	return result;
}

bool OuterCone::CheckEngaged()
{
	
	return engaged;
}

bool OuterCone::CheckIntersectOuterConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	//std::cout << "�Ĥ@���I�O�G" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
	
	

	Point* d = new Point();

	bool egdeEngaged[3] = { false , false , false };

	bool isIntersected = false;

	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//�o��O? ��X�O�����I�M�����I���s�u
	for( int i = 0 ; i < 3; i++ )
	{
		GLfloat tempSourceCorner[3];
		MathHelper::VectorMinus( tempSourceCorner , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( tempSourceCorner );
		MathHelper::VectorPlus( tempSourceCorner , sourceCorner[i] -> position );
		d -> Vector( tempSourceCorner , cornerEnd[i] ->position );
		/*
		std::cout << "�o�Ӥ������m�O" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		std::cout << "��0����m�O" << corner[0] -> position[0] << "," << corner[0] -> position[1] << "," << corner[0] -> position[2] << std::endl;
		std::cout << "��1����m�O" << corner[1] -> position[0] << "," << corner[1] -> position[1] << "," << corner[1] -> position[2] << std::endl;
		std::cout << "��2����m�O" << corner[2] -> position[0] << "," << corner[2] -> position[1] << "," << corner[2] -> position[2] << std::endl;
		std::cout << "��3����m�O" << corner[3] -> position[0] << "," << corner[3] -> position[1] << "," << corner[3] -> position[2] << std::endl;
		std::cout << "��4����m�O" << corner[4] -> position[0] << "," << corner[4] -> position[1] << "," << corner[4] -> position[2] << std::endl;
		std::cout << "��5����m�O" << corner[5] -> position[0] << "," << corner[5] -> position[1] << "," << corner[5] -> position[2] << std::endl;
		std::cout << "��6����m�O" << corner[6] -> position[0] << "," << corner[6] -> position[1] << "," << corner[6] -> position[2] << std::endl;
		std::cout << "��7����m�O" << corner[7] -> position[0] << "," << corner[7] -> position[1] << "," << corner[7] -> position[2] << std::endl;
		*/
		Point* s1 = new Point();
		s1 -> Cross( d , e2 );

		GLfloat divisor = s1 -> Dot( e1 );
   
		if ( abs(divisor) < SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			continue;
		}

		// Compute first barycentric coordinate

		Point* s = new Point();
		s -> Vector( point1 , tempSourceCorner );
		GLfloat b1 = s -> Dot( s1 ) / divisor;
		if( b1 < -SMALL_NUMBER || b1 > 1.0 + SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			delete s;
			continue;
		}


				
		// Compute second barycentric coordinate
		Point* s2 = new Point();
		s2 -> Cross( s , e1 );
		GLfloat b2 = d -> Dot( s2 ) / divisor;
		if( b2 < -SMALL_NUMBER || b1 + b2 > 1.0 + SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			delete s;
			delete s2;
			continue;
		}

				
		// Compute _t_ to intersection point
		GLfloat t = e2 -> Dot( s2 ) / divisor;
		if( t < -SMALL_NUMBER || t > 1.0 + SMALL_NUMBER )
		{
			delete s1;
			delete s;
			delete s2;
			//���ۥ�
			continue;
		}
				
		egdeEngaged[ i ] = true;
		engaged = true;
		isIntersected = true;
				
		delete s1;
		delete s;
		delete s2;
	}
	delete d;
	delete e1;
	delete e2;

	return isIntersected;

}

bool OuterCone::CheckIntersectOuterConeOnConeFace( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	//std::cout << "�Ĥ@���I�O�G" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
	
	Point* d = new Point();

	bool egdeEngaged[3] = { false , false , false };

	bool isIntersected = false;
	//�o��n���l���_�I���~���@�I�A�H�K��l�M�ۤv�s����������
	GLfloat tempSourceCorner[3][3];
	MathHelper::VectorMinus( tempSourceCorner[0] , cornerEnd[0] -> position , sourceCorner[0] -> position );
	MathHelper::VectorMinus( tempSourceCorner[1] , cornerEnd[1] -> position , sourceCorner[1] -> position );
	MathHelper::VectorMinus( tempSourceCorner[2] , cornerEnd[2] -> position , sourceCorner[2] -> position );
	MathHelper::VectorNormalize( tempSourceCorner[0] );
	MathHelper::VectorNormalize( tempSourceCorner[1] );
	MathHelper::VectorNormalize( tempSourceCorner[2] );
	MathHelper::VectorPlus( tempSourceCorner[0] , sourceCorner[0] -> position );
	MathHelper::VectorPlus( tempSourceCorner[1] , sourceCorner[1] -> position );
	MathHelper::VectorPlus( tempSourceCorner[2] , sourceCorner[2] -> position );
	//��o


	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//�o��O? ��X�O�����I�M�����I���s�u
	for( int i = 0 ; i < 3; i++ )
	{
		
		d -> Vector( tempSourceCorner[i] , cornerEnd[i] -> position );
		/*
		std::cout << "�o�Ӥ������m�O" << position[0] << "," << position[1] << "," << position[2] << std::endl;
		std::cout << "��0����m�O" << corner[0] -> position[0] << "," << corner[0] -> position[1] << "," << corner[0] -> position[2] << std::endl;
		std::cout << "��1����m�O" << corner[1] -> position[0] << "," << corner[1] -> position[1] << "," << corner[1] -> position[2] << std::endl;
		std::cout << "��2����m�O" << corner[2] -> position[0] << "," << corner[2] -> position[1] << "," << corner[2] -> position[2] << std::endl;
		std::cout << "��3����m�O" << corner[3] -> position[0] << "," << corner[3] -> position[1] << "," << corner[3] -> position[2] << std::endl;
		std::cout << "��4����m�O" << corner[4] -> position[0] << "," << corner[4] -> position[1] << "," << corner[4] -> position[2] << std::endl;
		std::cout << "��5����m�O" << corner[5] -> position[0] << "," << corner[5] -> position[1] << "," << corner[5] -> position[2] << std::endl;
		std::cout << "��6����m�O" << corner[6] -> position[0] << "," << corner[6] -> position[1] << "," << corner[6] -> position[2] << std::endl;
		std::cout << "��7����m�O" << corner[7] -> position[0] << "," << corner[7] -> position[1] << "," << corner[7] -> position[2] << std::endl;
		*/
		Point* s1 = new Point();
		s1 -> Cross( d , e2 );

		GLfloat divisor = s1 -> Dot( e1 );
   
		if ( abs(divisor) < SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			continue;
		}

		// Compute first barycentric coordinate

		Point* s = new Point();
		s -> Vector( point1 , tempSourceCorner[i] );
		GLfloat b1 = s -> Dot( s1 ) / divisor;
		if( b1 < -SMALL_NUMBER || b1 > 1.0 + SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			delete s;
			continue;
		}


				
		// Compute second barycentric coordinate
		Point* s2 = new Point();
		s2 -> Cross( s , e1 );
		GLfloat b2 = d -> Dot( s2 ) / divisor;
		if( b2 < -SMALL_NUMBER || b1 + b2 > 1.0 + SMALL_NUMBER )
		{
			//���ۥ�
			delete s1;
			delete s;
			delete s2;
			continue;
		}

				
		// Compute _t_ to intersection point
		GLfloat t = e2 -> Dot( s2 ) / divisor;
		if( t < -SMALL_NUMBER || t > 1.0 + SMALL_NUMBER )
		{
			delete s1;
			delete s;
			delete s2;
			//���ۥ�
			continue;
		}
				
		egdeEngaged[ i ] = true;
		isIntersected = true;
				
		delete s1;
		delete s;
		delete s2;
	}
	delete d;
	delete e1;
	delete e2;

	return isIntersected;

}

bool OuterCone::AddToLink()
{
	for( OuterCone* i = firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = this;
			break;
		}
	}
	return true;
}

bool OuterCone::TryDifferentType( int differentType )
{
	//�Φܤ@�Mengaged�~dublicated
	engaged = false;
	connectorEngaged[0] = false;
	connectorEngaged[1] = false;
	connectorEngaged[2] = false;

	dublicatedWithCone = false;
	dublicatedConeIndex = -1;

	dublicatedWithOuterCone = false;
	dublicatedOuterConeIndex = -1;
	cornerDublicated[0] = false;
	cornerDublicated[1] = false;
	cornerDublicated[2] = false;

	intersected = false;


	type = differentType;
	//���s�ؤ@�M��l
	if( !SelectConstructedHole() ) 
	{
		std::cout << "�o��outerCone" << this << "����" << type << "�زժk�L�k�դW�h" << std::endl;
		return false;
	
	}

	if( type != 0 ) 
	{
		CheckDublicatedCone();//��type�O0���ɭԡA�̭���constructHole[0]�|�X��
		CheckIntersectWithConeFace();
		//�o��]��X
	}


	if( this != firstOuterCone )
	{
		CheckDublicatedOuterCone();//�ˬd���X�Ӫ���V���S���M�O�H����
		//�M���S�����J�O��cone
		//���b�o�ѥX
	}

	if( !engaged && !dublicatedWithCone && !dublicatedWithOuterCone )
	{
		CheckIntersectWithConnector();//�ˬd���S�����
		//�b�b�o�ѥX
		
	}
	
	return true;
}

bool OuterCone::RefreshDistanceToSurface()
{
	for( int i = 0; i < TYPE_AMOUNT; i ++ ) 
	{
		distanceToSurface[ i ] = -1.0;
		weightedDistance[ i ] = -1.0;
	}
	type = 1;

	SelectConstructedHole();
	CheckDublicatedCone();

	return true;
}