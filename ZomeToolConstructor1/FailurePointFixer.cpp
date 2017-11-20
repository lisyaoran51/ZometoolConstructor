#include "FailurePointFixer.h"



FailurePoint::FailurePoint( std::vector< Cone* >* gA , std::vector< Cone* >* gB , Point* ePoint , FailurePoint* fFailurePoint )
{
	groupA = gA;
	groupB = gB;

	engagedPoint.push_back( ePoint );


	for( int i = 0; i < groupA -> size(); i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			if( groupA -> at(i) -> corner[j] -> CloselyCompare( ePoint -> position ) )
			{
				coneFromA.push_back( groupA -> at(i) );
			}
		}
	}

	for( int i = 0; i < groupB -> size(); i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			if( groupB -> at(i) -> corner[j] -> CloselyCompare( ePoint -> position ) )
			{
				coneFromB.push_back( groupB -> at(i) );
			}
		}
	}
	//看看有沒有第二個接處點

	for( int i = 0; i < coneFromA.size(); i++ )//ijijijij
	{
		for( int k = 0; k < coneFromB.size(); k++ )//klklklkl
		{
			for( int j = 0; j < 4; j++ )//ijijijij
			{
				if( coneFromA.at(i) -> corner[j] -> CloselyCompare( ePoint -> position ) ) continue;

				for( int l = 0; l < 4; l++ )//klklklkl
				{
					//if( coneFromB.at(k) -> corner[l] -> CloselyCompare( ePoint -> position ) ) continue;
					
					if( coneFromA.at(i) -> corner[j] -> CloselyCompare( coneFromB.at(k) -> corner[l] -> position ) )
					{
						if( engagedPoint.size() == 2 )
						{
							if( !(coneFromA.at(i) -> corner[j] -> CloselyCompare( engagedPoint.at(1) -> position )) )
							{
								std::cout << "FailurePoint::FailurePoint出錯：找到三個接觸點" << std::endl;
							}
						}
						//2/3修改，改成如果有兩個接觸點，就只留那兩個cone

						//把這兩個cone記下來
						Cone* tempConeA = coneFromA.at(i);
						Cone* tempConeB = coneFromB.at(k);
						//把A渠和B群青空
						coneFromA.clear();
						coneFromB.clear();
						//加入這兩個Cone
						coneFromA.push_back( tempConeA );
						coneFromB.push_back( tempConeB );

						engagedPoint.push_back( tempConeA -> corner[j] );
						


						std::cout << "找到第二個接觸點，兩個點為(" << ePoint -> position[0] << "," 
								  << ePoint -> position[1] << "," << ePoint -> position[2] << ")和(" 
								  << tempConeA -> corner[j] -> position[0] << ","
								  << tempConeA -> corner[j] -> position[1] << ","
								  << tempConeA -> corner[j] -> position[2] << ")" << std::endl;
						break;

					}

				}
				if( engagedPoint.size() == 2 ) break;
			}
			if( engagedPoint.size() == 2 ) break;
		}
		if( engagedPoint.size() == 2 ) break;
	}

	if( fFailurePoint == NULL )
	{
		firstFailurePoint = this;
	}
	else
	{
		firstFailurePoint = fFailurePoint;
		for( FailurePoint* fp = firstFailurePoint; fp != NULL; fp = fp -> link )
		{
			if( fp -> link == NULL )
			{
				fp -> link = this;
				break;
			}
		}
	}
}


//=============================================以上式point結構===========================================
//=============================================以下式fixer===============================================

FailurePointFixer::FailurePointFixer( Cone* fCone )
{
	firstCone = fCone;

	totalGroup = new std::vector< std:: vector< Cone* >* >();

	for( Cone* c = fCone; c != NULL; c = c -> link )
	{
		if( c -> activated == true )
		{
			std::vector< Cone* >* tempGroup = new std::vector< Cone* >();
			tempGroup -> push_back( c );
			totalGroup -> push_back( tempGroup );

			std::cout << "建立新的團：三角錐(" << c -> position[0] << "," << c -> position[1] << "," << c -> position[2] << ")" << std::endl;

		}
	}
	
	std::cout << "總共" << totalGroup -> size() << "個團" << std::endl;
}

bool FailurePointFixer::Combine( std::vector< std::vector< Cone* >* >* tGroup , int inserted , int inserter )
{
	for( int i = 0; i < tGroup -> at( inserter ) -> size(); i++ )
	{
		tGroup -> at( inserted ) -> push_back( tGroup -> at( inserter ) -> at(i) );
	}
	
	std::vector< Cone* >* deletedGroup = tGroup -> at( inserter );

	//現在要刪除原本的group
	std::vector< std::vector< Cone* >* >::iterator tempGroup = tGroup -> begin();
	for( int i = 0; i < inserter; i++ ) tempGroup++;
	tGroup -> erase( tempGroup );

	delete deletedGroup;

	return true;
}

bool FailurePointFixer::Assort()
{
	
	for( int i = 0; i < totalGroup -> size(); i++ ) 
	{
		for( int j = 0; j < totalGroup -> at(i) -> size(); j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				//如果這個相鄰錐不存在或是沒有啟動，就跳過
				if( totalGroup -> at(i) -> at(j) -> neighborCone[k] == NULL ) continue;
				if( totalGroup -> at(i) -> at(j) -> neighborCone[k] -> activated == false ) continue;

				
				//ijk式第一個cone
				//========================================================================
				bool combineDone = false;
				for( int i2 = 0; i2 < totalGroup -> size(); i2++ )
				{
					if( totalGroup -> at(i) == totalGroup -> at(i2) ) continue;
					for( int j2 = 0; j2 < totalGroup -> at(i2) -> size(); j2++ )
					{
						//i2j2式第二個cone，拿來和ijk比對，式同一個的話，就把兩群何體
						if( totalGroup -> at(i) -> at(j) -> neighborCone[k] == totalGroup -> at(i2) -> at(j2) )
						{
							//涼個式相連的群，何體
							Combine( totalGroup , i , i2 );
							i2--;
							if( i2 < i ) i--;//因為這一個群已經被削掉，所以要把指標往後退一個
							combineDone = true;//結合後直接跳出這個迴圈
							break;
						}

					}
					if( combineDone ) break;//結合後直接跳出這個迴圈
				}
				//========================================================================
			}
			
		}

	}
	//debug
	for( int i = 0; i < totalGroup -> size(); i++ ) 
	{
		if( totalGroup -> at(i) -> size() == 1 )
		{
			std::cout << "分類實錯誤的三角錐位置在(" 
					  << totalGroup -> at(i) -> at(0) -> position[0] << "," <<
						   totalGroup -> at(i) -> at(0) -> position[1] << "," <<
						   totalGroup -> at(i) -> at(0) -> position[2] << ")" << std::endl;
		}
		else
		{
			std::cout << "正常群的三角錐位置在";
			for( int j = 0; j < totalGroup -> at(i) -> size(); j++ )
			{
				std::cout << "(" 
					  << totalGroup -> at(i) -> at(j) -> position[0] << "," <<
						 totalGroup -> at(i) -> at(j) -> position[1] << "," <<
						 totalGroup -> at(i) -> at(j) -> position[2] << ") ";
			}
			std::cout << std::endl;
		}
	}
	/*
	GLfloat debugPos[3] = { 0 , 11.8249998 , -47.2999992 };
	for( int i = 0; i < totalGroup -> size(); i++ ) 
	{
		for( int j = 0; j < totalGroup -> at(i) -> size(); j++ )
		{
			if( MathHelper::VectorApproximation( totalGroup -> at(i) -> at(j) -> position , debugPos ) )
			{
				Cone* tempCone = totalGroup -> at(i) -> at(j);
				std::cout << "debug:找到位於錯誤三角錐" << std::endl;
				std::cout << "相鄰三角錐1:(" << tempCone -> neighborCone[0] -> position[0] << "," 
						  << tempCone -> neighborCone[0] -> position[1] << "," 
						  << tempCone -> neighborCone[0] -> position[2] << ")" << std::endl;
				std::cout << "相鄰三角錐2:(" << tempCone -> neighborCone[1] -> position[0] << "," 
						  << tempCone -> neighborCone[1] -> position[1] << "," 
						  << tempCone -> neighborCone[1] -> position[2] << ")" << std::endl;
				std::cout << "相鄰三角錐3:(" << tempCone -> neighborCone[2] -> position[0] << "," 
						  << tempCone -> neighborCone[2] -> position[1] << "," 
						  << tempCone -> neighborCone[2] -> position[2] << ")" << std::endl;
				std::cout << "相鄰三角錐4:(" << tempCone -> neighborCone[3] -> position[0] << "," 
						  << tempCone -> neighborCone[3] -> position[1] << "," 
						  << tempCone -> neighborCone[3] -> position[2] << ")" << std::endl;
			}
		}
	}
	GLfloat debugPos2[3] = { 0 , 0 , -35.4750023 };
	for( Cone* c = firstCone; c != NULL; c = c -> link )
	{
		if( MathHelper::VectorApproximation( c -> position , debugPos2 ) )
		{
			std::cout << "debug:找到位於錯誤三角錐" << std::endl;
		}
	}
	*/
	return true;
}

bool FailurePointFixer::FindFailurePoint()
{
	if( totalGroup -> size() == 1 ) return false;

	for( int i = 0; i < totalGroup -> size(); i++ )//第i群穩定三角錐群
	{
		for( int l = i+1; l < totalGroup -> size(); l++ )//第l群穩定三角錐群
		{
			//第i群和第l群座比較

			for( int j = 0; j < totalGroup -> at(i) -> size(); j++ )//第j顆三角錐
			{
				for( int m = 0; m < totalGroup -> at(l) -> size(); m++ )//第m顆三角錐
				{
					if( MathHelper::VectorDistance( totalGroup -> at(i) -> at(j) -> corner[0] -> position ,
													totalGroup -> at(l) -> at(m) -> corner[0] -> position ) > SCALE * 2.1 )//scale長度式藍色短的
					{
						continue;//這個代表j三角椎和m三角錐的距離太遠，連比對都不用
					}

					std::vector<int> coneJRecognized;
					std::vector<int> coneMRecognized;


					//這邊一個一個角去對
					for( int k = 0; k < 4; k++ )//三角錐的第k個角
					{
						for( int n = 0; n < 4; n++ )//三角錐的第n個角
						{
							if( MathHelper::VectorApproximation( totalGroup -> at(i) -> at(j) -> corner[k] -> position ,
																 totalGroup -> at(l) -> at(m) -> corner[n] -> position ) )
							{
								//找到破壞點
								coneJRecognized.push_back( k );//第k個角和第n個角香童位置
								coneMRecognized.push_back( n );
								std::cout << "找到第" << i << "群和第" << l << "群的接觸點!" << std::endl;
							}
						}
					}
					bool duplicated = false;
					if( coneJRecognized.size() >= 1 )
					{
						//看看這個角式不是有在別的FailurePoint裡面重複過，重複過救職接加到那個failurePoint裡
						for( FailurePoint* fp = firstFailurePoint; fp != NULL; fp = fp -> link )
						{
							
							if( fp -> engagedPoint.at(0) -> CloselyCompare( totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] ) )
							{
								//這個點已經存在現有的破壞點內了，所以把他門合起來
								//修改 如果一有重複，救職接跳過--------為什麼>?? 因為建構子救會職接把他的cone全部找出來
								duplicated = true;
								break;
								/*
								//將第i群第j個三角錐，加入破壞點中，這時要看這個錐是屬於coneFromA還是coneFromB
								if( InTheSameGroup( totalGroup -> at(i) -> at(j) , groupA ) )
								{
									//看這個三角錐是否已經存在在這個三角錐群中，如果沒有的話，在加入
									bool alreadyExistInGroup = false;
									for( int c = 0; c < fp -> coneFromA -> size(); c++ )
									{

									}
								}*/
							}
							else if( fp -> engagedPoint.size() == 2 )
							{
								if( fp -> engagedPoint.at(1) -> CloselyCompare( totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] ) )
								{
									duplicated = true;
									break;
								}
							}
						}

						if( !duplicated )
						{
							//這一個破壞點未在其他破壞點中重複，所以要新建一個破壞點
							std::cout << "找到新的破壞點：(" << totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] -> position[0] << ","
									  << totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] -> position[1] << ","
									  << totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] -> position[2] << ")" << std::endl;
							
							FailurePoint* tempFailurePoint = new FailurePoint( totalGroup -> at(i) , //A群
																			   totalGroup -> at(l) , //B群
																			   totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] , //接觸點
																			   firstFailurePoint );//第一個破壞點
							if( firstFailurePoint == NULL ) firstFailurePoint = tempFailurePoint;
							std::cout << "上述破壞點有" << tempFailurePoint -> engagedPoint.size() << "個接觸點" << std::endl;
						}
					}
				}
			}
		}
	}

	std::cout << "==========================列出所有破壞點結果==========================" << std::endl;
	for( FailurePoint* fp = firstFailurePoint; fp != NULL; fp = fp -> link )
	{
		int a = -1;
		int b = -1;
		for( int i = 0; i < totalGroup -> size(); i++ )
		{
			if( totalGroup -> at(i) == fp -> groupA ) a = i;
			if( totalGroup -> at(i) == fp -> groupB ) b = i;
		}
		
		std::cout << "此破壞點來自第" << a << "和" << b << "，有" << fp -> engagedPoint.size() << "個接觸點" << std::endl;
	}

	return true;
}

bool FailurePointFixer::ExistInTheGroup( Cone* cone , std::vector< Cone* >* group )
{
	for( int i = 0; i < group -> size(); i++ )
	{
		if( group -> at(i) == cone )
		{
			return true;
		}
	}
	return false;
}

bool FailurePointFixer::CheckIfNeighbor( Cone* coneA , Cone* coneB )
{
	for( int i = 0; i < 4; i++ )
	{
		if( coneA -> neighborCone[i] == coneB )
		{
			return true;
		}
	}
	return false;
}

bool FailurePointFixer::TryFillCone()
{
	if( totalGroup -> size() == 1 ) return true;

	for( FailurePoint * fp = firstFailurePoint; fp != NULL; fp = fp -> link )
	{
		if( fp -> engagedPoint.size() == 1 )
		{
			std::cout << "現在要尋找接觸點(" << fp -> engagedPoint.at(0) -> position[0] << ","
					  << fp -> engagedPoint.at(0) -> position[0] << ","
					  << fp -> engagedPoint.at(0) -> position[0] << ")的補滿" << std::endl;
		}
		else if ( fp -> engagedPoint.size() == 2 )
		{
			std::cout << "現在要尋找接觸點(" << fp -> engagedPoint.at(0) -> position[0] << ","
					  << fp -> engagedPoint.at(0) -> position[1] << ","
					  << fp -> engagedPoint.at(0) -> position[2] << ")和(" 
					  << fp -> engagedPoint.at(1) -> position[0] << ","
					  << fp -> engagedPoint.at(1) -> position[1] << ","
					  << fp -> engagedPoint.at(1) -> position[2] << ")和的補滿" << std::endl; 
		}
		else
		{
			std::cout << "FailurePointFixer::TryFillCone出錯：" << std::endl;
		}
		
		//========================================只補一個三角錐//========================================
		bool fillUpFound = false;
		for( int i = 0; i < fp -> coneFromA.size(); i++ )
		{
			std::cout << "現在要尋找三角錐(" << fp -> coneFromA.at(i) -> position[0] << ","
		  			  << fp -> coneFromA.at(i) -> position[1] << ","
		  			  << fp -> coneFromA.at(i) -> position[2] << ")和另一群" << std::endl << "---";
		for( int c = 0; c < fp -> coneFromB.size(); c++ )
		{
		  	std::cout << "(" << fp -> coneFromB.at(c) -> position[0] << ","
		  			  << fp -> coneFromB.at(c) -> position[1] << ","
		  			  << fp -> coneFromB.at(c) -> position[2] << ")";
		}
		std::cout << "的連接方法" << std::endl;

			for( int j = 0; j < 4; j++ )
			{
				if( fp -> coneFromA.at(i) -> neighborCone[j] == NULL ) continue;
				if( fp -> coneFromA.at(i) -> neighborCone[j] -> activated == false )
				{
					std::cout << "測試(" << fp -> coneFromA.at(i) -> neighborCone[j] -> position[0] << ","
		  					  << fp -> coneFromA.at(i) -> neighborCone[j] -> position[1] << ","
		  					  << fp -> coneFromA.at(i) -> neighborCone[j] -> position[2] << ")是否可以連接這兩個三角錐" << std::endl;
				
					for( int k = 0; k < 4; k++ )
					{
						if( ExistInTheGroup( fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] , fp -> groupB ) )
						{
							std::cout << "找到填補三角錐(" << fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] -> position[0] << ","
									  << fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] -> position[1] << ","
									  << fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] -> position[2] << ")" << std::endl;
							fillUpFound = true;
							fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] -> activated = true;
							break;
						}
					}
				}
				if( fillUpFound ) break;
			}
			if( fillUpFound ) break;
		}
		//========================================補兩個三角錐//========================================
		if( !fillUpFound )
		{
			std::cout << "此破壞點的接觸點只有1個，並且沒辦法用一個cone就把他門連接在一起" << std::endl;
			for( int i = 0; i < fp -> coneFromA.size(); i++ )//A群中的第i個三角錐
			{
				for( int l = 0; l < fp -> coneFromB.size(); l++ )//B群中的第l個三角錐
				{
					for( int j = 0; j < 4; j++ )//A群中的第i個三角錐的第j個鄰錐
					{
						if( fp -> coneFromA.at(i) -> neighborCone[j] == NULL ) continue;
						if( fp -> coneFromA.at(i) -> neighborCone[j] -> activated == true ) continue;
						//如果這個相鄰錐j已經存在，就跳過

						for( int m = 0; m < 4; m++ )
						{
							if( fp -> coneFromB.at(l) -> neighborCone[m] == NULL ) continue;
							if( fp -> coneFromB.at(l) -> neighborCone[m] -> activated == true ) continue;
							//如果這個相鄰錐m已經存在，就跳過

							if( CheckIfNeighbor( fp -> coneFromA.at(i) -> neighborCone[j] , fp -> coneFromB.at(l) -> neighborCone[m] ) )
							{
								std::cout << "找到兩個填補三角錐(" << fp -> coneFromA.at(i) -> neighborCone[j] -> position[0] << ","
									  << fp -> coneFromA.at(i) -> neighborCone[j] -> position[1] << ","
									  << fp -> coneFromA.at(i) -> neighborCone[j] -> position[2] << "),(" 
									  << fp -> coneFromA.at(l) -> neighborCone[m] -> position[0] << ","
									  << fp -> coneFromA.at(l) -> neighborCone[m] -> position[1] << ","
									  << fp -> coneFromA.at(l) -> neighborCone[m] -> position[2] << ")" << std::endl;
								fillUpFound = true;
								fp -> coneFromA.at(i) -> neighborCone[j] -> activated = true;
								fp -> coneFromB.at(l) -> neighborCone[m] -> activated = true;
								break;
							}
						}
						if( fillUpFound ) break;
					}
					if( fillUpFound ) break;
				}
				if( fillUpFound ) break;
			}
		}
		else if( !fillUpFound && fp -> engagedPoint.size() != 1 )
		{
			std::cout << "FailurePointFixer::TryFillCone出錯：找不到可以填補起來的方法，且接觸點有" << fp -> engagedPoint.size()
					  << "個" << std::endl;
		}


		if( !fillUpFound ) 
		{
			std::cout << "FailurePointFixer::TryFillCone出錯：找不到可以填補起來的方法" << std::endl;
		}
	}
	return true;
}

