#include "PolynomialList.h"



PolynomialList::PolynomialList()
{
	std::pair< int , float > constantTerm( -1 , 0.0f );
	polynomial.insert( constantTerm );
	isBall = false;
	isVirtualBall = false;
}

PolynomialList::PolynomialList( Eigen::SparseMatrix<float,Eigen::RowMajor>* m , Eigen::VectorXf* v )
{
	std::pair< int , float > constantTerm( -1 , (*v)(0) );
	polynomial.insert( constantTerm );
	firstPolynomial = this;
	for( int j = 0; j < m -> cols(); j++ )
	{
		if( m -> coeffRef(0,j) != 0 )
		{
			std::pair< int , float > tempPair( j , m -> coeffRef(0,j) );
			polynomial.insert( tempPair );
		}
	}
	

	int rowCount = 1;
	while( rowCount < m -> rows() )
	{
		if( m -> row(rowCount).nonZeros() > 0 )
		{
			PolynomialList* nextPoly =  new PolynomialList(m,v,rowCount);
			nextPoly -> AddToLink( this );
		}
		rowCount++;
	}
}

PolynomialList::PolynomialList( Eigen::SparseMatrix<float,Eigen::RowMajor>* m , Eigen::VectorXf* v , int row )
{
	std::pair< int , float > constantTerm( -1 , (*v)(row) );
	polynomial.insert( constantTerm );

	for( int j = 0; j < m -> cols(); j++ )
	{
		if( m -> coeffRef(row,j) != 0 )
		{
			std::pair< int , float > tempPair( j , m -> coeffRef(row,j) );
			polynomial.insert( tempPair );
		}
	}
}

PolynomialList::PolynomialList( ZomeStructure* b )
{
	std::pair< int , float > constantTerm( -1 , 0.0f );
	polynomial.insert( constantTerm );
	isBall = false;
	isVirtualBall = false;

	isBall = true;
	sourceBall = b;
}

PolynomialList::PolynomialList( VirtualBall* vBall )
{
	std::pair< int , float > constantTerm( -1 , 0.0f );
	polynomial.insert( constantTerm );
	isBall = false;
	isVirtualBall = false;

	isVirtualBall = true;
	sourceVirtualBall = vBall;
}

PolynomialList::PolynomialList( int connIndex , float solution )
{
	std::pair< int , float > constantTerm( -1 , -solution );
	polynomial.insert( constantTerm );
	std::pair< int , float > SolvedParameter( connIndex , 1.0 );
	polynomial.insert( SolvedParameter );
	isBall = false;
	isVirtualBall = false;
}

bool PolynomialList::Include( PolynomialList* included )
{
	//大小眉有比背包含的字子長，就一定不會包含
	if( polynomial.size() < included -> polynomial.size() )
		return false;

	int sameParameter = 0;
	for( std::map< int , float >::iterator i = included -> polynomial.begin(); i != included -> polynomial.end(); ++i )
	{
		bool isIncluded = false;
		for( std::map< int , float >::iterator j = polynomial.begin(); j != polynomial.end(); ++j )
		{
			if( i -> first == j -> first )
			{
				isIncluded = true;
				if( i -> second == j -> second )
				{
					sameParameter++;//這一段是看看有多少個參數是一樣的，會一次消掉兩個以上的參數。
				}
			}
		}
		if( !isIncluded )
		{
			return false;
		}
	}
	if( sameParameter > 1 ) return true;
	return true;
}

int PolynomialList::Overlap( PolynomialList* overlapped )
{
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); ++i )
	{
		for( std::map< int , float >::iterator j = overlapped -> polynomial.begin(); j != overlapped -> polynomial.end(); ++j )
		{
			if( i -> first == j -> first && i -> first != -1 )
			{
				return i -> first;
			}
		}
	}
	return -1;//-1代表沒有重疊
}

bool PolynomialList::DivideScalar()//把第一項變成1
{
	if( polynomial.size() == 1 )
	{
		std::cout << "PolynomialList::DivideScalar出錯:該式子沒有項" << std::endl;
		return false;
	}

	float firstScalar;
	std::map< int , float >::iterator firstTerm = polynomial.begin();
	if( firstTerm -> first == -1 )
	{
		++firstTerm;//先找出第一項，由於最前面的一項是常數，所以要跳過
	}
	if( firstTerm -> second == 0 )
	{
		std::cout << "PolynomialList::DivideScalar出錯:第一項是0，本式有" << polynomial.size() << "項" << std::endl;
		PrintPolynomialList();
		return false;
	}
	//開始除每一項
	float divisor = 1.0 / firstTerm -> second;
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		//std::pair< int , float > tempPair( i -> first , i -> second * divisor );
		//polynomial.insert( tempPair );
		i -> second *= divisor;
	}
	return true;
}

bool PolynomialList::DivideScalar( int term )//把第一項變成1
{
	if( polynomial.size() == 1 )
	{
		std::cout << "PolynomialList::DivideScalar出錯:該式子沒有項" << std::endl;
		return false;
	}

	if( polynomial.find( term ) == polynomial.end() )
	{
		std::cout << "PolynomialList::DivideScalar出錯:該式子中沒有term這一項" << std::endl;
		return false;
	}

	if( polynomial.find( term ) -> second == 0 )
	{
		std::cout << "PolynomialList::DivideScalar出錯:該係數為0" << std::endl;
		PrintPolynomialList();
		return false;
	}

	//開始除每一項
	float divisor = 1.0 / polynomial.find( term ) -> second;
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		//std::pair< int , float > tempPair( i -> first , i -> second * divisor );
		//polynomial.insert( tempPair );
		i -> second *= divisor;
	}
	return true;
}

bool PolynomialList::Eliminate( PolynomialList* eliminated )//將兩個多項式相消，如果不能相消，就回傳false，
{
	std::map< int , float >::iterator firstTerm = polynomial.begin();
	if( firstTerm -> first == -1 ) firstTerm++;
	return Eliminate( eliminated , firstTerm -> first );
}

bool PolynomialList::Eliminate( PolynomialList* eliminated , int term )//將兩個多項式相消，如果不能相消，就回傳false，
{
	

	if( !DivideScalar( term ) ) 
	{
		std::cout << "PolynomialList::Eliminate出錯:把本式第term項變成1失敗" << std::endl;
		return false;//先把第term項變成1
	}

	if( !eliminated -> DivideScalar( term ) )
	{
		std::cout << "PolynomialList::Eliminate出錯:把被消去式第term項變成1失敗" << std::endl;
		return false;//先把第term項變成1
	}

	//測式會不會一次消兩項 會就取消
	int sameCount = 0;
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		if( i -> first != -1 )
		{
			for( std::map< int , float >::iterator j = eliminated -> polynomial.begin(); j != eliminated -> polynomial.end(); j++ )
			{
				if( i -> first == j -> first && abs( i -> second - j -> second ) < 0.001 )//兩項相同
				{
					sameCount++;
				}
			}
		}
	}
	if( sameCount > 1 && polynomial.size() == 3 && eliminated -> polynomial.size() == 3 ) 
	{
		std::cout << "=======兩式有兩項相同，所已不消去=========" << std::endl;
		PrintPolynomialList();
		eliminated -> PrintPolynomialList();
		std::cout << "==========================================" << std::endl;
		return false;
	}


	std::cout << "消去前---------------vvv" << std::endl;
	PrintPolynomialList();
	eliminated -> PrintPolynomialList();

	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); ++i )
	{
		for( std::map< int , float >::iterator j = eliminated -> polynomial.begin(); j != eliminated -> polynomial.end(); ++j )
		{
			if( i -> first == j -> first )
			{
				//std::pair< int , float > tempPair( i -> first , j -> second - i -> second );
				//eliminated -> polynomial.insert( tempPair );
				j -> second -= i -> second;
				if( abs( j -> second ) < SMALL_NUMBER && i -> first != -1 )
				{
					j = eliminated -> polynomial.erase( j );
					--j;//因為刪掉以後，定位器j會變成下一個位置，所以要再減回來，回到上一個位子
				}
			}
		}
	}
	std::cout << "消去後---------------vvv" << std::endl;

	if( eliminated -> polynomial.size() == 1 )
	{
		std::cout << "PolynomialList::Eliminate出錯：消完只剩一項" << std::endl;
	}

	PrintPolynomialList();
	eliminated -> PrintPolynomialList();
	return true;
}

bool PolynomialList::IsOneUnknown()
{
	if( polynomial.size() == 2 )
	{
		return true;
	}
	else return false;
}

bool PolynomialList::IsEmpty()
{
	if( polynomial.size() == 1 )
	{
		return true;
	}
	else return false;
}

int PolynomialList::GetOneUnknown()//拿為衣的未知數
{
	if( polynomial.size() != 2 )
	{
		std::cout << "PolynomialList::GetOneUnknown出錯：有超過一個未知數" << std::endl;
		return -1;
	}

	std::map< int , float >::iterator term = polynomial.begin();//指標指向常數項
	if( term -> first == -1 )
	{
		term++;//指標指向第一項
	}

	return term -> first;
}

float PolynomialList::GetTermParameter( int term )
{
	if( polynomial.find( term ) == polynomial.end() )
	{
		std::cout << "PolynomialList::GetTermParameter出錯：沒有這個term" << std::endl;
		return -1;
	}
	else
	{
		return polynomial.find( term ) -> second;
	}
}

bool PolynomialList::PrintPolynomialList()
{
	std::cout << "第" << index << "式：";
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); ++i )
	{
		if( i -> first == -1 )
		{
			std::cout << i -> second;//輸出常數項
		}
		else
		{
			std::cout << i -> second << " * #(" << i -> first << ")";//輸出第i根的係數
		}
		if ( ++i != polynomial.end() )//看她是不是最後一項
		{
			std::cout << " + ";
		}
		--i;//把指標變回去
	}
	std::cout << " = 0" << std::endl; 
	return true;
}

bool PolynomialList::PrintAllPolynomialList()
{
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		p -> PrintPolynomialList();
	}
	return true;
}

bool PolynomialList::Substitute()
{
	if( polynomial.size() != 2 )
	{
		std::cout << "PolynomialList::Substitute出錯：該式並沒有剛好一項" << std::endl; 
		return false;
	}
	std::map< int , float >::iterator debugTerm = polynomial.begin();//指標指向常數項
	if( debugTerm -> first == -1 )
	{
		debugTerm++;//指標指向第一項
	}

	DivideScalar();//先把系數變成1，常數就是解
	std::map< int , float >::iterator firstTerm = polynomial.begin();//指標指向常數項
	float solution = polynomial.at( -1 );
	if( firstTerm -> first == -1 )
	{
		firstTerm++;//指標指向第一項
	}
	float unknown = firstTerm -> first;//取得變數為何


	if( unknown >= 0 )
	{
		std::cout << "將第#(" << unknown << ")根桿子所受的力" << solution << "代入所有式子中" << std::endl;
	}
	else
	{
		std::cout << "將第#(" << unknown << ")個反力的解" << solution << "代入所有式子中" << std::endl;
	}
	
	//把所有式子都帶入這項
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p == this ) continue;//不用代入自己
		if( p -> polynomial.find( unknown ) == p -> polynomial.end() ) continue;//這一個式子中沒有這一項
		if( p -> polynomial.size() == 2 ) continue;

		

		std::map< int , float >::iterator tempTerm = p -> polynomial.find( unknown );//找到要取代的項
		float tempCoefficient = tempTerm -> second;//抓出他的係數
		float constant = p -> polynomial.find( -1 ) -> second;//抓出他的常數
		//std::pair< int , float > constantTerm( -1 , constant - tempCoefficient * solution );//計算這個項被代入後是多少
		//p -> polynomial.insert( constantTerm );//代入進去
		p -> polynomial.find( -1 ) -> second = constant - tempCoefficient * solution;
		p -> polynomial.erase( p -> polynomial.find( unknown ) );//消掉被取代的項

		std::cout << "將第" << p -> index << "式代入後成為：";
		p -> PrintPolynomialList();
	}

	return true;
}

bool PolynomialList::Replace( PolynomialList* replacer )
{
	int sameTerm = -1;//找出要尋找的不相同的變數
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		if( i -> first == -1 ) continue;
		if( replacer -> polynomial.find( i -> first ) != replacer -> polynomial.end() )
		{
			sameTerm = i -> first;
		}
	}
	if( sameTerm == -1 )
	{
		std::cout << "PolynomialList::Replace出錯:沒找到兩個式子的相同項" << std::endl;
		return false;
	}

	if( !DivideScalar( sameTerm ) ) 
	{
		std::cout << "PolynomialList::Replace出錯:把本式第term項變成1失敗" << std::endl;
		return false;//先把第term項變成1
	}

	if( !replacer -> DivideScalar( sameTerm ) )
	{
		std::cout << "PolynomialList::Replace出錯:把被消去式第term項變成1失敗" << std::endl;
		return false;//先把第term項變成1
	}

	std::cout << "消去前---------------vvv" << std::endl;
	std::cout << "被替換的式：";PrintPolynomialList();
	std::cout << "替換式：";replacer -> PrintPolynomialList();

	polynomial.at( -1 ) -= replacer -> polynomial.at(-1);
	polynomial.erase( sameTerm );
	for( std::map< int , float >::iterator i = replacer -> polynomial.begin(); i != replacer -> polynomial.end(); ++i )
	{
		if( i -> first != -1 && i -> first != sameTerm )
		{
			polynomial.insert( std::pair< int , float >( i -> first , -i -> second ) );
		}
	}

	std::cout << "消去後---------------vvv" << std::endl;
	std::cout << "被替換的式：";PrintPolynomialList();
	std::cout << "替換式：";replacer -> PrintPolynomialList();
	return true;
}

bool PolynomialList::OneSameUnknownAmongTwo( PolynomialList* p )
{
	if( p -> polynomial.size() == 3 && polynomial.size() == 3 )
	{
		/*std::cout << "debug:有沒有重疊---------------vvv" << std::endl;
		std::cout << "被替換的式：";PrintPolynomialList();
		std::cout << "替換式：";p -> PrintPolynomialList();*/
		int matchCount = 0;
		for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
		{
			if( i -> first == -1 ) continue;
			for( std::map< int , float >::iterator j = p -> polynomial.begin(); j != p -> polynomial.end(); j++ )
			{
				if( i -> first == j -> first ) matchCount++;
			}
		}
		if( matchCount == 1 ) return true;
		else return false;
	}
	else 
	{
		return false;
	}
}

bool PolynomialList::FindChainPolynomial( std::vector< PolynomialList* >* chainPolynomial , PolynomialList* p )
{
	bool ifFoundLast = false;
	bool debugSentence = false;

	//由this開始 經過chainPolynomial，到p
	//chainPolynomial裡面會預先擺上原始的
	if( chainPolynomial -> size() == 0 )//式第一次呼叫
	{
		if( debugSentence ) 
		{
			std::cout << "FindChainPolynomial開始從";
			PrintPolynomialList();
			std::cout << "要找到能香聯尾端";
			p -> PrintPolynomialList();
		}

		chainPolynomial -> push_back( this );
		int tempTerm;//找出要尋找的不相同的變數
		int lastTerm;
		for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
		{
			if( i -> first == -1 ) continue;
			if( p -> polynomial.find( i -> first ) == p -> polynomial.end() )
			{
				tempTerm = i -> first;
			}
		}

		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); i++ )
		{
			if( i -> first != -1 && polynomial.find( i -> first ) ==  polynomial.end() )
			{
				lastTerm = i -> first;
			}
		}
		for( PolynomialList* p2 = firstPolynomial; p2 != NULL; p2 = p2 -> link )
		{
			if( p2 -> polynomial.size() != 3 ) continue;
			if( p2 == this ) continue;
			if( p2 == p ) continue;
			if( p2 -> polynomial.find(tempTerm) != p2 -> polynomial.end() && !Include( p2 ) )
			{
				if( p2 -> polynomial.find(lastTerm) != p2 -> polynomial.end() )
				{
					ifFoundLast = true;
					chainPolynomial -> push_back( p2 );
					return true;
				}
				else
				{
					chainPolynomial -> push_back( p2 );
					if( FindChainPolynomial( chainPolynomial , p ) )
					{
						return true;
					}
					else
					{
						chainPolynomial -> pop_back();
						if( debugSentence )std::cout << "眉找到，退回" << std::endl;
					}
				}
			}
		}
	}
	else
	{
		//if( debugSentence )std::cout << "FindChainPolynomial尋找";
		//chainPolynomial -> back() -> PrintPolynomialList();

		//尋找tempTerm和lastTerm
		int tempTerm;//找出要尋找的不相同的變數
		int lastTerm;
		PolynomialList* tempPolynomial = chainPolynomial -> back();
		for( int i = 0; i < chainPolynomial -> size(); i++ )
		{
			if( i == 0 )
			{
				for( std::map< int , float >::iterator j = chainPolynomial -> at(i) -> polynomial.begin(); 
					 j != chainPolynomial -> at(i) -> polynomial.end(); 
					 j++ )
				{
					if( p -> polynomial.find( j -> first ) == p -> polynomial.end() && j -> first != -1 )//這個項，最後的式子p沒有，且這個項也不是常數項
					{
						tempTerm = j -> first;
					}
				}
			}
			else
			{
				for( std::map< int , float >::iterator j = chainPolynomial -> at(i) -> polynomial.begin(); 
					 j != chainPolynomial -> at(i) -> polynomial.end(); 
					 j++ )
				{
					if( chainPolynomial -> at(i-1) -> polynomial.find( j -> first ) == chainPolynomial -> at(i-1) -> polynomial.end() && 
						j -> first != -1 )//這個項，前一個式子i-1沒有，且這個項也不是常數項
					{
						tempTerm = j -> first;
					}
				}
			}
		}

		for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); i++ )
		{
			if( i -> first != -1 && 
				chainPolynomial -> at(0) -> polynomial.find( i -> first ) == chainPolynomial -> at(0) -> polynomial.end() )
				//不是常數項，這個項也沒有在最頭的式子 chainPolynomial(0) 出現過
			{
				lastTerm = i -> first;
			}
		}
		for( PolynomialList* p2 = firstPolynomial; p2 != NULL; p2 = p2 -> link )
		{
			if( p2 -> polynomial.size() != 3 ) continue;
			if( p2 == p ) continue;
			bool ifDuplicated = false;
			for( std::vector< PolynomialList* >::iterator i =  chainPolynomial -> begin(); i != chainPolynomial -> end(); i++ )
			{
				if( p2 == *i ) ifDuplicated = true;
			}
			if( ifDuplicated ) continue;
			if( p2 -> polynomial.find(tempTerm) != p2 -> polynomial.end() && !p2 -> Include( tempPolynomial ) )
			{
				if( p2 -> polynomial.find(lastTerm) != p2 -> polynomial.end() )
				{
					ifFoundLast = true;
					chainPolynomial -> push_back( p2 );
					return true;
				}
				else
				{
					chainPolynomial -> push_back( p2 );
					if( FindChainPolynomial( chainPolynomial , p ) )
					{
						return true;
					}
					else
					{
						chainPolynomial -> pop_back();
						if( debugSentence )std::cout << "眉找到，退回" << std::endl;
					}
				}
			}
		}
	}

	return false;
}

bool PolynomialList::ChainSubstitute( std::vector< PolynomialList* >* chainPolynomial , PolynomialList* p )
{
	PolynomialList* chainHead = chainPolynomial -> at(0);
	for( std::vector< PolynomialList* >::iterator pChain = chainPolynomial -> begin(); pChain != chainPolynomial -> end(); pChain++ )
	{
		if( *pChain == chainHead )continue;
		chainHead -> Replace( *pChain );
	}
	return true;
}

bool PolynomialList::HasUnknown( int term )
{
	if( polynomial.find( term ) == polynomial.end() )
		return false;
	else
		return true;
}

bool PolynomialList::SolvePolynomialList()
{
	bool solutionDone = false;
	bool noMoreSolution = false;
	int solveTimes = 0;
	//std::vector< int > solvedUnknown;
	while( !solutionDone && !noMoreSolution )
	{
		noMoreSolution = true;
		std::cout << "第" << solveTimes << "次解方程式" << std::endl;
		for( PolynomialList* p1 = this; p1 != NULL; p1 = p1 -> link )
		{
			if( p1 -> IsOneUnknown() ) //紙有一個未知數
			{
				if( std::find( solvedConnections -> begin() , solvedConnections -> end() , p1 -> GetOneUnknown() ) == solvedConnections -> end() )
				{
					p1 -> Substitute();//如果只剩一項，就把這個解代入所有式子中
					solvedConnections -> push_back( p1 -> GetOneUnknown() );
					noMoreSolution = false;
					continue;
				}
				else
					continue;
			}

			for( PolynomialList* p2 = this; p2 != NULL; p2 = p2 -> link )
			{
				if( p1 == p2 ) continue;
				if( p2 -> polynomial.size() == 1 ) continue;
				if( p2 -> IsOneUnknown() ) //紙有一個未知數
				{
					if( std::find( solvedConnections -> begin() , solvedConnections -> end() , p2 -> GetOneUnknown() ) == solvedConnections -> end() )
					{
						p2 -> Substitute();//如果只剩一項，就把這個解代入所有式子中
						solvedConnections -> push_back( p2 -> GetOneUnknown() );
						noMoreSolution = false;
						continue;
					}
				}
					
				////用p2去消去p1，所以p2的所有項p1都要有，p1包含p2
				if( p1 -> Include( p2 ) )
				{//包含者 -> Include( 被包含者 )
					if( p2 -> Eliminate( p1 ) )//p2去消掉p1的項
						noMoreSolution = false; //;//如果消去成功了 才會當作是有結果
					//消去者 -> Eliminate( 被消去者 )
				}
				
				//這段有錯，不可以紙消去重疊的項，要每一項都削掉
				/*int overlappedTerm = p1 -> Overlap( p2 );//兩個是子有項重疊，就把那個項消掉
				if( overlappedTerm != -1 )
				{
					p2 -> Eliminate( p1 , overlappedTerm );
					noMoreSolution = false;
				}*/
				//2/15姐聯所未知數的聯立方程式
				//確認是布是有一個未知數相同，一個未知數不相同
				if( p1 -> OneSameUnknownAmongTwo( p2 ) )
				{
					//要先檢查這個式子有沒有消剪過
					bool ifMinimalist = true;
					for( PolynomialList* p3 = this; p3 != NULL; p3 = p3 -> link )
					{
						if( p3 -> polynomial.size() != 3 ) continue;//只找長度一樣式3的多項式
						if( p1 -> Include( p3 ) && p3 -> Include( p1 ) && p3 != p1 )
						{//包含者 -> Include( 被包含者 )
							ifMinimalist = false;
						}
					}
					if( ifMinimalist ) //===============================接下來才開始尋找連續
					{
						std::vector< PolynomialList* >* chainPolynomial = new std::vector< PolynomialList* >();
						if( p1 -> FindChainPolynomial( chainPolynomial , p2 ) && !p1 -> IsOneUnknown() )
						{
							p1 -> ChainSubstitute( chainPolynomial , p2 );

							if( p2 -> Eliminate( p1 ) ) //p2去消掉p1的項
								noMoreSolution = false;//如果消去成功了 才會當作是有結果
							//消去者 -> Eliminate( 被消去者 )
							
						}
						delete chainPolynomial;
					}
				}
				if( p1 -> IsOneUnknown() && //紙有一個未知數
					std::find( solvedConnections -> begin() , solvedConnections -> end() , p1 -> GetOneUnknown() ) == solvedConnections -> end() )//這個式子還沒代入過
				{
					p1 -> Substitute();//如果只剩一項，就把這個解代入所有式子中
					solvedConnections -> push_back( p1 -> GetOneUnknown() );//這邊要改
					noMoreSolution = false;
				}
			}
		}

		for( PolynomialList* p = this; p != NULL; p = p -> link )
		{
			if( !p -> IsOneUnknown() && !p -> IsEmpty() )//如果有任何一個式子還沒解完，就繼續while迴圈
			{
				break;
			}
			if( p -> link == NULL )//每個式子都解完了就結束while迴圈
			{
				solutionDone = true;
			}
		}
		solveTimes++;
	}
	return true;
}

PolynomialList* PolynomialList::AddToLink( PolynomialList* fPolynomial )
{
	if( fPolynomial == NULL )
	{
		firstPolynomial = this;
		index = 0;
		solvedConnections = new std::vector<int>();
	}
	else
	{
				
		for( PolynomialList* p = fPolynomial; p != NULL; p = p -> link )
		{
			if( p -> link == NULL )
			{
				firstPolynomial = fPolynomial;
				index = p -> index + 1;
				p -> link = this;
				break;
			}
		}
		solvedConnections = firstPolynomial -> solvedConnections;
	}
	return firstPolynomial;
}
