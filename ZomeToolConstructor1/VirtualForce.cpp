#include "VirtualForce.h"

/*int index;
	int color;
	int size;
	float scale;
	float cosAlpha;
	bool isSolved;
	float solution;*/

//=====================================Relation//=====================================
Relation::Relation( int rConnIndex , float coef , float cons )
{
	relatedConnIndex = rConnIndex;
	coefficient = coef;
	constant = cons;
}
//=====================================Relation//=====================================


//=====================================ConnectionData//=====================================��l���
ConnectionData::ConnectionData()
{};

ConnectionData::ConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection )
{
	index = i;
	sourceBall = sBall;
	isBall = true;
	connection = oConnection -> GetConn( i );
	//��ballFace
	ballFace = -1;
	for( int fi = 0; fi < 62; fi++ )
	{
		if( sourceBall -> faceConnection[ fi ] != NULL )
		{
			if( sourceBall -> faceConnection[ fi ] == connection )
			{
				ballFace = fi;
			}
		}
	}
	if( ballFace == -1 )
	{
		std::cout << "ConnectionData::ConnectionData�X���G�S�����Ӷ�yballFace" << std::endl;
	}


	//cosAlpha = cAlpha;

	color = connection -> color;
	size = connection -> size;
	//�H�U�Ӧ�ZomeDirection��GetColorLength()
	GLfloat length;
	if ( color == COLOR_BLUE )
	{
		if ( size == SIZE_S ) length = 1 * SCALE;//46.3
		if ( size == SIZE_M ) length = GOLDEN * SCALE;//74.9
		if ( size == SIZE_L ) length = GOLDEN * GOLDEN * SCALE;//121.2
	}
	else if ( color == COLOR_RED )
	{
		if ( size == SIZE_S ) length = sqrt( 2 + GOLDEN ) / 2 * SCALE;//43.8
		if ( size == SIZE_M ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * SCALE;//71
		if ( size == SIZE_L ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * GOLDEN * SCALE;//115
	}
	else if ( color == COLOR_YELLOW )
	{
		if ( size == SIZE_S ) length = sqrt(3) / 2 * SCALE;//40.1
		if ( size == SIZE_M ) length = sqrt(3) / 2 * GOLDEN * SCALE;//64.8
		if ( size == SIZE_L ) length = sqrt(3) / 2 * GOLDEN * GOLDEN * SCALE;//104.9
	}
	//=========
	scale = length / SCALE;
	isSolved = false;

}

ConnectionData::ConnectionData( int i , VirtualBall* sVirtualBall , ZomeConnection* oConnection )
{
	/*==================================================��
	||�s�������y����l��ơA���i��O�ϤO�A�ҥH�n�S�O�P�_||
	*///================================================��

	index = i;
	sourceVirtualBall = sVirtualBall;
	isBall = false;
	ballFace = -1;

	//���U�ӧP�_�o�Ӭ�l�O���O�N��ϤO�A�O���ܥL��index�|<0
	if( i < -1 )
	{
		//�o�O�@�ӤϤO�A���O��l
		connection = NULL;
		color = COLOR_BLUE;
		size = SIZE_S;
	}
	else
	{
		connection = oConnection -> GetConn( i );
		color = connection -> color;
		size = connection -> size;

		
		//��ballFace
		for( int fi = 0; fi < 6; fi++ )
		{
			if( sourceVirtualBall -> conn[ fi ] != NULL )
			{
				if(  sourceVirtualBall -> conn[ fi ] == connection )
				{
					ballFace = sourceVirtualBall -> connectFace[fi];
				}
			}
		}
		if( ballFace == -1 )
		{
			std::cout << "ConnectionData::ConnectionData�X���G�S�����Ӷ�yballFace" << std::endl;
		}
	}

	

	//cosAlpha = cAlpha;

	//�H�U�Ӧ�ZomeDirection��GetColorLength()
	GLfloat length;
	if ( color == COLOR_BLUE )
	{
		if ( size == SIZE_S ) length = 1 * SCALE;//46.3
		if ( size == SIZE_M ) length = GOLDEN * SCALE;//74.9
		if ( size == SIZE_L ) length = GOLDEN * GOLDEN * SCALE;//121.2
	}
	else if ( color == COLOR_RED )
	{
		if ( size == SIZE_S ) length = sqrt( 2 + GOLDEN ) / 2 * SCALE;//43.8
		if ( size == SIZE_M ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * SCALE;//71
		if ( size == SIZE_L ) length = sqrt( 2 + GOLDEN ) / 2 * GOLDEN * GOLDEN * SCALE;//115
	}
	else if ( color == COLOR_YELLOW )
	{
		if ( size == SIZE_S ) length = sqrt(3) / 2 * SCALE;//40.1
		if ( size == SIZE_M ) length = sqrt(3) / 2 * GOLDEN * SCALE;//64.8
		if ( size == SIZE_L ) length = sqrt(3) / 2 * GOLDEN * GOLDEN * SCALE;//104.9
	}
	//=========
	scale = length / SCALE;
	isSolved = false;

}
//=====================================ConnectionData//=====================================


//=====================================PolynomialConnectionData//=====================================��l�h���O���
PolynomialConnectionData::PolynomialConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection , float c ):
ConnectionData( i , sBall , oConnection ) , coefficient( c )
{}

PolynomialConnectionData::PolynomialConnectionData( int i , VirtualBall* sVirtualBall , ZomeConnection* oConnection , float c ):
ConnectionData( i , sVirtualBall, oConnection ) , coefficient( c )
{}
//=====================================PolynomialConnectionData//=====================================


//=====================================VirtualWorkConnectionData//=====================================���l�ݨѸ��
VirtualWorkConnectionData::VirtualWorkConnectionData( int i , ZomeStructure* sBall , ZomeConnection* oConnection ): ConnectionData( i , sBall , oConnection )
{
	cosAlpha = 0;
}

VirtualWorkConnectionData::VirtualWorkConnectionData( PolynomialConnectionData pConnectionData )
{

	index = pConnectionData .index;
	isBall = pConnectionData.isBall;

	if( isBall )
	{
		sourceBall = pConnectionData.sourceBall;
	}
	else
	{
		sourceVirtualBall = pConnectionData.sourceVirtualBall;
	}
	connection = pConnectionData.connection;
	//��ballFace
	ballFace = pConnectionData.ballFace;
	color = pConnectionData.color;
	size = pConnectionData.size;
	scale = pConnectionData.scale;
	isSolved = false;

	cosAlpha = 0;
}

bool VirtualWorkConnectionData::ComputeCosAlpha( GLfloat* outerForceDirection )
{
	//cosine�����O (V1 Dot V2)/(V1����xV2����)

	//(BallFace��V)
	//�u   F (�~�O��V)
	//  \  ^
	//   \ |
	//    \|
	//     o
	//  �������ӭn�OballFace���Ϥ�V�P�~�O�������A���O�~�|�P�~�O�c��

	GLfloat innerForceDirection[3];
	if( ballFace >= 0 )
	{
		StaticZomeDirection::GetFaceDirection( ballFace , innerForceDirection );//���趵
	}
	else
	{
		MathHelper::SetVector( innerForceDirection , 0 , -1 , 0 );
	}
	//MathHelper::VectorReverse( innerForceDirection );//���V����

	cosAlpha = MathHelper::VectorDot( innerForceDirection , outerForceDirection ) / 
			   MathHelper::VectorAbsolution( innerForceDirection ) /
			   MathHelper::VectorAbsolution( outerForceDirection );//cosine�����O (V1 Dot V2)/(V1����xV2����)

	return true;
}

//=====================================VirtualWorkConnectionData//=====================================



//=====================================BallVirtualForceData//=====================================�y�ݨѸ��
BallVirtualForceData::BallVirtualForceData( PolynomialList* pDimension1 , PolynomialList* pDimension2 , PolynomialList* pDimension3 , ZomeStructure* b , ZomeConnection* oConnection )
{
	/*int ballIndex;
	std::vector<ConnectionData> connections[3];//�T�����Ҧ���󰵥\���
	float displacement[3];//�T��������
	float OuterForce[3];//�T�쪺�~�O
	bool solved;*/


	
	isBall = true;
	ball = b;
	originalConn = oConnection;
	ballIndex = ball -> index;
	solved = false;
	
	std::cout << "�إ߲�" << ballIndex << "���y���h�����y���" << std::endl;

	//��Ҧ����s����J�i�h======
	for( std::map< int , float >::iterator i = pDimension1 -> polynomial.begin(); i != pDimension1 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension2 -> polynomial.begin(); i != pDimension2 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension3 -> polynomial.begin(); i != pDimension3 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 )connectionNum.insert( i -> first );
	}
	//=============================

	//��O��0����
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension1 -> polynomial.find( *i ) == pDimension1 -> polynomial.end() )//�p�G�h���O�S���A�N�إߤ@��cosA��0��ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , pDimension1 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
	}
	//��O��1����
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension2 -> polynomial.find( *i ) == pDimension2 -> polynomial.end() )//�p�G�h���O�S���A�N�إߤ@��cosA��0��ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , pDimension2 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
	}
	//��O��2����
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension3 -> polynomial.find( *i ) == pDimension3 -> polynomial.end() )//�p�G�h���O�S���A�N�إߤ@��cosA��0��ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , ball , originalConn , pDimension3 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
	}

	//��J�T�����~�O
	///......

}

BallVirtualForceData::BallVirtualForceData( PolynomialList* pDimension1 , PolynomialList* pDimension2 , PolynomialList* pDimension3 , VirtualBall* vBall , ZomeConnection* oConnection )
{
	/*int ballIndex;
	std::vector<ConnectionData> connections[3];//�T�����Ҧ���󰵥\���
	float displacement[3];//�T��������
	float OuterForce[3];//�T�쪺�~�O
	bool solved;*/
	
	isBall = false;
	virtualBall = vBall;
	originalConn = oConnection;
	ballIndex = virtualBall -> index;
	solved = false;
	
	std::cout << "�إ߲�" << ballIndex << "�������y���h�����y���" << std::endl;

	//��Ҧ����s����J�i�h======
	for( std::map< int , float >::iterator i = pDimension1 -> polynomial.begin(); i != pDimension1 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 || i -> first < -1 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension2 -> polynomial.begin(); i != pDimension2 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 || i -> first < -1 )connectionNum.insert( i -> first );
	}
	for( std::map< int , float >::iterator i = pDimension3 -> polynomial.begin(); i != pDimension3 -> polynomial.end(); i++ )
	{
		if( i -> first >= 0 || i -> first < -1 )connectionNum.insert( i -> first );
	}
	//=============================

	//��O��0����
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension1 -> polynomial.find( *i ) == pDimension1 -> polynomial.end() )//�p�G�h���O�S���A�N�إߤ@��cosA��0��ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , pDimension1 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[0].push_back( tempPolynomialConnectionData );
		}
	}
	//��O��1����
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension2 -> polynomial.find( *i ) == pDimension2 -> polynomial.end() )//�p�G�h���O�S���A�N�إߤ@��cosA��0��ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , pDimension2 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[1].push_back( tempPolynomialConnectionData );
		}
	}
	//��O��2����
	for( std::set<int>::iterator i = connectionNum.begin(); i != connectionNum.end(); i++ )
	{
		if( pDimension3 -> polynomial.find( *i ) == pDimension3 -> polynomial.end() )//�p�G�h���O�S���A�N�إߤ@��cosA��0��ConnectionData
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , 0 );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
		else
		{
			//index color size scale cosAlpha isSoved solution
			PolynomialConnectionData tempPolynomialConnectionData( *i , virtualBall , originalConn , pDimension3 -> polynomial.find( *i ) -> second );//index ,originalConn ,cosAlpha
			connections[2].push_back( tempPolynomialConnectionData );
		}
	}

	//��J�T�����~�O
	///......

}

bool BallVirtualForceData::MakeVirtualWorkConnectionData()
{
	//�������Ǭ�l�٨S���ѡA�٦���h���O����ƿ�J���\�k��l��Ʋz
	
	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( !connections[0].at(j).isSolved ) 
		{
			virtualWorkConnections.push_back( VirtualWorkConnectionData( connections[0].at(j) ) );
		}
	}


	return true;
}

int BallVirtualForceData::GetVirtualWorkConnectionIndexFromConnIndex( int connIndex )
{
	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		if( virtualWorkConnections[i].index == connIndex )
		{
			return i;
		}
	}
	std::cout << "BallVirtualForceData::GetVirtualWorkConnectionIndexFromConnIndex�X���G�䤣���\�k��l�����ӽs��" << std::endl;
	return -1;

}

VirtualWorkConnectionData* BallVirtualForceData::GetVirtualWorkConnection( int index )
{
	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		if( virtualWorkConnections[i].index == index )
		{
			return &(virtualWorkConnections[i]);
		}
	}
}

bool BallVirtualForceData::ComputeCosAlpha()
{
	//��X�~�O�X�O��V

	GLfloat outerForceDirection[3];
	MathHelper::CopyVector( outerForceDirection , OuterForce );
	MathHelper::VectorNormalize( outerForceDirection );

	std::cout << "�p�⩵�~�O��V���l��" << std::endl;

	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		if( !virtualWorkConnections[i].isSolved )
		{
			virtualWorkConnections[i].ComputeCosAlpha( outerForceDirection );

			std::cout << "   ��" << virtualWorkConnections[i].index << "�ڬ�l���~�O��V��cosAlpha��" << virtualWorkConnections[i].cosAlpha << std::endl;
		}
		else
		{
			virtualWorkConnections[i].cosAlpha = 0;
		}
	}
	return true;
}

bool BallVirtualForceData::Analyze()
{
	if( solved )
	{
		std::cout << "BallVirtualForceData::Analyze()�X���G�w�g�ѹL�F" << std::endl;
		return false;
	}

	//�������Ǭ�l�٨S���ѡA�٦���h���O����ƿ�J���\�k��l��Ʋz
	std::vector<int> solvedConnections;
	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( connections[0].at(j).isSolved ) 
		{
			solvedConnections.push_back( connections[0].at(j).index );
		}
	}
	

	//��X���Ѫ���l�������l��
	//ComputeCosAlpha();//�ʨ�~���h����

	//���\�k====================================================================
		//�ܧ� v  = (Fl/EA)/( ( cos(a)2/Scale(a) + ... + cos(z)2/Scale(z) ))
		//���O Fi = ( cos(i)/Scale(i) ) * F / �ܧ�v
		//��X����v
	//�֮a�C�@�Ӭ�l�y������첾�A
	float displacementDivisor = 0;
	float tempOuterForce = 0; //- Const(2) * Cos(2) - Const(3) * Cos(3)...
	//�o��n��
	for( int i = 0; i < CosAlphaOrder.size(); i++ )
	{
		//if( virtualWorkConnections.at(i).isSolved ) continue;//�������ӭn�u����
		if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() != 0 )
		{
			if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() == 1 )
			{
				//Rate(2) * ( cos(1) / Scale(1) ) * cos(2)

				int relatedIndex = GetVirtualWorkConnectionIndexFromConnIndex( virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].relatedConnIndex ); 
				float debugcoefficient = virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].coefficient;
				float debugcosAlpha	   = virtualWorkConnections[   relatedIndex   ].cosAlpha;
				float debugscale       = virtualWorkConnections[   relatedIndex   ].scale;
				float debugcosAlpha2   = virtualWorkConnections[   relatedIndex   ].cosAlpha;

				float temp = debugcoefficient * debugcosAlpha / debugscale *debugcosAlpha2;

				displacementDivisor += virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].coefficient *
									   virtualWorkConnections[   relatedIndex   ].cosAlpha /
									   virtualWorkConnections[   relatedIndex   ].scale *
									   virtualWorkConnections[   relatedIndex   ].cosAlpha;

				//F - Const(2) * cos(2)
				tempOuterForce -= virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].constant * 
								  virtualWorkConnections[   relatedIndex   ].cosAlpha;
			}
			else
			{
				//�n�ݤ@�U�@�}�l������|�y�����relation
				//		   F(i) +   Coef(j) * F(        j        ) +   Const  = 0  ��F(i) = -Coef(j) * F(j) - Const
				//					  �� 				�� 				�� 
				//		        -coefficient    relatedConnIndex    -constant
				//							 ��							|			
				//Relation( relatedConnIndex ,  coefficient ,        constant )

				//��i�x�����O�O�Ѳ�j�x���˦ӱo�A�Ⱞ�����Y�� (i�x���O) = (coefficient) ���H (j�x���O) �a (const)
				
				//virtualWorkConnections[ CosAlphaOrder[i] ].relation[0] -> ��j�x�� coefficient �M const


				std::cout << "BallVirtualForceData::Analyze()�X���G�@�Ӭ�l������[�t" << std::endl;
				return false;
			}

		}
		else
		{
			float temp = pow( virtualWorkConnections[ CosAlphaOrder[i] ].cosAlpha , 2 ) / 
							  virtualWorkConnections[ CosAlphaOrder[i] ].scale;
			// cos(a)2/Scale(a)
			displacementDivisor += pow( virtualWorkConnections[ CosAlphaOrder[i] ].cosAlpha , 2 ) / 
										virtualWorkConnections[ CosAlphaOrder[i] ].scale;
		}
	}
	if( displacementDivisor == 0 )
	{
		std::cout << "BallVirtualForceData::Analyze()�X���G������0�A�S����������l" << std::endl;
	}

	//�첾 = EAF / [ cos(a)2/Scale(a) + ... + cos(a)2/Scale(a) ]
	displacement = 1.0 / displacementDivisor;


	//��X��
	for( int i = 0; i < CosAlphaOrder.size(); i++ )//�]�����Y�����ǬO F1 = �t��*F2 + �`�� �ҥH�ѵ�\���ɭԡA���ǭn�O3->2->1
	{//\\���ǬO�j���p�A�[�t���O �p=a�j+c �A�Ҥw���Ѥj���b�Ѥp��
		if( virtualWorkConnections[ CosAlphaOrder[i] ].isSolved ) continue;
		//�p�G�O��relation����l�A�N�Υt�@�Ӭ�l���j�N�i�h
		if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() != 0 )
		{
			if( virtualWorkConnections[ CosAlphaOrder[i] ].relation.size() == 1 )
			{
				//(i)  =  relatedConnIndex * coefficient + constant

				//										F- Const(2) * Cos(2) - Const(3) * Cos(3) - ...												    Cos(1)
		        // Fi =  ----------------------------------------------------------------------------------------------------------------- * Coef(i) * -------- + Const(i)
				//         Cos2(1) / Scale(1) + Coef(2) * ( Cos(1) / Scale(1) ) * Cos(2) + Coef(3) * ( Cos(1) / Scale(1) ) * Cos(3) + ...			   Scale(1)

				float relationCosAlpha;
				float relatedScale;
				float relatedConnForce;//���������Ӭ�l���O
				//=========================================================================
				for( int j = 0; j < virtualWorkConnections.size(); j++ )//��X���ӤO
				{
					if( virtualWorkConnections[j].index == virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].relatedConnIndex )
					{
						if( !virtualWorkConnections[j].isSolved )
						{
							std::cout << "BallVirtualForceData::Analyze�X���G���������Ӭ�l���j�S�����ѥX��" << std::endl;
						}
						relationCosAlpha = virtualWorkConnections[j].cosAlpha;
						relatedScale	 = virtualWorkConnections[j].scale;
						relatedConnForce = virtualWorkConnections[j].solution;
					}
				}
				//==========================================================================

				/*//�o�@���U�����檺�N��@��
				virtualWorkConnections.at(i).solution = virtualWorkConnections[i].relation[0].coefficient *				  //Coef(i)
														relationCosAlpha / relatedScale *								  //cos(j)/Scale(j)
														( MathHelper::VectorAbsolution( OuterForce ) + tempOuterForce ) * //F - Const(2) * Cos(2) - Const(3) * Cos(3)
														displacement +													  //�ܧ�v
														virtualWorkConnections[i].relation[0].constant;					  //Const(i)
				*/
				virtualWorkConnections[ CosAlphaOrder[i] ].solution = 
						virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].coefficient * //Coef(i) *
						relatedConnForce +													 //Fj +
						virtualWorkConnections[ CosAlphaOrder[i] ].relation[0].constant;	 //Const(i)
				virtualWorkConnections[ CosAlphaOrder[i] ].isSolved = true;

			}
			else
			{
				std::cout << "�o���٬ܼg" << std::endl;
				//return false;
			}
		}
		else
		{
			//���O Fi = ( cos(i)/Scale(i) ) * F * �ܧ�v
			virtualWorkConnections[ CosAlphaOrder[i] ].solution = virtualWorkConnections[ CosAlphaOrder[i] ].cosAlpha / 
																  virtualWorkConnections[ CosAlphaOrder[i] ].scale * //cos(i)/Scale(i)
																  ( MathHelper::VectorAbsolution( OuterForce ) + tempOuterForce ) * //F - Const(2) * Cos(2) - Const(3) * Cos(3)
																  displacement;  //�ܧ�v
			virtualWorkConnections[ CosAlphaOrder[i] ].isSolved = true;
		}
	}

	if( isBall )
	{
		std::cout << "==================��" << ballIndex << "���y����\�k��" << "=================="<< std::endl;
	}
	else
	{
		std::cout << "==================��" << ballIndex << "�ӥ~���I����\�k��" << "=================="<< std::endl;
	}

	//��T�����ѥ[�`
	for( int i = 0; i < virtualWorkConnections.size(); i++ )
	{
		/*if( std::find( solvedConnections.begin() , solvedConnections.end() , connections[0].at(j).index ) != 
			solvedConnections.end() )//�b�w�ѥX����l�̭������A�N���L
		{
			continue;
		}*/

		//��i�h��
		solutions.insert( std::pair<int,float>( virtualWorkConnections[ CosAlphaOrder[i] ].index , virtualWorkConnections[ CosAlphaOrder[i] ].solution ) );
		std::cout << "||��" << virtualWorkConnections[ CosAlphaOrder[i] ].index << "�ڬ�l���ѬO" << virtualWorkConnections[ CosAlphaOrder[i] ].solution << "g" << std::endl;
		//////////////////�o�Ӧ������A����n�R��
	}
	
	std::cout << "===================================================="<< std::endl;


	/*
	//��T�Ӭ��׳����@��
	for( int i = 0; i < 3; i++ )
	{	
		//�ܧ� v  = (Fl/EA)/( ( cos(a)2/Scale(a) + ... + cos(z)2/Scale(z) ))
		//���O Fi = ( cos(i)/Scale(i) ) * F / �ܧ�v
		//��X����v
		//displacement[0] = 0;
		float displacementDivisor = 0;
		for( int j = 0; j < connections[i].size(); j++ )
		{
			if( connections[i].at(j).isSolved ) continue;
			// cos(a)2/Scale(a)
			displacementDivisor += pow( connections[i].at(j).cosAlpha , 2 ) / connections[i].at(j).scale;
		}
		if( displacementDivisor == 0 )
		{
			std::cout << "BallVirtualForceData::Analyze()�X���G������0�A�S����������l" << std::endl;
		}


		displacement[i] = 1.0 / displacementDivisor;

		//��X��
		for( int j = 0; j < connections[i].size(); j++ )
		{
			if( connections[i].at(j).isSolved ) continue;
			//���O Fi = ( cos(i)/Scale(i) ) * F / �ܧ�v
			connections[i].at(j).solution = connections[i].at(j).cosAlpha / connections[i].at(j).scale * //cos(i)/Scale(i)
											OuterForce[i] / //F
											displacement[i];  //�ܧ�v
			connections[i].at(j).isSolved = true;
		}

		
	}

	if( isBall )
	{
		std::cout << "==================��" << ballIndex << "���y����\�k��" << "=================="<< std::endl;
	}
	else
	{
		std::cout << "==================��" << ballIndex << "�ӥ~���I����\�k��" << "=================="<< std::endl;
	}

	//��T�����ѥ[�`
	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( std::find( solvedConnections.begin() , solvedConnections.end() , connections[0].at(j).index ) != 
			solvedConnections.end() )//�b�w�ѥX����l�̭������A�N���L
		{
			continue;
		}

		float tempSolutionAddedUp = 0.0;
		for( int i = 0; i < 1; i++ )///////3���令1 �����@�U
		{
			tempSolutionAddedUp += connections[i].at(j).solution;
		}
		//��i�h��
		solutionsAddedUp.insert( std::pair<int,float>( connections[0].at(j).index , tempSolutionAddedUp ) );
		std::cout << "||��" << connections[0].at(j).index << "�ڬ�l���ѬO" << tempSolutionAddedUp << "g" << std::endl;
		break;//////////////////�o�Ӧ������A����n�R��
	}
	
	std::cout << "===================================================="<< std::endl;*/
}

bool BallVirtualForceData::hasConnection( int connIndex )
{
	if( connectionNum.find( connIndex ) != connectionNum.end() )
	{
		return true;
	}
	else return false;
}

bool BallVirtualForceData::Substitute( int connIndex , float solution )
{
	//�~�O=�~�O-�ܼƸ�*�t��

	for( int j = 0; j < connections[0].size(); j++ )
	{
		if( connections[0].at(j).index == connIndex && !connections[0].at(j).isSolved )
		{
			for( int i = 0; i < 3; i++ )
			{
				connections[i].at(j).solution = solution;
				connections[i].at(j).isSolved = true;
				OuterForce[i] -= solution * connections[i].at(j).coefficient;
			}
		}
	}
	return true;
}
//=====================================BallVirtualForceData//=====================================




//=====================================VirtualForceAnalyzer//=====================================
//public:
VirtualForceAnalyzer::VirtualForceAnalyzer( PolynomialList* fPolynomial , int bAmount , ZomeConnection* oConnection , ZomeStructure* oBall )
{
	//��J�ɤ@�w�n�O�٨S��²�ƹL���h����
	firstPolynomial = fPolynomial;
	ballAmount = bAmount;
	originalConn = oConnection;
	originalBall = oBall;

	//����Ҧ����l��J�A�C�T�Ӧ��l�n�\�i�h�@�� BallVirtualForceData �סA�C�Ӧ��l�jballAmount���U�@�Ӧ��l�A�N�O�L���U�Ӻ��ת����l
	int ballCount = 0;
	for( PolynomialList* p = fPolynomial; p != NULL; p = p -> link )
	{
		if( ballCount == bAmount ) break;
		
		
		//��ĤG�Ӻ��ת��h����
		PolynomialList* polynomialDimension2 = p;
		for(int i = 0; i < bAmount; i++ ) polynomialDimension2 = polynomialDimension2 -> link;
		//�[�J
		
		//��ĤT�Ӻ��ת��h����
		PolynomialList* polynomialDimension3 = polynomialDimension2;
		for(int i = 0; i < bAmount; i++ ) polynomialDimension3 = polynomialDimension3 -> link;
		//�[�J
		//tempBallVirtualForce.AddDimensionData( 2 , polynomialDimension3 );
		if( p -> isBall )
		{
			BallVirtualForceData tempBallVirtualForce( p , polynomialDimension2 , polynomialDimension3 , p -> sourceBall , originalConn );
			ballVirtualForceData.push_back( tempBallVirtualForce );
		}
		else if( p -> isVirtualBall )
		{
			BallVirtualForceData tempBallVirtualForce( p , polynomialDimension2 , polynomialDimension3 , p -> sourceVirtualBall , originalConn );
			ballVirtualForceData.push_back( tempBallVirtualForce );
		}
		else
		{
			std::cout << "VirtualForceAnalyzer::VirtualForceAnalyzer�X���G�S�����ӷ����y" << std::endl;
		}
		
		ballCount++;
	}
}

bool VirtualForceAnalyzer::Analyze()
{
	//����s�C�Ӳy����T
	Renew();

	bool debugOnce = true;/////////////////////debug��

	//�M��̤֥����ƪ��y
	while ( hasBallUnsolved() /*&& debugOnce*/ )
	{
		int leastUnsolvedBallIndex = GetLeastUnsolvedBallIndex();
		int leastUnsolvedBallVectorIndex;
		for( int i = 0; i < ballVirtualForceData.size(); i++ )
		{
			if( ballVirtualForceData[i].ballIndex == leastUnsolvedBallIndex )
			{
				leastUnsolvedBallVectorIndex = i;
			}
		}

	//�j�X���Ӳy�A�M�᭫�s�Ѥ@���Ҧ��h����
		//���R
		if( !VirtualWorkAnalyze( leastUnsolvedBallIndex ) ) return false;
		//�N�J
 		SubstituteSolution( leastUnsolvedBallIndex );
		//�Ѩ�L���l
		firstPolynomial -> SolvePolynomialList();

		//��s�C�Ӳy����T
		Renew();
		//debugOnce = false;/////////////////////debug��
	}


	
	return true;
	//�b��U�@�Ӳy
}
//private:
bool VirtualForceAnalyzer::Renew()
{
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p -> polynomial.size() == 2 )
		{


			int parameter;
			float solution;
			p -> DivideScalar();
			for( std::map< int , float >::iterator i = p -> polynomial.begin(); i != p -> polynomial.end(); ++i )
			{
				if( i -> first != -1 )
				{
					parameter = i -> first;

				}
				else
				{
					solution = -i -> second;
				}
			}

			if( std::find( solvedConnections.begin(), 
						   solvedConnections.end() , 
						   parameter ) == solvedConnections.end() )
			{
				//�N�J�C�@���y
				for( int i = 0; i < ballVirtualForceData.size(); i++ )
				{
					if( ballVirtualForceData[i].hasConnection( parameter ) )
					{
						ballVirtualForceData[i].Substitute( parameter , solution );
					}
				}
				//�����o���y�w�g��J�L
				solvedConnections.push_back( parameter );
			}
		}
	}
	return true;
}

bool VirtualForceAnalyzer::hasBallUnsolved()
{
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		for( int j = 0; j < ballVirtualForceData[i].connections[0].size(); j++ )
		{
			if( !(ballVirtualForceData[i].connections[0].at(j).isSolved) )
			{
				return true;
			}
			if( !(ballVirtualForceData[i].connections[1].at(j).isSolved) )
			{
				return true;
			}
			if( !(ballVirtualForceData[i].connections[2].at(j).isSolved) )
			{
				return true;
			}
		}
	}
	return false;
}

int VirtualForceAnalyzer::GetLeastUnsolvedBallIndex()
{
	int leastBallVectorIndex;
	int leastBallIndex;
	int leastBallUnsolvedAmount = 999;

	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		int unsolvedAmount = 0;
		if( ballVirtualForceData[i].ballIndex < 0 ) continue;
		for( int j = 0; j < ballVirtualForceData[i].connections[0].size(); j++ )
		{
			bool isUnsolved = false;

			if( !(ballVirtualForceData[i].connections[0].at(j).isSolved) )
			{
				isUnsolved = true;
			}
			if( !(ballVirtualForceData[i].connections[1].at(j).isSolved) )
			{
				isUnsolved = true;
			}
			if( !(ballVirtualForceData[i].connections[2].at(j).isSolved) )
			{
				isUnsolved = true;
			}

			if( isUnsolved )
			{
				unsolvedAmount++;
			}
		}

		if( unsolvedAmount < leastBallUnsolvedAmount && //�����
			unsolvedAmount > 0						 && //??
			unsolvedAmount < ballVirtualForceData[i].connections[0].size() )//�S���Ѫ���l�Ƥ�����Ҧ�����l��(�ܥb�ڬ�l���ѥX��)
		{
			leastBallVectorIndex = i;
			leastBallIndex = ballVirtualForceData[i].ballIndex;
			leastBallUnsolvedAmount = unsolvedAmount;
		}
	}
	std::cout << "�{�b�n�C�X�̤֥����ƪ��y���Ҧ���T�G" << std::endl;
	std::cout << "||�y�丹���G" <<  leastBallIndex << "�C" << std::endl;
	std::cout << "||�Ҧ��x�l�s�����G";
	for( std::set<int>::iterator i = ballVirtualForceData[ leastBallVectorIndex ].connectionNum.begin(); 
		 i != ballVirtualForceData[ leastBallVectorIndex ].connectionNum.end();
		 i++ )
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl;
	std::cout << "||�C�Ӭ�l����T���G" << std::endl;

	for( int j = 0; j < ballVirtualForceData[ leastBallVectorIndex ].connections[0].size(); j++ )
	{
		std::cout << "||��" << ballVirtualForceData[ leastBallVectorIndex ].connections[0].at(j).index << "����l�G";
		if( ballVirtualForceData[ leastBallVectorIndex ].connections[0].at(j).isSolved )
		{
			std::cout << "X�G" << ballVirtualForceData[ leastBallVectorIndex ].connections[0].at(j).solution << "�A";
			std::cout << "Y�G" << ballVirtualForceData[ leastBallVectorIndex ].connections[1].at(j).solution << "�A";
			std::cout << "Z�G" << ballVirtualForceData[ leastBallVectorIndex ].connections[2].at(j).solution << "�C" << std::endl;
		}
		else
		{
			std::cout << "���ѥX����" << std::endl;
		}
	}
	std::cout << "||�y����~�O���G";
	std::cout << "X:" << ballVirtualForceData[ leastBallVectorIndex ].OuterForce[0] << ", ";
	std::cout << "Y:" << ballVirtualForceData[ leastBallVectorIndex ].OuterForce[1] << ", ";
	std::cout << "Z:" << ballVirtualForceData[ leastBallVectorIndex ].OuterForce[2] << "�C" << std::endl;

	std::cout << "||�y�������q���G" << ballVirtualForceData[ leastBallVectorIndex ].displacement << "mm" << std::endl;
	


	std::cout << "��X�̤֥����ƪ��y�G#" << leastBallIndex << "�A�����Ʀ�" << leastBallUnsolvedAmount << "��" << std::endl;
	return leastBallIndex;
}

bool VirtualForceAnalyzer::MakeConnectionRelation( int ballIndex )
{
	//��X�����y�b�}�C������m
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == ballIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}

	if( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size() == 0 )
	{
		std::cout << "VirtualForceAnalyzer::MakeConnectionRelation�X���G�S����\���" << std::endl;
		return false;
	}
	//���`�Ϊ��F��g�ӥN��
	std::vector<int>* CosAlphaOrder = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder);
	std::vector<VirtualWorkConnectionData>* virtualWorkConnections = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections);
	
	//�n���ܥ�
	std::vector<int> virtualWorkConnIndex;
	for( int i = 0; i < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); i++ )
	{
		virtualWorkConnIndex.push_back( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].index );
	}

	
	//�n���h�d�����@�Ӫ�cosAlpha�̤j�A�M��Ψ��Ӭ�l���[�p�A�ƦC���ǬO�q�j��p
	//CheckCosAlphaOrder();
	for( int i = 0; i < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); i++ )//�M�� 1 -> 2 3
	{
		//��̤j��cosAlpha�A�j���b�e�A�p���b��A��vector
		int maxCosAlphaIndex;
		float maxCosAlpha = -1;
		for( int j = 0; j < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); j++ )
		{
			if( std::find( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.begin() , 
						   ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.end() , 
						   j ) == 
				ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.end() )//
			{
				if( abs( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[j].cosAlpha ) > 
					maxCosAlpha )
				{
					maxCosAlpha = abs( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[j].cosAlpha );
					maxCosAlphaIndex = j;
				}
			}
		}
		ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder.push_back( maxCosAlphaIndex );
	}
	//
	std::cout << "�����l��CosAlpha�j�p���Ǭ��G";
	for( int i = 0; i < (*CosAlphaOrder).size(); i++ )
	{
		std::cout << (*CosAlphaOrder)[i] << "(��" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "�ڬ�l) ";
		if( i == (*CosAlphaOrder).size() - 1 )
		{
			std::cout << "(�j��p)" << std::endl;
		}
		else
		{
			std::cout << "��";
		}
	}

//=======================================================================================================================================
	//ConnectRelation();
	for( PolynomialList* p = firstPolynomial; p != NULL; p = p -> link )
	{
		if( p -> polynomial.size() == 3 )
		{
			//�令��CosAlpha�j�p���ǨӰ��x�p�A�q�p��j�A�]��CosAlphaOrder���Ӫ����ǬO�q�j��p�A�Ҥwi�n�q�᭱���e
			for( int i = (*CosAlphaOrder).size() - 1; i >= 0 ; i-- )//�M�� �p->��  �j
			{																											 //     �� -> �j   �����Y
				for( int j = i - 1; j >= 0; j-- )// �p�Gi�O�p j �N�O �����j
				{
					if( p -> HasUnknown( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index ) && //�o�Ӧ��l�P�ɦ���Ӱ��l��쪾��
						p -> HasUnknown( (*virtualWorkConnections)[ (*CosAlphaOrder)[j] ].index ) )//�N��N�O �p = a�j + c
					{
						p -> DivideScalar( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index ); 
						//���l�쪬���ӬO Coef(i) * (i) + Coef(j) * (j) + Const = 0
						//�վ�t�ƫ��ܦ�    1    * (i) + Coef(j) * (j) + Const = 0
						//                  (i)  =  relatedConnIndex * coefficient + constant
						//�bj�̭��x�s relatedConnIndex = j 
						//			  coefficient = -Coef(j)
						//			  constant = -Const
						
						//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i
						//²�d�o�����Y���S���w�g�I�ѤJ�L��������������������������
						bool isAlreadyFound = false;
						for( int k = 0; k < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); k++ )
						{
							if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[k].relatedConnIndex == (*virtualWorkConnections)[ (*CosAlphaOrder)[j] ].index )
							{
								std::cout << "�o�����Y�w�I�ѤJ�G��" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "����" << 
																	   (*virtualWorkConnections)[         j           ].index << "��" << std::endl;
								isAlreadyFound = true;
							}
						}
						if( isAlreadyFound ) continue;
						//===============================��������������������������
						//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i


						// F(i) +   Coef(j)   * F(       j        ) +   Const   = 0
						//			  �� 				�� 				  �� 
						//        -coefficient    relatedConnIndex    -constant

						Relation relation( (*virtualWorkConnections)[  (*CosAlphaOrder)[j] ].index ,						  //relatedConnIndex
										   -p -> GetTermParameter( (*virtualWorkConnections)[ (*CosAlphaOrder)[j] ].index ) , //coefficient
										   -p -> GetTermParameter( CONSTANT ) );											  //relatedConnIndex , coefficient , constant									//constant

						(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.push_back( relation );//�N���Y�[�J��\��Ƥ�
						
						std::cout << "��J�s���Y�G#(" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << 
									 ") = " << relation.coefficient << " * #(" << relation.relatedConnIndex << ") + " << relation.constant << std::endl; 

					}
				}
			}
		}
	}

	if( ballIndex == 8 )
	{
		std::cout << "debug";
	}

	RedistributeDublicatedRelation( ballIndex );//��P�@�Ӭ�l������[�t�����X�h
	/*
	//��P�@�Ӭ�l������[�t�����X�h
	//�p�G�e���bConnectRelation�� �Ai�O�p j�O���ɫإߤ@�Ӥp�������Y�Aj�O�j�ɤS�إߤ@�Ӥp���j���Y
	//i�N�|�إߨ�����Y�A�o�ɴN��@�����Y���� �� �A�ܦ������p���Y
	//�o���޿褣�n ���ӭn�令 �����j���Y
	//RedistributeDublicatedRelation( ballIndex );
	for( int i = (*CosAlphaOrder).size() - 1; i >= 0 ; i-- )//�M�� �p->��  �j
	{
		if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size() > 1 )
		{

			//����X�Ҧ����Y���ACosAlpha�̤j���������Y
			int maxCosAlphaIndex;
			float MaxCosAlpha = -1;
			VirtualWorkConnectionData* maxCosAlphaVirtualWorkConnection;
			Relation* maxCosAlphaRelation;//��̤j��CosAlpha�����Y���s�_�ӡA���@�U��

			//�o�䪺���ǬO�G�p���p �p���� �p���j ...
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )
			{
				VirtualWorkConnectionData* tempVirtualWorkConnection = 
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
					GetVirtualWorkConnection( 
					(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[ j ].relatedConnIndex );

				if( abs( tempVirtualWorkConnection -> cosAlpha ) > MaxCosAlpha )
				{
					MaxCosAlpha = abs( tempVirtualWorkConnection -> cosAlpha );
					maxCosAlphaIndex = tempVirtualWorkConnection -> index;
					maxCosAlphaRelation = &( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j] );//��̤j��CosAlpha�����Y���s�_�ӡA���@�U��
					maxCosAlphaVirtualWorkConnection = tempVirtualWorkConnection;
				}
			}

			//�NCosAlpha���O�̤j���������Y�A��o�����Y�ഫ���o�Ӱ��l�MCosAlpha�̤j���Ӭ�l���������Y
			//�A�ç�ثe���o�����Y�R��
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )//�M��o�Ӭ�l����@���[�t
			{
				if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex != maxCosAlphaIndex )
				{
					//�{�b�����Y�n��j����l�A���L�h�P�̤jCosAlpha����l�إ����Y�A���l�O
					// i = coef(j) * j + const(j)
					// i = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)     
					// �� coef(j) * j + const(j) = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)

					// �� j = coef(MaxCosAlpha) / coef(j) * MaxCosAlpha + ( const(MaxCosAlpha) - const(j) ) / coef(j)
					//		  �¡¡¡¡¡¡¡¡¡¡¡¡��¡¡¡¡¡¡¡¡¡¡¡¡�				  �¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡��¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡�
					//					newCoef												newConst

					//				coef(MaxCosAlpha) / coef(j)
					float newCoef = maxCosAlphaRelation -> coefficient / 
									(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//				( const(MaxCosAlpha) - const(j) ) / coef(j)
					float newConst = ( 
										maxCosAlphaRelation -> constant - 
									    (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant 
									 ) / 
									 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					
					//relation j �ҥu�����o�Ӭ�l
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
							GetVirtualWorkConnection( 
								(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex );

					//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i
					//²�d�o�����Y���S���w�g�I�ѤJ�L��������������������������
					bool alreadyHaveRelation = false;
					for( int k = 0; k < tempVirtualWorkConnection -> relation.size(); k++ )
					{
						if( tempVirtualWorkConnection -> relation[k].relatedConnIndex == maxCosAlphaIndex )
						{
							//�p�G�w�g��J�L�A�n�ˬd�w�g��J�L�����Y�����O�Ʀr���@�ˡA���@�˪��ܴN�����D
							if( abs( tempVirtualWorkConnection -> relation[k].coefficient - newCoef ) < SMALL_NUMBER &&
								abs( tempVirtualWorkConnection -> relation[k].constant   - newConst ) < SMALL_NUMBER )
							{
								std::cout << "�o�����Y�w�I�ѤJ�G��" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "����" << 
																	   (*virtualWorkConnections)[         j           ].index << "��" << std::endl;
							}
							else
							{
								std::cout << "RedistributeDublicatedRelation()�X���G�ഫ���Y�H��A�o�{�P�즳�����Y�Ʀr���@��" << std::endl;
								std::cout << "�s�ت����Y�G ��" << tempVirtualWorkConnection -> index << "�� = Coeffifient( " << tempVirtualWorkConnection -> relation[k].coefficient <<
											 " ) �� ��" << maxCosAlphaIndex << "�� + Constant( " << tempVirtualWorkConnection -> relation[k].constant << " ) " << std::endl;
								std::cout << "�ª����Y�G ��" << tempVirtualWorkConnection -> index << "�� = Coeffifient( " << newCoef <<
											 " ) �� ��" << maxCosAlphaIndex << "�� + Constant( " << newConst << " ) " << std::endl;
							}
							alreadyHaveRelation = true;
						}
					}
					if( alreadyHaveRelation ) continue;
					//===============================��������������������������
					//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i

					// j �� maxCosAlpha �����Y
					Relation tempRelation( maxCosAlphaIndex ,
										   newCoef,
										   newConst );

					//��Jj�̡A�n�bCosAlpha�j�p���ǥ��T����m���J�Arelation���j�p���ǬO i = 0��n �A �j���p
					for( std::vector<Relation>::iterator it = tempVirtualWorkConnection -> relation.begin(); 
						 it != tempVirtualWorkConnection -> relation.end();
						 it++ )
					{
						//insert���\���m�O�A�b���@�Ӧ�m���J�A�쥻�b���Ӧ�m�������N�|���~���@��
						//�ҥH�n���Ĥ@��CosAlpha��L�p���[�t�A�M�ᴡ�J�b�������Y����m
						VirtualWorkConnectionData* tempVirtualWorkConnectionForSorting = 
							ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
								GetVirtualWorkConnection( it -> relatedConnIndex );
						if( abs( tempVirtualWorkConnectionForSorting -> cosAlpha ) < abs( maxCosAlphaVirtualWorkConnection -> cosAlpha ) )
						{
							tempVirtualWorkConnection -> relation.insert( it , tempRelation );
							break;
						}
					}
					//tempVirtualWorkConnection -> relation.push_back( tempRelation );


					//�R���N�����Y
					std::vector<Relation>::iterator relationToErase = (*virtualWorkConnections)[i].relation.begin() + j;
					(*virtualWorkConnections)[i].relation.erase( relationToErase );
					j--;//�]���R���H��Arelation�|����h�@�ӡA�ҥHj�]�n-1



					/*  �o�@�q�O�g�����A�n�ﱼ�C
					//��쪺���l�������k��աA�쥻�O
					// i = coef(j) * j + const(j)
					// �վ���ܦ�
					// 1/coef(j) * i - const(j)/coef(j) = j
					//
					float newCoef = 1.0 / (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					float newConst =	 -(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant / 
										  (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//�� ���Yj �����p����\����� ������
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].GetVirtualWorkConnection( (*virtualWorkConnections)[i].relation[j].relatedConnIndex );

					//�إߤ@�ӵ� j���ҨϥΪ�
					Relation tempRelation( (*virtualWorkConnections)[i].index , 
										   newCoef ,
										   newConst );
					tempVirtualWorkConnection -> relation.push_back( tempRelation );

					//�R���N�����Y
					std::vector<Relation>::iterator relationToErase;
					for( std::vector<Relation>::iterator it = (*virtualWorkConnections)[i].relation.begin(); 
						 it != (*virtualWorkConnections)[i].relation.end();
						 it++ )
					{
						if( it -> relatedConnIndex == (*virtualWorkConnections)[i].relation[j].relatedConnIndex )
						{
							relationToErase = it;
						}
					}
					(*virtualWorkConnections)[i].relation.erase( relationToErase );
					j--;//�]���R���H��Arelation�|����h�@�ӡA�ҥHj�]�n-1
					*/ /*
				}
			}
		}
	}

	//return true;/////////���ե� ����n�R��*/

	CombineChainRelation( ballIndex );//�ů����S���p�Ҫ��x�t�A�����ܴN��L�����X
	/* 
	//�ů����S���p�Ҫ��x�t�A�����ܴN��L�����X
	//CombineChainRelation();
	for( int i = 0; i < (*virtualWorkConnections).size(); i++ )//�M��C�@�ӵ�\��l�A�ݬݥL���C�@���[�t
	{	
		bool foundChain = false;
		for( int j = 0; j < (*virtualWorkConnections)[i].relation.size(); j++ )//�M��o�Ӭ�l����@���[�t
		{
			for( int k = 0; k < (*virtualWorkConnections).size(); k++ )//�M��o���[�t�ҹ�쪺��l�A
			{
				if( (*virtualWorkConnections)[i].relation[j].relatedConnIndex != //i��l����j�[�t�A�O�_����k��l
					(*virtualWorkConnections)[k].index )
					continue;
				for( int l = 0; l < (*virtualWorkConnections)[k].relation.size(); l++ )
				{
					//�o��ӧ��i��l����j�[�t�A�|�p���k��l����l�[�t�A��o��ӽc���_��
					//���l�쪬���ӬO Coef(i) * (i) + Coef(j) * (j) + Const = 0
					//�վ�t�ƫ��ܦ�    1    * (i) + Coef(j) * (j) + Const = 0

					// j = k
					// i = coef(j) * j + const(j)
					// k = coef(l) * l + const(l)
					// i = coef(j) * ( coef(l) * l + const(l) ) + const(j)
					//   = coef(j) * coef(l) * l + coef(j) * const(l) + const(j)
					

					Relation relation( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].relatedConnIndex , //l         //relatedConnIndex
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
									      ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].coefficient ) , //coef(l)   //coefficient
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].constant +      //const(l)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].constant ) );	 //const(j)  //constant	
					//relatedConnIndex , coefficient , constant									//constant

					//�bj�̭��x�s relatedConnIndex = l
					//			  coefficient = coef(j) * coef(l)
					//			  constant = coef(j) * const(l) + const(j)
					//

					//�ק粒�H��A�쥻��relation�R��
					std::vector< Relation >::iterator relationToErase;
					for( relationToErase = ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.begin();
						 relationToErase != ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.end();
						 relationToErase++ )
					{
						if( (*relationToErase).relatedConnIndex == ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].relatedConnIndex )
							break;
					}

					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.erase( relationToErase );//�R��
					//�ѤJ�ק�n�����G
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.push_back( relation );//

				}
			}
		}
	}*/

	//PrintAllRelation();
	for( int i = 0; i < (*virtualWorkConnections).size(); i++ )
	{
		for( int j = 0; j < (*virtualWorkConnections)[i].relation.size(); j++ )
		{
			std::cout << "�B�z�����Y�G#(" << (*virtualWorkConnections)[i].index << 
									 ") = " << (*virtualWorkConnections)[i].relation[j].coefficient << 
									 " * #(" << (*virtualWorkConnections)[i].relation[j].relatedConnIndex << 
									 ") + " <<  (*virtualWorkConnections)[i].relation[j].constant << std::endl; 

		}
	}
	

	return true;
}

bool VirtualForceAnalyzer::VirtualWorkAnalyze( int leastUnsolvedBallIndex )
{
	//��X�����y�b�}�C������m
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == leastUnsolvedBallIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}

	if( leastUnsolvedBallIndex == 23 )
	{
		std::cout << "debug" << std::endl;
		return false;//�]����23���y�|��� �ҥH�����o�N�n
	}


	ballVirtualForceData[ leastUnsolvedBallVectorIndex ].MakeVirtualWorkConnectionData();
	//��X�~�O�X�O��V
	//��X�Ҧ���l��cosA
	ballVirtualForceData[ leastUnsolvedBallVectorIndex ].ComputeCosAlpha();

	MakeConnectionRelation( leastUnsolvedBallIndex );//�إ߬�l�P��l���������Y
	//�βy�����R
	if( !ballVirtualForceData[ leastUnsolvedBallVectorIndex ].Analyze() )return false;
	return true;
}

bool VirtualForceAnalyzer::SubstituteSolution( int leastUnsolvedBallIndex )
{
	//��X�����y�b�}�C������m
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == leastUnsolvedBallIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}
	//��C�ӬܥN�J�L���������X��
	for( int i = 0; i < ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.size(); i++ )
	{
		int connIndex = ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections.at(i).index;
		
		//�S���N�J�L
		//���إ߷s���h�����A�̭��\�ѡA�[�J��C���̫�
			
		PolynomialList* tempPolynomial = new PolynomialList( connIndex , ballVirtualForceData[ leastUnsolvedBallVectorIndex ].solutions.at( connIndex ) );//�令�Υ[�`�᪺�j�ӥN�J
		//PolynomialList* tempPolynomial = new PolynomialList( connIndex , firstPolynomial -> solvedConnections -> at( connIndex ) );//�o�ӬO����N��????????
		//PolynomialList* tempPolynomial = new PolynomialList( connIndex , ballVirtualForceData[ leastUnsolvedBallVectorIndex ].connections[0].at(i).solution );
		tempPolynomial -> AddToLink( firstPolynomial );
		std::cout << "�ھڵ�\�k�A�إ߷s���ѡG";
		tempPolynomial -> PrintPolynomialList();
		//�N�J�Ҧ����l
		tempPolynomial -> Substitute();

		//�n���n��o�өj�b���JsolvedConnections��?
		firstPolynomial -> solvedConnections -> push_back( connIndex );

		//break;////////////////////////////���ե� �n��^��
		

		//�]�n��j�X�Ӫ��s����s���\�k�̪�solvedConnections
		if( std::find( solvedConnections.begin() , solvedConnections.end() , connIndex ) == solvedConnections.end() )
		{
			Renew();
		}

	}

	return true;
}

bool VirtualForceAnalyzer::RedistributeDublicatedRelation( int ballIndex )
{
	//��X�����y�b�}�C������m
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == ballIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}
	
	//���`�Ϊ��F��g�ӥN��
	std::vector<int>* CosAlphaOrder = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder);
	std::vector<VirtualWorkConnectionData>* virtualWorkConnections = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections);
	
	//��P�@�Ӭ�l������[�t�����X�h
	//�p�G�e���bConnectRelation�� �Ai�O�p j�O���ɫإߤ@�Ӥp�������Y�Aj�O�j�ɤS�إߤ@�Ӥp���j���Y
	//i�N�|�إߨ�����Y�A�o�ɴN��@�����Y���� �� �A�ܦ������p���Y
	//�o���޿褣�n ���ӭn�令 �����j���Y
	//RedistributeDublicatedRelation();
	for( int i = (*CosAlphaOrder).size() - 1; i >= 0 ; i-- )//�M�� �p->��  �j
	{
		if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size() > 1 )
		{

			//����X�Ҧ����Y���ACosAlpha�̤j���������Y
			int maxCosAlphaIndex;
			float MaxCosAlpha = -1;
			VirtualWorkConnectionData* maxCosAlphaVirtualWorkConnection = NULL;
			Relation* maxCosAlphaRelation = NULL;//��̤j��CosAlpha�����Y���s�_�ӡA���@�U��

			//�o�䪺���ǬO�G�p���p �p���� �p���j ...
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )
			{
				VirtualWorkConnectionData* tempVirtualWorkConnection = 
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
						GetVirtualWorkConnection( 
							(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex );

				if( abs( tempVirtualWorkConnection -> cosAlpha ) > MaxCosAlpha )
				{
					MaxCosAlpha = abs( tempVirtualWorkConnection -> cosAlpha );
					maxCosAlphaIndex = tempVirtualWorkConnection -> index;
					maxCosAlphaRelation = &( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j] );//��̤j��CosAlpha�����Y���s�_�ӡA���@�U��
					maxCosAlphaVirtualWorkConnection = tempVirtualWorkConnection;
				}
			}

			//�NCosAlpha���O�̤j���������Y�A��o�����Y�ഫ���o�Ӱ��l�MCosAlpha�̤j���Ӭ�l���������Y
			//�A�ç�ثe���o�����Y�R��
			for( int j = 0; j < (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.size(); j++ )//�M��o�Ӭ�l����@���[�t
			{
				if( (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex != maxCosAlphaIndex )
				{
					//�{�b�����Y�n��j����l�A���L�h�P�̤jCosAlpha����l�إ����Y�A���l�O
					// i = coef(j) * j + const(j)
					// i = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)     
					// �� coef(j) * j + const(j) = coef(MaxCosAlpha) * MaxCosAlpha + const(MaxCosAlpha)

					// �� j = coef(MaxCosAlpha) / coef(j) * MaxCosAlpha + ( const(MaxCosAlpha) - const(j) ) / coef(j)
					//		  �¡¡¡¡¡¡¡¡¡¡¡¡��¡¡¡¡¡¡¡¡¡¡¡¡�				  �¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡��¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡�
					//					newCoef												newConst

					//				coef(MaxCosAlpha) / coef(j)
					float newCoef = maxCosAlphaRelation -> coefficient / 
									(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//				( const(MaxCosAlpha) - const(j) ) / coef(j)
					float newConst = ( 
										maxCosAlphaRelation -> constant - 
									    (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant 
									 ) / 
									 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					
					//relation j �ҥu�����o�Ӭ�l
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
							GetVirtualWorkConnection( 
								(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex );

					//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i
					//²�d�o�����Y���S���w�g�I�ѤJ�L��������������������������
					bool alreadyHaveRelation = false;
					for( int k = 0; k < tempVirtualWorkConnection -> relation.size(); k++ )
					{
						if( tempVirtualWorkConnection -> relation[k].relatedConnIndex == maxCosAlphaIndex )
						{
							//�p�G�w�g��J�L�A�n�ˬd�w�g��J�L�����Y�����O�Ʀr���@�ˡA���@�˪��ܴN�����D
							if( abs( tempVirtualWorkConnection -> relation[k].coefficient - newCoef ) < SMALL_NUMBER &&
								abs( tempVirtualWorkConnection -> relation[k].constant   - newConst ) < SMALL_NUMBER )
							{
								std::cout << "�o�����Y�w�I�ѤJ�G��" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].index << "����" << 
																	   (*virtualWorkConnections)[         j           ].index << "��" << std::endl;
							}
							else
							{
								std::cout << "RedistributeDublicatedRelation()�X���G�ഫ���Y�H��A�o�{�P�즳�����Y�Ʀr���@��" << std::endl;
								std::cout << "�s�ت����Y�G ��" << tempVirtualWorkConnection -> index << "�� = Coeffifient( " << tempVirtualWorkConnection -> relation[k].coefficient <<
											 " ) �� ��" << maxCosAlphaIndex << "�� + Constant( " << tempVirtualWorkConnection -> relation[k].constant << " ) " << std::endl;
								std::cout << "�ª����Y�G ��" << tempVirtualWorkConnection -> index << "�� = Coeffifient( " << newCoef <<
											 " ) �� ��" << maxCosAlphaIndex << "�� + Constant( " << newConst << " ) " << std::endl;

							}
							alreadyHaveRelation = true;
						}
					}
					if( alreadyHaveRelation ) continue;
					//===============================��������������������������
					//�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i�i

					// j �� maxCosAlpha �����Y
					Relation tempRelation( maxCosAlphaIndex ,
										   newCoef,
										   newConst );

					//��Jj�̡A�n�bCosAlpha�j�p���ǥ��T����m���J�Arelation���j�p���ǬO i = 0��n �A �j���p
					if( tempVirtualWorkConnection -> relation.size() == 0 )
					{
						//�p�G�o��relation���Ū��A�N�����\�b�Ĥ@��
						tempVirtualWorkConnection -> relation.push_back( tempRelation );
						std::cout << "�i�ഫ���Y(�ª�)�G��" << 
										tempVirtualWorkConnection -> index << "�� = Coeffifient( " << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient << " ) �� ��" << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "�� + Constant( " << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant << " ) " << std::endl;
						std::cout << "						����������	" << std::endl;
						std::cout << "�i�ഫ��(�s��)�G��" << 
										(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "�� = Coeffifient( " << 
										newCoef << " ) �� ��" << 
										maxCosAlphaIndex << "�� + Constant( " << 
										newConst << " ) " << std::endl;
					}
					else
					{
						for( std::vector<Relation>::iterator it = tempVirtualWorkConnection -> relation.begin(); 
							 it != tempVirtualWorkConnection -> relation.end();
							 it++ )
						{
							//insert���\���m�O�A�b���@�Ӧ�m���J�A�쥻�b���Ӧ�m�������N�|���~���@��
							//�ҥH�n���Ĥ@��CosAlpha��L�p���[�t�A�M�ᴡ�J�b�������Y����m
							VirtualWorkConnectionData* tempVirtualWorkConnectionForSorting = 
								ballVirtualForceData[ leastUnsolvedBallVectorIndex ].
									GetVirtualWorkConnection( it -> relatedConnIndex );
							if( abs( tempVirtualWorkConnectionForSorting -> cosAlpha ) < abs( maxCosAlphaVirtualWorkConnection -> cosAlpha ) )
							{
								tempVirtualWorkConnection -> relation.insert( it , tempRelation );
								std::cout << "�i�ഫ���Y(�ª�)�G��" << tempVirtualWorkConnection -> index << "�� = Coeffifient( " << 
											 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient << " ) �� ��" << 
											 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "�� + Constant( " << 
											 (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant << " ) " << std::endl;
								std::cout << "						����������	" << std::endl;
								std::cout << "�i�ഫ��(�s��)�G��" << (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex << "�� = Coeffifient( " << 
											 newCoef << " ) �� ��" << 
											 maxCosAlphaIndex << "�� + Constant( " << 
											 newConst << " ) " << std::endl;
								break;
							}
						}
						//tempVirtualWorkConnection -> relation.push_back( tempRelation );
					}
					


					//�R���N�����Y
					std::vector<Relation>::iterator relationToErase = (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.begin() + j;
					/*
					std::vector<Relation>::iterator relationToErase = (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.begin();
					for( std::vector<Relation>::iterator it = (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.begin(); 
						 it != (*virtualWorkConnections)[i].relation.end();
						 it++ )
					{
						if( it -> relatedConnIndex == (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].relatedConnIndex )
						{
							relationToErase = it;
						}
					}*/
					(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation.erase( relationToErase );
					j--;//�]���R���H��Arelation�|����h�@�ӡA�ҥHj�]�n-1

					/*  �o�@�q�O�g�����A�n�ﱼ�C
					//��쪺���l�������k��աA�쥻�O
					// i = coef(j) * j + const(j)
					// �վ���ܦ�
					// 1/coef(j) * i - const(j)/coef(j) = j
					//
					float newCoef = 1.0 / (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;
					float newConst =	 -(*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].constant / 
										  (*virtualWorkConnections)[ (*CosAlphaOrder)[i] ].relation[j].coefficient;

					//�� ���Yj �����p����\����� ������
					VirtualWorkConnectionData* tempVirtualWorkConnection = 
						ballVirtualForceData[ leastUnsolvedBallVectorIndex ].GetVirtualWorkConnection( (*virtualWorkConnections)[i].relation[j].relatedConnIndex );

					//�إߤ@�ӵ� j���ҨϥΪ�
					Relation tempRelation( (*virtualWorkConnections)[i].index , 
										   newCoef ,
										   newConst );
					tempVirtualWorkConnection -> relation.push_back( tempRelation );

					//�R���N�����Y
					std::vector<Relation>::iterator relationToErase;
					for( std::vector<Relation>::iterator it = (*virtualWorkConnections)[i].relation.begin(); 
						 it != (*virtualWorkConnections)[i].relation.end();
						 it++ )
					{
						if( it -> relatedConnIndex == (*virtualWorkConnections)[i].relation[j].relatedConnIndex )
						{
							relationToErase = it;
						}
					}
					(*virtualWorkConnections)[i].relation.erase( relationToErase );
					j--;//�]���R���H��Arelation�|����h�@�ӡA�ҥHj�]�n-1
					*/
				}
			}
		}
	}

	return true;
}

bool VirtualForceAnalyzer::CombineChainRelation( int ballIndex )
{
	//��X�����y�b�}�C������m
	int leastUnsolvedBallVectorIndex;
	for( int i = 0; i < ballVirtualForceData.size(); i++ )
	{
		if( ballVirtualForceData[i].ballIndex == ballIndex )
		{
			leastUnsolvedBallVectorIndex = i;
		}
	}
	
	//���`�Ϊ��F��g�ӥN��
	std::vector<int>* CosAlphaOrder = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].CosAlphaOrder);
	std::vector<VirtualWorkConnectionData>* virtualWorkConnections = &(ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections);
	 
	//�ů����S���p�Ҫ��x�t�A�����ܴN��L�����X
	//CombineChainRelation();
	for( int i = 0; i < (*virtualWorkConnections).size(); i++ )//�M��C�@�ӵ�\��l�A�ݬݥL���C�@���[�t
	{	
		bool foundChain = false;
		for( int j = 0; j < (*virtualWorkConnections)[i].relation.size(); j++ )//�M��o�Ӭ�l����@���[�t
		{
			for( int k = 0; k < (*virtualWorkConnections).size(); k++ )//�M��o���[�t�ҹ�쪺��l�A
			{
				if( (*virtualWorkConnections)[i].relation[j].relatedConnIndex != //i��l����j�[�t�A�O�_����k��l
					(*virtualWorkConnections)[k].index )
					continue;
				for( int l = 0; l < (*virtualWorkConnections)[k].relation.size(); l++ )
				{
					//�o��ӧ��i��l����j�[�t�A�|�p���k��l����l�[�t�A��o��ӽc���_��
					//���l�쪬���ӬO Coef(i) * (i) + Coef(j) * (j) + Const = 0
					//�վ�t�ƫ��ܦ�    1    * (i) + Coef(j) * (j) + Const = 0

					// j = k
					// i = coef(j) * j + const(j)
					// k = coef(l) * l + const(l)
					// i = coef(j) * ( coef(l) * l + const(l) ) + const(j)
					//   = coef(j) * coef(l) * l + coef(j) * const(l) + const(j)
					

					Relation relation( ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].relatedConnIndex , //l         //relatedConnIndex
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
									      ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].coefficient ) , //coef(l)   //coefficient
									   (  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].coefficient *   //coef(j)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[k].relation[l].constant +      //const(l)
										  ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].constant ) );	 //const(j)  //constant	
					//relatedConnIndex , coefficient , constant									//constant

					//�bj�̭��x�s relatedConnIndex = l
					//			  coefficient = coef(j) * coef(l)
					//			  constant = coef(j) * const(l) + const(j)
					//

					//�ק粒�H��A�쥻��relation�R��
					std::vector< Relation >::iterator relationToErase;
					for( relationToErase = ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.begin();
						 relationToErase != ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.end();
						 relationToErase++ )
					{
						if( (*relationToErase).relatedConnIndex == ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation[j].relatedConnIndex )
							break;
					}

					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.erase( relationToErase );//�R��
					//�ѤJ�ק�n�����G
					ballVirtualForceData[ leastUnsolvedBallVectorIndex ].virtualWorkConnections[i].relation.push_back( relation );//

				}
			}
		}
	}
	return true;
}

//=====================================VirtualForceAnalyzer//=====================================