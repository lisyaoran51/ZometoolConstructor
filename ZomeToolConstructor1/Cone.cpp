#include "Cone.h"


#pragma region Cone

Cone::Cone( Point* centerPoint )
{
	index = 0;

	type = 0;//���`�T���@
	position = new GLfloat[3];
	b0Direction = new GLfloat[3];
	b5Direction = new GLfloat[3];
	//MathHelper::CopyVector( position , centerPoint -> position );
	MathHelper::SetVector( b0Direction , -1 , 0 , 0 );
	MathHelper::SetVector( b5Direction , 0 , 0 , -1 );//�o�䦳��??
	//type = t;

	firstCone = this;
	engagedFace = new bool[4];
	for(int i = 0; i < 4; i++ )engagedFace[i] = false;
	facePropagated = new bool[4];
	for(int i = 0; i < 4; i++ )facePropagated[i] = false;
	faceApproached = new bool[4];
	for(int i = 0; i < 4; i++ )faceApproached[i] = false;
	faceApproachedIndex = new int[4];
	for(int i = 0; i < 4; i++ )faceApproachedIndex[i] = -1;

	engagedDistance = -1;

	activated = false;
	engaged = false;
	//engagedInDistance = false;
	neighborCone = new Cone*[4];
	neighborCone[0] = NULL;
	neighborCone[1] = NULL;
	neighborCone[2] = NULL;
	neighborCone[3] = NULL;

	zomeDirection = new ZomeDirection();

	//parent = pyramidGroup;

	SetCorner( centerPoint );
}

Cone::Cone( Cone* cone , int facePropagatedFrom )
{
	type = 0;//���`�T���@
	position = new GLfloat[3];
	b0Direction = new GLfloat[3];
	b5Direction = new GLfloat[3];
	//MathHelper::CopyVector( position , cone -> position );

	
	firstCone = cone -> firstCone;
	engagedFace = new bool[4];
	for(int i = 0; i < 4; i++ )engagedFace[i] = false;
	facePropagated = new bool[4];
	for(int i = 0; i < 4; i++ )facePropagated[i] = false;
	//faceApproached = new bool[4];
	//for(int i = 0; i < 4; i++ )faceApproached[i] = false;

	engagedDistance = -1;

	//�M��o�ӤT���Ϊ�type�A�μW�����T���������M�W�������ӧP�_
	//�t�~�T�{�L�ҳQ�W������facePropagated�A�L���y��
	
	switch( facePropagatedFrom )
	{
		case 0:
			facePropagated[1] = true;
			break;
		case 1:
			facePropagated[0] = true;
			break;
		case 2:
			facePropagated[3] = true;
			break;
		case 3:
			facePropagated[2] = true;
			break;
	}
	
	


	engaged = false;
	//engagedInDistance = false;
	neighborCone = new Cone*[4];
	neighborCone[0] = NULL;
	neighborCone[1] = NULL;
	neighborCone[2] = NULL;
	neighborCone[3] = NULL;

	zomeDirection = firstCone -> zomeDirection;
	

	SetCorner( cone , facePropagatedFrom );


	dublicated = false;
	if( !CheckDuplicated() )
	{
		CheckNeighbor();
		AddToLink();
		std::cout << "index�O" << index << ",�q��" << cone -> index << "�ӤT���@����" << facePropagatedFrom << "�������L��" << std::endl;
	}
	else
	{
		dublicated = true;
	}
}

Cone::~Cone()
{
	
	delete[] position;
	delete[] b0Direction;
	delete[] b5Direction;

	delete[] engagedFace;
	delete[] facePropagated;

	delete[] neighborCone;
	//delete firstGrid;
	//delete link;
	if( corner )
	{
		/*
		delete corner[0] ,
			   corner[1] , 
			   corner[2] , 
			   corner[3];*/
		delete[] corner;
	}
	if( localCorner )
	{
		/*
		delete localCorner[0] ,
			   localCorner[1] , 
			   localCorner[2] , 
			   localCorner[3];*/
		delete[] localCorner;
	}
	
	//delete zomeDirection;
}

void Cone::SetCorner( Point* centerPoint )
{
	//�S�U���O�Ĥ@���I
	corner = new Point*[4];
	localCorner = new Point*[4];
	//�M�䩹�U�@�Ө������
	GLfloat* translate = new GLfloat[3];
	GLfloat length = 0;
	index = 0;

	//�I0�G���I
	corner[0] = new Point( centerPoint -> position[0] , centerPoint ->  position[1] , centerPoint ->  position[2] );
	localCorner[0] = new Point( 0 , 0 , 0 );
	//�I1�G��1�� �Ŧ�
	length = zomeDirection -> GetFaceLength ( 1 , SIZE_S );
	MathHelper::SetVector( translate , zomeDirection->faceDirection[1*3] * length , 
										zomeDirection->faceDirection[1*3+1] * length ,
										zomeDirection->faceDirection[1*3+2] * length );
	corner[1] = new Point( centerPoint ->  position[0] + translate[0] , 
						   centerPoint ->  position[1] + translate[1] , 
						   centerPoint ->  position[2] + translate[2] );
	localCorner[1] = new Point( translate[0] , translate[1] , translate[2] );
	//�I2�G��55�� ����
	length = zomeDirection -> GetFaceLength ( 55 , SIZE_S );
	MathHelper::SetVector( translate , zomeDirection->faceDirection[55*3] * length , 
										zomeDirection->faceDirection[55*3+1] * length ,
										zomeDirection->faceDirection[55*3+2] * length );
	corner[2] = new Point( centerPoint ->  position[0] + translate[0] , 
						   centerPoint ->  position[1] + translate[1] , 
						   centerPoint ->  position[2] + translate[2] );
	localCorner[2] = new Point( translate[0] , translate[1] , translate[2] );
	//�I3�G��59�� ����
	length = zomeDirection -> GetFaceLength ( 59 , SIZE_S );
	MathHelper::SetVector( translate , zomeDirection->faceDirection[59*3] * length , 
										zomeDirection->faceDirection[59*3+1] * length ,
										zomeDirection->faceDirection[59*3+2] * length );
	corner[3] = new Point( centerPoint ->  position[0] + translate[0] , 
						   centerPoint ->  position[1] + translate[1] , 
						   centerPoint ->  position[2] + translate[2] );
	localCorner[3] = new Point( translate[0] , translate[1] , translate[2] );

	delete translate;

	position[0] = ( corner[0] -> position[0] + 
					corner[1] -> position[0] + 
					corner[2] -> position[0] + 
					corner[3] -> position[0] ) / 4.0;
	position[1] = ( corner[0] -> position[1] + 
					corner[1] -> position[1] + 
					corner[2] -> position[1] + 
					corner[3] -> position[1] ) / 4.0;
	position[2] = ( corner[0] -> position[2] + 
					corner[1] -> position[2] + 
					corner[2] -> position[2] + 
					corner[3] -> position[2] ) / 4.0;
}

void Cone::SetCorner( Cone* cone , int facePropagatedFrom )
{
	//�S�U���O�Ĥ@���I
	corner = new Point*[4];
	localCorner = new Point*[4];
	GLfloat* rotateMatrix = new GLfloat[16];
	MathHelper::makeIdentity( rotateMatrix );

	//�ܰ�����x�}
	switch( facePropagatedFrom )
	{
		case 0:
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , -1 ) ) //¶z����
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );//��ƬO�H�f���V�����A�]�N�O����w�h�A�ҥH���׭n��-1
				//std::cout << "¶z����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , 1 ) ) //¶z�f��
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶z�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , -1 , 0 , 0 ) ) //¶x����
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶x����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 1 , 0 , 0 ) ) //¶x�f��
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶x�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , -1 , 0 ) ) //¶y����
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶y����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 1 , 0 ) ) //¶y�f��
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶y�f��" << std::endl; 
			}
			break;
		case 1:
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , -1 ) ) //¶z�f��
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶z�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , 1 ) ) //¶z����
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶z����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , -1 , 0 , 0 ) ) //¶x�f��
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶x�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 1 , 0 , 0 ) ) //¶x����
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶x����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , -1 , 0 ) ) //¶y�f��
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶y�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 1 , 0 ) ) //¶y����
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶y����" << std::endl; 
			}
			break;
		case 2:
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , -1 ) ) //¶z����
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶z����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , 1 ) ) //¶z�f��
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶z�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , -1 , 0 , 0 ) ) //¶x����
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶x����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 1 , 0 , 0 ) ) //¶x�f��
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶x�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , -1 , 0 ) ) //¶y����
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶y����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 1 , 0 ) ) //¶y�f��
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶y�f��" << std::endl; 
			}
			break;
		case 3:
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , -1 ) ) //¶z�f��
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶z�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , 1 ) ) //¶z����
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶z����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , -1 , 0 , 0 ) ) //¶x�f��
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶x�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 1 , 0 , 0 ) ) //¶x����
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶x����" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , -1 , 0 ) ) //¶y�f��
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "¶y�f��" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 1 , 0 ) ) //¶y����
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "¶y����" << std::endl; 
			}
			break;
	}

	MathHelper::CleanMatrixSmall( rotateMatrix );

	//MathHelper::print4x4Matrix( rotateMatrix );
	//std::cout << "||" << rotateMatrix[0] << "," << rotateMatrix[1] << "," << rotateMatrix[2] << "," << rotateMatrix[3] << "||" << std::endl;
	//std::cout << "||" << rotateMatrix[4] << "," << rotateMatrix[5] << "," << rotateMatrix[6] << "," << rotateMatrix[7] << "||" << std::endl;
	//std::cout << "||" << rotateMatrix[8] << "," << rotateMatrix[9] << "," << rotateMatrix[10] << "," << rotateMatrix[11] << "||" << std::endl;
	//std::cout << "||" << rotateMatrix[12] << "," << rotateMatrix[13] << "," << rotateMatrix[14] << "," << rotateMatrix[15] << "||" << std::endl;


	localCorner[0] = new Point();
	localCorner[1] = new Point();
	localCorner[2] = new Point();
	localCorner[3] = new Point();
	//�|�Ө����y�б���
	MathHelper::vectorMultMatrix( localCorner[0] -> position , cone -> localCorner[0] -> position , rotateMatrix );
	MathHelper::vectorMultMatrix( localCorner[1] -> position , cone -> localCorner[1] -> position , rotateMatrix );
	MathHelper::vectorMultMatrix( localCorner[2] -> position , cone -> localCorner[2] -> position , rotateMatrix );
	MathHelper::vectorMultMatrix( localCorner[3] -> position , cone -> localCorner[3] -> position , rotateMatrix );
	//b0�Mb5������
	MathHelper::vectorMultMatrix( b0Direction , cone -> b0Direction , rotateMatrix );
	MathHelper::vectorMultMatrix( b5Direction , cone -> b5Direction , rotateMatrix );
	
	/*
	std::cout << "�쥻localCorner[0]:" << cone -> localCorner[0] -> position[0] << ","
								   << cone -> localCorner[0] -> position[1] << ","
								   << cone -> localCorner[0] -> position[2] << std::endl;
	std::cout << "�쥻localCorner[1]:" << cone -> localCorner[1] -> position[0] << ","
								   << cone -> localCorner[1] -> position[1] << ","
								   << cone -> localCorner[1] -> position[2] << std::endl;
	std::cout << "�쥻localCorner[2]:" << cone -> localCorner[2] -> position[0] << ","
								   << cone -> localCorner[2] -> position[1] << ","
								   << cone -> localCorner[2] -> position[2] << std::endl;
	std::cout << "�쥻localCorner[3]:" << cone -> localCorner[3] -> position[0] << ","
								   << cone -> localCorner[3] -> position[1] << ","
								   << cone -> localCorner[3] -> position[2] << std::endl;
	

	std::cout << "localCorner[0]:" << localCorner[0] -> position[0] << ","
								   << localCorner[0] -> position[1] << ","
								   << localCorner[0] -> position[2] << std::endl;
	std::cout << "localCorner[1]:" << localCorner[1] -> position[0] << ","
								   << localCorner[1] -> position[1] << ","
								   << localCorner[1] -> position[2] << std::endl;
	std::cout << "localCorner[2]:" << localCorner[2] -> position[0] << ","
								   << localCorner[2] -> position[1] << ","
								   << localCorner[2] -> position[2] << std::endl;
	std::cout << "localCorner[3]:" << localCorner[3] -> position[0] << ","
								   << localCorner[3] -> position[1] << ","
								   << localCorner[3] -> position[2] << std::endl;
	*/
	corner[0] = new Point();
	corner[1] = new Point();
	corner[2] = new Point();
	corner[3] = new Point();

	GLfloat* vectorAdder = new GLfloat[3];

	switch( facePropagatedFrom )
	{
		case 0:
			
			MathHelper::VectorMinus( vectorAdder , localCorner[0] -> position , localCorner[1] -> position ); //�I0�O�W�������I�A�o����I1���I0����V
			MathHelper::CopyVector( corner[1] -> position , cone -> corner[0] -> position );
			MathHelper::CopyVector( corner[2] -> position , cone -> corner[2] -> position );
			MathHelper::CopyVector( corner[3] -> position , cone -> corner[3] -> position );
			MathHelper::VectorPlus( corner[0] -> position , corner[1] -> position , vectorAdder );//�I0 = �I1 + �I1���I0���Z��
			break;
		case 1:
			MathHelper::VectorMinus( vectorAdder , localCorner[1] -> position , localCorner[0] -> position ); //�I1�O�W�������I�A�o����I0���I1����V
			MathHelper::CopyVector( corner[0] -> position , cone -> corner[1] -> position );
			MathHelper::CopyVector( corner[2] -> position , cone -> corner[2] -> position );
			MathHelper::CopyVector( corner[3] -> position , cone -> corner[3] -> position );
			MathHelper::VectorPlus( corner[1] -> position , corner[0] -> position , vectorAdder );//�I1 = �I0 + �I0���I1���Z��
			break;
		case 2:
			MathHelper::VectorMinus( vectorAdder , localCorner[2] -> position , localCorner[3] -> position ); //�I2�O�W�������I�A�o����I3���I2����V
			MathHelper::CopyVector( corner[0] -> position , cone -> corner[0] -> position );
			MathHelper::CopyVector( corner[1] -> position , cone -> corner[1] -> position );
			MathHelper::CopyVector( corner[3] -> position , cone -> corner[2] -> position );
			MathHelper::VectorPlus( corner[2] -> position , corner[3] -> position , vectorAdder );//�I2 = �I3 + �I2���I3���Z��
			break;//�o��X���F
		case 3:
			MathHelper::VectorMinus( vectorAdder , localCorner[3] -> position , localCorner[2] -> position ); //�I3�O�W�������I�A�o����I2���I3����V
			MathHelper::CopyVector( corner[0] -> position , cone -> corner[0] -> position );
			MathHelper::CopyVector( corner[1] -> position , cone -> corner[1] -> position );
			MathHelper::CopyVector( corner[2] -> position , cone -> corner[3] -> position );
			MathHelper::VectorPlus( corner[3] -> position , corner[2] -> position , vectorAdder );//�I3 = �I2 + �I3���I2���Z��
			break;
	}
	delete vectorAdder;
	delete rotateMatrix;

	/*
	std::cout << "�쥻corner[0]:" << cone -> corner[0] -> position[0] << ","
								<< cone -> corner[0] -> position[1] << ","
								<< cone -> corner[0] -> position[2] << std::endl;
	std::cout << "�쥻corner[1]:" << cone -> corner[1] -> position[0] << ","
								<< cone -> corner[1] -> position[1] << ","
								<< cone -> corner[1] -> position[2] << std::endl;
	std::cout << "�쥻corner[2]:" << cone -> corner[2] -> position[0] << ","
								<< cone -> corner[2] -> position[1] << ","
								<< cone -> corner[2] -> position[2] << std::endl;
	std::cout << "�쥻corner[3]:" << cone -> corner[3] -> position[0] << ","
								<< cone -> corner[3] -> position[1] << ","
								<< cone -> corner[3] -> position[2] << std::endl;


	std::cout << "corner[0]:" << corner[0] -> position[0] << ","
							<< corner[0] -> position[1] << ","
							<< corner[0] -> position[2] << std::endl;
	std::cout << "corner[1]:" << corner[1] -> position[0] << ","
							<< corner[1] -> position[1] << ","
							<< corner[1] -> position[2] << std::endl;
	std::cout << "corner[2]:" << corner[2] -> position[0] << ","
							<< corner[2] -> position[1] << ","
							<< corner[2] -> position[2] << std::endl;
	std::cout << "corner[3]:" << corner[3] -> position[0] << ","
							<< corner[3] -> position[1] << ","
							<< corner[3] -> position[2] << std::endl;
	*/
	position[0] = ( corner[0] -> position[0] + 
					corner[1] -> position[0] + 
					corner[2] -> position[0] + 
					corner[3] -> position[0] ) / 4.0;
	position[1] = ( corner[0] -> position[1] + 
					corner[1] -> position[1] + 
					corner[2] -> position[1] + 
					corner[3] -> position[1] ) / 4.0;
	position[2] = ( corner[0] -> position[2] + 
					corner[1] -> position[2] + 
					corner[2] -> position[2] + 
					corner[3] -> position[2] ) / 4.0;
	//std::cout << "�ا�" << std::endl;
}

bool Cone::CheckDuplicated()
{
	bool isDuplicated = false;
	int lastIndex = 0;
	for( Cone* i = firstCone; i != NULL; i = i -> link)
	{
		if( MathHelper::VectorApproximation( i -> position , position ) )
		{
			isDuplicated = true;
		}
		lastIndex = i -> index;
	}
	if( !isDuplicated )
	{
		index = lastIndex + 1;
	}
	return isDuplicated;
}

bool Cone::CheckNeighbor()
{
	/*
	GLfloat** facePlane = new GLfloat*[4];//�ܰ��T���@���|�ӭ�
	facePlane[0] = new GLfloat[4];
	facePlane[1] = new GLfloat[4];
	facePlane[2] = new GLfloat[4];
	facePlane[3] = new GLfloat[4];
	MathHelper::makePlane( facePlane[0] , corner[0] -> position , corner[2] -> position , corner[3] -> position );
	MathHelper::makePlane( facePlane[1] , corner[1] -> position , corner[2] -> position , corner[3] -> position );
	MathHelper::makePlane( facePlane[2] , corner[0] -> position , corner[1] -> position , corner[2] -> position );
	MathHelper::makePlane( facePlane[3] , corner[0] -> position , corner[1] -> position , corner[3] -> position );

	GLfloat** mirrorPosition = new GLfloat*[4];//�ܰ��T���@���|�ӭ�
	mirrorPosition[0] = new GLfloat[3];
	mirrorPosition[1] = new GLfloat[3];
	mirrorPosition[2] = new GLfloat[3];
	mirrorPosition[3] = new GLfloat[3];

	MathHelper::MirrorVector( mirrorPosition[0] , position , facePlane[0] );
	MathHelper::MirrorVector( mirrorPosition[1] , position , facePlane[1] );
	MathHelper::MirrorVector( mirrorPosition[2] , position , facePlane[2] );
	MathHelper::MirrorVector( mirrorPosition[3] , position , facePlane[3] );
	
	std::cout << "���I�G" << position[0] << "," << position[1] << "," << position[2] << std::endl;
	std::cout << "��0�����l���I�G" << (mirrorPosition[0])[0] << "," << (mirrorPosition[0])[1] << "," << (mirrorPosition[0])[2] << std::endl;
	std::cout << "��1�����l���I�G" << (mirrorPosition[1])[0] << "," << (mirrorPosition[1])[1] << "," << (mirrorPosition[1])[2] << std::endl;
	std::cout << "��2�����l���I�G" << (mirrorPosition[2])[0] << "," << (mirrorPosition[2])[1] << "," << (mirrorPosition[2])[2] << std::endl;
	std::cout << "��3�����l���I�G" << (mirrorPosition[3])[0] << "," << (mirrorPosition[3])[1] << "," << (mirrorPosition[3])[2] << std::endl;
	*/
	//===============================�e�s�@�~����//===============================
	//��0���G0,1,2
	//��1���G1,2,3
	//��2���G0,1,2
	//��3���G0,1,3

	bool hasAnotherNeighbor = false;
	for( Cone* i = firstCone; i != NULL; i = i -> link )
	{
		if( i -> corner[0] -> CloselyCompare( corner[0] ) && 
			i -> corner[2] -> CloselyCompare( corner[3] ) && 
			i -> corner[3] -> CloselyCompare( corner[2] ) )
		{
			//����0��->�P��0���۾F
			neighborCone[0] = i;
			i -> neighborCone[0] = this;
			facePropagated[0] = true;
			i -> facePropagated[0] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[1] -> CloselyCompare( corner[0] ) && 
			i -> corner[2] -> CloselyCompare( corner[2] ) && 
			i -> corner[3] -> CloselyCompare( corner[3] ) )
		{
			//����1��->�P��0���۾F
			neighborCone[0] = i;
			i -> neighborCone[1] = this;
			facePropagated[0] = true;
			i -> facePropagated[1] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[1] ) && 
			i -> corner[2] -> CloselyCompare( corner[2] ) && 
			i -> corner[3] -> CloselyCompare( corner[3] ) )
		{
			//����0��->�P��1���۾F
			neighborCone[1] = i;
			i -> neighborCone[0] = this;
			facePropagated[1] = true;
			i -> facePropagated[0] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[1] -> CloselyCompare( corner[1] ) && 
			i -> corner[2] -> CloselyCompare( corner[3] ) && 
			i -> corner[3] -> CloselyCompare( corner[2] ) )
		{
			//����1��->�P��1���۾F
			neighborCone[1] = i;
			i -> neighborCone[1] = this;
			facePropagated[1] = true;
			i -> facePropagated[1] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[1] ) && //debug0207:�ⶶ�Ǳq(0->0 ,1->1 ,2->3)
			i -> corner[1] -> CloselyCompare( corner[0] ) && //�令(0->1 ,1->0 ,2->2)
			i -> corner[2] -> CloselyCompare( corner[2] ) )
		{
			//����2��->�P��2���۾F
			neighborCone[2] = i;
			i -> neighborCone[2] = this;
			facePropagated[2] = true;
			i -> facePropagated[2] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[0] ) && 
			i -> corner[1] -> CloselyCompare( corner[1] ) && 
			i -> corner[3] -> CloselyCompare( corner[2] ) )
		{
			//����3��->�P��2���۾F
			neighborCone[2] = i;
			i -> neighborCone[3] = this;
			facePropagated[2] = true;
			i -> facePropagated[3] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[0] ) && //debug0207:�ⶶ�Ǳq(0->0 ,1->1 ,2->3)
			i -> corner[1] -> CloselyCompare( corner[1] ) && //�令(0->1 ,1->0 ,2->2)
			i -> corner[2] -> CloselyCompare( corner[3] ) )
		{
			//����2��->�P��3���۾F
			neighborCone[3] = i;
			i -> neighborCone[2] = this;
			facePropagated[3] = true;
			i -> facePropagated[2] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[1] ) && //debug0207:�ⶶ�Ǳq(0->0 ,1->1 ,2->3)
			i -> corner[1] -> CloselyCompare( corner[0] ) && //�令(0->1 ,1->0 ,2->2)
			i -> corner[3] -> CloselyCompare( corner[3] ) )
		{
			//����3��->�P��3���۾F
			neighborCone[3] = i;
			i -> neighborCone[3] = this;
			facePropagated[3] = true;
			i -> facePropagated[3] = true;
			hasAnotherNeighbor = true;
		}
		/*debug0208�M��ܹ�쪺�۾F�T���@
		bool debugMatrix[4][4] = {{false,false,false,false},{false,false,false,false},{false,false,false,false},{false,false,false,false}};
		int debugMatchCount = 0;
		for( int j = 0; j < 4; j++ )
		{
			for ( int k = 0; k < 4; k++ )
			{
				if( i -> corner[j] -> CloselyCompare( corner[k] ) )
				{
					debugMatchCount++;
					debugMatrix[j][k] = true;
				}
			}
		}
		bool debugCorrection = false;
		if( debugMatrix[0][0] && debugMatrix[2][3] & debugMatrix[3][2] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[1][0] && debugMatrix[2][2] & debugMatrix[3][3] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[0][1] && debugMatrix[2][2] & debugMatrix[3][3] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[1][1] && debugMatrix[2][3] & debugMatrix[3][2] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[0][1] && debugMatrix[1][0] & debugMatrix[2][2] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[0][0] && debugMatrix[1][1] & debugMatrix[3][2] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[0][0] && debugMatrix[1][1] & debugMatrix[2][3] && debugMatchCount == 3 ) debugCorrection = true;
		if( debugMatrix[0][1] && debugMatrix[1][0] & debugMatrix[3][3] && debugMatchCount == 3 ) debugCorrection = true;
		if( !debugCorrection && debugMatchCount == 3 )
		{
			std::cout << "debug:���s���f�t" << std::endl;
		}*/

	}
	/*
	delete mirrorPosition[0];
	delete mirrorPosition[1];
	delete mirrorPosition[2];
	delete mirrorPosition[3];
	delete mirrorPosition;

	delete facePlane[0];
	delete facePlane[1];
	delete facePlane[2];
	delete facePlane[3];
	delete facePlane;
	*/
	return hasAnotherNeighbor;
}

void Cone::AddToLink()
{
	for( Cone* i = firstCone; i != NULL; i = i -> link)
	{
		if( i -> link == NULL )
		{
			i -> link = this;
			index = i -> index + 1;
			break;
		}
	}
}

bool Cone::CheckIntersectConeOnTri( GLfloat* point1 , GLfloat* point2 , GLfloat* point3 )
{
	//std::cout << "�Ĥ@���I�O�G" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
	
	Point* d = new Point();

	bool egdeEngaged[6] = { false , false , false , false , false , false };

	bool isIntersected = false;

	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//�o��O? ��X�O�����I�M�����I���s�u
	for( int i = 0 ; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			int engagedEdge = -1;
			//��X�o�O���@�� ��4��type 4���I 6����
			switch( i )
			{
				case 0:
					if( j == 1 ) engagedEdge = 0;
					if( j == 2 ) engagedEdge = 2;
					if( j == 3 ) engagedEdge = 3;
					break;
				case 1:
					if( j == 2 ) engagedEdge = 1;
					if( j == 3 ) engagedEdge = 4;
					break;
				case 2:
					if( j == 3 ) engagedEdge = 5;
					break;
			}
			
			//�p�G�o�ӬO�� �Z���|����1 �N�}�l�⭻��
			//if( MathHelper::VectorDistance( corner[i] -> position , corner[j] -> position ) == zomeDirection.GetColorLength( COLOR_BLUE , SIZE_S ) && 
			//	!egdeEngaged[ engagedEdge ] )
			if( engagedEdge >= 0 && !egdeEngaged[ engagedEdge ] )
				//�o��O�� �p�Gi��j�O�@���� �L��engagedEdge�N�|�j�󵥩�0
				//�p�G�o�ӽs������w�g����L���S���ۥ�F �N���ΦA�i�Ӻ�@��
			{//�o��X�� �٨S�]�w corner
				d -> Vector( corner[i] , corner[j] );
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
   
				if (divisor == 0.)
				{
					//���ۥ�
					delete s1;
					continue;
				}

				// Compute first barycentric coordinate

				Point* s = new Point();
				s -> Vector( point1 , corner[i] -> position );
				GLfloat b1 = s -> Dot( s1 ) / divisor;
				if( b1 < 0. || b1 > 1.)
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
				if( b2 < 0. || b1 + b2 > 1.)
				{
					//���ۥ�
					delete s1;
					delete s;
					delete s2;
					continue;
				}

				
				// Compute _t_ to intersection point
				GLfloat t = e2 -> Dot( s2 ) / divisor;
				if( t < 0. || t > 1.)
				{
					delete s1;
					delete s;
					delete s2;
					//���ۥ�
					continue;
				}
				
				egdeEngaged[ engagedEdge ] = true;
				isIntersected = true;
				
				delete s1;
				delete s;
				delete s2;
				
			}
		}
	}

	//�ݭ��X�ӭ����ۥ�
	for( int i = 0; i < 6; i++ )
	{
		if( egdeEngaged[ i ] )
		{
			engaged = true;
			
			switch( i )
			{
				case 0:
					engagedFace[2] = true;
					engagedFace[3] = true;
					break;
				case 1:
					engagedFace[1] = true;
					engagedFace[2] = true;
					break;
				case 2:
					engagedFace[0] = true;
					engagedFace[2] = true;
					break;
				case 3:
					engagedFace[0] = true;
					engagedFace[3] = true;
					break;
				case 4:
					engagedFace[1] = true;
					engagedFace[3] = true;
					break;
				case 5:
					engagedFace[2] = true;
					engagedFace[3] = true;
					break;
			}
		}
	}

	delete d;
	delete e1;
	delete e2;

	return isIntersected;
}

bool Cone::Compare( Cone* cone )
{
	if( MathHelper::CompareVector( cone -> position , position ) )
	{
		return true;
	}
	return false;
}

bool Cone::CloselyCompare( Cone* cone )
{
	if( MathHelper::VectorApproximation( cone -> position , position ) )
	{
		return true;
	}
	return false;
}


#pragma endregion

