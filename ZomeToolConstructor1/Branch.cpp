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
		std::cout << "Branch::Branch出錯：躍增生樹支的面以有連接其他樹支" << std::endl;
	}
	//相連不可以在這邊做，不然之後山吊樹支會出錯
	//把上一個樹枝跟這個連起來
	//lastBranch -> neighborBranch[ branchIndex ] = this;
	//把這個樹枝跟上一個連起來
	//int face = GetBranchIndexFace( branchIndex );
	//int oppositeFace = zomeDirection -> GetOppositeFace(face);
	//neighborBranch[ GetFaceBranchIndex( oppositeFace ) ] = lastBranch;
}

Branch::~Branch()
{
	delete sourceCorner[0];
	delete sourceCorner[1];
}
//把branch加入這一串樹枝裡
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
	//先算出頭尾端位置
	
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
	//面0：032
	//面1：123
	//面2：210
	//面3：301
	int faceCorner[4][3] = { { 0 , 3 , 2 } , 
							 { 1 , 2 , 3 } ,
							 { 2 , 1 , 0 } , 
							 { 3 , 0 , 1 } };
	GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 2.0;


	//看他有沒有和其他的cone的面交錯
	for( Cone* c = sourceOuterCone -> sourceCone -> firstCone; c != NULL; c = c -> link )
	{
		if( c -> activated )
		{
			//如果距離太遠就跳過
			if( MathHelper::VectorDistance( c -> corner[0] -> position , sourceCorner[0] -> position ) > threshold ) 
				continue;
			//4個面
			for( int i = 0; i < 4; i++ )
			{
				if( MathHelper::CheckIntersectOnTriangle( initPoint ,
														  endPoint , 
														  c -> corner[ faceCorner[i][0] ] -> position ,
														  c -> corner[ faceCorner[i][1] ] -> position ,
														  c -> corner[ faceCorner[i][2] ] -> position ) )
				{
					isIntersected = true;
					std::cout << "新的樹支會插入其他Cone面" << std::endl;
					return true;
				}
			}
		}
	}
	return false;
}

bool Branch::CheckIntersectWithConnector()
{
	//先算出頭尾端位置
	GLfloat endPoint[3] , initPoint[3];
	MathHelper::VectorMinus( endPoint , sourceCorner[1] -> position , sourceCorner[0] -> position );
	MathHelper::VectorNormalize( endPoint );
	MathHelper::CopyVector( initPoint , endPoint );
	MathHelper::VectorPlus( initPoint , sourceCorner[0] -> position );
	MathHelper::VectorMutiply( endPoint ,  zomeDirection -> GetColorLength( COLOR_RED , SIZE_S ) - NODE_DIAMETER / 2.0 + LATCH );
	MathHelper::VectorPlus( endPoint , sourceCorner[0] -> position );
	
	bool isIntersected = false;

	//看他有沒有和其他的cone的桿子交錯
	for( Cone* i = sourceOuterCone -> sourceCone -> firstCone; i != NULL; i = i -> link )
	{
		if( i -> activated )
		{
			//4個點
			for( int j = 0; j < 4 - 1; j++ )//三角錐的某一點
			{
				//4個點
				for( int k = j + 1; k < 4; k++ )//三角錐的另一點 他們連線
				{
					//跟數支比
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
							std::cout << "新的數枝與其他Cone的桿子交錯" << std::endl;
							return true;
						}
					}
				}
			}
		}
	}

	//看他有沒有跟其他的outercone竿子交錯
	for( OuterCone* i = sourceOuterCone -> firstOuterCone; i != NULL; i = i -> link )
	{
		if( i -> type == 0 ) continue;
		//別的cone得3個外支架
		for( int j = 0; j < 3; j++ )
		{
			//跟自己測距離
			if( !i -> sourceCorner[j] -> CloselyCompare( sourceCorner[0] ) &&
				!i -> sourceCorner[j] -> CloselyCompare( sourceCorner[1] )  )
			{
				if( MathHelper::CheckTwoLineDistance( i -> sourceCorner[j] -> position , 
													  i -> cornerEnd[j] -> position , 
													  initPoint , endPoint ) < TRUSS_RADIUS * 2.0 )
				{
					std::cout << "新的數枝與其他OuterCone的桿子交錯" << std::endl;
					isIntersected = true;
					return true;
				}
			}
			else if( MathHelper::CompareDirection( i -> sourceCorner[j] -> position ,
												   i -> cornerEnd[j] -> position ,
												   initPoint , 
												   endPoint ) )
			{
				std::cout << "新的數枝與其他OuterCone的桿子重疊" << std::endl;
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
	//先算出頭尾端位置
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
				std::cout << "從(" << sourceCorner[0] -> position[0] << "," << 
									  sourceCorner[0] -> position[1] << "," << 
									  sourceCorner[0] -> position[2] << ")向(" << 
									  sourceCorner[1] -> position[0] << "," << 
									  sourceCorner[1] -> position[1] << "," << 
									  sourceCorner[1] -> position[2] << ")" << std::endl;
				std::cout << "從(" << b->sourceCorner[0] -> position[0] << "," << 
									  b->sourceCorner[0] -> position[1] << "," << 
									  b->sourceCorner[0] -> position[2] << ")向(" << 
									  b->sourceCorner[1] -> position[0] << "," << 
									  b->sourceCorner[1] -> position[1] << "," << 
									  b->sourceCorner[1] -> position[2] << ")" << std::endl;
			}
		}

		//如果不是同一個頂點，兩個相距也部會太遠
		if( ( !sourceCorner[0] -> CloselyCompare ( b -> sourceCorner[0] ) && 
			sourceCorner[0] -> DistanceToPoint( b -> sourceCorner[0] -> position ) < threshold ) &&
			!sourceCorner[0] -> CloselyCompare ( b -> sourceCorner[1] ) &&//我的頭碰到別人尾
			!sourceCorner[1] -> CloselyCompare ( b -> sourceCorner[0] ) &&//我的委碰到別人頭
			!sourceCorner[1] -> CloselyCompare ( b -> sourceCorner[1] ) )//我的委碰到別人尾
		{
			if(  MathHelper::CheckTwoLineDistance( b -> sourceCorner[0] -> position , 
												   b -> sourceCorner[1] -> position , 
												   initPoint , 
												   endPoint ) < TRUSS_RADIUS + SOCKET_RADIUS )
			{
				std::cout << "新的數枝與其他樹枝交錯" << std::endl;
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
			std::cout << "新的數枝與目前其他樹枝重複" << std::endl;
			return true;
		}
	}
	return false;
}
//這個樹支的尾巴有沒有連到目前這條的別的樹支的頭
bool Branch::CheckConnectToOtherHead()
{
	for( Branch* b = firstBranch; b != NULL; b = b -> link )//這個地方比較不一樣
	{
		if( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[1] ) )
		{
			//isDublicated = true;
			std::cout << "新的數枝有連到別的樹支的頭" << std::endl;
			return true;
		}
	}
	return false;
}
//把附近相連的樹支連起來
bool Branch::ConnectWithNeighborBranch( Branch* tempBranch )
{
	if( tempBranch == NULL )
	{
		return false;
	}


	for( Branch* b = tempBranch -> firstBranch; b != NULL; b = b -> link )
	{
		//有四種相連方式 1視他的頭連到我的頭 2視他的頭連到我的尾 3視他的尾連到我的頭 4視他的尾連到我的尾
		//1視他的頭連到我的頭
		if( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[0] ) )
		{
			int me = 0;
			int other = 0;
			//方向是我的0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[1] -> position , sourceCorner[0] -> position );
			MathHelper::VectorNormalize( direction );
			//從相連的那個樹支 網這一個樹支的面
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch出錯：沒有找到連接面" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][0] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][0] = sourceCorner[0];
			
			//從這一個樹支 網相連的那個樹支的面
			MathHelper::VectorMinus( direction , b -> sourceCorner[1] -> position , b -> sourceCorner[0] -> position );
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][0] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][0] = b -> sourceCorner[0];
		}
		//2視他的頭連到我的尾
		if( b -> sourceCorner[0] -> CloselyCompare( sourceCorner[1] ) )
		{
			int other = 0;
			int me = 1;
			//方向是我的0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[0] -> position , sourceCorner[1] -> position );
			MathHelper::VectorNormalize( direction );
			//從相連的那個樹支 網這一個樹支的面
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch出錯：沒有找到連接面" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][0] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][0] = sourceCorner[1];
			
			//從這一個樹支 網相連的那個樹支的面
			MathHelper::VectorMinus( direction , b -> sourceCorner[1] -> position , b -> sourceCorner[0] -> position );
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][1] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][1] = b -> sourceCorner[0];
		}
		//3視他的尾連到我的頭
		if( b -> sourceCorner[1] -> CloselyCompare( sourceCorner[0] ) )
		{
			int other = 1;
			int me = 0;
			//方向是我的0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[1] -> position , sourceCorner[0] -> position );//0到1的方向
			MathHelper::VectorNormalize( direction );
			//從相連的那個樹支 網這一個樹支的面
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch出錯：沒有找到連接面" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][1] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][1] = sourceCorner[0];
			
			//從這一個樹支 網相連的那個樹支的面
			MathHelper::VectorMinus( direction , b -> sourceCorner[0] -> position , b -> sourceCorner[1] -> position );//1到0的方向
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][0] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][0] = b -> sourceCorner[1];
		}
		//4視他的尾連到我的尾
		if( b -> sourceCorner[1] -> CloselyCompare( sourceCorner[1] ) )
		{
			int me = 1;
			int other = 1;
			//方向是我的0~1
			GLfloat direction[3];
			MathHelper::VectorMinus( direction , sourceCorner[0] -> position , sourceCorner[1] -> position );//0到1的方向
			MathHelper::VectorNormalize( direction );
			//從相連的那個樹支 網這一個樹支的面
			int faceToMe = zomeDirection -> GetDirectionFace( direction );
			if( faceToMe < 30 )
			{
				std::cout << "Branch::ConnectWithNeighborBranch出錯：沒有找到連接面" << std::endl;
			}
			b -> neighborBranch[ GetFaceBranchIndex( faceToMe ) ][1] = this;
			b -> neighborCorner[ GetFaceBranchIndex( faceToMe ) ][1] = sourceCorner[1];
			
			//從這一個樹支 網相連的那個樹支的面
			MathHelper::VectorMinus( direction , b -> sourceCorner[0] -> position , b -> sourceCorner[1] -> position );//1到0的方向
			MathHelper::VectorNormalize( direction );

			int faceToOther = zomeDirection -> GetDirectionFace( direction );
			neighborBranch[ GetFaceBranchIndex( faceToOther ) ][1] = b;
			neighborCorner[ GetFaceBranchIndex( faceToOther ) ][1] = b -> sourceCorner[1];
		}
	}
	return true;
}
//設下一個距離
bool Branch::SetPathDistance( int lastDistance , bool direction )
{
	//這邊是要從最尾巴的樹枝，反方向的往回去紀錄距離，所以如果這根樹枝的方向向尾巴，就要往頭去算他的上一個距離
	//如果這根樹枝的方向向頭，就要往他的尾巴的樹枝去算上一個距離
	distance = lastDistance + 1;
	pathDirection = direction;
	//連接的方向是：從三角錐往表面點，所以最原點會在三角錐上
	if( pathDirection == TOWARD_HEAD )
	{
		if( sourceCorner[1] -> CloselyCompare( sourceOuterCone -> sourceCorner[0] ) ||
			sourceCorner[1] -> CloselyCompare( sourceOuterCone -> sourceCorner[1] ) ||
			sourceCorner[1] -> CloselyCompare( sourceOuterCone -> sourceCorner[2] ) )
		{
			std::cout << "第" << index << "跟樹枝的距離是" << distance << "，方向向頭(反向)，樹枝的尾巴連向外層三角錐，結束這一條路徑" << std::endl;
			return true;
		}
	}
	else if( pathDirection == TOWARD_TAIL )
	{
		if( sourceCorner[0] -> CloselyCompare( sourceOuterCone -> sourceCorner[0] ) ||
			sourceCorner[0] -> CloselyCompare( sourceOuterCone -> sourceCorner[1] ) ||
			sourceCorner[0] -> CloselyCompare( sourceOuterCone -> sourceCorner[2] ) )
		{
			std::cout << "第" << index << "跟樹枝的距離是" << distance << "，方向向尾(正向)，樹枝的頭連向外層三角錐，結束這一條路徑" << std::endl;
			return true;
		}
	}
	std::cout << "第" << index << "跟樹枝的距離是" << distance << "，方向向" << (direction==TOWARD_TAIL?"尾(正向)":"頭(反向)") << std::endl;
	for( int i = 0; i < 12; i++ )
	{
		//如果方向是從頭向尾走，往頭的那端算距離
		if( neighborBranch[i][ BRANCH_HEAD ] != NULL && pathDirection == TOWARD_TAIL )
		{
			//如果下一個樹支還沒射距離，或是距離大於現在距離+1
			if( neighborBranch[i][ BRANCH_HEAD ] -> distance == -1 || neighborBranch[i][ BRANCH_HEAD ] -> distance > distance + 1 )
			{
				if( neighborCorner[i][ BRANCH_HEAD ] == neighborBranch[i][ BRANCH_HEAD ] -> sourceCorner[0] )
				{
					//TOWRAD_END
					std::cout << "第" << index << "根樹枝往頭方向設定距離，下一根樹枝" << neighborBranch[i][ BRANCH_HEAD ] -> index << "要從頭往他的尾巴走到現在這根樹枝" << std::endl;
					neighborBranch[i][ BRANCH_HEAD ] -> SetPathDistance( distance , TOWARD_HEAD );
				}
				else if( neighborCorner[i][ BRANCH_HEAD ] == neighborBranch[i][ BRANCH_HEAD ] -> sourceCorner[1] )
				{
					//TOWARD_HEAD
					std::cout << "第" << index << "根樹枝往頭方向設定距離，下一根樹枝" << neighborBranch[i][ BRANCH_HEAD ] -> index << "要從尾巴往他的頭走到現在這根樹枝" << std::endl;
					neighborBranch[i][ BRANCH_HEAD ] -> SetPathDistance( distance , TOWARD_TAIL );
				}
				else
				{
					std::cout << "Branch::SetPathDistance出錯" << std::endl;
				}
			}
		}
		//如果方向是從尾向頭走，網尾巴那端算距離
		if( neighborBranch[i][ BRANCH_TAIL ] != NULL && pathDirection == TOWARD_HEAD )
		{
			if( neighborBranch[i][ BRANCH_TAIL ] -> distance == -1 || neighborBranch[i][ BRANCH_TAIL ] -> distance > distance + 1 )
			{
				if( neighborCorner[i][ BRANCH_TAIL ] == neighborBranch[i][ BRANCH_TAIL ] -> sourceCorner[1] )
				{
					//TOWRAD_END
					std::cout << "第" << index << "根樹枝往尾巴方向的樹枝設定距離，下一根樹枝" << neighborBranch[i][ BRANCH_TAIL ] -> index << "要從頭往他的尾巴走到現在這根樹枝" << std::endl;
					neighborBranch[i][ BRANCH_TAIL ] -> SetPathDistance( distance , TOWARD_HEAD );
				}
				else if( neighborCorner[i][ BRANCH_TAIL ] == neighborBranch[i][ BRANCH_TAIL ] -> sourceCorner[0] )
				{
					//TOWARD_HEAD
					std::cout << "第" << index << "根樹枝往尾巴方向的樹枝設定距離，下一根樹枝" << neighborBranch[i][ BRANCH_TAIL ] -> index << "要從尾巴往他的頭走到現在這根樹枝" << std::endl;
					neighborBranch[i][ BRANCH_TAIL ] -> SetPathDistance( distance , TOWARD_TAIL );
				}
				else
				{
					std::cout << "Branch::SetPathDistance出錯" << std::endl;
				}
			}
		}
	}
	return true;
}
//找處最短的路並且做記號，方便之後把其他常的錄刪掉
bool Branch::SetShortestPath( int lastDistance )
{
	if( lastDistance == 1 )
	{
		onPath = true;
		return true;
	}

	if( !( distance == lastDistance - 1 ) || distance == -1 )
	{
		//代表這條不是最短的路徑
		return false;
	}
	std::cout << "第" << index << "跟樹枝微最短路徑，距離" << distance << std::endl;
	//debug
	for( int i = 0; i < 12; i++ )
	{
		if( neighborBranch[i][ 0 ] != NULL )
		{
			std::cout << "往頭生長的樹枝位置(" << neighborBranch[i][0]->sourceCorner[0]->position[0] << "," <<  
									neighborBranch[i][0]->sourceCorner[0]->position[1] << "," <<  
									neighborBranch[i][0]->sourceCorner[0]->position[2] << ")到(" << 
									neighborBranch[i][0]->sourceCorner[1]->position[0] << "," <<  
									neighborBranch[i][0]->sourceCorner[1]->position[1] << "," <<  
									neighborBranch[i][0]->sourceCorner[1]->position[2] << ")的樹枝距離" << neighborBranch[i][0] ->distance << std::endl;

		}
		if( neighborBranch[i][ 1 ] != NULL )
		{
			std::cout << "往尾巴生長的樹枝位置(" << neighborBranch[i][1]->sourceCorner[0]->position[0] << "," <<  
									neighborBranch[i][1]->sourceCorner[0]->position[1] << "," <<  
									neighborBranch[i][1]->sourceCorner[0]->position[2] << ")到(" << 
									neighborBranch[i][1]->sourceCorner[1]->position[0] << "," <<  
									neighborBranch[i][1]->sourceCorner[1]->position[1] << "," <<  
									neighborBranch[i][1]->sourceCorner[1]->position[2] << ")的樹枝距離" << neighborBranch[i][1] ->distance << std::endl;

		}
	}



	//debug

	onPath = true;
	int cornerNumber;
	if( pathDirection == TOWARD_HEAD )//TOWARD_TAIL true
	{//往頭走，所以向頭走			   TOWARD_HEAD false
		cornerNumber = BRANCH_HEAD;//0
	}
	else
	{//往尾巴走
		cornerNumber = BRANCH_TAIL;//1
	}

	for( int i = 0; i < 12; i++ )
	{
		if( neighborBranch[i][ cornerNumber ] != NULL )
		{
			//只找一個最短路徑，所以一找到就return
			if ( neighborBranch[i][ cornerNumber ] -> SetShortestPath( distance ) )//cornerNumber改成0和1
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
	branchDirection = branch -> connectFace[0] - 30;//方向是原本面的編號簡30


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

	//根據方向把樹之末稍插上去
	if( !SelectConstructedHole() ) 
	{
		std::cout << "這個數之末梢" << this << "的第" << t << "種組法的第" << bEndDorection << "個方向無法組上去" << std::endl;
		return false;
	}
	//先看有沒有穿過三角錐面
	if( CheckIntersectWithConeFace() )
	{
		std::cout << "這個數之末梢" << this << "的第" << t << "種組法的第" << bEndDorection << "個方向穿過三角錐，所以刪除" << std::endl;
		return false;
	}
	//在看有沒有跟三角錐之價交錯
	if( CheckIntersectWithConnector() )
	{
		std::cout << "這個數之末梢" << this << "的第" << t << "種組法的第" << bEndDorection << "個方向與樹枝交錯，所以刪除" << std::endl;
		return false;
	}

	if( firstBranchEnd != NULL )
	{
		if( CheckIntersectWithBranchEnd() )
		{
			std::cout << "這個數之末梢" << this << "的第" << t << "種組法的第" << bEndDorection << "個方向與樹枝末梢交錯，所以刪除" << std::endl;
			return false;
		}
	}
	
	return true;
}

bool BranchEnd::SelectConstructedHole()
{
	//先找出在coneDirection是0的方向下，他基本的接髮，下一步在旋轉他的接髮以後找出對應的洞
	Point* baseDirection = new Point[2]; //方向0的情況下的接髮 方向0就是30
	Point* tempDirection = new Point[2]; //轉到該方向以後的接髮
	GLfloat rotateXMatrix[16]; MathHelper::makeIdentity( rotateXMatrix );
	GLfloat rotateYMatrix[16]; MathHelper::makeIdentity( rotateYMatrix );
	GLfloat rotateZMatrix[16]; MathHelper::makeIdentity( rotateZMatrix );

	GLfloat quaternion[16]; MathHelper::makeIdentity( quaternion );
	
	//先用方向在30面的情況建初基本的方向，之後在旋轉
	switch( type )
	{
		case 0:
			delete[] baseDirection;
			delete[] tempDirection;
			return true;
			break;
		case 1://短藍短紅
			baseDirection[0].index = 2;
			baseDirection[1].index = 33;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			break;
		case 2://短紅短藍 篇前
			baseDirection[0].index = 34;
			baseDirection[1].index = 8;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_S;
			break;
		case 3://3.中黃短黃 
			baseDirection[0].index = 46;
			baseDirection[1].index = 47;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			break;
		case 4://4.短黃中黃 篇前
			baseDirection[0].index = 55;
			baseDirection[1].index = 45;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			break;
		case 5://5.中藍短紅 篇前
			baseDirection[0].index = 14;
			baseDirection[1].index = 34;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_S;
			break;
		case 6://6.短紅中藍
			baseDirection[0].index = 33;
			baseDirection[1].index = 1;
			constructedSize[0] = SIZE_S;
			constructedSize[1] = SIZE_M;
			break;
		case 7://7.中紅中黃 篇前
			baseDirection[0].index = 34;
			baseDirection[1].index = 55;
			constructedSize[0] = SIZE_M;
			constructedSize[1] = SIZE_M;
			break;
		case 8://8.中黃中紅
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
	switch( branchEndDirection )//選擇末端的方向，共有五個
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

	switch( branchDirection )//opengl旋轉方向和右手定律相反 //先轉z在y在x
	{
		case 0:
			break;
		case 1:
			MathHelper::makeRotateX( rotateXMatrix , M_PI );//繞x軸逆轉180度
			break;
		case 2:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸逆轉180度
			break;
		case 3:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI );//繞z軸逆轉180度
			MathHelper::makeRotateX( rotateXMatrix , M_PI );//繞x軸逆轉180度
			break;
		case 4:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正轉90度
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//繞y軸正轉90度
			break;
		case 5:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正轉90度
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆轉90度
			break;
		case 6:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆轉90度
			MathHelper::makeRotateY( rotateYMatrix , -M_PI / 2.0 );//繞y軸正轉90度
			break;
		case 7:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆轉90度
			MathHelper::makeRotateY( rotateYMatrix , M_PI / 2.0 );//繞y軸逆轉90度
			break;
		case 8:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆轉90度
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//繞x軸逆轉90度
			break;
		case 9:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正轉90度
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正轉90度
			break;
		case 10:
			MathHelper::makeRotateZ( rotateZMatrix , -M_PI / 2.0 );//繞z軸正轉90度
			MathHelper::makeRotateX( rotateXMatrix , M_PI / 2.0 );//繞x軸逆轉90度
			break;
		case 11:
			MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸逆轉90度
			MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正轉90度
			break;
	}

	//將原本的方向乘上旋轉矩陣
	MathHelper::vectorMultMatrix( tempDirection[0].position , baseDirection[0].position , quaternion );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[0].position , rotateXMatrix );

	MathHelper::vectorMultMatrix( tempDirection[1].position , baseDirection[1].position , quaternion );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateZMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateYMatrix );
	MathHelper::vectorMultMatrix( tempDirection[1].position , rotateXMatrix );

	//根據旋轉以後的方向，找出他的面
	constructedHole[0] = zomeDirection -> GetDirectionFace( tempDirection[0].position );
	constructedHole[1] = zomeDirection -> GetDirectionFace( tempDirection[1].position );

	delete[] baseDirection;
	delete[] tempDirection;

	//這裡根據選好的洞，把zometool竿子的另一端未至算出來
	GLfloat endX = 0 , endY = 0 , endZ = 0;

	if( constructedHole[0] == -1 || constructedHole[1] == -1 )
	{
		std::cout << "BranchEnd::SelectConstructedHole出錯：GetFaceLength出錯(輸入-1)，沒有找到對應邊" << std::endl;
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
	//面0：032
	//面1：123
	//面2：210
	//面3：301
	int faceCorner[4][3] = { { 0 , 3 , 2 } , 
							 { 1 , 2 , 3 } ,
							 { 2 , 1 , 0 } , 
							 { 3 , 0 , 1 } };
	GLfloat threshold = zomeDirection -> GetColorLength( COLOR_BLUE , SIZE_L ) * 2.0;

	for( int i = 0; i < 2; i++ )
	{
		//先算出頭尾端位置
		GLfloat endPoint[3] , initPoint[3];
		MathHelper::VectorMinus( endPoint , cornerEnd[i] -> position , sourceCorner[i] -> position );
		MathHelper::VectorNormalize( endPoint );
		MathHelper::CopyVector( initPoint , endPoint );
		MathHelper::VectorPlus( initPoint , sourceCorner[i] -> position );
		MathHelper::VectorMutiply( endPoint , -1.0 );
		MathHelper::VectorPlus( endPoint , cornerEnd[i] -> position );

		//看他有沒有和其他的cone的面交錯
		for( Cone* c = sourceBranch -> sourceOuterCone -> sourceCone -> firstCone; c != NULL; c = c -> link )
		{
			if( c -> activated )
			{
				//如果距離太遠就跳過
				if( MathHelper::VectorDistance( c -> corner[0] -> position , sourceCorner[0] -> position ) > threshold ) 
					continue;
				//4個面
				for( int j = 0; j < 4; j++ )
				{
					if( MathHelper::CheckIntersectOnTriangle( initPoint ,
															  endPoint , 
															  c -> corner[ faceCorner[j][0] ] -> position ,
															  c -> corner[ faceCorner[j][1] ] -> position ,
															  c -> corner[ faceCorner[j][2] ] -> position ) )
					{
						isIntersected = true;
						std::cout << "新的樹支會插入其他Cone面" << std::endl;
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

		//先算出頭尾端位置
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
				//4個點
				for( int j = 0; j < 4 - 1; j++ )//三角錐的某一點
				{
					//4個點
					for( int k = j + 1; k < 4; k++ )//三角錐的另一點 他們連線
					{
						//跟數支比
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
								std::cout << "新的數枝與其他Cone的桿子交錯" << std::endl;
								return true;
							}
						}//這裡會有bug，當兩根之價重疊時這邊不會發現
					}
				}
			}
		}

		//看他有沒有跟其他的outercone竿子交錯
		for( OuterCone* o = sourceBranch -> sourceOuterCone -> firstOuterCone; o != NULL; o = o -> link )
		{
			if( o -> type == 0 ) continue;
			//別的cone得3個外支架
			for( int j = 0; j < 3; j++ )
			{
				//跟自己測距離
				if( !o -> sourceCorner[j] -> CloselyCompare( sourceCorner[i] ) &&
					!o -> sourceCorner[j] -> CloselyCompare( cornerEnd[i] )  )
				{
					if( MathHelper::CheckTwoLineDistance( o -> sourceCorner[j] -> position , 
														  o -> cornerEnd[j] -> position , 
														  initPoint , endPoint ) < TRUSS_RADIUS * 2.0 )
					{
						std::cout << "新的數枝與其他OuterCone的桿子交錯" << std::endl;
						isIntersected = true;
						return true;
					}
				}
				else if( MathHelper::CompareDirection( o -> sourceCorner[j] -> position ,
													   o -> cornerEnd[j] -> position ,
													   initPoint , 
													   endPoint ) )
				{
					std::cout << "新的數枝與其他OuterCone的桿子重疊" << std::endl;
					isIntersected = true;
					return true;
				}
			}
		}
	}
	//看他有沒有和其他的cone的桿子交錯
	return false;
}

bool BranchEnd::CheckIntersectWithBranchEnd()
{

	for( int i = 0; i < 2; i++ )
	{

		//先算出頭尾端位置
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
						std::cout << "新的數枝與其他樹枝的桿子交錯" << std::endl;
						return true;
					}
				}
				else if( MathHelper::CompareDirection( b -> sourceCorner[j] -> position ,
													   b -> cornerEnd[j] -> position ,
													   initPoint , 
													   endPoint ) )
				{
					std::cout << "新的數枝與其他樹枝的桿子重疊" << std::endl;
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
		std::cout << "第一根的末端位置：(" << cornerEnd[0] -> position[0] << "," <<
											  cornerEnd[0] -> position[1] << "," <<
											  cornerEnd[0] -> position[2] << ")，第二根的末端位置(" <<
											  cornerEnd[1] -> position[0] << "," <<
											  cornerEnd[1] -> position[1] << "," <<
											  cornerEnd[1] -> position[2] << ")" << std::endl;
	}


	for( int i = 0; i < 2; i++ )
	{
		//先算出頭尾端位置
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
			//如果不是同一個頂點，兩個相距也部會太遠
			if( ( !sourceCorner[i] -> CloselyCompare ( b -> sourceCorner[0] ) && 
				sourceCorner[i] -> DistanceToPoint( b -> sourceCorner[0] -> position ) < threshold ) &&
				!sourceCorner[i] -> CloselyCompare ( b -> sourceCorner[1] ) )//我的頭碰到別人尾
			{
				if(  MathHelper::CheckTwoLineDistance( b -> sourceCorner[0] -> position , 
													   b -> sourceCorner[1] -> position , 
													   initPoint , 
													   endPoint ) < TRUSS_RADIUS + SOCKET_RADIUS )
				{
					std::cout << "新的數枝末梢與其他樹枝交錯" << std::endl;
					isIntersected = true;
					return true;
				}
			}
			else if( MathHelper::CompareDirection( b -> sourceCorner[0] -> position ,
												   b -> sourceCorner[1] -> position ,
												   initPoint , 
												   endPoint ) )
			{
				std::cout << "新的數枝末梢與其他樹枝的桿子重疊" << std::endl;
				//isIntersected = true;
				return true;
			}
		}
	}
	return false;
}