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
	//�j�p�ܦ���I�]�t���r�l���A�N�@�w���|�]�t
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
					sameParameter++;//�o�@�q�O�ݬݦ��h�֭ӰѼƬO�@�˪��A�|�@��������ӥH�W���ѼơC
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
	return -1;//-1�N��S�����|
}

bool PolynomialList::DivideScalar()//��Ĥ@���ܦ�1
{
	if( polynomial.size() == 1 )
	{
		std::cout << "PolynomialList::DivideScalar�X��:�Ӧ��l�S����" << std::endl;
		return false;
	}

	float firstScalar;
	std::map< int , float >::iterator firstTerm = polynomial.begin();
	if( firstTerm -> first == -1 )
	{
		++firstTerm;//����X�Ĥ@���A�ѩ�̫e�����@���O�`�ơA�ҥH�n���L
	}
	if( firstTerm -> second == 0 )
	{
		std::cout << "PolynomialList::DivideScalar�X��:�Ĥ@���O0�A������" << polynomial.size() << "��" << std::endl;
		PrintPolynomialList();
		return false;
	}
	//�}�l���C�@��
	float divisor = 1.0 / firstTerm -> second;
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		//std::pair< int , float > tempPair( i -> first , i -> second * divisor );
		//polynomial.insert( tempPair );
		i -> second *= divisor;
	}
	return true;
}

bool PolynomialList::DivideScalar( int term )//��Ĥ@���ܦ�1
{
	if( polynomial.size() == 1 )
	{
		std::cout << "PolynomialList::DivideScalar�X��:�Ӧ��l�S����" << std::endl;
		return false;
	}

	if( polynomial.find( term ) == polynomial.end() )
	{
		std::cout << "PolynomialList::DivideScalar�X��:�Ӧ��l���S��term�o�@��" << std::endl;
		return false;
	}

	if( polynomial.find( term ) -> second == 0 )
	{
		std::cout << "PolynomialList::DivideScalar�X��:�ӫY�Ƭ�0" << std::endl;
		PrintPolynomialList();
		return false;
	}

	//�}�l���C�@��
	float divisor = 1.0 / polynomial.find( term ) -> second;
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		//std::pair< int , float > tempPair( i -> first , i -> second * divisor );
		//polynomial.insert( tempPair );
		i -> second *= divisor;
	}
	return true;
}

bool PolynomialList::Eliminate( PolynomialList* eliminated )//�N��Ӧh�����ۮ��A�p�G����ۮ��A�N�^��false�A
{
	std::map< int , float >::iterator firstTerm = polynomial.begin();
	if( firstTerm -> first == -1 ) firstTerm++;
	return Eliminate( eliminated , firstTerm -> first );
}

bool PolynomialList::Eliminate( PolynomialList* eliminated , int term )//�N��Ӧh�����ۮ��A�p�G����ۮ��A�N�^��false�A
{
	

	if( !DivideScalar( term ) ) 
	{
		std::cout << "PolynomialList::Eliminate�X��:�⥻����term���ܦ�1����" << std::endl;
		return false;//�����term���ܦ�1
	}

	if( !eliminated -> DivideScalar( term ) )
	{
		std::cout << "PolynomialList::Eliminate�X��:��Q���h����term���ܦ�1����" << std::endl;
		return false;//�����term���ܦ�1
	}

	//�����|���|�@�����ⶵ �|�N����
	int sameCount = 0;
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); i++ )
	{
		if( i -> first != -1 )
		{
			for( std::map< int , float >::iterator j = eliminated -> polynomial.begin(); j != eliminated -> polynomial.end(); j++ )
			{
				if( i -> first == j -> first && abs( i -> second - j -> second ) < 0.001 )//�ⶵ�ۦP
				{
					sameCount++;
				}
			}
		}
	}
	if( sameCount > 1 && polynomial.size() == 3 && eliminated -> polynomial.size() == 3 ) 
	{
		std::cout << "=======�⦡���ⶵ�ۦP�A�Ҥw�����h=========" << std::endl;
		PrintPolynomialList();
		eliminated -> PrintPolynomialList();
		std::cout << "==========================================" << std::endl;
		return false;
	}


	std::cout << "���h�e---------------vvv" << std::endl;
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
					--j;//�]���R���H��A�w�쾹j�|�ܦ��U�@�Ӧ�m�A�ҥH�n�A��^�ӡA�^��W�@�Ӧ�l
				}
			}
		}
	}
	std::cout << "���h��---------------vvv" << std::endl;

	if( eliminated -> polynomial.size() == 1 )
	{
		std::cout << "PolynomialList::Eliminate�X���G�����u�Ѥ@��" << std::endl;
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

int PolynomialList::GetOneUnknown()//�����窺������
{
	if( polynomial.size() != 2 )
	{
		std::cout << "PolynomialList::GetOneUnknown�X���G���W�L�@�ӥ�����" << std::endl;
		return -1;
	}

	std::map< int , float >::iterator term = polynomial.begin();//���Ы��V�`�ƶ�
	if( term -> first == -1 )
	{
		term++;//���Ы��V�Ĥ@��
	}

	return term -> first;
}

float PolynomialList::GetTermParameter( int term )
{
	if( polynomial.find( term ) == polynomial.end() )
	{
		std::cout << "PolynomialList::GetTermParameter�X���G�S���o��term" << std::endl;
		return -1;
	}
	else
	{
		return polynomial.find( term ) -> second;
	}
}

bool PolynomialList::PrintPolynomialList()
{
	std::cout << "��" << index << "���G";
	for( std::map< int , float >::iterator i = polynomial.begin(); i != polynomial.end(); ++i )
	{
		if( i -> first == -1 )
		{
			std::cout << i -> second;//��X�`�ƶ�
		}
		else
		{
			std::cout << i -> second << " * #(" << i -> first << ")";//��X��i�ڪ��Y��
		}
		if ( ++i != polynomial.end() )//�ݦo�O���O�̫�@��
		{
			std::cout << " + ";
		}
		--i;//������ܦ^�h
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
		std::cout << "PolynomialList::Substitute�X���G�Ӧ��èS����n�@��" << std::endl; 
		return false;
	}
	std::map< int , float >::iterator debugTerm = polynomial.begin();//���Ы��V�`�ƶ�
	if( debugTerm -> first == -1 )
	{
		debugTerm++;//���Ы��V�Ĥ@��
	}

	DivideScalar();//����t���ܦ�1�A�`�ƴN�O��
	std::map< int , float >::iterator firstTerm = polynomial.begin();//���Ы��V�`�ƶ�
	float solution = polynomial.at( -1 );
	if( firstTerm -> first == -1 )
	{
		firstTerm++;//���Ы��V�Ĥ@��
	}
	float unknown = firstTerm -> first;//���o�ܼƬ���


	if( unknown >= 0 )
	{
		std::cout << "�N��#(" << unknown << ")�ڱ�l�Ҩ����O" << solution << "�N�J�Ҧ����l��" << std::endl;
	}
	else
	{
		std::cout << "�N��#(" << unknown << ")�ӤϤO����" << solution << "�N�J�Ҧ����l��" << std::endl;
	}
	
	//��Ҧ����l���a�J�o��
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p == this ) continue;//���ΥN�J�ۤv
		if( p -> polynomial.find( unknown ) == p -> polynomial.end() ) continue;//�o�@�Ӧ��l���S���o�@��
		if( p -> polynomial.size() == 2 ) continue;

		

		std::map< int , float >::iterator tempTerm = p -> polynomial.find( unknown );//���n���N����
		float tempCoefficient = tempTerm -> second;//��X�L���Y��
		float constant = p -> polynomial.find( -1 ) -> second;//��X�L���`��
		//std::pair< int , float > constantTerm( -1 , constant - tempCoefficient * solution );//�p��o�Ӷ��Q�N�J��O�h��
		//p -> polynomial.insert( constantTerm );//�N�J�i�h
		p -> polynomial.find( -1 ) -> second = constant - tempCoefficient * solution;
		p -> polynomial.erase( p -> polynomial.find( unknown ) );//�����Q���N����

		std::cout << "�N��" << p -> index << "���N�J�ᦨ���G";
		p -> PrintPolynomialList();
	}

	return true;
}

bool PolynomialList::Replace( PolynomialList* replacer )
{
	int sameTerm = -1;//��X�n�M�䪺���ۦP���ܼ�
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
		std::cout << "PolynomialList::Replace�X��:�S����Ӧ��l���ۦP��" << std::endl;
		return false;
	}

	if( !DivideScalar( sameTerm ) ) 
	{
		std::cout << "PolynomialList::Replace�X��:�⥻����term���ܦ�1����" << std::endl;
		return false;//�����term���ܦ�1
	}

	if( !replacer -> DivideScalar( sameTerm ) )
	{
		std::cout << "PolynomialList::Replace�X��:��Q���h����term���ܦ�1����" << std::endl;
		return false;//�����term���ܦ�1
	}

	std::cout << "���h�e---------------vvv" << std::endl;
	std::cout << "�Q���������G";PrintPolynomialList();
	std::cout << "�������G";replacer -> PrintPolynomialList();

	polynomial.at( -1 ) -= replacer -> polynomial.at(-1);
	polynomial.erase( sameTerm );
	for( std::map< int , float >::iterator i = replacer -> polynomial.begin(); i != replacer -> polynomial.end(); ++i )
	{
		if( i -> first != -1 && i -> first != sameTerm )
		{
			polynomial.insert( std::pair< int , float >( i -> first , -i -> second ) );
		}
	}

	std::cout << "���h��---------------vvv" << std::endl;
	std::cout << "�Q���������G";PrintPolynomialList();
	std::cout << "�������G";replacer -> PrintPolynomialList();
	return true;
}

bool PolynomialList::OneSameUnknownAmongTwo( PolynomialList* p )
{
	if( p -> polynomial.size() == 3 && polynomial.size() == 3 )
	{
		/*std::cout << "debug:���S�����|---------------vvv" << std::endl;
		std::cout << "�Q���������G";PrintPolynomialList();
		std::cout << "�������G";p -> PrintPolynomialList();*/
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

	//��this�}�l �g�LchainPolynomial�A��p
	//chainPolynomial�̭��|�w���\�W��l��
	if( chainPolynomial -> size() == 0 )//���Ĥ@���I�s
	{
		if( debugSentence ) 
		{
			std::cout << "FindChainPolynomial�}�l�q";
			PrintPolynomialList();
			std::cout << "�n���୻�p����";
			p -> PrintPolynomialList();
		}

		chainPolynomial -> push_back( this );
		int tempTerm;//��X�n�M�䪺���ۦP���ܼ�
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
						if( debugSentence )std::cout << "�ܧ��A�h�^" << std::endl;
					}
				}
			}
		}
	}
	else
	{
		//if( debugSentence )std::cout << "FindChainPolynomial�M��";
		//chainPolynomial -> back() -> PrintPolynomialList();

		//�M��tempTerm�MlastTerm
		int tempTerm;//��X�n�M�䪺���ۦP���ܼ�
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
					if( p -> polynomial.find( j -> first ) == p -> polynomial.end() && j -> first != -1 )//�o�Ӷ��A�̫᪺���lp�S���A�B�o�Ӷ��]���O�`�ƶ�
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
						j -> first != -1 )//�o�Ӷ��A�e�@�Ӧ��li-1�S���A�B�o�Ӷ��]���O�`�ƶ�
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
				//���O�`�ƶ��A�o�Ӷ��]�S���b���Y�����l chainPolynomial(0) �X�{�L
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
						if( debugSentence )std::cout << "�ܧ��A�h�^" << std::endl;
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
		std::cout << "��" << solveTimes << "���Ѥ�{��" << std::endl;
		for( PolynomialList* p1 = this; p1 != NULL; p1 = p1 -> link )
		{
			if( p1 -> IsOneUnknown() ) //�Ȧ��@�ӥ�����
			{
				if( std::find( solvedConnections -> begin() , solvedConnections -> end() , p1 -> GetOneUnknown() ) == solvedConnections -> end() )
				{
					p1 -> Substitute();//�p�G�u�Ѥ@���A�N��o�ӸѥN�J�Ҧ����l��
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
				if( p2 -> IsOneUnknown() ) //�Ȧ��@�ӥ�����
				{
					if( std::find( solvedConnections -> begin() , solvedConnections -> end() , p2 -> GetOneUnknown() ) == solvedConnections -> end() )
					{
						p2 -> Substitute();//�p�G�u�Ѥ@���A�N��o�ӸѥN�J�Ҧ����l��
						solvedConnections -> push_back( p2 -> GetOneUnknown() );
						noMoreSolution = false;
						continue;
					}
				}
					
				////��p2�h���hp1�A�ҥHp2���Ҧ���p1���n���Ap1�]�tp2
				if( p1 -> Include( p2 ) )
				{//�]�t�� -> Include( �Q�]�t�� )
					if( p2 -> Eliminate( p1 ) )//p2�h����p1����
						noMoreSolution = false; //;//�p�G���h���\�F �~�|��@�O�����G
					//���h�� -> Eliminate( �Q���h�� )
				}
				
				//�o�q�����A���i�H�Ȯ��h���|�����A�n�C�@�����d��
				/*int overlappedTerm = p1 -> Overlap( p2 );//��ӬO�l�������|�A�N�⨺�Ӷ�����
				if( overlappedTerm != -1 )
				{
					p2 -> Eliminate( p1 , overlappedTerm );
					noMoreSolution = false;
				}*/
				//2/15�j�p�ҥ����ƪ��p�ߤ�{��
				//�T�{�O���O���@�ӥ����ƬۦP�A�@�ӥ����Ƥ��ۦP
				if( p1 -> OneSameUnknownAmongTwo( p2 ) )
				{
					//�n���ˬd�o�Ӧ��l���S�����ŹL
					bool ifMinimalist = true;
					for( PolynomialList* p3 = this; p3 != NULL; p3 = p3 -> link )
					{
						if( p3 -> polynomial.size() != 3 ) continue;//�u����פ@�˦�3���h����
						if( p1 -> Include( p3 ) && p3 -> Include( p1 ) && p3 != p1 )
						{//�]�t�� -> Include( �Q�]�t�� )
							ifMinimalist = false;
						}
					}
					if( ifMinimalist ) //===============================���U�Ӥ~�}�l�M��s��
					{
						std::vector< PolynomialList* >* chainPolynomial = new std::vector< PolynomialList* >();
						if( p1 -> FindChainPolynomial( chainPolynomial , p2 ) && !p1 -> IsOneUnknown() )
						{
							p1 -> ChainSubstitute( chainPolynomial , p2 );

							if( p2 -> Eliminate( p1 ) ) //p2�h����p1����
								noMoreSolution = false;//�p�G���h���\�F �~�|��@�O�����G
							//���h�� -> Eliminate( �Q���h�� )
							
						}
						delete chainPolynomial;
					}
				}
				if( p1 -> IsOneUnknown() && //�Ȧ��@�ӥ�����
					std::find( solvedConnections -> begin() , solvedConnections -> end() , p1 -> GetOneUnknown() ) == solvedConnections -> end() )//�o�Ӧ��l�٨S�N�J�L
				{
					p1 -> Substitute();//�p�G�u�Ѥ@���A�N��o�ӸѥN�J�Ҧ����l��
					solvedConnections -> push_back( p1 -> GetOneUnknown() );//�o��n��
					noMoreSolution = false;
				}
			}
		}

		for( PolynomialList* p = this; p != NULL; p = p -> link )
		{
			if( !p -> IsOneUnknown() && !p -> IsEmpty() )//�p�G������@�Ӧ��l�٨S�ѧ��A�N�~��while�j��
			{
				break;
			}
			if( p -> link == NULL )//�C�Ӧ��l���ѧ��F�N����while�j��
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
