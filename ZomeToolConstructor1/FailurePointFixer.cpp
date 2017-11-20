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
	//�ݬݦ��S���ĤG�ӱ��B�I

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
								std::cout << "FailurePoint::FailurePoint�X���G���T�ӱ�Ĳ�I" << std::endl;
							}
						}
						//2/3�ק�A�令�p�G����ӱ�Ĳ�I�A�N�u�d�����cone

						//��o���cone�O�U��
						Cone* tempConeA = coneFromA.at(i);
						Cone* tempConeB = coneFromB.at(k);
						//��A��MB�s�C��
						coneFromA.clear();
						coneFromB.clear();
						//�[�J�o���Cone
						coneFromA.push_back( tempConeA );
						coneFromB.push_back( tempConeB );

						engagedPoint.push_back( tempConeA -> corner[j] );
						


						std::cout << "���ĤG�ӱ�Ĳ�I�A����I��(" << ePoint -> position[0] << "," 
								  << ePoint -> position[1] << "," << ePoint -> position[2] << ")�M(" 
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


//=============================================�H�W��point���c===========================================
//=============================================�H�U��fixer===============================================

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

			std::cout << "�إ߷s���ΡG�T���@(" << c -> position[0] << "," << c -> position[1] << "," << c -> position[2] << ")" << std::endl;

		}
	}
	
	std::cout << "�`�@" << totalGroup -> size() << "�ӹ�" << std::endl;
}

bool FailurePointFixer::Combine( std::vector< std::vector< Cone* >* >* tGroup , int inserted , int inserter )
{
	for( int i = 0; i < tGroup -> at( inserter ) -> size(); i++ )
	{
		tGroup -> at( inserted ) -> push_back( tGroup -> at( inserter ) -> at(i) );
	}
	
	std::vector< Cone* >* deletedGroup = tGroup -> at( inserter );

	//�{�b�n�R���쥻��group
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
				//�p�G�o�Ӭ۾F�@���s�b�άO�S���ҰʡA�N���L
				if( totalGroup -> at(i) -> at(j) -> neighborCone[k] == NULL ) continue;
				if( totalGroup -> at(i) -> at(j) -> neighborCone[k] -> activated == false ) continue;

				
				//ijk���Ĥ@��cone
				//========================================================================
				bool combineDone = false;
				for( int i2 = 0; i2 < totalGroup -> size(); i2++ )
				{
					if( totalGroup -> at(i) == totalGroup -> at(i2) ) continue;
					for( int j2 = 0; j2 < totalGroup -> at(i2) -> size(); j2++ )
					{
						//i2j2���ĤG��cone�A���өMijk���A���P�@�Ӫ��ܡA�N���s����
						if( totalGroup -> at(i) -> at(j) -> neighborCone[k] == totalGroup -> at(i2) -> at(j2) )
						{
							//�D�Ӧ��۳s���s�A����
							Combine( totalGroup , i , i2 );
							i2--;
							if( i2 < i ) i--;//�]���o�@�Ӹs�w�g�Q�d���A�ҥH�n����Щ���h�@��
							combineDone = true;//���X�᪽�����X�o�Ӱj��
							break;
						}

					}
					if( combineDone ) break;//���X�᪽�����X�o�Ӱj��
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
			std::cout << "��������~���T���@��m�b(" 
					  << totalGroup -> at(i) -> at(0) -> position[0] << "," <<
						   totalGroup -> at(i) -> at(0) -> position[1] << "," <<
						   totalGroup -> at(i) -> at(0) -> position[2] << ")" << std::endl;
		}
		else
		{
			std::cout << "���`�s���T���@��m�b";
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
				std::cout << "debug:�������~�T���@" << std::endl;
				std::cout << "�۾F�T���@1:(" << tempCone -> neighborCone[0] -> position[0] << "," 
						  << tempCone -> neighborCone[0] -> position[1] << "," 
						  << tempCone -> neighborCone[0] -> position[2] << ")" << std::endl;
				std::cout << "�۾F�T���@2:(" << tempCone -> neighborCone[1] -> position[0] << "," 
						  << tempCone -> neighborCone[1] -> position[1] << "," 
						  << tempCone -> neighborCone[1] -> position[2] << ")" << std::endl;
				std::cout << "�۾F�T���@3:(" << tempCone -> neighborCone[2] -> position[0] << "," 
						  << tempCone -> neighborCone[2] -> position[1] << "," 
						  << tempCone -> neighborCone[2] -> position[2] << ")" << std::endl;
				std::cout << "�۾F�T���@4:(" << tempCone -> neighborCone[3] -> position[0] << "," 
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
			std::cout << "debug:�������~�T���@" << std::endl;
		}
	}
	*/
	return true;
}

bool FailurePointFixer::FindFailurePoint()
{
	if( totalGroup -> size() == 1 ) return false;

	for( int i = 0; i < totalGroup -> size(); i++ )//��i�sí�w�T���@�s
	{
		for( int l = i+1; l < totalGroup -> size(); l++ )//��l�sí�w�T���@�s
		{
			//��i�s�M��l�s�y���

			for( int j = 0; j < totalGroup -> at(i) -> size(); j++ )//��j���T���@
			{
				for( int m = 0; m < totalGroup -> at(l) -> size(); m++ )//��m���T���@
				{
					if( MathHelper::VectorDistance( totalGroup -> at(i) -> at(j) -> corner[0] -> position ,
													totalGroup -> at(l) -> at(m) -> corner[0] -> position ) > SCALE * 2.1 )//scale���צ��Ŧ�u��
					{
						continue;//�o�ӥN��j�T���թMm�T���@���Z���ӻ��A�s��ﳣ����
					}

					std::vector<int> coneJRecognized;
					std::vector<int> coneMRecognized;


					//�o��@�Ӥ@�Ө��h��
					for( int k = 0; k < 4; k++ )//�T���@����k�Ө�
					{
						for( int n = 0; n < 4; n++ )//�T���@����n�Ө�
						{
							if( MathHelper::VectorApproximation( totalGroup -> at(i) -> at(j) -> corner[k] -> position ,
																 totalGroup -> at(l) -> at(m) -> corner[n] -> position ) )
							{
								//���}�a�I
								coneJRecognized.push_back( k );//��k�Ө��M��n�Ө�������m
								coneMRecognized.push_back( n );
								std::cout << "����" << i << "�s�M��" << l << "�s����Ĳ�I!" << std::endl;
							}
						}
					}
					bool duplicated = false;
					if( coneJRecognized.size() >= 1 )
					{
						//�ݬݳo�Ө������O���b�O��FailurePoint�̭����ƹL�A���ƹL��¾���[�쨺��failurePoint��
						for( FailurePoint* fp = firstFailurePoint; fp != NULL; fp = fp -> link )
						{
							
							if( fp -> engagedPoint.at(0) -> CloselyCompare( totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] ) )
							{
								//�o���I�w�g�s�b�{�����}�a�I���F�A�ҥH��L���X�_��
								//�ק� �p�G�@�����ơA��¾�����L--------������>?? �]���غc�l�Ϸ|¾����L��cone������X��
								duplicated = true;
								break;
								/*
								//�N��i�s��j�ӤT���@�A�[�J�}�a�I���A�o�ɭn�ݳo���@�O�ݩ�coneFromA�٬OconeFromB
								if( InTheSameGroup( totalGroup -> at(i) -> at(j) , groupA ) )
								{
									//�ݳo�ӤT���@�O�_�w�g�s�b�b�o�ӤT���@�s���A�p�G�S�����ܡA�b�[�J
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
							//�o�@�ӯ}�a�I���b��L�}�a�I�����ơA�ҥH�n�s�ؤ@�ӯ}�a�I
							std::cout << "���s���}�a�I�G(" << totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] -> position[0] << ","
									  << totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] -> position[1] << ","
									  << totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] -> position[2] << ")" << std::endl;
							
							FailurePoint* tempFailurePoint = new FailurePoint( totalGroup -> at(i) , //A�s
																			   totalGroup -> at(l) , //B�s
																			   totalGroup -> at(i) -> at(j) -> corner[ coneJRecognized.at(0) ] , //��Ĳ�I
																			   firstFailurePoint );//�Ĥ@�ӯ}�a�I
							if( firstFailurePoint == NULL ) firstFailurePoint = tempFailurePoint;
							std::cout << "�W�z�}�a�I��" << tempFailurePoint -> engagedPoint.size() << "�ӱ�Ĳ�I" << std::endl;
						}
					}
				}
			}
		}
	}

	std::cout << "==========================�C�X�Ҧ��}�a�I���G==========================" << std::endl;
	for( FailurePoint* fp = firstFailurePoint; fp != NULL; fp = fp -> link )
	{
		int a = -1;
		int b = -1;
		for( int i = 0; i < totalGroup -> size(); i++ )
		{
			if( totalGroup -> at(i) == fp -> groupA ) a = i;
			if( totalGroup -> at(i) == fp -> groupB ) b = i;
		}
		
		std::cout << "���}�a�I�Ӧ۲�" << a << "�M" << b << "�A��" << fp -> engagedPoint.size() << "�ӱ�Ĳ�I" << std::endl;
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
			std::cout << "�{�b�n�M�䱵Ĳ�I(" << fp -> engagedPoint.at(0) -> position[0] << ","
					  << fp -> engagedPoint.at(0) -> position[0] << ","
					  << fp -> engagedPoint.at(0) -> position[0] << ")���ɺ�" << std::endl;
		}
		else if ( fp -> engagedPoint.size() == 2 )
		{
			std::cout << "�{�b�n�M�䱵Ĳ�I(" << fp -> engagedPoint.at(0) -> position[0] << ","
					  << fp -> engagedPoint.at(0) -> position[1] << ","
					  << fp -> engagedPoint.at(0) -> position[2] << ")�M(" 
					  << fp -> engagedPoint.at(1) -> position[0] << ","
					  << fp -> engagedPoint.at(1) -> position[1] << ","
					  << fp -> engagedPoint.at(1) -> position[2] << ")�M���ɺ�" << std::endl; 
		}
		else
		{
			std::cout << "FailurePointFixer::TryFillCone�X���G" << std::endl;
		}
		
		//========================================�u�ɤ@�ӤT���@//========================================
		bool fillUpFound = false;
		for( int i = 0; i < fp -> coneFromA.size(); i++ )
		{
			std::cout << "�{�b�n�M��T���@(" << fp -> coneFromA.at(i) -> position[0] << ","
		  			  << fp -> coneFromA.at(i) -> position[1] << ","
		  			  << fp -> coneFromA.at(i) -> position[2] << ")�M�t�@�s" << std::endl << "---";
		for( int c = 0; c < fp -> coneFromB.size(); c++ )
		{
		  	std::cout << "(" << fp -> coneFromB.at(c) -> position[0] << ","
		  			  << fp -> coneFromB.at(c) -> position[1] << ","
		  			  << fp -> coneFromB.at(c) -> position[2] << ")";
		}
		std::cout << "���s����k" << std::endl;

			for( int j = 0; j < 4; j++ )
			{
				if( fp -> coneFromA.at(i) -> neighborCone[j] == NULL ) continue;
				if( fp -> coneFromA.at(i) -> neighborCone[j] -> activated == false )
				{
					std::cout << "����(" << fp -> coneFromA.at(i) -> neighborCone[j] -> position[0] << ","
		  					  << fp -> coneFromA.at(i) -> neighborCone[j] -> position[1] << ","
		  					  << fp -> coneFromA.at(i) -> neighborCone[j] -> position[2] << ")�O�_�i�H�s���o��ӤT���@" << std::endl;
				
					for( int k = 0; k < 4; k++ )
					{
						if( ExistInTheGroup( fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] , fp -> groupB ) )
						{
							std::cout << "����ɤT���@(" << fp -> coneFromA.at(i) -> neighborCone[j] -> neighborCone[k] -> position[0] << ","
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
		//========================================�ɨ�ӤT���@//========================================
		if( !fillUpFound )
		{
			std::cout << "���}�a�I����Ĳ�I�u��1�ӡA�åB�S��k�Τ@��cone�N��L���s���b�@�_" << std::endl;
			for( int i = 0; i < fp -> coneFromA.size(); i++ )//A�s������i�ӤT���@
			{
				for( int l = 0; l < fp -> coneFromB.size(); l++ )//B�s������l�ӤT���@
				{
					for( int j = 0; j < 4; j++ )//A�s������i�ӤT���@����j�ӾF�@
					{
						if( fp -> coneFromA.at(i) -> neighborCone[j] == NULL ) continue;
						if( fp -> coneFromA.at(i) -> neighborCone[j] -> activated == true ) continue;
						//�p�G�o�Ӭ۾F�@j�w�g�s�b�A�N���L

						for( int m = 0; m < 4; m++ )
						{
							if( fp -> coneFromB.at(l) -> neighborCone[m] == NULL ) continue;
							if( fp -> coneFromB.at(l) -> neighborCone[m] -> activated == true ) continue;
							//�p�G�o�Ӭ۾F�@m�w�g�s�b�A�N���L

							if( CheckIfNeighbor( fp -> coneFromA.at(i) -> neighborCone[j] , fp -> coneFromB.at(l) -> neighborCone[m] ) )
							{
								std::cout << "����Ӷ�ɤT���@(" << fp -> coneFromA.at(i) -> neighborCone[j] -> position[0] << ","
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
			std::cout << "FailurePointFixer::TryFillCone�X���G�䤣��i�H��ɰ_�Ӫ���k�A�B��Ĳ�I��" << fp -> engagedPoint.size()
					  << "��" << std::endl;
		}


		if( !fillUpFound ) 
		{
			std::cout << "FailurePointFixer::TryFillCone�X���G�䤣��i�H��ɰ_�Ӫ���k" << std::endl;
		}
	}
	return true;
}

