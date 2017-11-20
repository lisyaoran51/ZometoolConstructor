#include "Branch.h"



Branch::Branch()
{

}

Branch::Branch( GLfloat position[3] , int branchIndex , OuterCone* outerCone , GLfloat* fPoint )
{
	sourceOuterCone = outerCone;
	zomeDirection = outerCone -> zomeDirection;
	MathHelper::CopyVector( farPoint , fPoint );
	int face = GetBranchIndexFace( branchIndex );

	sourceCorner[0] = new Point( position );
	root = sourceCorner[0];
	sourceCorner[1] = new Point( &( zomeDirection -> faceDirection[ face * 3 ] ) );

	isEnd = false;

	sourceCorner[1] -> Multiply( zomeDirection -> GetFaceLength( face , SIZE_S ) );
	sourceCorner[1] -> Plus( position );
	connectFace[0] = face;
	connectFace[1] = zomeDirection -> GetOppositeFace( face );
	distanceToSurface[0] = 999.9;
	distanceToSurface[1] = 999.9;

	nearestPointOnSurface = new Point();

	distance = -1;
	onPath = false;
	firstBranch = this;
}

Branch::Branch( int branchIndex , Branch* lastBranch )
:Branch( lastBranch -> sourceCorner[1] -> position , 
		 branchIndex , 
		 lastBranch -> sourceOuterCone , 
		 lastBranch -> farPoint )
{
	firstBranch = lastBranch -> firstBranch;
	
	if( lastBranch -> neighborBranch[ branchIndex ][1] != NULL )
	{
		std::cout << "Branch::Branch�X���G�D�W�;�䪺���H���s����L���" << std::endl;
	}
	//�۳s���i�H�b�o�䰵�A���M����s�Q���|�X��
	//��W�@�Ӿ�K��o�ӳs�_��
	//lastBranch -> neighborBranch[ branchIndex ] = this;
	//��o�Ӿ�K��W�@�ӳs�_��
	//int face = GetBranchIndexFace( branchIndex );
	//int oppositeFace = zomeDirection -> GetOppositeFace(face);
	//neighborBranch[ GetFaceBranchIndex( oppositeFace ) ] = lastBranch;
}

Branch::~Branch()
{
	delete sourceCorner[0];
	delete sourceCorner[1];
}
//��branch�[�J�o�@���K��
bool Branch::AddToLink( Branch* branch )
{
	branch -> firstBranch = firstBranch;
	for( Branch* b = firstBranch; b != NULL; b = b -> link )
	{
		if( b -> link == NULL )
		{
			b -> link = branch;
			branch -> index = b -> index + 1;
			break;
		}
	}
	return true;
}

int Branch::GetFaceBranchIndex( int face )
{
	//30~41
	return face - 30;
}

int Branch::GetBranchIndexFace( int branchIndex )
{
	//30~41
	return branchIndex + 30;
}

int Branch::GetOppositeBranchIndex( int branchIndex )
{
	//30~41
	int face = branchIndex + 30;
	int oppoFace = zomeDirection -> GetOppositeFace( face );
	return GetFaceBranchIndex( oppoFace );
}

bool Branch::CheckIntersectWithConeFace()
{
	//����X�Y���ݦ�m
	
	GLfloat endPoint[3] , initPoint[3];
	MathHelper::VectorMinus( endPoint , sourceCorner[1] -> position , sourceCorner[0] -> position );
	MathHelper::VectorNormalize( endPoint );
	MathHelper::CopyVector( initPoint , endPoint );
	MathHelper::VectorPlus( initPoint , sourceCorner[0] -> position );
	MathHelper::VectorMutiply( endPoint , zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - 1.0 );
	MathHelper::VectorPlus( endPoint , sourceCorner[0] -> position );
	/*
	GLfloat endPoint[3] , initPoint[3];
	MathHelper::CopyVector( initPoint , sourceCorner[0] -> position );
	MathHelper::CopyVector( endPoint , sourceCorner[1] -> position );
	*/
	bool isIntersected = false;
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
	for( Cone* c = sourceOuterCone -> sourceCone -> firstCone; c != NULL; c = c -> link )
	{
		if( c -> activated )
		{
			//�p�G�Z���ӻ��N���L
			if( MathHelper::VectorDistance( c -> corner[0] -> position , sourceCorner[0] -> position ) > threshold ) 
				continue;
			//4�ӭ�
			for( int i = 0; i < 4; i++ )
			{
				if( MathHelper::CheckIntersectOnTriangle( initPoint ,
														  endPoint , 
														  c -> corner[ faceCorner[i][0] ] -> position ,
														  c -> corner[ faceCorner[i][1] ] -> position ,
														  c -> corner[ faceCorner[i][2] ] -> position ) )
				{
					isIntersected = true;
					std::cout << "�s�����|���J��LCone��" << std::endl;
					return true;
				}
			}
		}
	}
	return false;
}

bool Branch::CheckIntersectWithConnector()
{
	//����X�Y���ݦ�m
	GLfloat endPoint[3] , initPoint[3];
	MathHelper::VectorMinus( endPoint , sourceCorner[1] -> position , sourceCorner[0] -> position );
	MathHelper::VectorNormalize( endPoint );
	MathHelper::CopyVector( initPoint , endPoint );
	MathHelper::VectorPlus( initPoint , sourceCorner[0] -> position );
	MathHelper::VectorMutiply( endPoint ,  zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - NODE_DIAMETER / 2.0 + LATCH );
	MathHelper::VectorPlus( endPoint , sourceCorner[0] -> position );
	
	bool isIntersected = false;

	//�ݥL���S���M��L��cone����l���
	for( Cone* i = sourceOuterCone -> sourceCone -> firstCone; i != NULL; i = i -> link )
	{
		if( i -> activated )
		{
			//4���I
			for( int j = 0; j < 4 - 1; j++ )//�T���@���Y�@�I
			{
				//4���I
				for( int k = j + 1; k < 4; k++ )//�T���@���t�@�I �L�̳s�u
				{
					//��Ƥ��
					if( !i -> corner[j] -> CloselyCompare( sourceCorner[0] ) && 
						!i -> corner[k] -> CloselyCompare( sourceCorner[0] ) &&
						!i -> corner[j] -> CloselyCompare( sourceCorner[1] ) &&
						!i -> corner[k] -> CloselyCompare( sourceCorner[1] ) )
					{
						GLfloat distance = MathHelper::CheckTwoLineDistance( i -> corner[j] -> position , 
																			 i -> corner[k] -> position , 
																			 initPoint , 
																			 endPoint );
						if( distance < TRUSS_RADIUS + SOCKET_RADIUS )
						{
							isIntersected = true;
							std::cout << "�s���ƪK�P��LCone����l���" << std::endl;
							return true;
						}
					}
				}
			}
		}
	}

	//�ݥL���S�����L��outercone��l���
	for( OuterCone* i = sourceOuterCone -> firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 ) continue;
		//�O��cone�o3�ӥ~��[
		for( int j = 0; j < 3; j++ )
		{
			//��ۤv���Z��
			if( !i -> sourceCorner[j] -> CloselyCompare( sourceCorner[0] ) &&
				!i -> sourceCorner[j] -> CloselyCompare( sourceCorner[1] )  )
			{
				if( MathHelper::CheckTwoLineDistance( i -> sourceCorner[j] -> position , 
													  i -> cornerEnd[j] -> position , 
													  initPoint , endPoint ) < TRUSS_RADIUS * 2.0 )
				{
					std::cout << "�s���ƪK�P��LOuterCone����l���" << std::endl;
					isIntersected = true;
					return true;
				}
			}
			else if( MathHelper::CompareDirection( i -> sourceCorner[j] -> position ,
												   i -> cornerEnd[j] -> position ,
												   initPoint , 
												   endPoint ) )
			{
				std::cout << "�s���ƪK�P��LOuterCone����l���|" << std::endl;
				isIntersected = true;
				return true;
			}
		}
	}
	return false;
}

bool Branch::CheckIntersectWithBranch( Branch* fBranch )
{
	if( fBranch == NULL )
	{
		return false;
	}
	//����X�Y���ݦ�m
	GLfloat endPoint[3] , initPoint[3];
	MathHelper::VectorMinus( endPoint , sourceCorner[1] -> position , sourceCorner[0] -> position );
	MathHelper::VectorNormalize( endPoint );
	MathHelper::CopyVector( initPoint , endPoint );
	MathHelper::VectorPlus( initPoint , sourceCorner[0] -> position );
	MathHelper::VectorMutiply( endPoint ,  zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - 0.1 );
	MathHelper::VectorPlus( endPoint , sourceCorner[0] -> position );

	GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 1.0;
	bool isIntersected = false;

	for( Branch* b = fBranch -> firstBranch; b != NULL; b = b -> link )
	{
		//debug
		GLfloat test[3] = {-107,132,46};
		GLfloat test2[3] = {-108,140,22};
		if( sourceCorner[0] -> DistanceToPoint( test2 ) < 10.0 || sourceCorner[1] -> DistanceToPoint( test2 )< 10.0  )
		{
			if( b->sourceCorner[0] -> DistanceToPoint( test ) < 10.0 || b->sourceCorner[1] -> DistanceToPoint( test ) < 10.0 )
			{
				std::cout << "�q(" << sourceCorner[0] -> position[0] << "," << 
									  sourceCorner[0] -> position[1] << "," << 
									  sourceCorner[0] -> position[2] << ")�V(" << 
									  sourceCorner[1] -> position[0] << "," << 
									  sourceCorner[1] -> position[1] << "," << 
									  sourceCorner[1] -> position[2] << ")" << std::endl;
				std::cout << "�q(" << b->sourceCorner[0] -> position[0] << "," << 
									  b->sourceCorner[0] -> position[1] << "," << 
									  b->sourceCorner[0] -> position[2] << ")�V(" << 
									  b->sourceCorner[1] -> position[0] << "," << 
									  b->sourceCorner[1] -> position[1] << "," << 
									  b->sourceCorner[1] -> position[2] << ")" << std::endl;
			}
		}

		//�p�G���O�P�@�ӳ��I�A��Ӭ۶Z�]���|�ӻ�
		if( ( !sourceCorner[0] -> CloselyCompare ( b -> sourceCorner[0] ) && 
			sourceCorner[0] -> DistanceToPoint( b -> sourceCorner[0] -> position ) < threshold ) &&
			!sourceCorner[0] -> CloselyCompare ( b -> sourceCorner[1] ) &&//�ڪ��Y�I��O�H��
			!sourceCorner[1] -> CloselyCompare ( b -> sourceCorner[0] ) &&//�ڪ��e�I��O�H�Y
			!sourceCorner[1] -> CloselyCompare ( b -> sourceCorner[1] ) )//�ڪ��e�I��O�H��
		{
			if(  MathHelper::CheckTwoLineDistance( b -> sourceCorner[0] -> position , 
												   b -> sourceCorner[1] -> position , 
												   initPoint , 
												   endPoint ) < TRUSS_RADIUS + SOCKET_RADIUS )
			{
				std::cout << "�s���ƪK�P��L��K���" << std::endl;
				isIntersected = true;
				return true;
			}
		}
	}
	return false;
}

bool Branch::CheckDuplicated( Branch* tempFirstBranch )
{
	bool isDublicated = false;

	for( Branch* b = tempFirstBranch; b != NULL; b = b -> link )
	{
		if( ( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[0] ) && 
			  b -> sourceCorner[1] -> CloselyCompare( sourceCorner[1] ) ) ||
			( b -> sourceCorner[1] -> CloselyCompare( sourceCorner[0] ) && 
			  b -> sourceCorner[0] -> CloselyCompare( sourceCorner[1] ) ) )
		{
			isDublicated = true;
			std::cout << "�s���ƪK�P�ثe��L��K����" << std::endl;
			return true;
		}
	}
	return false;
}
//�o�Ӿ�䪺���ڦ��S���s��ثe�o�����O����䪺�Y
bool Branch::CheckConnectToOtherHead()
{
	for( Branch* b = firstBranch; b != NULL; b = b -> link )//�o�Ӧa�������@��
	{
		if( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[1] ) )
		{
			//isDublicated = true;
			std::cout << "�s���ƪK���s��O����䪺�Y" << std::endl;
			return true;
		}
	}
	return false;
}
//�����۳s�����s�_��
bool Branch::ConnectWithNeighborBranch( Branch* tempBranch )
{
	if( tempBranch == NULL )
	{
		return false;
	}


	for( Branch* b = tempBranch -> firstBranch; b != NULL; b = b -> link )
	{
		//���|�ج۳s�覡 1���L���Y�s��ڪ��Y 2���L���Y�s��ڪ��� 3���L�����s��ڪ��Y 4���L�����s��ڪ���
		//1���L���Y�s��ڪ��Y
		if( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[0] ) )
		{
			int me = 0;
			int other = 0;
			//��V�O�ڪ�0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[1] -> position , sourceCorner[0] -> position );
			MathHelper::VectorNormalize( direction );
			//�q�۳s�����Ӿ�� ���o�@�Ӿ�䪺��
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch�X���G�S�����s����" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][0] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][0] = sourceCorner[0];
			
			//�q�o�@�Ӿ�� ���۳s�����Ӿ�䪺��
			MathHelper::VectorMinus( direction , b -> sourceCorner[1] -> position , b -> sourceCorner[0] -> position );
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][0] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][0] = b -> sourceCorner[0];
		}
		//2���L���Y�s��ڪ���
		if( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[1] ) )
		{
			int other = 0;
			int me = 1;
			//��V�O�ڪ�0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[0] -> position , sourceCorner[1] -> position );
			MathHelper::VectorNormalize( direction );
			//�q�۳s�����Ӿ�� ���o�@�Ӿ�䪺��
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch�X���G�S�����s����" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][0] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][0] = sourceCorner[1];
			
			//�q�o�@�Ӿ�� ���۳s�����Ӿ�䪺��
			MathHelper::VectorMinus( direction , b -> sourceCorner[1] -> position , b -> sourceCorner[0] -> position );
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][1] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][1] = b -> sourceCorner[0];
		}
		//3���L�����s��ڪ��Y
		if( b -> sourceCorner[1] -> CloselyCompare( sourceCorner[0] ) )
		{
			int other = 1;
			int me = 0;
			//��V�O�ڪ�0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[1] -> position , sourceCorner[0] -> position );//0��1����V
			MathHelper::VectorNormalize( direction );
			//�q�۳s�����Ӿ�� ���o�@�Ӿ�䪺��
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch�X���G�S�����s����" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][1] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][1] = sourceCorner[0];
			
			//�q�o�@�Ӿ�� ���۳s�����Ӿ�䪺��
			MathHelper::VectorMinus( direction , b -> sourceCorner[0] -> position , b -> sourceCorner[1] -> position );//1��0����V
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][0] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][0] = b -> sourceCorner[1];
		}
		//4���L�����s��ڪ���
		if( b -> sourceCorner[1] -> CloselyCompare( sourceCorner[1] ) )
		{
			int me = 1;
			int other = 1;
			//��V�O�ڪ�0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[0] -> position , sourceCorner[1] -> position );//0��1����V
			MathHelper::VectorNormalize( direction );
			//�q�۳s�����Ӿ�� ���o�@�Ӿ�䪺��
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch�X���G�S�����s����" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][1] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][1] = sourceCorner[1];
			
			//�q�o�@�Ӿ�� ���۳s�����Ӿ�䪺��
			MathHelper::VectorMinus( direction , b -> sourceCorner[0] -> position , b -> sourceCorner[1] -> position );//1��0����V
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][1] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][1] = b -> sourceCorner[1];
		}
	}
	return true;
}
//�]�U�@�ӶZ��
bool Branch::SetPathDistance( int lastDistance , bool direction )
{
	//�o��O�n�q�̧��ڪ���K�A�Ϥ�V�����^�h�����Z���A�ҥH�p�G�o�ھ�K����V�V���ڡA�N�n���Y�h��L���W�@�ӶZ��
	//�p�G�o�ھ�K����V�V�Y�A�N�n���L�����ڪ���K�h��W�@�ӶZ��
	distance = lastDistance + 1;
	pathDirection = direction;
	//�s������V�O�G�q�T���@�����I�A�ҥH�̭��I�|�b�T���@�W
	if( pathDirection == TOWARD_HEAD )
	{
		if( sourceCorner[1] -> CloselyCompare( sourceOuterCone -> sourceCorner[0] ) ||
			sourceCorner[1] -> CloselyCompare( sourceOuterCone -> sourceCorner[1] ) ||
			sourceCorner[1] -> CloselyCompare( sourceOuterCone -> sourceCorner[2] ) )
		{
			std::cout << "��" << index << "���K���Z���O" << distance << "�A��V�V�Y(�ϦV)�A��K�����ڳs�V�~�h�T���@�A�����o�@�����|" << std::endl;
			return true;
		}
	}
	else if( pathDirection == TOWARD_TAIL )
	{
		if( sourceCorner[0] -> CloselyCompare( sourceOuterCone -> sourceCorner[0] ) ||
			sourceCorner[0] -> CloselyCompare( sourceOuterCone -> sourceCorner[1] ) ||
			sourceCorner[0] -> CloselyCompare( sourceOuterCone -> sourceCorner[2] ) )
		{
			std::cout << "��" << index << "���K���Z���O" << distance << "�A��V�V��(���V)�A��K���Y�s�V�~�h�T���@�A�����o�@�����|" << std::endl;
			return true;
		}
	}
	std::cout << "��" << index << "���K���Z���O" << distance << "�A��V�V" << (direction==TOWARD_TAIL?"��(���V)":"�Y(�ϦV)") << std::endl;
	for( int i = 0; i < 12; i++ )
	{
		//�p�G��V�O�q�Y�V�����A���Y�����ݺ�Z��
		if( neighborBranch[i][ BRANCH_HEAD ] != NULL && pathDirection == TOWARD_TAIL )
		{
			//�p�G�U�@�Ӿ���٨S�g�Z���A�άO�Z���j��{�b�Z��+1
			if( neighborBranch[i][ BRANCH_HEAD ] -> distance == -1 || neighborBranch[i][ BRANCH_HEAD ] -> distance > distance + 1 )
			{
				if( neighborCorner[i][ BRANCH_HEAD ] == neighborBranch[i][ BRANCH_HEAD ] -> sourceCorner[0] )
				{
					//TOWRAD_END
					std::cout << "��" << index << "�ھ�K���Y��V�]�w�Z���A�U�@�ھ�K" << neighborBranch[i][ BRANCH_HEAD ] -> index << "�n�q�Y���L�����ڨ���{�b�o�ھ�K" << std::endl;
					neighborBranch[i][ BRANCH_HEAD ] -> SetPathDistance( distance , TOWARD_HEAD );
				}
				else if( neighborCorner[i][ BRANCH_HEAD ] == neighborBranch[i][ BRANCH_HEAD ] -> sourceCorner[1] )
				{
					//TOWARD_HEAD
					std::cout << "��" << index << "�ھ�K���Y��V�]�w�Z���A�U�@�ھ�K" << neighborBranch[i][ BRANCH_HEAD ] -> index << "�n�q���ک��L���Y����{�b�o�ھ�K" << std::endl;
					neighborBranch[i][ BRANCH_HEAD ] -> SetPathDistance( distance , TOWARD_TAIL );
				}
				else
				{
					std::cout << "Branch::SetPathDistance�X��" << std::endl;
				}
			}
		}
		//�p�G��V�O�q���V�Y���A�����ڨ��ݺ�Z��
		if( neighborBranch[i][ BRANCH_TAIL ] != NULL && pathDirection == TOWARD_HEAD )
		{
			if( neighborBranch[i][ BRANCH_TAIL ] -> distance == -1 || neighborBranch[i][ BRANCH_TAIL ] -> distance > distance + 1 )
			{
				if( neighborCorner[i][ BRANCH_TAIL ] == neighborBranch[i][ BRANCH_TAIL ] -> sourceCorner[1] )
				{
					//TOWRAD_END
					std::cout << "��" << index << "�ھ�K�����ڤ�V����K�]�w�Z���A�U�@�ھ�K" << neighborBranch[i][ BRANCH_TAIL ] -> index << "�n�q�Y���L�����ڨ���{�b�o�ھ�K" << std::endl;
					neighborBranch[i][ BRANCH_TAIL ] -> SetPathDistance( distance , TOWARD_HEAD );
				}
				else if( neighborCorner[i][ BRANCH_TAIL ] == neighborBranch[i][ BRANCH_TAIL ] -> sourceCorner[0] )
				{
					//TOWARD_HEAD
					std::cout << "��" << index << "�ھ�K�����ڤ�V����K�]�w�Z���A�U�@�ھ�K" << neighborBranch[i][ BRANCH_TAIL ] -> index << "�n�q���ک��L���Y����{�b�o�ھ�K" << std::endl;
					neighborBranch[i][ BRANCH_TAIL ] -> SetPathDistance( distance , TOWARD_TAIL );
				}
				else
				{
					std::cout << "Branch::SetPathDistance�X��" << std::endl;
				}
			}
		}
	}
	return true;
}
//��B�̵u�����åB���O���A��K������L�`�����R��
bool Branch::SetShortestPath( int lastDistance )
{
	if( lastDistance == 1 )
	{
		onPath = true;
		return true;
	}

	if( !( distance == lastDistance - 1 ) || distance == -1 )
	{
		//�N��o�����O�̵u�����|
		return false;
	}
	std::cout << "��" << index << "���K�L�̵u���|�A�Z��" << distance << std::endl;
	//debug
	for( int i = 0; i < 12; i++ )
	{
		if( neighborBranch[i][ 0 ] != NULL )
		{
			std::cout << "���Y�ͪ�����K��m(" << neighborBranch[i][0]->sourceCorner[0]->position[0] << "," <<  
									neighborBranch[i][0]->sourceCorner[0]->position[1] << "," <<  
									neighborBranch[i][0]->sourceCorner[0]->position[2] << ")��(" << 
									neighborBranch[i][0]->sourceCorner[1]->position[0] << "," <<  
									neighborBranch[i][0]->sourceCorner[1]->position[1] << "," <<  
									neighborBranch[i][0]->sourceCorner[1]->position[2] << ")����K�Z��" << neighborBranch[i][0] ->distance << std::endl;

		}
		if( neighborBranch[i][ 1 ] != NULL )
		{
			std::cout << "�����ڥͪ�����K��m(" << neighborBranch[i][1]->sourceCorner[0]->position[0] << "," <<  
									neighborBranch[i][1]->sourceCorner[0]->position[1] << "," <<  
									neighborBranch[i][1]->sourceCorner[0]->position[2] << ")��(" << 
									neighborBranch[i][1]->sourceCorner[1]->position[0] << "," <<  
									neighborBranch[i][1]->sourceCorner[1]->position[1] << "," <<  
									neighborBranch[i][1]->sourceCorner[1]->position[2] << ")����K�Z��" << neighborBranch[i][1] ->distance << std::endl;

		}
	}



	//debug

	onPath = true;
	int cornerNumber;
	if( pathDirection == TOWARD_HEAD )//TOWARD_TAIL true
	{//���Y���A�ҥH�V�Y��			   TOWARD_HEAD false
		cornerNumber = BRANCH_HEAD;//0
	}
	else
	{//�����ڨ�
		cornerNumber = BRANCH_TAIL;//1
	}

	for( int i = 0; i < 12; i++ )
	{
		if( neighborBranch[i][ cornerNumber ] != NULL )
		{
			//�u��@�ӳ̵u���|�A�ҥH�@���Nreturn
			if ( neighborBranch[i][ cornerNumber ] -> SetShortestPath( distance ) )//cornerNumber�令0�M1
			{
				return true;
			}
			//test
			//if ( neighborBranch[i][ 1 ] -> SetShortestPath( distance ) )
			//{
			//	return true;
			//}
		}
	}

	return false;
}

//================BranchEnd//================
//================BranchEnd//================
//================BranchEnd//================

BranchEnd::BranchEnd( Branch* branch , BranchEnd* fBranchEnd )
{
	index = -1;
	type = 0;
	branchDirection = branch -> connectFace[0] - 30;//��V�O�쥻�����s��²30


	sourceBranch = branch;
	firstBranchEnd = fBranchEnd;
	sourceCorner[0] = new Point( branch -> sourceCorner[0] -> position );
	sourceCorner[1] = new Point( branch -> sourceCorner[1] -> position );
	cornerEnd[0] = new Point();
	cornerEnd[1] = new Point();

	for( int i = 0; i < BRANCH_END_TYPE_AMOUNT; i++ )
	{
		for( int j = 0; j < BRANCH_END_DIRECTION_AMOUNT; j++ )
		{
			distance[i][j] = -1;
			weightedDistance[i][j] = -1;
		}
	}

	nearestPointOnSurface = new Point();

	zomeDirection = branch -> zomeDirection;
}

bool BranchEnd::TryDifferentType( int t , int bEndDorection )
{
	type = t;
	branchEndDirection = bEndDorection;
	if( t == 0 )
		return true;

	//�ھڤ�V��𤧥��y���W�h
	if( !SelectConstructedHole() ) 
	{
		std::cout << "�o�ӼƤ�����" << this << "����" << t << "�زժk����" << bEndDorection << "�Ӥ�V�L�k�դW�h" << std::endl;
		return false;
	}
	//���ݦ��S����L�T���@��
	if( CheckIntersectWithConeFace() )
	{
		std::cout << "�o�ӼƤ�����" << this << "����" << t << "�زժk����" << bEndDorection << "�Ӥ�V��L�T���@�A�ҥH�R��" << std::endl;
		return false;
	}
	//�b�ݦ��S����T���@�������
	if( CheckIntersectWithConnector() )
	{
		std::cout << "�o�ӼƤ�����" << this << "����" << t << "�زժk����" << bEndDorection << "�Ӥ�V�P��K����A�ҥH�R��" << std::endl;
		return false;
	}

	if( firstBranchEnd != NULL )
	{
		if( CheckIntersectWithBranchEnd() )
		{
			std::cout << "�o�ӼƤ�����" << this << "����" << t << "�زժk����" << bEndDorection << "�Ӥ�V�P��K�������A�ҥH�R��" << std::endl;
			return false;
		}
	}
	
	return true;
}

bool BranchEnd::SelectConstructedHole()
{
	//����X�bconeDirection�O0����V�U�A�L�򥻪����v�A�U�@�B�b����L�����v�H���X�������}
	Point* baseDirection = new Point[2]; //��V0�����p�U�����v ��V0�N�O30
	Point* tempDirection = new Point[2]; //���Ӥ�V�H�᪺���v
	GLfloat rotateXMatrix[16]; MathHelper::makeIdentity( rotateXMatrix );
	GLfloat rotateYMatrix[16]; MathHelper::makeIdentity( rotateYMatrix );
	GLfloat rotateZMatrix[16]; MathHelper::makeIdentity( rotateZMatrix );

	GLfloat quaternion[16]; MathHelper::makeIdentity( quaternion );
	
	//���Τ�V�b30�������p�ت�򥻪���V�A����b����
	switch( type )
	{
		case 0:
			delete[] baseDirection;
			delete[] tempDirection;
			return true;
			break;
		case 1://�u�ŵu��
			baseDirection[0].index = 2;
			baseDirection[1].index = 33;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			break;
		case 2://�u���u�� �g�e
			baseDirection[0].index = 34;
			baseDirection[1].index = 8;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			break;
		case 3://3.�����u�� 
			baseDirection[0].index = 46;
			baseDirection[1].index = 47;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			break;
		case 4://4.�u������ �g�e
			baseDirection[0].index = 55;
			baseDirection[1].index = 45;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			break;
		case 5://5.���ŵu�� �g�e
			baseDirection[0].index = 14;
			baseDirection[1].index = 34;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			break;
		case 6://6.�u������
			baseDirection[0].index = 33;
			baseDirection[1].index = 1;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			break;
		case 7://7.�������� �g�e
			baseDirection[0].index = 34;
			baseDirection[1].index = 55;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			break;
		case 8://8.��������
			baseDirection[0].index = 47;
			baseDirection[1].index = 33;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			break;
	}

	baseDirection[0].SetPosition( zomeDirection -> faceDirection[ baseDirection[0].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[0].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[0].index * 3 + 2 ] );
	baseDirection[1].SetPosition( zomeDirection -> faceDirection[ baseDirection[1].index * 3 ] ,
									zomeDirection -> faceDirection[ baseDirection[1].index * 3 + 1 ] ,
									zomeDirection -> faceDirection[ baseDirection[1].index * 3 + 2 ] );


	GLfloat direction0[3]; 
	MathHelper::CopyVector( direction0 , &( zomeDirection -> faceDirection[ 30 * 3 ] ) );
	switch( branchEndDirection )//��ܥ��ݪ���V�A�@������
	{
		case 0:
			break;
		case 1:
			MathHelper::makeQuaternion( quaternion , direction0 , 72.0 / 180.0 * M_PI );
			break;
		case 2:
			MathHelper::makeQuaternion( quaternion , direction0 , 72.0 * 2.0 / 180.0 * M_PI );
			break;
		case 3:
			MathHelper::makeQuaternion( quaternion , direction0 , 72.0 * 3.0 / 180.0 * M_PI );
			break;
		case 4:
			MathHelper::makeQuaternion( quaternion , direction0 , 72.0 * 4.0 / 180.0 * M_PI );
			break;
	}

	switch( branchDirection )//opengl�����V�M�k��w�߬ۤ� //����z�by�bx
	{
		case 0:
			break;
		case 1:
			MathHelper::makeRotateX( rotateXMatrix , M_PI );//¶x�b�f��180��
			break;
		case 2:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b�f��180��
			break;
		case 3:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//¶z�b�f��180��
			MathHelper::makeRotateX( rotateXMatrix , M_PI );//¶x�b�f��180��
			break;
		case 4:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b����90��
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//¶y�b����90��
			break;
		case 5:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b����90��
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f��90��
			break;
		case 6:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f��90��
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//¶y�b����90��
			break;
		case 7:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f��90��
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//¶y�b�f��90��
			break;
		case 8:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f��90��
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//¶x�b�f��90��
			break;
		case 9:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b����90��
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//¶x�b����90��
			break;
		case 10:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//¶z�b����90��
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//¶x�b�f��90��
			break;
		case 11:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//¶z�b�f��90��
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//¶x�b����90��
			break;
	}

	//�N�쥻����V���W����x�}
	MathHelper::vectorMultMatrix( tempDirection[0].position , baseDirection[0].position , quaternion );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateXMatrix );

	MathHelper::vectorMultMatrix( tempDirection[1].position , baseDirection[1].position , quaternion );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateXMatrix );

	//�ھڱ���H�᪺��V�A��X�L����
	constructedHole[0] = zomeDirection -> GetDirectionFace( tempDirection[0].position );
	constructedHole[1] = zomeDirection -> GetDirectionFace( tempDirection[1].position );

	delete[] baseDirection;
	delete[] tempDirection;

	//�o�̮ھڿ�n���}�A��zometool��l���t�@�ݥ��ܺ�X��
	GLfloat endX = 0 , endY = 0 , endZ = 0;

	if( constructedHole[0] == -1 || constructedHole[1] == -1 )
	{
		std::cout << "BranchEnd::SelectConstructedHole�X���GGetFaceLength�X��(��J-1)�A�S����������" << std::endl;
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

}

bool BranchEnd::CheckIntersectWithConeFace()
{
	bool isIntersected = false;
	//��0�G032
	//��1�G123
	//��2�G210
	//��3�G301
	int faceCorner[4][3] = { { 0 , 3 , 2 } , 
							 { 1 , 2 , 3 } ,
							 { 2 , 1 , 0 } , 
							 { 3 , 0 , 1 } };
	GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 2.0;

	for( int i = 0; i < 2; i++ )
	{
		//����X�Y���ݦ�m
		GLfloat endPoint[3] , initPoint[3];
		MathHelper::VectorMinus( endPoint , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( endPoint );
		MathHelper::CopyVector( initPoint , endPoint );
		MathHelper::VectorPlus( initPoint , sourceCorner[i] -> position );
		MathHelper::VectorMutiply( endPoint , -1.0 );
		MathHelper::VectorPlus( endPoint , cornerEnd[i] -> position );

		//�ݥL���S���M��L��cone�������
		for( Cone* c = sourceBranch -> sourceOuterCone -> sourceCone -> firstCone; c != NULL; c = c -> link )
		{
			if( c -> activated )
			{
				//�p�G�Z���ӻ��N���L
				if( MathHelper::VectorDistance( c -> corner[0] -> position , sourceCorner[0] -> position ) > threshold ) 
					continue;
				//4�ӭ�
				for( int j = 0; j < 4; j++ )
				{
					if( MathHelper::CheckIntersectOnTriangle( initPoint ,
															  endPoint , 
															  c -> corner[ faceCorner[j][0] ] -> position ,
															  c -> corner[ faceCorner[j][1] ] -> position ,
															  c -> corner[ faceCorner[j][2] ] -> position ) )
					{
						isIntersected = true;
						std::cout << "�s�����|���J��LCone��" << std::endl;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool BranchEnd::CheckIntersectWithConnector()
{
	bool isIntersected = false;

	for( int i = 0; i < 2; i++ )
	{

		//����X�Y���ݦ�m
		GLfloat endPoint[3] , initPoint[3];
		MathHelper::VectorMinus( endPoint , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( endPoint );
		MathHelper::CopyVector( initPoint , endPoint );
		MathHelper::VectorPlus( initPoint , sourceCorner[i] -> position );
		MathHelper::VectorMutiply( endPoint , -1.0 );
		MathHelper::VectorPlus( endPoint , cornerEnd[i] -> position );

		for( Cone* c = sourceBranch -> sourceOuterCone -> sourceCone -> firstCone; c != NULL; c = c -> link )
		{
			if( c -> activated )
			{
				//4���I
				for( int j = 0; j < 4 - 1; j++ )//�T���@���Y�@�I
				{
					//4���I
					for( int k = j + 1; k < 4; k++ )//�T���@���t�@�I �L�̳s�u
					{
						//��Ƥ��
						if( !c -> corner[j] -> CloselyCompare( sourceCorner[i] ) && 
							!c -> corner[k] -> CloselyCompare( sourceCorner[i] ) &&
							!c -> corner[j] -> CloselyCompare( cornerEnd[i] ) &&
							!c -> corner[k] -> CloselyCompare( cornerEnd[i] ) )
						{
							GLfloat distance = MathHelper::CheckTwoLineDistance( c -> corner[j] -> position , 
																				 c -> corner[k] -> position , 
																				 initPoint , 
																				 endPoint );
							if( distance < TRUSS_RADIUS + SOCKET_RADIUS )
							{
								isIntersected = true;
								std::cout << "�s���ƪK�P��LCone����l���" << std::endl;
								return true;
							}
						}//�o�̷|��bug�A���ڤ������|�ɳo�䤣�|�o�{
					}
				}
			}
		}

		//�ݥL���S�����L��outercone��l���
		for( OuterCone* o = sourceBranch -> sourceOuterCone -> firstOuterCone; o != NULL; o = o -> link )
		{
			if( o -> type == 0 ) continue;
			//�O��cone�o3�ӥ~��[
			for( int j = 0; j < 3; j++ )
			{
				//��ۤv���Z��
				if( !o -> sourceCorner[j] -> CloselyCompare( sourceCorner[i] ) &&
					!o -> sourceCorner[j] -> CloselyCompare( cornerEnd[i] )  )
				{
					if( MathHelper::CheckTwoLineDistance( o -> sourceCorner[j] -> position , 
														  o -> cornerEnd[j] -> position , 
														  initPoint , endPoint ) < TRUSS_RADIUS * 2.0 )
					{
						std::cout << "�s���ƪK�P��LOuterCone����l���" << std::endl;
						isIntersected = true;
						return true;
					}
				}
				else if( MathHelper::CompareDirection( o -> sourceCorner[j] -> position ,
													   o -> cornerEnd[j] -> position ,
													   initPoint , 
													   endPoint ) )
				{
					std::cout << "�s���ƪK�P��LOuterCone����l���|" << std::endl;
					isIntersected = true;
					return true;
				}
			}
		}
	}
	//�ݥL���S���M��L��cone����l���
	return false;
}

bool BranchEnd::CheckIntersectWithBranchEnd()
{

	for( int i = 0; i < 2; i++ )
	{

		//����X�Y���ݦ�m
		GLfloat endPoint[3] , initPoint[3];
		MathHelper::VectorMinus( endPoint , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( endPoint );
		MathHelper::CopyVector( initPoint , endPoint );
		MathHelper::VectorPlus( initPoint , sourceCorner[i] -> position );
		MathHelper::VectorMutiply( endPoint , -1.0 );
		MathHelper::VectorPlus( endPoint , cornerEnd[i] -> position );

		for( BranchEnd* b = firstBranchEnd; b != NULL; b = b -> link )
		{
			if( b == this ) continue;
			if( b -> type == 0 ) continue;

			for( int j = 0; j < 2; j++ )
			{
				if( !b -> sourceCorner[j] -> CloselyCompare( sourceCorner[i] ) )
				{
					GLfloat tempDistance = MathHelper::CheckTwoLineDistance( b -> sourceCorner[j] -> position , 
																		 b -> cornerEnd[j] -> position , 
																		 initPoint , 
																		 endPoint );
					if( tempDistance < TRUSS_RADIUS + SOCKET_RADIUS )
					{
						//isIntersected = true;
						std::cout << "�s���ƪK�P��L��K����l���" << std::endl;
						return true;
					}
				}
				else if( MathHelper::CompareDirection( b -> sourceCorner[j] -> position ,
													   b -> cornerEnd[j] -> position ,
													   initPoint , 
													   endPoint ) )
				{
					std::cout << "�s���ƪK�P��L��K����l���|" << std::endl;
					//isIntersected = true;
					return true;
				}
			}
		}
	}
	return false;
}

bool BranchEnd::CheckIntersectWithBranch( Branch* fBranch )
{
	if( fBranch == NULL )
	{
		return false;
	}

	//debug
	GLfloat test[3] = {-107 , 169,23};
	GLfloat test2[3] = { -70 , 148 , 23 };
	if( (( MathHelper::VectorDistance( test , sourceCorner[0] -> position ) < 10.0 && 
		  MathHelper::VectorDistance( test2 , sourceCorner[1] -> position ) < 10.0 ) ||
		( MathHelper::VectorDistance( test , sourceCorner[1] -> position ) < 10.0 && 
		  MathHelper::VectorDistance( test2 , sourceCorner[0] -> position ) < 10.0 ))&&
		  ( type == 7 || type == 8 ) )
	{
		std::cout << "�Ĥ@�ڪ����ݦ�m�G(" << cornerEnd[0] -> position[0] << "," <<
											  cornerEnd[0] -> position[1] << "," <<
											  cornerEnd[0] -> position[2] << ")�A�ĤG�ڪ����ݦ�m(" <<
											  cornerEnd[1] -> position[0] << "," <<
											  cornerEnd[1] -> position[1] << "," <<
											  cornerEnd[1] -> position[2] << ")" << std::endl;
	}


	for( int i = 0; i < 2; i++ )
	{
		//����X�Y���ݦ�m
		GLfloat endPoint[3] , initPoint[3];
		MathHelper::VectorMinus( endPoint , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( endPoint );
		MathHelper::CopyVector( initPoint , endPoint );
		MathHelper::VectorPlus( initPoint , sourceCorner[i] -> position );
		MathHelper::VectorMutiply( endPoint , -0.1 );
		MathHelper::VectorPlus( endPoint , cornerEnd[i] -> position );

		GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 1.5;
		bool isIntersected = false;

		for( Branch* b = fBranch -> firstBranch; b != NULL; b = b -> link )
		{
			//�p�G���O�P�@�ӳ��I�A��Ӭ۶Z�]���|�ӻ�
			if( ( !sourceCorner[i] -> CloselyCompare ( b -> sourceCorner[0] ) && 
				sourceCorner[i] -> DistanceToPoint( b -> sourceCorner[0] -> position ) < threshold ) &&
				!sourceCorner[i] -> CloselyCompare ( b -> sourceCorner[1] ) )//�ڪ��Y�I��O�H��
			{
				if(  MathHelper::CheckTwoLineDistance( b -> sourceCorner[0] -> position , 
													   b -> sourceCorner[1] -> position , 
													   initPoint , 
													   endPoint ) < TRUSS_RADIUS + SOCKET_RADIUS )
				{
					std::cout << "�s���ƪK����P��L��K���" << std::endl;
					isIntersected = true;
					return true;
				}
			}
			else if( MathHelper::CompareDirection( b -> sourceCorner[0] -> position ,
												   b -> sourceCorner[1] -> position ,
												   initPoint , 
												   endPoint ) )
			{
				std::cout << "�s���ƪK����P��L��K����l���|" << std::endl;
				//isIntersected = true;
				return true;
			}
		}
	}
	return false;
}