#include "Cone.h"


#pragma region Cone

Cone::Cone( Point* centerPoint )
{
	index = 0;

	type = 0;//正常三角錐
	position = new GLfloat[3];
	b0Direction = new GLfloat[3];
	b5Direction = new GLfloat[3];
	//MathHelper::CopyVector( position , centerPoint -> position );
	MathHelper::SetVector( b0Direction , -1 , 0 , 0 );
	MathHelper::SetVector( b5Direction , 0 , 0 , -1 );//這邊有錯??
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
	type = 0;//正常三角錐
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

	//尋找這個三角形的type，用增長的三角形類型和增長的面來判斷
	//另外確認他所被增長的面facePropagated，他的座標
	
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
		std::cout << "index是" << index << ",從第" << cone -> index << "個三角錐的第" << facePropagatedFrom << "面延伸過來" << std::endl;
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
	//又下角是第一個點
	corner = new Point*[4];
	localCorner = new Point*[4];
	//尋找往下一個角的方味
	GLfloat* translate = new GLfloat[3];
	GLfloat length = 0;
	index = 0;

	//點0：原點
	corner[0] = new Point( centerPoint -> position[0] , centerPoint ->  position[1] , centerPoint ->  position[2] );
	localCorner[0] = new Point( 0 , 0 , 0 );
	//點1：第1面 藍色
	length = zomeDirection -> GetFaceLength ( 1 , SIZE_S );
	MathHelper::SetVector( translate , zomeDirection->faceDirection[1*3] * length , 
										zomeDirection->faceDirection[1*3+1] * length ,
										zomeDirection->faceDirection[1*3+2] * length );
	corner[1] = new Point( centerPoint ->  position[0] + translate[0] , 
						   centerPoint ->  position[1] + translate[1] , 
						   centerPoint ->  position[2] + translate[2] );
	localCorner[1] = new Point( translate[0] , translate[1] , translate[2] );
	//點2：第55面 黃色
	length = zomeDirection -> GetFaceLength ( 55 , SIZE_S );
	MathHelper::SetVector( translate , zomeDirection->faceDirection[55*3] * length , 
										zomeDirection->faceDirection[55*3+1] * length ,
										zomeDirection->faceDirection[55*3+2] * length );
	corner[2] = new Point( centerPoint ->  position[0] + translate[0] , 
						   centerPoint ->  position[1] + translate[1] , 
						   centerPoint ->  position[2] + translate[2] );
	localCorner[2] = new Point( translate[0] , translate[1] , translate[2] );
	//點3：第59面 黃色
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
	//又下角是第一個點
	corner = new Point*[4];
	localCorner = new Point*[4];
	GLfloat* rotateMatrix = new GLfloat[16];
	MathHelper::makeIdentity( rotateMatrix );

	//至做旋轉矩陣
	switch( facePropagatedFrom )
	{
		case 0:
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , -1 ) ) //繞z正轉
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );//函數是以逆轉方向為正，也就是左手定則，所以角度要乘-1
				//std::cout << "繞z正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , 1 ) ) //繞z逆轉
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞z逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , -1 , 0 , 0 ) ) //繞x正轉
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞x正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 1 , 0 , 0 ) ) //繞x逆轉
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞x逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , -1 , 0 ) ) //繞y正轉
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞y正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 1 , 0 ) ) //繞y逆轉
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞y逆轉" << std::endl; 
			}
			break;
		case 1:
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , -1 ) ) //繞z逆轉
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞z逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 0 , 1 ) ) //繞z正轉
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞z正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , -1 , 0 , 0 ) ) //繞x逆轉
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞x逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 1 , 0 , 0 ) ) //繞x正轉
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞x正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , -1 , 0 ) ) //繞y逆轉
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞y逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b5Direction , 0 , 1 , 0 ) ) //繞y正轉
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞y正轉" << std::endl; 
			}
			break;
		case 2:
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , -1 ) ) //繞z正轉
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞z正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , 1 ) ) //繞z逆轉
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞z逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , -1 , 0 , 0 ) ) //繞x正轉
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞x正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 1 , 0 , 0 ) ) //繞x逆轉
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞x逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , -1 , 0 ) ) //繞y正轉
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞y正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 1 , 0 ) ) //繞y逆轉
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞y逆轉" << std::endl; 
			}
			break;
		case 3:
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , -1 ) ) //繞z逆轉
			{
				MathHelper::makeRotateZ( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞z逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 0 , 1 ) ) //繞z正轉
			{
				MathHelper::makeRotateZ( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞z正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , -1 , 0 , 0 ) ) //繞x逆轉
			{
				MathHelper::makeRotateX( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞x逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 1 , 0 , 0 ) ) //繞x正轉
			{
				MathHelper::makeRotateX( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞x正轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , -1 , 0 ) ) //繞y逆轉
			{
				MathHelper::makeRotateY( rotateMatrix , 90.0 * M_PI / 180.0 );
				//std::cout << "繞y逆轉" << std::endl; 
			}
			if( MathHelper::CompareVector( cone -> b0Direction , 0 , 1 , 0 ) ) //繞y正轉
			{
				MathHelper::makeRotateY( rotateMatrix , -90.0 * M_PI / 180.0 );
				//std::cout << "繞y正轉" << std::endl; 
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
	//四個角的座標旋轉
	MathHelper::vectorMultMatrix( localCorner[0] -> position , cone -> localCorner[0] -> position , rotateMatrix );
	MathHelper::vectorMultMatrix( localCorner[1] -> position , cone -> localCorner[1] -> position , rotateMatrix );
	MathHelper::vectorMultMatrix( localCorner[2] -> position , cone -> localCorner[2] -> position , rotateMatrix );
	MathHelper::vectorMultMatrix( localCorner[3] -> position , cone -> localCorner[3] -> position , rotateMatrix );
	//b0和b5的旋轉
	MathHelper::vectorMultMatrix( b0Direction , cone -> b0Direction , rotateMatrix );
	MathHelper::vectorMultMatrix( b5Direction , cone -> b5Direction , rotateMatrix );
	
	/*
	std::cout << "原本localCorner[0]:" << cone -> localCorner[0] -> position[0] << ","
								   << cone -> localCorner[0] -> position[1] << ","
								   << cone -> localCorner[0] -> position[2] << std::endl;
	std::cout << "原本localCorner[1]:" << cone -> localCorner[1] -> position[0] << ","
								   << cone -> localCorner[1] -> position[1] << ","
								   << cone -> localCorner[1] -> position[2] << std::endl;
	std::cout << "原本localCorner[2]:" << cone -> localCorner[2] -> position[0] << ","
								   << cone -> localCorner[2] -> position[1] << ","
								   << cone -> localCorner[2] -> position[2] << std::endl;
	std::cout << "原本localCorner[3]:" << cone -> localCorner[3] -> position[0] << ","
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
			
			MathHelper::VectorMinus( vectorAdder , localCorner[0] -> position , localCorner[1] -> position ); //點0是增長的心點，這邊算點1到點0的方向
			MathHelper::CopyVector( corner[1] -> position , cone -> corner[0] -> position );
			MathHelper::CopyVector( corner[2] -> position , cone -> corner[2] -> position );
			MathHelper::CopyVector( corner[3] -> position , cone -> corner[3] -> position );
			MathHelper::VectorPlus( corner[0] -> position , corner[1] -> position , vectorAdder );//點0 = 點1 + 點1到點0的距離
			break;
		case 1:
			MathHelper::VectorMinus( vectorAdder , localCorner[1] -> position , localCorner[0] -> position ); //點1是增長的心點，這邊算點0到點1的方向
			MathHelper::CopyVector( corner[0] -> position , cone -> corner[1] -> position );
			MathHelper::CopyVector( corner[2] -> position , cone -> corner[2] -> position );
			MathHelper::CopyVector( corner[3] -> position , cone -> corner[3] -> position );
			MathHelper::VectorPlus( corner[1] -> position , corner[0] -> position , vectorAdder );//點1 = 點0 + 點0到點1的距離
			break;
		case 2:
			MathHelper::VectorMinus( vectorAdder , localCorner[2] -> position , localCorner[3] -> position ); //點2是增長的心點，這邊算點3到點2的方向
			MathHelper::CopyVector( corner[0] -> position , cone -> corner[0] -> position );
			MathHelper::CopyVector( corner[1] -> position , cone -> corner[1] -> position );
			MathHelper::CopyVector( corner[3] -> position , cone -> corner[2] -> position );
			MathHelper::VectorPlus( corner[2] -> position , corner[3] -> position , vectorAdder );//點2 = 點3 + 點2到點3的距離
			break;//這行出錯了
		case 3:
			MathHelper::VectorMinus( vectorAdder , localCorner[3] -> position , localCorner[2] -> position ); //點3是增長的心點，這邊算點2到點3的方向
			MathHelper::CopyVector( corner[0] -> position , cone -> corner[0] -> position );
			MathHelper::CopyVector( corner[1] -> position , cone -> corner[1] -> position );
			MathHelper::CopyVector( corner[2] -> position , cone -> corner[3] -> position );
			MathHelper::VectorPlus( corner[3] -> position , corner[2] -> position , vectorAdder );//點3 = 點2 + 點3到點2的距離
			break;
	}
	delete vectorAdder;
	delete rotateMatrix;

	/*
	std::cout << "原本corner[0]:" << cone -> corner[0] -> position[0] << ","
								<< cone -> corner[0] -> position[1] << ","
								<< cone -> corner[0] -> position[2] << std::endl;
	std::cout << "原本corner[1]:" << cone -> corner[1] -> position[0] << ","
								<< cone -> corner[1] -> position[1] << ","
								<< cone -> corner[1] -> position[2] << std::endl;
	std::cout << "原本corner[2]:" << cone -> corner[2] -> position[0] << ","
								<< cone -> corner[2] -> position[1] << ","
								<< cone -> corner[2] -> position[2] << std::endl;
	std::cout << "原本corner[3]:" << cone -> corner[3] -> position[0] << ","
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
	//std::cout << "建完" << std::endl;
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
	GLfloat** facePlane = new GLfloat*[4];//至做三角錐的四個面
	facePlane[0] = new GLfloat[4];
	facePlane[1] = new GLfloat[4];
	facePlane[2] = new GLfloat[4];
	facePlane[3] = new GLfloat[4];
	MathHelper::makePlane( facePlane[0] , corner[0] -> position , corner[2] -> position , corner[3] -> position );
	MathHelper::makePlane( facePlane[1] , corner[1] -> position , corner[2] -> position , corner[3] -> position );
	MathHelper::makePlane( facePlane[2] , corner[0] -> position , corner[1] -> position , corner[2] -> position );
	MathHelper::makePlane( facePlane[3] , corner[0] -> position , corner[1] -> position , corner[3] -> position );

	GLfloat** mirrorPosition = new GLfloat*[4];//至做三角錐的四個面
	mirrorPosition[0] = new GLfloat[3];
	mirrorPosition[1] = new GLfloat[3];
	mirrorPosition[2] = new GLfloat[3];
	mirrorPosition[3] = new GLfloat[3];

	MathHelper::MirrorVector( mirrorPosition[0] , position , facePlane[0] );
	MathHelper::MirrorVector( mirrorPosition[1] , position , facePlane[1] );
	MathHelper::MirrorVector( mirrorPosition[2] , position , facePlane[2] );
	MathHelper::MirrorVector( mirrorPosition[3] , position , facePlane[3] );
	
	std::cout << "原點：" << position[0] << "," << position[1] << "," << position[2] << std::endl;
	std::cout << "第0面的勁像點：" << (mirrorPosition[0])[0] << "," << (mirrorPosition[0])[1] << "," << (mirrorPosition[0])[2] << std::endl;
	std::cout << "第1面的勁像點：" << (mirrorPosition[1])[0] << "," << (mirrorPosition[1])[1] << "," << (mirrorPosition[1])[2] << std::endl;
	std::cout << "第2面的勁像點：" << (mirrorPosition[2])[0] << "," << (mirrorPosition[2])[1] << "," << (mirrorPosition[2])[2] << std::endl;
	std::cout << "第3面的勁像點：" << (mirrorPosition[3])[0] << "," << (mirrorPosition[3])[1] << "," << (mirrorPosition[3])[2] << std::endl;
	*/
	//===============================前製作業完成//===============================
	//第0面：0,1,2
	//第1面：1,2,3
	//第2面：0,1,2
	//第3面：0,1,3

	bool hasAnotherNeighbor = false;
	for( Cone* i = firstCone; i != NULL; i = i -> link )
	{
		if( i -> corner[0] -> CloselyCompare( corner[0] ) && 
			i -> corner[2] -> CloselyCompare( corner[3] ) && 
			i -> corner[3] -> CloselyCompare( corner[2] ) )
		{
			//對方第0面->與第0面相鄰
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
			//對方第1面->與第0面相鄰
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
			//對方第0面->與第1面相鄰
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
			//對方第1面->與第1面相鄰
			neighborCone[1] = i;
			i -> neighborCone[1] = this;
			facePropagated[1] = true;
			i -> facePropagated[1] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[1] ) && //debug0207:把順序從(0->0 ,1->1 ,2->3)
			i -> corner[1] -> CloselyCompare( corner[0] ) && //改成(0->1 ,1->0 ,2->2)
			i -> corner[2] -> CloselyCompare( corner[2] ) )
		{
			//對方第2面->與第2面相鄰
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
			//對方第3面->與第2面相鄰
			neighborCone[2] = i;
			i -> neighborCone[3] = this;
			facePropagated[2] = true;
			i -> facePropagated[3] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[0] ) && //debug0207:把順序從(0->0 ,1->1 ,2->3)
			i -> corner[1] -> CloselyCompare( corner[1] ) && //改成(0->1 ,1->0 ,2->2)
			i -> corner[2] -> CloselyCompare( corner[3] ) )
		{
			//對方第2面->與第3面相鄰
			neighborCone[3] = i;
			i -> neighborCone[2] = this;
			facePropagated[3] = true;
			i -> facePropagated[2] = true;
			hasAnotherNeighbor = true;
		}
		if( i -> corner[0] -> CloselyCompare( corner[1] ) && //debug0207:把順序從(0->0 ,1->1 ,2->3)
			i -> corner[1] -> CloselyCompare( corner[0] ) && //改成(0->1 ,1->0 ,2->2)
			i -> corner[3] -> CloselyCompare( corner[3] ) )
		{
			//對方第3面->與第3面相鄰
			neighborCone[3] = i;
			i -> neighborCone[3] = this;
			facePropagated[3] = true;
			i -> facePropagated[3] = true;
			hasAnotherNeighbor = true;
		}
		/*debug0208尋找眉對到的相鄰三角錐
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
			std::cout << "debug:找到新的搭配" << std::endl;
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
	//std::cout << "第一個點是：" << point1[0] << "," << point1[1] << "," << point1[2] << std::endl;
	
	Point* d = new Point();

	bool egdeEngaged[6] = { false , false , false , false , false , false };

	bool isIntersected = false;

	Point* e1 = new Point();
	Point* e2 = new Point();
	e1 -> Vector( point1 , point2 );
	e2 -> Vector( point1 , point3 );

	//這邊是? 找出是哪個點和哪個點的連線
	for( int i = 0 ; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			int engagedEdge = -1;
			//找出這是哪一邊 有4種type 4個點 6個邊
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
			
			//如果這個是邊 距離會等於1 就開始算香蕉
			//if( MathHelper::VectorDistance( corner[i] -> position , corner[j] -> position ) == zomeDirection.GetColorLength( COLOR_BLUE , SIZE_S ) && 
			//	!egdeEngaged[ engagedEdge ] )
			if( engagedEdge >= 0 && !egdeEngaged[ engagedEdge ] )
				//這行是說 如果i到j是一個邊 他的engagedEdge就會大於等於0
				//如果這個編號的邊已經有算過有沒有相交了 就不用再進來算一次
			{//這邊出錯 還沒設定 corner
				d -> Vector( corner[i] , corner[j] );
				/*
				std::cout << "這個方塊的位置是" << position[0] << "," << position[1] << "," << position[2] << std::endl;
				std::cout << "角0的位置是" << corner[0] -> position[0] << "," << corner[0] -> position[1] << "," << corner[0] -> position[2] << std::endl;
				std::cout << "角1的位置是" << corner[1] -> position[0] << "," << corner[1] -> position[1] << "," << corner[1] -> position[2] << std::endl;
				std::cout << "角2的位置是" << corner[2] -> position[0] << "," << corner[2] -> position[1] << "," << corner[2] -> position[2] << std::endl;
				std::cout << "角3的位置是" << corner[3] -> position[0] << "," << corner[3] -> position[1] << "," << corner[3] -> position[2] << std::endl;
				std::cout << "角4的位置是" << corner[4] -> position[0] << "," << corner[4] -> position[1] << "," << corner[4] -> position[2] << std::endl;
				std::cout << "角5的位置是" << corner[5] -> position[0] << "," << corner[5] -> position[1] << "," << corner[5] -> position[2] << std::endl;
				std::cout << "角6的位置是" << corner[6] -> position[0] << "," << corner[6] -> position[1] << "," << corner[6] -> position[2] << std::endl;
				std::cout << "角7的位置是" << corner[7] -> position[0] << "," << corner[7] -> position[1] << "," << corner[7] -> position[2] << std::endl;
				*/
				Point* s1 = new Point();
				s1 -> Cross( d , e2 );

				GLfloat divisor = s1 -> Dot( e1 );
   
				if (divisor == 0.)
				{
					//不相交
					delete s1;
					continue;
				}

				// Compute first barycentric coordinate

				Point* s = new Point();
				s -> Vector( point1 , corner[i] -> position );
				GLfloat b1 = s -> Dot( s1 ) / divisor;
				if( b1 < 0. || b1 > 1.)
				{
					//不相交
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
					//不相交
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
					//不相交
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

	//看哪幾個面有相交
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

