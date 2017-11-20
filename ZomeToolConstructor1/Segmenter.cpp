#include "Segmenter.h"


//==========================================Segmenter//==========================================

Segmenter::Segmenter( Polyhedron* p , Point* sPointList )
{
	count = 0;
	

	polyhedron = p;
	surfacePointList = sPointList;
	segmentAmount = 0;


	GenerateSegment( GENERATOR );

 	ConstructSplitSurface();
}

Segmenter::Segmenter( Polyhedron* p , Point* sPointList , bool isGenerator )
{
	count = 0;

	polyhedron = p;
	surfacePointList = sPointList;
	segmentAmount = 0;

	GenerateSegment( isGenerator );

	if( isGenerator )
	{
		ConstructSplitSurface();
	}
}

//�����X�Ĥ@�ӥ���
bool Segmenter::CheckIntersectionOnSplitPlane( Point* surfacePoint , Plane* splitPlane ,  SegmentFace*& segmentFace )
{
	//��X��ӭ��Q���ΥH�᪺�ˤl
	//����@�ӥ�������ҫ��q����b�A����d�U�̫�@�s���O�S���Q���󭱤��쪺�A�N�O�ڭ̳̲ת��H��
	//�n����C�@�ӭ�����Ĥ@�ӥ����ycheckIntersection�A�o�{�L�̦��S���򥭭��ۤ��A�p�G�����ܴN������S���Q���쪺������
	//�S�����쪺�ܴN�~�򩵦���U�@�ӭ���checkIntersection
	//�p�G�����쪺���A�����⨺���ѵ�������p���H���A����ncheckIntersection�ɡA�N�u�P�_���ӧ�p���H��
	
	int initFace = polyhedron -> GetClosestFace( surfacePoint -> position );//�a���I
	if( initFace == -2 ) 
	{
		std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�S�������I�b���ӭ��W" << std::endl;
		return false;//�N��S������I�b���ӥ���
	}
	/*
	std::cout << "�Ĥ@���I����m�O(" << surfacePoint -> position[0] << "," << surfacePoint -> position[1] << "," << surfacePoint -> position[2] << ")" << std::endl;
	std::cout << "�Ĥ@�Ӻ⪺���O�I1:(" << polyhedron -> face[initFace] -> connectedPoint[0] -> position[0] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[0] -> position[1] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[0] -> position[2] << ")�A�I2:("
									   << polyhedron -> face[initFace] -> connectedPoint[1] -> position[0] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[1] -> position[1] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[1] -> position[2] << ")�A�I3:("
									   << polyhedron -> face[initFace] -> connectedPoint[2] -> position[0] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[2] -> position[1] << ","
									   << polyhedron -> face[initFace] -> connectedPoint[2] -> position[2] << ")" << std::endl;
									   */

	segmentFace = new SegmentFace( polyhedron -> GetPolyhedronFace( initFace ) );
	segmentFace -> firstSegmentFace = segmentFace;


	//1.��Ҧ����������@���A�ݤ��X�Ӫ��h�������
	for( Plane* i = splitPlane; i != NULL; i = i -> link )
	{		
		if( !segmentFace -> isPolygon )
		{

			if( i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[1] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[1] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) )//�٨S�g
			{
#pragma region 1.1:�Q�Υ������ΥX�s�h���
				//int intersectedFaceNumber = segmentFace -> polyhedronFace -> GetPolyhedronFace( 0 , 1 );//�٨S�g
				//edgeIntesrected[ intersectedNumber ] = true;
				segmentFace -> isPolygon = true;
				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
														  segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
														  segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
														  i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
														   segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
														   segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
														   i , IS_OUTSIDE );//�٨S�g//------------------------------0323
				//�o��O��Y�@��polygon�o�K�ۥ����A���o�S���u���Q������������ɡA�¥γo��P�_�A�n�ܦ��S�����󫬪��A�٬O�d�U����T����
#pragma endregion
#pragma region 1.2:�ˬd�o�ӧΪ��O���O��n�Q�����Ψ�����A�O���ܴN�������쥻������
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					if( splitPlane -> IsInside( surfacePoint -> position ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//���I�binside�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//���I�binside�A���X�Ӫ��Ϊ�������
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//���I�bOUTSIDE�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//���I�bOUTSIDE�A���X�Ӫ��Ϊ�������
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
#pragma endregion
#pragma region 1.3:��ܭ��@�Ӧh�䫬����a����I
				GLfloat distanceToPolygonInside = polygonInside -> DistanceToPoint( surfacePoint );//�٨S�g
				GLfloat distanceToPolygonOutside = polygonOutside -> DistanceToPoint( surfacePoint );
				if( distanceToPolygonInside < distanceToPolygonOutside )
				{
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
				}
				else
				{
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
				}
				//segmentFace -> intersected = true;
#pragma endregion
			}
		}
		else
		{
			if( segmentFace -> polygon -> CheckIntersectionPolygonOnPlane( i ) )//3=0323
			{
#pragma region 1.1:�Q�Υ������ΥX�s�h���
				if( segmentFace -> polygon -> cornerAmount <= 3 )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�n�ؤߦh��ή��¦h�䫬�u����ӳ��I�C" << std::endl;
				}

				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polygon , i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polygon , i , IS_OUTSIDE );//�٨S�g
#pragma endregion
#pragma region 1.2:�ˬd�o�ӧΪ��O���O��n�Q�����Ψ�����
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					if( splitPlane -> IsInside( surfacePoint -> position ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//���I�binside�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//���I�binside�A���X�Ӫ��Ϊ�������
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//���I�bOUTSIDE�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//���I�bOUTSIDE�A���X�Ӫ��Ϊ�������
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
#pragma endregion
#pragma region 1.3:��ܭ��@�Ӧh�䫬����a����I
				GLfloat distanceToPolygonInside = polygonInside -> DistanceToPoint( surfacePoint );//�٨S�g
				GLfloat distanceToPolygonOutside = polygonOutside -> DistanceToPoint( surfacePoint );

				MyPolygon *oldPolygon = segmentFace -> polygon;//�������I�i���I�ӥ�
				if( distanceToPolygonInside < distanceToPolygonOutside )
				{
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
				}
				else
				{
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
				}
				delete oldPolygon;
#pragma endregion
			}
		}
	}
	if(  segmentFace -> polygon != NULL )
	{
		if( segmentFace -> polygon -> cornerAmount == 2 )
		{
			std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���A�s�h�䫬�u����ӳ��I" << std::endl;
		}
	}
	

	//2.�����p�G���S����A�N�i�H�����V�U�����A�p�G������A�N�n�ھڤ���H��i�H������������
	if( !segmentFace -> isPolygon )
	{
		for( int i = 0; i < 3; i++ )
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			if( !segmentFace -> ListContainFace( segmentFace -> polyhedronFace -> connectedFace[i] -> index ) )
			{
				PolyhedronPoint *connectedPoint0 , *connectedPoint1;
				connectedPoint0 = NULL;
				connectedPoint1 = NULL;
				segmentFace -> polyhedronFace -> GetPropagatingLine( segmentFace -> polyhedronFace -> connectedFace[i] , 
																	 connectedPoint0 , connectedPoint1 );
				Line* propagatingLine = new Line( connectedPoint0 -> position , connectedPoint1 -> position );
				
				SegmentFace* propagatedSegmentFace = new SegmentFace( segmentFace -> polyhedronFace -> connectedFace[i] , propagatingLine );
				segmentFace -> AddToLink( propagatedSegmentFace );
				if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�H�����n�L�j" << std::endl;
					segmentFace -> firstSegmentFace -> isTooBig = true;
					return false;
				}
				//�}�l�U�@���a�^�A�d�ݤU�@�ӭ����S���ۥ�
				CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
			}
		}
	}
	else
	{
		std::cout << std::endl << "�h��Υ]�t";
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )
		{
			std::cout << "�I(" << segmentFace -> polygon -> corner[i] -> position[0] << ","
							   << segmentFace -> polygon -> corner[i] -> position[1] << ","
							   << segmentFace -> polygon -> corner[i] -> position[2] << ")�B" << std::endl;
		}

		
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )//��C��corner��L���U�@���I���s�u���@��line�A�ݬݬO���O�i�H������t�@�ӭ���line
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			Line* propagatingLine;
			if( i == segmentFace -> polygon -> cornerAmount - 1 ) //����i���I�M��i+1���I���s�u
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[0] -> position );
			else
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[i+1] -> position );
			
			if( segmentFace -> polyhedronFace -> Contain( propagatingLine ) )
			{
				PolyhedronPoint* propagatingPolyhedronFace = segmentFace -> polyhedronFace -> GetPolyhedronFace( propagatingLine );//�٨S�g
				
				
				if( !segmentFace -> ListContainFace( propagatingPolyhedronFace -> index ) )//�p�G�o�ӭ��٨S���Q�B�z�L�A�~�n�B�z
				{
					SegmentFace* propagatedSegmentFace = new SegmentFace( propagatingPolyhedronFace , propagatingLine );
					segmentFace -> AddToLink( propagatedSegmentFace );
					if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
					{
						std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�H�����n�L�j" << std::endl;
						segmentFace -> firstSegmentFace -> isTooBig = true;
						return false;
					}

					CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
				}
			}
		}
	}



	return true;


}
//�����X�S���Q����������
bool Segmenter::CheckIntersectionOnSplitPlane( Line* sLine , Plane* splitPlane , SegmentFace* segmentFace )
{
	std::cout << count << ":�s��segmentFace" << segmentFace << "�n�d�ݬO�_�Q���έ����ΡA�ثe���n�O" << segmentFace -> GetTotalArea() << std::endl;
	count++;
	
	//1.��Ҧ����������@���A�ݤ��X�Ӫ��h�������
	for( Plane* i = splitPlane; i != NULL; i = i -> link )
	{
		//���Ʀh��δN�O�T���ΡA�ҥH�u�n�ˬd�T����
		if( !segmentFace -> isPolygon )
		{
			if( i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[1] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[0] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) ||
				i -> CheckIntersection( segmentFace -> polyhedronFace -> connectedPoint[1] -> position , 
										segmentFace -> polyhedronFace -> connectedPoint[2] -> position ) )//�٨S�g
			{
#pragma region 1.1:�Q�Υ������ΥX�s�h���
				
				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
													 i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polyhedronFace -> connectedPoint[0] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[1] -> position ,
													 segmentFace -> polyhedronFace -> connectedPoint[2] -> position ,
													 i , IS_OUTSIDE );//�٨S�g
#pragma endregion
#pragma region 1.2:�ˬd�o�ӧΪ��O���O��n�Q�����Ψ�����
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					GLfloat middlePointOfSourceLine[] = { 0 , 0 , 0 };
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> initPosition );
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> endPosition );
					/**/MathHelper::VectorMutiply( middlePointOfSourceLine , 0.5 );//���ӷ��u�q�����I

					if( splitPlane -> IsInside( middlePointOfSourceLine ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//���I�binside�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//���I�binside�A���X�Ӫ��Ϊ�������
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//���I�bOUTSIDE�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							segmentFace -> isPolygon = false;
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//���I�bOUTSIDE�A���X�Ӫ��Ϊ�������
						{
							segmentFace -> isPolygon = false;
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
				segmentFace -> isPolygon = true;
#pragma endregion
#pragma region 1.3:��ܭ��@�Ӧh�䫬����a����I
				//�p�G�T���Φ��ۥ�A�N�n���ΥX�h��ΡA�M��ھڳo�Ӧh��Φ��S���]�t�ӷ���A�ӿ�ܭn�έ��@�Ӧh���
				if( polygonInside -> Contain( sLine ) )
				{
					std::cout << "�إ߷s�h���polygonInside" << std::endl;
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
				}
				else if( polygonOutside -> Contain( sLine ) )
				{
					std::cout << "�إ߷s�h���polygonOutside" << std::endl;
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
				}
				else
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G���ΥX�Ӫ��h��Τ���M�����ӷ���۲�" << std::endl;
					segmentFace -> isPolygon = false;
					delete polygonInside;
					delete polygonOutside;

				}
#pragma endregion
			}
		}
		else
		{
			if( segmentFace -> polygon -> CheckIntersectionPolygonOnPlane( i ) )
			{
#pragma region 1.1:�Q�Υ������ΥX�s�h���
				MyPolygon *polygonInside = new MyPolygon( segmentFace -> polygon , i , IS_INSIDE );
				MyPolygon *polygonOutside = new MyPolygon( segmentFace -> polygon , i , IS_OUTSIDE );//�٨S�g
#pragma endregion
#pragma region 1.2:�ˬd�o�ӧΪ��O���O��n�Q�����Ψ�����
				if( polygonInside -> corner == NULL || polygonOutside -> corner == NULL )
				{
					GLfloat middlePointOfSourceLine[] = { 0 , 0 , 0 };
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> initPosition );
					/**/MathHelper::VectorPlus( middlePointOfSourceLine , sLine -> endPosition );
					/**/MathHelper::VectorMutiply( middlePointOfSourceLine , 0.5 );//���ӷ��u�q�����I

					if( splitPlane -> IsInside( middlePointOfSourceLine ) )//IS_INSIDE
					{
						if( polygonInside -> corner == NULL )//���I�binside�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
						else//���I�binside�A���X�Ӫ��Ϊ�������
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
					}
					else//IS_OUTSIDE
					{
						if( polygonInside -> corner == NULL )//���I�bOUTSIDE�A���X�Ӫ��Ϊ����s�b(�S����)
						{
							delete polygonInside;
							delete polygonOutside;
							continue;
						}
						else//���I�bOUTSIDE�A���X�Ӫ��Ϊ�������
						{
							DeleteFromLink( segmentFace );
							delete polygonInside;
							delete polygonOutside;
							//delete segmentFace;
							return false;
						}
					}
				}
#pragma endregion
#pragma region 1.3:��ܭ��@�Ӧh�䫬����a����I
				MyPolygon *oldPolygon = segmentFace -> polygon;//�������I�i���I�ӥ�
				if( polygonInside -> Contain( sLine ) )
				{
					std::cout << "�N���h�����ΥX�s�h���polygonInside" << std::endl;
					segmentFace -> polygon = polygonInside;
					delete polygonOutside;
					delete oldPolygon;
				}
				else if( polygonOutside -> Contain( sLine ) )
				{
					std::cout << "�N���h�����ΥX�s�h���polygonOutside" << std::endl;
					segmentFace -> polygon = polygonOutside;
					delete polygonInside;
					delete oldPolygon;
				}
				else
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G���ΥX�Ӫ��h��Τ���M�����ӷ���۲�" << std::endl;
					delete polygonInside;
					delete polygonOutside;
				}
				
#pragma endregion
			}
		}
	}

	//2.�����p�G���S����A�N�i�H�����V�U�����A�p�G������A�N�n�ھڤ���H��i�H������������
	if( !segmentFace -> isPolygon )
	{
		for( int i = 0; i < 3; i++ )
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			//�U���o��ƻ� ���segmentface����C���������]�t�o��index�A�N�N���Ӧ�C�̭����S���o�ӭ��A�N�i�H�ӫ�
			if( !segmentFace -> ListContainFace( segmentFace -> polyhedronFace -> connectedFace[i] -> index ) )
			{
				if( segmentFace -> polyhedronFace -> connectedFace[i] -> type != POLYHEDRON_FACE )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�����쪺�U�@�ӭ��G�@POLYHEDRON_FACE" << std::endl;
					continue;
				}


				//���إX�@���u�A�Ψӳs��U�@�ӭ�
				PolyhedronPoint *connectedPoint0 , *connectedPoint1; 
				connectedPoint0 = NULL;
				connectedPoint1 = NULL;
				segmentFace -> polyhedronFace -> GetPropagatingLine( segmentFace -> polyhedronFace -> connectedFace[i] , 
																	 connectedPoint0 , connectedPoint1 );//���S�g
				Line* propagatingLine = new Line( connectedPoint0 -> position , connectedPoint1 -> position );//!!!!!!�o�̥X��
				if( segmentFace -> polyhedronFace -> connectedFace[i] -> index > 999999 )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�{�䪺�����줣�s�b��" << std::endl;
				}
				//��O�U�@�ӭ�
				
				SegmentFace* propagatedSegmentFace = new SegmentFace( segmentFace -> polyhedronFace -> connectedFace[i] , propagatingLine );//�٨S�g
				segmentFace -> AddToLink( propagatedSegmentFace );
				if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
				{
					std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�H�����n�L�j" << std::endl;
					segmentFace -> firstSegmentFace -> isTooBig = true;
					return false;
				}
				//�}�l�U�@���a�^�A�d�ݤU�@�ӭ����S���ۥ�
				CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
			}
		}
	}
	else
	{
		std::cout << std::endl << "�h��Υ]�t";
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )
		{
			std::cout << "�I(" << segmentFace -> polygon -> corner[i] -> position[0] << ","
							   << segmentFace -> polygon -> corner[i] -> position[1] << ","
							   << segmentFace -> polygon -> corner[i] -> position[2] << ")�B" << std::endl;
		}
		for( int i = 0; i < segmentFace -> polygon -> cornerAmount; i++ )//��C��corner��L���U�@���I���s�u���@��line�A�ݬݬO���O�i�H������t�@�ӭ���line
		{
			if( segmentFace -> firstSegmentFace -> isTooBig ) return false;
			Line* propagatingLine;
			if( i == segmentFace -> polygon -> cornerAmount - 1 ) //����i���I�M��i+1���I���s�u
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[0] -> position );
			else
				propagatingLine = new Line( segmentFace -> polygon -> corner[i] -> position , 
											segmentFace -> polygon -> corner[i+1] -> position );
			
			if( segmentFace -> polyhedronFace -> Contain( propagatingLine ) )//�u�b�o���H������]�t�o���u�A�]�N�O�o���u�b��W
			{
				PolyhedronPoint* propagatingPolyhedronFace = segmentFace -> polyhedronFace -> GetPolyhedronFace( propagatingLine );//������@���o���䪺��
				
				if( !segmentFace -> ListContainFace( propagatingPolyhedronFace -> index ) )//�p�G�o�ӭ��٨S���Q�B�z�L�A�~�n�B�z
				{
					if( propagatingPolyhedronFace -> type != POLYHEDRON_FACE )//�ˬd������
					{
						std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�{�䪺�����줣�s�b��" << std::endl;
						continue;
					}
					SegmentFace* propagatedSegmentFace = new SegmentFace( propagatingPolyhedronFace , propagatingLine );
					segmentFace -> AddToLink( propagatedSegmentFace );
					//�p�G�H�����n�L�j�A�¥@���X��
					if( propagatedSegmentFace -> GetTotalArea() > SEGMENT_AREA_LIMIT )
					{
						std::cout << "Segmenter::CheckIntersectionOnSplitPlane�X���G�H�����n�L�j" << std::endl;
						segmentFace -> firstSegmentFace -> isTooBig = true;
						return false;
					}
					CheckIntersectionOnSplitPlane( propagatingLine , splitPlane , propagatedSegmentFace );
				}
			}
		}
	}

	return true;
}
//��O�H��
bool Segmenter::GenerateSegment( bool isGenerator )
{
	int count = 0;
	//��C�@��surfacePointList�����L�@�M�A���@�ӳ����ͤ@�ӸH��
	for( Point* i = surfacePointList; i != NULL; i = i -> link )
	{
		std::cout << "===�}�l�p���" << count << "�ӸH���Asocket�O(" << i -> position[0] << "," << i -> position[1] << "," << i -> position[2] << ")" << std::endl;
		//std::vector<int> faceNumber;//�]�t���T���θ��X
		std::vector<int> splitPlaneNumber;
		Plane* splitPlane; splitPlane = NULL;
		SegmentFace* segmentFace; segmentFace = NULL;

		//��C���I�إߤ�����
		for( Point* j = surfacePointList; j != NULL; j = j -> link )
		{
			if( i == j )continue;
			//std::cout << "�������I���Z���@" << i -> DistanceToPoint( j -> position ) << std::endl;
			if( i -> DistanceToPoint( j -> position ) > SPLIT_SURFACE_LIMIT ) continue;
			Plane* tempSplitPlane = new Plane( splitPlaneNumber.size() );//�N���歱���s��+1
			splitPlaneNumber.push_back( splitPlaneNumber.size() );
			tempSplitPlane -> SetSplitSurface( i -> position , j -> position );
			if( splitPlane == NULL )//�N�o�Ӥ��歱�[�i�h�m�`��C��
			{
				splitPlane = tempSplitPlane;
				splitPlane -> firstPlane = splitPlane;
			}
			else
			{
				splitPlane -> AddToLink( tempSplitPlane );
			}
		}
		//std::cout << "�`�@�إ�" << splitPlaneNumber.size() << "�ӥ���" << std::endl;
		//int initFace = polyhedron -> GetClosestFace( i -> position );//�a���I
		//faceNumber.push_back( initFace );

		//��X��ӭ��Q���ΥH�᪺�ˤl
		//����@�ӥ�������ҫ��q����b�A����d�U�̫�@�s���O�S���Q���󭱤��쪺�A�N�O�ڭ̳̲ת��H��
		//�n����C�@�ӭ�����Ĥ@�ӥ����ycheckIntersection�A�o�{�L�̦��S���򥭭��ۤ��A�p�G�����ܴN������S���Q���쪺������
		//�S�����쪺�ܴN�~�򩵦���U�@�ӭ���checkIntersection
		//�p�G�����쪺���A�����⨺���ѵ�������p���H���A����ncheckIntersection�ɡA�N�u�P�_���ӧ�p���H��
		

		CheckIntersectionOnSplitPlane( i , splitPlane , segmentFace );

		std::cout << count << std::endl;
		//�M��n�إX���έ�
		if( segmentFace!= NULL )
		{
			if( !segmentFace -> firstSegmentFace -> isTooBig )
			{
				GenerateSplitSurface( splitPlane , segmentFace , i );
			}
		}
		
		//Delete( segmentFace );
		Delete( splitPlane );//�٨S�g
	}

	//�p�G�O�Ψӥͦ����έ����A�N���n�ȷN�h�X�֤��ݩ����H�������A���M�|�X��
	if( isGenerator == GENERATOR ) return true;

	//���U�ӭn�M��A���S���ҫ��������ݩ����@��segment�A�p�G�����ܥN��o�ӭ����ӬO�ݩ�F��segment�A���O�Q���~�����ζ}��
	//�ҥH�N�n��۾F��segment�P�o�ǭ��X�֦b�@�_
	for( int i = 0; i < polyhedron -> faceAmount; i++ )
	{
		SegmentFace *tempModelFace = new SegmentFace( polyhedron -> GetPolyhedronFace( i ) );
		bool isBelongToSegment = false;
		for( Segment* s = segment; s != NULL; s = s -> link )
		{
			if( tempModelFace -> BelongToSegment( s -> segmentFace ) )//�o�ӭn�hsegmentface�̭��g
			{
				isBelongToSegment = true;
			}
		}
		Segment* segmentToCombine;
		if( !isBelongToSegment )
		{
			std::cout << "���T���Τ��ݩ����@�H���G(" << 
						 tempModelFace -> polyhedronFace -> connectedPoint[0] -> position[0] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[0] -> position[1] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[0] -> position[2] << "),(" <<
						 tempModelFace -> polyhedronFace -> connectedPoint[1] -> position[0] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[1] -> position[1] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[1] -> position[2] << "),(" <<
						 tempModelFace -> polyhedronFace -> connectedPoint[2] -> position[0] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[2] -> position[1] << "," <<
						 tempModelFace -> polyhedronFace -> connectedPoint[2] -> position[2] << ")" << std::endl;
			//����X�۾F���H���A�M���o�Ǥ��ݩ����H�������[�J�o�ӸH����
			segmentToCombine = FindAdjacentSegment( tempModelFace -> polyhedronFace );//�o��Ӽg�bsegmenter�̭�
			std::cout << "��T���Υ[�J�o�ӸH���G(" << segmentToCombine -> surfacePoint -> position[0] << "," <<
													  segmentToCombine -> surfacePoint -> position[1] << "," <<
													  segmentToCombine -> surfacePoint -> position[2] << ")" << std::endl;
			CombineSegment( tempModelFace , segmentToCombine );
		}
	}

	return true;
}
//��O���έ�
bool Segmenter::GenerateSplitSurface( Plane* splitPlane , SegmentFace* segmentFace , Point* sPoint )
{

	if( segmentFace -> firstSegmentFace -> isTooBig )
	{
		std::cout << " Segmenter::GenerateSplitSurface�X���G�ӸH�����n�Ӥj" << std::endl;
	}
	Segment* tempSegment = new Segment( segmentFace , polyhedron );
	tempSegment -> surfacePoint = sPoint;
	//debug
	//GLfloat test[3] = { 125 , 287 , -18 };
	//if( sPoint ->DistanceToPoint( test ) < 10.0 )
	//{
	//	PrintSegment( tempSegment );
	//}

	for( Plane* j = splitPlane -> firstPlane; j != NULL; j = j -> link )
	{
		//�P�_j�O���O���Ψ쪺��		
		if( segmentFace -> IsConnectedWith( j ) )//i�H���O�_���Mj�����۳s
		{
			//�����ܡA�}�l�䭱���̤p����
			Point* endPoint0 = new Point();
			Point* endPoint1 = new Point();
			//����̨�ݪ��I�A�A�⨺����I�����L�������A�ݭ���ӥ������Ψ�o����I
			segmentFace -> FindEndPointOnPlane( j , endPoint0 -> position , endPoint1 -> position );//�٨S�g//i�H���h�M��Mj�����۳s���u�_�������
			//�̫�N�γo�T�ӥ����������έ�

			std::cout << "��(" << endPoint0 -> position[0] << "," << endPoint0 -> position[1] << "," << endPoint0 -> position[2] << ")" << std::endl;
			std::cout << "�M(" << endPoint1 -> position[0] << "," << endPoint1 -> position[1] << "," << endPoint1 -> position[2] << ")��@���έ��ⰼ" << std::endl;

			//�������u
			Line* line0 = new Line();
			Line* line1 = new Line();


			for( Plane* k = splitPlane -> firstPlane; k != NULL; k = k -> link )
			{
				if( j == k ) continue;
				if( segmentFace -> IsConnectedWith( k ) )//��k�����]�Pi�۳s
				{
					if( k -> Contain( endPoint0 -> position ) )//���I�bk�����W�A�N��k�O�䭱
					{
						j -> Get2PlaneIntersectLine_2( k , line0 );//�٨S�g
					}

					if( k -> Contain( endPoint1 -> position ) )
					{
						j -> Get2PlaneIntersectLine_2( k , line1 );
					}
				}
			}

			if( MathHelper::VectorAbsolution( line0 -> endPosition) == 0 )
			{
				std::cout << "Segmenter::GenerateSplitSurface�X���G�Ĥ@���u�S���V�q" << std::endl;
				return false;
			}

			std::cout << "���ʫe��uLine0���I�G(" << line0 -> initPosition[0] << "," << line0 -> initPosition[1] << "," << line0 -> initPosition[2] 
					  << ")�A��V�G("  << line0 -> endPosition[0] << "," << line0 -> endPosition[1] << "," << line0 -> endPosition[2] << ")" << std::endl;
			std::cout << "���ʫe��uLine1���I�G(" << line1 -> initPosition[0] << "," << line1 -> initPosition[1] << "," << line1 -> initPosition[2] 
					  << ")�A��V�G("  << line1 -> endPosition[0] << "," << line1 -> endPosition[1] << "," << line1 -> endPosition[2] << ")" << std::endl;


			//���Ӳ{�����I����ۻE�̪񪺦�m
			line0 -> TranslateToNearest_2( line1 );
			//line0 -> TranslateToNearest( line1 );

			//������u���V�q���u�V��Ӻ��I endPoint0 , endPoint1
			GLfloat line0ToEndPoint0[3];
			GLfloat line1ToEndPoint1[3];
			MathHelper::VectorMinus( line0ToEndPoint0 , endPoint0 -> position , line0 -> initPosition );//���I0��line0���I
			MathHelper::VectorMinus( line1ToEndPoint1 , endPoint1 -> position , line1 -> initPosition );//���I1��line1���I
			if( !( MathHelper::VectorDot( line0 -> endPosition , line0ToEndPoint0 ) > 0 ) ) 
			{
				std::cout << "�u�q0�N��V����" << std::endl;
				MathHelper::VectorReverse( line0 -> endPosition );
			}
				
			if( !( MathHelper::VectorDot( line1 -> endPosition , line1ToEndPoint1 ) > 0 ) ) 
			{
				std::cout << "�u�q1�N��V����" << std::endl;
				MathHelper::VectorReverse( line1 -> endPosition );
			}
				
			///������u���V�q���u�V��Ӻ��I endPoint0 , endPoint1
			GLfloat aa = MathHelper::VectorDot( line1 -> endPosition , line1ToEndPoint1 );

			std::cout << "��uLine0���I�G(" << line0 -> initPosition[0] << "," << line0 -> initPosition[1] << "," << line0 -> initPosition[2] 
					  << ")�A��V�G("  << line0 -> endPosition[0] << "," << line0 -> endPosition[1] << "," << line0 -> endPosition[2] << ")" << std::endl;
			std::cout << "��uLine1���I�G(" << line1 -> initPosition[0] << "," << line1 -> initPosition[1] << "," << line1 -> initPosition[2] 
					  << ")�A��V�G("  << line1 -> endPosition[0] << "," << line1 -> endPosition[1] << "," << line1 -> endPosition[2] << ")" << std::endl;

			Line* connectingLine; connectingLine = NULL;
			segmentFace -> GetConnectingLine( j , connectingLine );//�٨S�g//

			SplitSurface* tempSplitSurface = new SplitSurface( j );//�٨S�g
			tempSplitSurface -> SetCorner( connectingLine , line0 , line1 );

			//��o�@�Ӥ��έ��[��H��i�����έ���C��
			if( tempSegment -> splitSurface == NULL )
			{
				tempSegment -> splitSurface = tempSplitSurface;
				tempSegment -> splitSurface -> firstSplitSurface = tempSplitSurface;
			}
			else
			{
				tempSegment -> splitSurface -> AddToLink( tempSplitSurface );//�٨S�g
			}

			delete endPoint0;
			delete endPoint1;
			delete line0;
			delete line1;
			//Delete( connectingLine );//�٨S�g
		}
	}
	if( segment == NULL )
	{
		segment = tempSegment;
		segment -> firstSegment = tempSegment;
	}
	else
	{
		segment -> AddToLink( tempSegment );//�٨S�g
	}
	//}
	return true;
}

bool Segmenter::ConstructSplitSurface()
{
	//splitSurfaceConstructor = new SplitSurfaceConstructor();
	//splitSurfaceConstructor -> segment = segment;
	//splitSurfaceConstructor -> originalSplitSurface = segment -> splitSurface;//���ոլݥu�Τ@���H��
	
	//1.������Ӥ��έ��إ߾��}�C
	int segmentCount = 0;
	int splitSurfaceAmount = 0;
	for( Segment* s = segment -> firstSegment; s != NULL; s = s -> link )
	{
		segmentCount++;
		//splitSurfaceConstructor -> originalSplitSurface = s -> splitSurface;
		for( SplitSurface* ss = s -> splitSurface -> firstSplitSurface; ss != NULL; ss = ss -> link )
		{
			//NewSplitSurface( ss );
			splitSurfaceAmount++;
		}
	}
	splitSurfaceConstructor = new SplitSurfaceConstructor*[ segmentCount ];

	//2.�A�ئU�O�إ߾�������
	segmentCount = 0;
	splitSurfaceAmount = 0;
	for( Segment* s = segment -> firstSegment; s != NULL; s = s -> link )
	{
		
		//splitSurfaceConstructor -> originalSplitSurface = s -> splitSurface;
		for( SplitSurface* ss = s -> splitSurface -> firstSplitSurface; ss != NULL; ss = ss -> link )
		{
			//NewSplitSurface( ss );
			splitSurfaceAmount++;
		}
		splitSurfaceConstructor[ segmentCount ] = new SplitSurfaceConstructor( splitSurfaceAmount );
		segmentCount++;
		splitSurfaceAmount = 0;
	}
	segmentAmount = segmentCount;


	//3.�A��C�ӫإ߮�o���έ����[�i�h
	segmentCount = 0;
	splitSurfaceAmount = 0;
	for( Segment* s = segment -> firstSegment; s != NULL; s = s -> link )
	{
		//splitSurfaceConstructor -> originalSplitSurface = s -> splitSurface;
		for( SplitSurface* ss = s -> splitSurface -> firstSplitSurface; ss != NULL; ss = ss -> link )
		{
			//�o��X��
			//int tempCount = splitSurfaceConstructor -> NewSplitSurface( ss );
			splitSurfaceConstructor[ segmentCount ] -> NewSplitSurface( ss );
		}
		segmentCount++;
	}
	return true;
}

bool Segmenter::Delete( Plane* plane )
{
	if( plane == NULL )
	{
		std::cout << "Segmenter::Delete�X���G�ӸH���S�����󥭭�" << std::endl;
		return false;
	}

	for( Plane* p = plane -> firstPlane; p != NULL; )
	{
		Plane* temp = p -> link;
		delete p;
		p = temp;
	}
	return true;
}

bool Segmenter::Delete( SegmentFace* segmentFace )
{
	if( segmentFace == NULL )
	{
		std::cout << "Segmenter::Delete�X���G�ӸH���S������H��" << std::endl;
		return false;
	}

	for( SegmentFace* p = segmentFace -> firstSegmentFace; p != NULL; )
	{
		SegmentFace* temp = p -> link;
		delete p;
		p = temp;
	}
	return true;
}

bool Segmenter::DeleteFromLink( SegmentFace* segmentFace )
{
	if( segmentFace == NULL )
	{
		std::cout << "Segmenter::DeleteFromLink�X���G�ӸH���S������H��" << std::endl;
		return false;
	}
	for( SegmentFace* p = segmentFace -> firstSegmentFace; p != NULL; p = p -> link )
	{
		if( p -> link == segmentFace )
		{
			delete p -> link;
			p -> link = NULL;
			return true;
		}
	}
	std::cout << "Segmenter::DeleteFromLink�X���G�S�����n�R�����H��" << std::endl;
	return false;
}
//��X�PsegmentFace�۾F��segment�A
Segment* Segmenter::FindAdjacentSegment( PolyhedronPoint* polyhedronFace )
{
	std::vector< int >* faceSearched = new std::vector< int >();
	faceSearched -> push_back( polyhedronFace -> index );
	//�٨S����segmentFace��first�O��
	for( int i = 0; i < 3; i++ )
	{
		Segment* segmentBelongedTo = FindAdjacentSegment( polyhedronFace -> connectedFace[i] , faceSearched );
		if( segmentBelongedTo != NULL )
		{
			delete faceSearched;
			return segmentBelongedTo;
		}
	}
	delete faceSearched;
	return NULL;
}

Segment* Segmenter::FindAdjacentSegment( PolyhedronPoint* polyhedronFace , std::vector<int>* faceSearched )
{
	//���ݳo�ӭ����S����L
	for( int i = 0; i < faceSearched -> size(); i++  )
	{
		if( faceSearched -> at(i) == polyhedronFace -> index )
		{
			return NULL;
		}
	}
	faceSearched -> push_back( polyhedronFace -> index );

	for( int i = 0; i < 3; i++ )
	{
		Segment* segmentBelongedTo = FindBelongToSegment( polyhedronFace -> connectedFace[i] );
		if( segmentBelongedTo != NULL )
		{
			return segmentBelongedTo;
		}
		else
		{
			//��S�����ƹL�����A�ݦ��S������ӸH�����|
			segmentBelongedTo = FindAdjacentSegment( polyhedronFace -> connectedFace[i] , faceSearched );
			if( segmentBelongedTo != NULL )
			{
				return segmentBelongedTo;
			}
		}
	}
	return NULL;
}
//��X�����ӸH���P�o�ӤT���έ��|
Segment* Segmenter::FindBelongToSegment( PolyhedronPoint* polyhedronFace )
{
	for( Segment* s = segment; s != NULL; s = s -> link )
	{
		for( SegmentFace* sf = s -> segmentFace; sf != NULL; sf = sf -> link )
		{
			if( sf -> polyhedronFace -> index == polyhedronFace -> index )
			{
				return s;
			}
			/*
			for( int i = 0; i < 3; i++ )
			{
				if( sf -> isPolygon )
				{
					for( int j = 0; j < sf -> polygon -> cornerAmount; j++ )
					{
						if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] -> position ,
													   sf -> polygon -> corner[j] -> position ) )
						{
							return s;
						}
					}
				}
				else
				{
					for( int j = 0; j < 3; j++ )
					{
						if( MathHelper::CompareVector( polyhedronFace -> connectedPoint[i] -> position ,
													   sf -> polyhedronFace -> connectedPoint[j] -> position ) )
						{
							return s;
						}
					}
				}
			}
			*/

		}
	}
	return NULL;
}
//�M���o�Ǥ��ݩ����H�������[�J�o�ӸH����
bool Segmenter::CombineSegment( SegmentFace* segmentFace , Segment* segmentToCombine )
{
	//segmentFace -> firstSegmentFace = segmentToCombine -> segmentFace -> firstSegmentFace;
	segmentToCombine -> segmentFace -> AddToLink( segmentFace );

	for( int i = 0; i < 3; i++ )
	{
		//�p�G�o�ӥ������ݩ����H���A�N��o�ӥ����[�J���X�֪��H����
		Segment* tempSegment = FindBelongToSegment( segmentFace -> polyhedronFace -> connectedFace[i] );
		if( tempSegment == NULL )
		{
			SegmentFace* tempSegmentFace = new SegmentFace( segmentFace -> polyhedronFace -> connectedFace[i] );
			CombineSegment( tempSegmentFace , segmentToCombine );
		}
		
	}
	return true;
}

void Segmenter::PrintSegment( Segment* s )
{
	for( SegmentFace* sf = s -> segmentFace; sf != NULL; sf = sf -> link )
	{
		if( !sf -> isPolygon )
		{
			std::cout << "�o���H�������@���O�T���ΡA�L���T���I���O�O�G" << std::endl << "(";
			std::cout << sf -> polyhedronFace -> connectedPoint[0] -> position[0] << "," <<
						 sf -> polyhedronFace -> connectedPoint[0] -> position[1] << "," <<
						 sf -> polyhedronFace -> connectedPoint[0] -> position[2] << ")" << std::endl << "(";
			std::cout << sf -> polyhedronFace -> connectedPoint[1] -> position[0] << "," <<
						 sf -> polyhedronFace -> connectedPoint[1] -> position[1] << "," <<
						 sf -> polyhedronFace -> connectedPoint[1] -> position[2] << ")" << std::endl << "(";
			std::cout << sf -> polyhedronFace -> connectedPoint[2] -> position[0] << "," <<
						 sf -> polyhedronFace -> connectedPoint[2] -> position[1] << "," <<
						 sf -> polyhedronFace -> connectedPoint[2] -> position[2] << ")" << std::endl;
		}
		else
		{
			std::cout << "�o���H�������@���O�h���A�L��" << sf -> polygon -> cornerAmount << "���I���O�O�G" << std::endl;
			for( int i = 0; i < sf -> polygon -> cornerAmount; i++ )
			{
				std::cout << "(";
				std::cout << sf -> polygon -> corner[i] -> position[0] << "," <<
							 sf -> polygon -> corner[i] -> position[1] << "," <<
							 sf -> polygon -> corner[i] -> position[2] << ")" << std::endl;
			}
		}
		std::cout << "========================================" << std::endl;
	}
}
