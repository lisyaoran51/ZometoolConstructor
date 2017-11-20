#include "StructureAnalyzer.h"



StructureAnalyzer::StructureAnalyzer( Constructor* constructor )
{
	cons = constructor;
	zD = constructor -> zomeDirection;
}

bool StructureAnalyzer::FindLowestHeight()
{

	lowestEndHeight = 1000.0;
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex == -1 )
		{
			if( ( i -> endPosition )[1] < lowestEndHeight )
			{
				lowestEndHeight = ( i -> endPosition )[1];
			}
		}
	}
	std::cout << "�̧C�����׳]��" << lowestEndHeight << std::endl;
	//��̧C���X�Ӧa��ХX��
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex == -1 )
		{
			if( ( i -> endPosition )[1] < lowestEndHeight + BOTTOM_INTERVAL && i -> towardIndex == -1 )
			{
				Bottom.push_back( i -> index );
				std::cout << "��" << i -> index << "�ڬ�l���ݬ����a�A�y�Ь�(" 
						  <<  ( i -> endPosition )[0] << "," 
						  <<  ( i -> endPosition )[1] << "," 
						  <<  ( i -> endPosition )[2] << ")" << std::endl;
			}
		}
	}

	return true;
}

bool StructureAnalyzer::SetVirtualBalls()
{
	realBCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
		realBCount++;



	//�إ߳s�������I���q
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex != -1 )continue;
		bool newVirtualBall = true;
		for(int j = 0; j < virtualBalls.size(); j++ )
		{
			VirtualBall* tempVirtualBall = virtualBalls.at( j );
			if( MathHelper::VectorApproximation( tempVirtualBall -> position ,
												 i -> endPosition ) )//�p�G�o�ڴΤl��s�������I��m�c�P�A�N��o�a�i�h
			{
				newVirtualBall = false;
				//tempVirtualBall -> AddConn( i );
				tempVirtualBall -> connectionAmount++;
				if( tempVirtualBall -> connectionAmount > 5 )
				{
					
					std::cout << "Constructor::StructureAnalyze()�X��:�s�������I�ұ�����l�W�L6��" << std::endl;
					std::cout << "�h�X�Ӫ���l�s���O�G" << i -> index << "�A���O�G" << zD -> GetOppositeFace( i -> fromFace ) << std::endl;
					errorBalls.push_back( i -> index );
					tempVirtualBall -> connectionAmount--;
					std::cout << "�����y�y�СG(" << tempVirtualBall -> position[0] << "," 
												 << tempVirtualBall -> position[1] << "," 
												 << tempVirtualBall -> position[2] << ")" << std::endl;
					continue;
				} 
				int towardFace = zD -> GetOppositeFace( i -> fromFace );
				tempVirtualBall -> connectFace[ tempVirtualBall -> connectionAmount ] = towardFace;//��J�P��l�s�������s��
				tempVirtualBall -> connIndex[ tempVirtualBall -> connectionAmount ] = i -> index;//��J��l���s��
				tempVirtualBall -> conn[ tempVirtualBall -> connectionAmount ] = i;//��J��l������
				std::cout << "�b��" << j << "�ӵ����y(" << tempVirtualBall -> position[0] << "," 
						  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")���[�J��" << i -> index << "����l�A�s�����O" << towardFace << "�A�ثe�@" 
						  << tempVirtualBall -> connectionAmount << "�ڬ�l" << std::endl;
				bool debug = 0;
			}
		}
		if( newVirtualBall )
		{
			VirtualBall* tempVirtualBall = new VirtualBall( realBCount + virtualBalls.size() , i );//�S���o���I���ܴN�s�W�@���I
			int towardFace = zD -> GetOppositeFace( i -> fromFace );
			tempVirtualBall -> connectFace[ 0 ] = towardFace;
			if( i -> endPosition[1] < lowestEndHeight + BOTTOM_INTERVAL )
			{
				tempVirtualBall -> isBottom = true;
			}
			virtualBalls.push_back( tempVirtualBall );
			std::cout << "�s�W��" << virtualBalls.size() << "�ӵ����y(" << tempVirtualBall -> position[0] << ","
					  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")�A�[�J��" 
					  << i -> index << "����l�A�s�����O" << towardFace << "�A�ثe�@" 
					  << tempVirtualBall -> connectionAmount << "�ڬ�l" << std::endl;
			
		}
	}
	return true;
}

bool StructureAnalyzer::SDSetVirtualBalls()
{
	realBCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
		realBCount++;



	//�إ߳s�������I���q
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		if( i -> towardIndex != -1 )continue;
		bool newVirtualBall = true;
		for(int j = 0; j < virtualBalls.size(); j++ )
		{
			VirtualBall* tempVirtualBall = virtualBalls.at( j );
			if( MathHelper::VectorApproximation( tempVirtualBall -> position ,
												 i -> endPosition ) )//�p�G�o�ڴΤl��s�������I��m�c�P�A�N��o�a�i�h
			{
				newVirtualBall = false;
				//tempVirtualBall -> AddConn( i );
				bool dubConn = false;
				for( int c = 0; c <= tempVirtualBall -> connectionAmount; c++ )
				{
					GLfloat* connPos = tempVirtualBall -> conn[c] -> position;
					if( MathHelper::VectorApproximation( i -> position , connPos ) )
					{
						dubConn = true;
						std::cout << "���Ƭ�l�s��" << i -> index << std::endl;
					}
				}
				if(dubConn) continue;

				tempVirtualBall -> connectionAmount++;
				if( tempVirtualBall -> connectionAmount > 5 )
				{
					
					std::cout << "Constructor::StructureAnalyze()�X��:�s�������I�ұ�����l�W�L6��" << std::endl;
					std::cout << "�h�X�Ӫ���l�s���O�G" << i -> index << "�A���O�G" << zD -> GetOppositeFace( i -> fromFace ) << std::endl;
					errorBalls.push_back( i -> index );
					tempVirtualBall -> connectionAmount--;
					std::cout << "�����y�y�СG(" << tempVirtualBall -> position[0] << "," 
												 << tempVirtualBall -> position[1] << "," 
												 << tempVirtualBall -> position[2] << ")" << std::endl;
					continue;
				} 
				int towardFace = zD -> GetOppositeFace( i -> fromFace );
				tempVirtualBall -> connectFace[ tempVirtualBall -> connectionAmount ] = towardFace;//��J�P��l�s�������s��
				tempVirtualBall -> connIndex[ tempVirtualBall -> connectionAmount ] = i -> index;//��J��l���s��
				tempVirtualBall -> conn[ tempVirtualBall -> connectionAmount ] = i;//��J��l������
				std::cout << "�b��" << j << "�ӵ����y(" << tempVirtualBall -> position[0] << "," 
						  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")���[�J��" << i -> index << "����l�A�s�����O" << towardFace << "�A�ثe�@" 
						  << tempVirtualBall -> connectionAmount << "�ڬ�l" << std::endl;
				bool debug = 0;
			}
		}
		if( newVirtualBall )
		{
			VirtualBall* tempVirtualBall = new VirtualBall( realBCount + virtualBalls.size() , i );//�S���o���I���ܴN�s�W�@���I
			int towardFace = zD -> GetOppositeFace( i -> fromFace );
			tempVirtualBall -> connectFace[ 0 ] = towardFace;
			if( i -> endPosition[1] < lowestEndHeight + BOTTOM_INTERVAL )
			{
				tempVirtualBall -> isBottom = true;
			}
			virtualBalls.push_back( tempVirtualBall );
			std::cout << "�s�W��" << virtualBalls.size() << "�ӵ����y(" << tempVirtualBall -> position[0] << ","
					  << tempVirtualBall -> position[1] << "," << tempVirtualBall -> position[2] << ")�A�[�J��" 
					  << i -> index << "����l�A�s�����O" << towardFace << "�A�ثe�@" 
					  << tempVirtualBall -> connectionAmount << "�ڬ�l" << std::endl;
			
		}
	}
	return true;
}

bool StructureAnalyzer::SetMatrix()
{

//���`�@���X���y
	realBCount = 0;
	bCount = 0;
	cCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
		realBCount++;
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		cCount++;
	}
	bCount = realBCount + virtualBalls.size();
//===============
	
	stiffMat.resize(bCount*3,bCount*3);		stiffMat.setZero();
	f.resize(bCount*3);						f		.setZero();					
	disp.resize(bCount*3);					disp	.setZero();
	innerF.resize(cCount);					innerF	.setZero();

	std::cout << "SetMatrix�G�x�}��" << bCount*3 << "x" << bCount*3 << "���C" << std::endl;

//�令��C�Ӭ�l�g�@�qMatrix
	//SetStiffness();
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		int fromB = i -> fromIndex;
		int toB = GetTowardBallIndex( i );
		float s = GetStiffness( i );
//rotation���q�ӭ쭱���T�b���q
		float res[3] = { i -> rotation[0] ,
						 i -> rotation[1] ,
						 i -> rotation[2] };

		SM tempM( bCount*3 , bCount*3 );
		VT tempVT;

/*
�l�ׯx�}�G
					from		 to
				  �I1�첾	   �I2�첾
				   / |  \      /  | \
			|    xx	 xy	 xz	-xx	-xy	-xz	|   \
			|	 xy	 yy	 yz	-xy	-yy	-yz |   - �I�@�I�O from
			|	 xz	 yz	 zz	-xz	-yz	-zz |   /
			|	-xx	-xy	-xz	 xx	 xy	 xz	|	\
			|  	-xy	-yy	-yz	 xy	 yy	 yz	|	- �I2�v�O   to
			|  	-xz	-yz	-zz	 xz	 yz	 zz	|	/
*/
		//float EAdivL = YOUNG_MODULUS * 10.0 / zD -> GetFaceLength( i -> fromFace , i -> size ); //10.0�O�I���n 
//(�ĴX��C,�ĴX����)
		for( int j = 0; j < 3; j++ )
		{
			for( int k = 0; k < 3; k++ )
			{
//��j�C��k�� ���W  (from, from)
				tempVT.push_back( T( fromB*3+j , fromB*3+k ,  res[j]*res[k] * s ) );
//��j�C��k�� �k�W  (from, to)
				tempVT.push_back( T( fromB*3+j , toB*3+k   , -res[j]*res[k] * s ) );
//��j�C��k�� ���U  (to, from)
				tempVT.push_back( T( toB*3+j   , fromB*3+k , -res[j]*res[k] * s ) );
//��j�C��k�� �k�U  (to, to)
				tempVT.push_back( T( toB*3+j   , toB*3+k   ,  res[j]*res[k] * s ) );
			}
		}
//�N�C�@�ӫl�ץ[��}���x�}�A�b�N�}���x�}�֥[
		tempM.setFromTriplets( tempVT.begin() , tempVT.end() );
		stiffMat += tempM;

//�N��l�����q�[�Jforce��
		f[ fromB*3 + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f; 
		f[ toB*3   + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f;	// �p�G�o���I�ݩ󩳳��A�᭱�|�N�L�����O�k�s

		totalWeight -= zD -> GetWeight( i -> color ,i -> size );				// �`���q
	}


//�]�w�첾�M���O
	//SetOuterForce();
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
	{
		f[ b->index*3 + 1 ] -= zD -> GetWeight( WEIGHT_WHITE , 0 ) ;
		totalWeight -= zD -> GetWeight( WEIGHT_WHITE , 0 );			// �`���q
	}
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( !virtualBalls[i] -> isBottom )
		{	
			f[ virtualBalls[i]->index*3 + 1 ] -= SEGMENT_WEIGHT ;	// ���b�������H���A�[�W���q
			totalWeight -= SEGMENT_WEIGHT;							// �`���q
		}
		else
		{
			totalWeight -= f[ virtualBalls[i]->index*3 + 1 ];	// �⩳�����H�������q����
			f[ virtualBalls[i]->index*3 + 1 ] = 0;
		}
	}

//�]�w�w�����O�M�첾
	//SetIfKnown();
	knF = new bool[ bCount*3 ];		for(int i=0;i<bCount*3;i++)knF[i]=true;    //�j�h�~�O���w��
	knDisp = new bool[ bCount*3 ];	for(int i=0;i<bCount*3;i++)knDisp[i]=false;//�j�h�첾������
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{	
			knF[ realBCount*3 + i*3 + 1 ] = false;		//������y��V�~�O����
			knDisp[ realBCount*3 + i*3 + 1 ] = true;	//������y��V�첾�w��
		}
	}
//���Ĥ@�ө���VirtualBall��@�T�w�I
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{
			knF[ realBCount*3 + i*3 ] = false;		// �����T�w�I���~�O����
			knF[ realBCount*3 + i*3 + 1 ] = false;
			knF[ realBCount*3 + i*3 + 2 ] = false;
			knDisp[ realBCount*3 + i*3 ] = true;		// �����T�w�I���첾�w��
			knDisp[ realBCount*3 + i*3 + 1 ] = true;
			knDisp[ realBCount*3 + i*3 + 2 ] = true;
			break;
		}
	}
	knFAmount = 0;
	for( int i = 0; i < bCount*3; i++ )
	{
		if( !knF[i] ) knFAmount++;
	}


	return true;
}

bool StructureAnalyzer::SDSetMatrix()
{

//���`�@���X���y
	realBCount = 0;
	bCount = 0;
	cCount = 0;
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
	{
		realBCount++;
	
	}
		
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		cCount++;
	}
	bCount = realBCount + virtualBalls.size();
//===============
	
	stiffMat.resize(bCount*3,bCount*3);		stiffMat.setZero();
	f.resize(bCount*3);						f		.setZero();					
	disp.resize(bCount*3);					disp	.setZero();
	innerF.resize(cCount);					innerF	.setZero();

	std::cout << "SetMatrix�G�x�}��" << bCount*3 << "x" << bCount*3 << "���C" << std::endl;

//�令��C�Ӭ�l�g�@�qMatrix
	//SetStiffness();
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{

		int fromB = i -> fromIndex;
		int toB = GetTowardBallIndex( i );
		float s = GetStiffness( i );
		
//rotation���q�ӭ쭱���T�b���q
		float res[3] = {0,0,0};
		zD -> GetFaceDirection( i -> fromFace , res );

		SM tempM( bCount*3 , bCount*3 );
		VT tempVT;

/*
�l�ׯx�}�G
					from		 to
				  �I1�첾	   �I2�첾
				   / |  \      /  | \
			|    xx	 xy	 xz	-xx	-xy	-xz	|   \
			|	 xy	 yy	 yz	-xy	-yy	-yz |   - �I�@�I�O from
			|	 xz	 yz	 zz	-xz	-yz	-zz |   /
			|	-xx	-xy	-xz	 xx	 xy	 xz	|	\
			|  	-xy	-yy	-yz	 xy	 yy	 yz	|	- �I2�v�O   to
			|  	-xz	-yz	-zz	 xz	 yz	 zz	|	/
*/
		//float EAdivL = YOUNG_MODULUS * 10.0 / zD -> GetFaceLength( i -> fromFace , i -> size ); //10.0�O�I���n 
//(�ĴX��C,�ĴX����)
		for( int j = 0; j < 3; j++ )
		{
			for( int k = 0; k < 3; k++ )
			{
//��j�C��k�� ���W  (from, from)
				tempVT.push_back( T( fromB*3+j , fromB*3+k ,  res[j]*res[k] * s ) );
//��j�C��k�� �k�W  (from, to)
				tempVT.push_back( T( fromB*3+j , toB*3+k   , -res[j]*res[k] * s ) );
//��j�C��k�� ���U  (to, from)
				tempVT.push_back( T( toB*3+j   , fromB*3+k , -res[j]*res[k] * s ) );
//��j�C��k�� �k�U  (to, to)
				tempVT.push_back( T( toB*3+j   , toB*3+k   ,  res[j]*res[k] * s ) );
			}
		}
//�N�C�@�ӫl�ץ[��}���x�}�A�b�N�}���x�}�֥[
		tempM.setFromTriplets( tempVT.begin() , tempVT.end() );
		stiffMat += tempM;

//�N��l�����q�[�Jforce��
		f[ fromB*3 + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f; 
		f[ toB*3   + 1 ] -= zD -> GetWeight( i -> color ,i -> size ) * 0.5f;	// �p�G�o���I�ݩ󩳳��A�᭱�|�N�L�����O�k�s

		totalWeight -= zD -> GetWeight( i -> color ,i -> size );				// �`���q
	}


//�]�w�첾�M���O
	//SetOuterForce();
	for( ZomeStructure* b = cons -> originalBall; b != NULL; b = b -> link )
	{
		f[ b->index*3 + 1 ] -= zD -> GetWeight( WEIGHT_WHITE , 0 ) ;
		totalWeight -= zD -> GetWeight( WEIGHT_WHITE , 0 );			// �`���q

		if( b -> index == 12 || b -> index == 16 )
		{
			f[ b->index*3 + 1 ] -= 500.0;
			totalWeight -= 500.0;
		}
	}
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( !virtualBalls[i] -> isBottom )
		{	
			f[ virtualBalls[i]->index*3 + 1 ] -= SEGMENT_WEIGHT ;	// ���b�������H���A�[�W���q
			totalWeight -= SEGMENT_WEIGHT;							// �`���q
		}
		else
		{
			totalWeight -= f[ virtualBalls[i]->index*3 + 1 ];	// �⩳�����H�������q����
			f[ virtualBalls[i]->index*3 + 1 ] = 0;
		}
	}

//�]�w�w�����O�M�첾
	//SetIfKnown();
	knF = new bool[ bCount*3 ];		for(int i=0;i<bCount*3;i++)knF[i]=true;    //�j�h�~�O���w��
	knDisp = new bool[ bCount*3 ];	for(int i=0;i<bCount*3;i++)knDisp[i]=false;//�j�h�첾������
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{	
			knF[ realBCount*3 + i*3 + 1 ] = false;		//������y��V�~�O����
			knDisp[ realBCount*3 + i*3 + 1 ] = true;	//������y��V�첾�w��
		}
	}

	// SD
	//knF[ 0 ] = false;		//������y��V�~�O����
	//knF[ 1 ] = false;		//������y��V�~�O����
	//knF[ 2 ] = false;		//������y��V�~�O����
	//knF[ 4 ] = false;		//������y��V�~�O����
	//knF[ 7 ] = false;		//������y��V�~�O����
	//knDisp[ 0 ] = true;		//������y��V�첾�w��
	//knDisp[ 1 ] = true;		//������y��V�첾�w��
	//knDisp[ 2 ] = true;		//������y��V�첾�w��
	//knDisp[ 4 ] = true;		//������y��V�첾�w��
	//knDisp[ 7 ] = true;		//������y��V�첾�w��
	//f[ 1 ] = 0;
	//f[ 4 ] = 0;
	//f[ 7 ] = 0;
	totalWeight = f[ 10 ];	// �u�p�⳻�y



//���Ĥ@�ө���VirtualBall��@�T�w�I
	for( int i = 0; i < virtualBalls.size(); i++ )
	{
		if( virtualBalls[i] -> isBottom )
		{
			knF[ realBCount*3 + i*3 ] = false;		// �����T�w�I���~�O����
			knF[ realBCount*3 + i*3 + 1 ] = false;
			knF[ realBCount*3 + i*3 + 2 ] = false;
			knDisp[ realBCount*3 + i*3 ] = true;		// �����T�w�I���첾�w��
			knDisp[ realBCount*3 + i*3 + 1 ] = true;
			knDisp[ realBCount*3 + i*3 + 2 ] = true;
			break;
		}
	}
	knFAmount = 0;
	for( int i = 0; i < bCount*3; i++ )
	{
		if( !knF[i] ) knFAmount++;
	}


	return true;
}

;	int StructureAnalyzer::GetTowardBallIndex( ZomeConnection* c )
{
	if( c -> towardIndex == -1 )
	{
		for( int i = 0; i < virtualBalls.size(); i++ )
		{
			if( virtualBalls[i] -> conn[0] == c ||
				virtualBalls[i] -> conn[1] == c ||
				virtualBalls[i] -> conn[2] == c ||
				virtualBalls[i] -> conn[3] == c )
			{
				return virtualBalls[i] -> index;
			}
		}
		std::cout << "int StructureAnalyzer::GetTowardBallIndex�X���G�S�����virtualBall" << std::endl;
	}
	return c -> towardIndex;
}

;	float StructureAnalyzer::GetStiffness( ZomeConnection* c )
{
	// E * A / L
	//return 1. / zD -> GetFaceLength( c -> fromFace , c -> size ) * SCALE;
	return YOUNG_MODULUS * 10.0 / zD -> GetFaceLength( c -> fromFace , c -> size );
}

bool StructureAnalyzer::SolveMatrix()
{


//		k =
//		|	K(�w���~�O�A�����첾)	|	K(�w���~�O�A�w���첾)	|	| �����첾	| =	|�w���~�O|
//		|	K(�����~�O�A�����첾)	|	K(�����~�O�A�w���첾)	|	|	  0		| 	|�����~�O|
//
//		����U����ӧR���A�A��k�W�R���A�A�Ѥ�{��

	//DebugPrintStiffness( stiffMat , f );

//���R�h���Ϊ��C(�ӦC�����O�����A�]���S���ܧ�)
	//DeleteUnknownForce();
	/*
	SM tempM = *stiffMat;
	V tempF = *f;
	for( int i = 0; i < bCount * 3; i++ )
	{
		if( !knF[i] )//���O�����A�ҥH�ܧΤw��(�L�ܧ�)�A�¤��ݭn�p���ܧ�
		{
			tempM.row(i) = tempM.row(i-1);
			tempF[i] = tempF[i-1];
		}
	}
//	�n��w���첾��0�������A�t���k�s
	for( int j = 0; j < disp -> size(); j++ )
	{
		if( knDisp[j] )
		{
			for( int i = 0; i < f -> size(); i++ )
			{
				tempM.coeffRef(i,j) = 0;
			}
		}
	}
	*/
	//ExtractKnownFUnknownDisp();
	Eigen::SparseMatrix<double> tempM( stiffMat.rows()-knFAmount , stiffMat.rows()-knFAmount );
	tempM.makeCompressed();
	std::vector< Eigen::Triplet<double> > tempMCoef;
	Eigen::VectorXd tempF( stiffMat.rows()-knFAmount );
	Eigen::VectorXd tempDisp;
	int ii=0;
	for( int i = 0; i < stiffMat.rows(); i++ )
	{
		if( !knF[i] ) continue;
		int jj = 0;
		for( int j = 0; j < stiffMat.cols(); j++ )
		{
			if( !knF[j] ) continue;
			if( stiffMat.coeffRef(i,j) != 0 )
			{
				tempMCoef.push_back( Eigen::Triplet<double>( ii,jj,stiffMat.coeffRef(i,j) ) );
			}
			jj++;
		}
		tempF[ii] = f[i];
		ii++;
	}
	std::cout << "�~�O�G" << tempF;
	tempM.setFromTriplets(tempMCoef.begin(),tempMCoef.end());
	DebugPrintStiffness( &tempM , &tempF );


// �ۤv�g�@��LU
	//StiffMatrixLU();
	SparseLU<Eigen::SparseMatrix<double> > sparseLU(tempM);
	sparseLU.PrintPLU();
	
	
	tempDisp = sparseLU.Solve(tempF);


	std::cout << "�첾�G" << tempDisp;

	//PutBackDisp();
	ii=0;
	for( int i = 0; i < disp.size(); i++ )
	{
		if( !knF[i] ) continue;
		disp[i] = tempDisp[ii];
		ii++;
	}


	f = stiffMat * disp;

	   
	std::cout << "�첾�G" << disp;
	std::cout << "�~�O�G" << f;

	float debugWeight = 0;
	for( int i = 0; i < f.size(); i++ )
	{
		if( !knF[i] )
		{
			debugWeight += f[i];
		}
	}
	std::cout << "�`���q��" << totalWeight << "g�A�ϤO���`�q�O" << debugWeight << "g" << std::endl;


	return true;
}

;	bool StructureAnalyzer::DebugPrintStiffness( Eigen::SparseMatrix<double>* tempM , Eigen::VectorXd* v )
	{
		FILE* formulaFile = fopen("DebugStiffness.txt", "w");
		for( int i = 0; i < tempM -> rows(); i++ )
		{
			for( int j = 0; j < tempM -> cols(); j++ )
			{
				fprintf(formulaFile, "%6.6f ", tempM -> coeffRef(i,j) );
			}
			fprintf(formulaFile, "\n");
		}

		for( int i = 0; i < v -> size(); i++ )
		{
			fprintf(formulaFile, "%6.3f\n", (*v)(i) );
		}

		fclose( formulaFile );
		return true;


	}

bool StructureAnalyzer::SolveInnerForce()
{
	int debugCount = 0;
	//���C�Ӭ�l�����
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{
		//std::cout << "debugCount" << debugCount << std::endl;


// ���L���T�b����
		Eigen::RowVectorXf disp6(6);	// (��x,��y,��z,��x,��y,��z)
		int from = i -> fromIndex;		//
		int to = GetTowardBallIndex(i);		//

		std::cout << "��" << i -> index		<< "�ڬ�l�Y�첾��(" 
				  << disp[ from*3     ]		<< "," 
				  << disp[ from*3 + 1 ]		<< ","
				  << disp[ from*3 + 2 ]		<< "),���첾��("
				  << disp[ to*3       ]		<< ","
				  << disp[ to*3 + 1   ]		<< ","
				  << disp[ to*3 + 2   ]		<< ")" << std::endl;

		disp6 << disp[ from*3 ] , disp[ from*3 + 1 ] , disp[ from*3 + 2 ] ,
				 disp[ to*3   ] , disp[ to*3 + 1   ] , disp[ to*3 + 2   ];

// ��X�L��k'T
		/*     
		p' = k'T u (��l���O = k'T * ��l����)
		k'T =	|	x	y	z	-x	-y	-z	|
				|	-x	-y	-z	x	y	z	|
		�u���Ĥ@�C
		*/
		float res[3] = { i -> rotation[0] ,
						 i -> rotation[1] ,
						 i -> rotation[2] };

		Eigen::RowVectorXf localK_T(6);
		localK_T << res[0] , res[1] , res[2] , 
					-res[0] , -res[1] , -res[2];



//��X���O
		// p' = k'T u (��l���O = k'T * ��l����)
		innerF[ i -> index ] = disp6.dot( localK_T ) * GetStiffness(i);

		std::cout << "��" << i -> index << "�ڬ�l�����O��" << innerF[ i -> index ] << "g" << std::endl;
		debugCount++;
	}

	return true;
}

bool StructureAnalyzer::SDSolveInnerForce()
{
	int debugCount = 0;
	//���C�Ӭ�l�����
	for( ZomeConnection* i = cons -> originalConn; i != NULL; i = i -> link )
	{

// ���L���T�b����
		Eigen::RowVectorXd disp6(6);	// (��x,��y,��z,��x,��y,��z)
		int from = i -> fromIndex;		//
		int to = GetTowardBallIndex(i);		//

		std::cout << "��" << i -> index		<< "�ڬ�l�Y�첾��(" 
				  << disp[ from*3     ]		<< "," 
				  << disp[ from*3 + 1 ]		<< ","
				  << disp[ from*3 + 2 ]		<< "),���첾��("
				  << disp[ to*3       ]		<< ","
				  << disp[ to*3 + 1   ]		<< ","
				  << disp[ to*3 + 2   ]		<< ")" << std::endl;

		disp6 << disp[ from*3 ] , disp[ from*3 + 1 ] , disp[ from*3 + 2 ] ,
				 disp[ to*3   ] , disp[ to*3 + 1   ] , disp[ to*3 + 2   ];

// ��X�L��k'T
		/*     
		p' = k'T u (��l���O = k'T * ��l����)
		k'T =	|	x	y	z	-x	-y	-z	|
				|	-x	-y	-z	x	y	z	|
		�u���Ĥ@�C
		*/
		float res[3] = {0,0,0};
		zD -> GetFaceDirection( i -> fromFace , res );

		Eigen::RowVectorXd localK_T(6);
		localK_T << res[0] , res[1] , res[2] , 
					-res[0] , -res[1] , -res[2];



//��X���O
		// p' = k'T u (��l���O = k'T * ��l����)
		innerF[ i -> index ] = disp6.dot( localK_T ) * GetStiffness(i);

		std::cout << "��" << i -> index << "�ڬ�l�����O��" << innerF[ i -> index ] << "g" << std::endl;
		debugCount++;
	}

	return true;
}

bool StructureAnalyzer::PrintAnalysis()
{
	//debug��X���G
	//PrintPolynomialList();
	//debug
	FILE* formulaFile = fopen("StructureFormulaSolved.txt", "w");
	fprintf(formulaFile,"�`�I�첾\n");
	for( int i = 0; i < disp.size(); i++ )
	{
		fprintf(formulaFile, "%d : %12.12lf \n", i , disp[i] );
	}

	fprintf(formulaFile,"�`�I���O(���)\n");
	for( int i = 0; i < disp.size(); i++ )
	{
		fprintf(formulaFile, "%d : %12.12lf \n", i , f[i] );
	}

	fprintf(formulaFile,"��l���O\n");
	for( int i = 0; i < innerF.size(); i++ )
	{
		fprintf(formulaFile, "%d : %12.12lf g\n", i , innerF[i] );
	}

	
	fclose( formulaFile );
	return true;
}

bool StructureAnalyzer::MapToMaterial()
{

	for( int i = 0; i < innerF.size(); i++ )
	{
		
		for( std:: map<int,int>::iterator it = cons -> matIndex2Index -> begin(); 
					 it != cons -> matIndex2Index -> end();
					 it++ )
		{
			if( i == it -> second/*(obj�ɸ̭������ƽs��,ZomeConnetion�s��)*/ )
			{
				matForce.insert( std::pair< int , float >( it -> first , innerF[i] * FORCE_SCALE ) );//(obj�ɸ̭������ƽs��,���O�j�p)
				//�N�o�ڱ�󪺨��O�A�s�W���趶�Ǫ��s���C

				std::cout << "�N��" << i << "�ڬ�l���L�b����W�����ǽs����" 
							<< it -> first << "�A�M���J����O��" << innerF[i] << "g" << std::endl;
			}
		}
	}


	//	matIndex2Index (obj�ɸ̭������ƽs��,ZomeConnetion�s��)
	//	matForce	   (obj�ɸ̭������ƽs��,���O�j�p)

// �o��M��S���s�����O����l�A�ç�L�����O�אּ999
	for( ZomeConnection* z = cons -> originalConn; z != NULL; z = z -> link )
	{
		bool existInMatForce = false;
		//����X�o�@�Ӭ�l�bobj�������ƽs��
		int faceIndex = -1;
		for( std:: map<int,int>::iterator it = cons -> matIndex2Index -> begin(); 
			it != cons -> matIndex2Index -> end();
			it++ )/*(obj�ɸ̭������ƽs��,ZomeConnetion�s��)*/ 
		{
			if( it -> second == z -> index )
			{
				faceIndex = it -> first;
				break;
			}
		}
		if( faceIndex == -1 )
		{
			std::cout << "ZomeGenerator::ZomeStructureAnalyze()�X���G�S�����o�Ӭ�l�bobj�������ƽs��" << std::endl;
		}
		//�b��X�o�ӭ��ƽs���O�_���bmatForce�̡A�x�s���O�j�p
		for( std:: map<int,float>::iterator it = matForce.begin(); 
			 it != matForce.end();
			 it++ )/*(obj�ɸ̭������ƽs��,���O�j�p)*/ 
		{
			if( it -> first == faceIndex )//��l�s��
			{
				existInMatForce = true;
			}
		}
		//�p�G�S���o�ӽs���A�N��ܺ�X�L�����O
		if( !existInMatForce )//
		{
			matForce.insert( std::pair< int , float >( faceIndex , 1000.0 ) );
			std::cout << "ZomeGenerator::ZomeStructureAnalyze()�X���G��" << z -> index << "���l�A�䭱�ƽs����" 
					  << faceIndex << "�A�S����X���O�j�p" << std::endl;
		}

	}
	return true;
}

bool StructureAnalyzer::Analyze()
{
	
	FindLowestHeight();
	
	//�إ߳s�������I���q
	SetVirtualBalls();
	
	//�}�l�C�h�����A��C�@��D�إߤ@�ӤT�Ӧ��l�A�C���I���O��x�By�Bz�U�ؤ@�Ӧ��l
	SetMatrix();
	
	//�ѥX�첾�ѥX�~�O
	SolveMatrix();

	//�Υ�����X���O
	SolveInnerForce();

	//��X��constructor
	PrintAnalysis();

	//�ഫ���~¾
	MapToMaterial();

	return true;
}

bool StructureAnalyzer::SDAnalyze()
{
	
	
	//�إ߳s�������I���q
	//SetVirtualBalls();
	SDSetVirtualBalls();

	//�}�l�C�h�����A��C�@��D�إߤ@�ӤT�Ӧ��l�A�C���I���O��x�By�Bz�U�ؤ@�Ӧ��l
	SDSetMatrix();
	
	//�ѥX�첾�ѥX�~�O
	SolveMatrix();

	//�Υ�����X���O
	SDSolveInnerForce();

	//��X��constructor
	PrintAnalysis();

	//�ഫ���~¾
	MapToMaterial();

	return true;
}