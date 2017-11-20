#include "Plane.h"

//====================================================Line//====================================================
Line::Line()
{
}

Line::Line( GLfloat* iPosition , GLfloat* ePosition )
{
	MathHelper::CopyVector( initPosition , iPosition );
	MathHelper::CopyVector( endPosition , ePosition );
}

Line::~Line()
{
}

bool Line::AddToLink( Line* line )
{
	line -> firstLine = firstLine;

	for( Line* fl = firstLine; fl != NULL; fl = fl -> link )
	{
		if( fl -> link == NULL )
		{
			fl -> link = line;
			break;
		}
	}
	return true;
}

bool Line::InsideInterval( GLfloat number0 , GLfloat number1 , GLfloat testNumber )
{
	if( number0 > number1 )
	{
		if( testNumber <= number0 && testNumber >= number1 )
		{
			return true;
		}
	}
	else
	{
		if( testNumber >= number0 && testNumber <= number1 )
		{
			return true;
		}
	}
	return false;
}


bool Line::Containing( Line* contained )
{/**/
	GLfloat zero[3] = { 0 , 0 , 0 };

	GLfloat init2Init[3];
	/**/MathHelper::VectorMinus( init2Init , contained -> initPosition , initPosition );
	/**/if( MathHelper::VectorApproximation( zero , init2Init ) ) MathHelper::CopyVector( init2Init , zero );
	/**/MathHelper::VectorNormalize( init2Init );

	GLfloat init2End[3];
	/**/MathHelper::VectorMinus( init2End , contained -> endPosition , initPosition );
	/**/if( MathHelper::VectorApproximation( zero , init2End ) ) MathHelper::CopyVector( init2End , zero );
	/**/MathHelper::VectorNormalize( init2End );

	GLfloat end2Init[3];
	/**/MathHelper::VectorMinus( end2Init , contained -> initPosition , endPosition );
	/**/if( MathHelper::VectorApproximation( zero , end2Init ) ) MathHelper::CopyVector( end2Init , zero );
	/**/MathHelper::VectorNormalize( end2Init );

	GLfloat end2End[3];
	/**/MathHelper::VectorMinus( end2End , contained -> endPosition , endPosition );
	/**/if( MathHelper::VectorApproximation( zero , end2End ) ) MathHelper::CopyVector( end2End , zero );
	/**/MathHelper::VectorNormalize( end2End );

	GLfloat direction[3];
	/**/MathHelper::VectorMinus( direction , endPosition , initPosition );
	/**/MathHelper::VectorNormalize( direction );

	GLfloat directionInverse[3];
	/**/MathHelper::CopyVector( directionInverse , direction );
	/**/MathHelper::VectorReverse( directionInverse );

	if( ( MathHelper::VectorApproximation( direction , init2Init ) || MathHelper::CompareVector( zero , init2Init ) ) &&
		( MathHelper::VectorApproximation( direction , init2End ) || MathHelper::CompareVector( zero , init2End ) ) &&
		( MathHelper::VectorApproximation( directionInverse , end2Init ) || MathHelper::CompareVector( zero , end2Init ) ) &&
		( MathHelper::VectorApproximation( directionInverse , end2End ) || MathHelper::CompareVector( zero , end2End ) ) )
	{
		return true;
	}
	if( ( MathHelper::VectorApproximation( directionInverse , init2Init ) || MathHelper::CompareVector( zero , init2Init ) ) &&
		( MathHelper::VectorApproximation( directionInverse , init2End ) || MathHelper::CompareVector( zero , init2End ) ) &&
		( MathHelper::VectorApproximation( direction , end2Init ) || MathHelper::CompareVector( zero , end2Init ) ) &&
		( MathHelper::VectorApproximation( direction , end2End ) || MathHelper::CompareVector( zero , end2End ) ) )
	{
		return true;
	}

	return false;


	/*
	if( InsideInterval( initPosition[0] , endPosition[0] , contained -> initPosition[0] ) &&
		InsideInterval( initPosition[1] , endPosition[1] , contained -> initPosition[1] ) &&
		InsideInterval( initPosition[2] , endPosition[2] , contained -> initPosition[2] ) &&
		InsideInterval( initPosition[0] , endPosition[0] , contained -> endPosition[0] ) &&
		InsideInterval( initPosition[1] , endPosition[1] , contained -> endPosition[1] ) &&
		InsideInterval( initPosition[2] , endPosition[2] , contained -> endPosition[2] ) )
	{
		//���դ�V�O�_�ۦP
		GLfloat direction[3];
		GLfloat toContained[3];
		GLfloat containedDirection[3];
		GLfloat containedDirectionReverse[3];
		GLfloat isZero[3] = { 0 , 0 , 0 };

		MathHelper::VectorMinus( direction , endPosition , initPosition );
		MathHelper::VectorMinus( toContained , contained -> initPosition , initPosition );
		MathHelper::VectorMinus( containedDirection , contained -> endPosition , contained -> initPosition );

		MathHelper::VectorNormalize( direction );
		MathHelper::VectorNormalize( toContained );
		MathHelper::VectorNormalize( containedDirection );
		MathHelper::CopyVector( containedDirectionReverse , containedDirection ); 
		MathHelper::VectorReverse( containedDirectionReverse );
		if( ( MathHelper::VectorApproximation( direction , toContained ) || MathHelper::VectorApproximation( isZero , toContained ) ) &&
			( MathHelper::VectorApproximation( direction , containedDirection ) || MathHelper::VectorApproximation( direction , containedDirectionReverse ) ) )
		{
			return true;
		}
	}
	return false;
	*/
}

bool Line::TranslateToNearest( Line* line )
{
	// dist3D_Line_to_Line(): get the 3D minimum distance between 2 lines
//    Input:  two 3D lines L1 and L2
//    Return: the shortest distance between L1 and L2
	
    //Vector   u = L1.P1 - L1.P0;
	Point* u = new Point( endPosition );
    //Vector   v = L2.P1 - L2.P0;
	Point* v = new Point( line -> endPosition );
    //Vector   w = L1.P0 - L2.P0;
	Point* w = new Point(); w -> Vector( line -> initPosition , initPosition );
    //float    a = dot(u,u);         // always >= 0
	GLfloat a = u -> Dot( u );
    //float    b = dot(u,v);
	GLfloat b = u -> Dot( v );
    //float    c = dot(v,v);         // always >= 0
	GLfloat c = v -> Dot( v );
    //float    d = dot(u,w);
	GLfloat d = u -> Dot( w );
    //float    e = dot(v,w);
	GLfloat e = v -> Dot( w );
    GLfloat D = a*c - b*b;        // always >= 0
    float    sc, tc;

	//GLfloat SMALL_NUM = 0.0001
    // compute the line parameters of the two closest points
    //if( D < SMALL_NUM ) 
	if( D < SMALL_NUMBER )
	{          // the lines are almost parallel
        sc = 0.0;
        tc = (b>c ? d/b : e/c);    // use the largest denominator
		//����
		//line�����I = �쥻���I + �V�q*tc
		/*
		Point* tempLinePosition = new Point( line -> endPosition );
		tempLinePosition -> Multiply( tc );
		tempLinePosition -> Plus( line -> initPosition );
		MathHelper::CopyVector( line -> initPosition , tempLinePosition -> position );
		*/
		/*
		//��ӽu�����I���V��h�@�d��
		Point* vectorMul1000 = new Point( endPosition );
		vectorMul1000 -> Multiply( 1000 );
		MathHelper::VectorMinus( initPosition , vectorMul1000 -> position );
		MathHelper::VectorMinus( line -> initPosition , vectorMul1000 -> position );
		
		delete vectorMul1000;
		delete u;
		delete v;
		delete w;
		delete tempLinePosition;
		return true;*/
    }
    else 
	{
        sc = (b*e - c*d) / D;
        tc = (a*e - b*d) / D;
    }

	// w �O p0 o<---------o linep0
    // get the difference of the two closest points
    //Vector   dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)
	Point* dP = new Point( w -> position );
	Point* scMulu = new Point( u -> position );
	scMulu -> Multiply( sc );
	Point* tcMulv = new Point( v -> position );
	scMulu -> Multiply( tc );
	dP -> Plus( scMulu );
	dP -> Minus( tcMulv );
	std::cout << "Line::TranslateToNearest���I���̤p�Z���O" << dP -> Absolution() << std::endl;
	delete dP;
	delete scMulu;
	delete tcMulv;


	//���I = �쥻���I + �V�q*sc
	Point* tempLinePosition = new Point( endPosition );
	tempLinePosition -> Multiply( sc );
	tempLinePosition -> Plus( initPosition );
	MathHelper::CopyVector( initPosition , tempLinePosition -> position );
	//line�����I = �쥻���I + �V�q*tc
	tempLinePosition -> Copy( line -> endPosition );
	tempLinePosition -> Multiply( tc );
	tempLinePosition -> Plus( line -> initPosition );
	MathHelper::CopyVector( line -> initPosition , tempLinePosition -> position );
	/*
	//��ӽu�����I���V��h�@�d��
	Point* vectorMul1000 = new Point( endPosition );
	vectorMul1000 -> Multiply( 1000 );
	MathHelper::VectorMinus( initPosition , vectorMul1000 -> position );
	if( vectorMul1000 -> Dot( line -> endPosition) > 0 )//�N����line����V�O�ۦP��
	{
		vectorMul1000 -> Copy( line -> endPosition );
		vectorMul1000 -> Multiply( 1000 );
		MathHelper::VectorMinus( line -> initPosition , vectorMul1000 -> position );
	}
	else//�N����line����V�O�ۤϪ��A�N�n�ΤϤ�V���V�q�h��
	{
		vectorMul1000 -> Copy( line -> endPosition );
		vectorMul1000 -> Multiply( 1000 );
		MathHelper::VectorPlus( line -> initPosition , vectorMul1000 -> position );
	}

	delete vectorMul1000;*/
	delete u;
	delete v;
	delete w;
	delete tempLinePosition;
    return true;   // return the closest distance

}

bool Line::TranslateToNearest_2( Line* line )
{
	/*
	https://www.mathworks.com/matlabcentral/newsreader/view_thread/246420
	nA = dot(cross(B2-B1,A1-B1),cross(A2-A1,B2-B1));
	 nB = dot(cross(A2-A1,A1-B1),cross(A2-A1,B2-B1));
	 d = dot(cross(A2-A1,B2-B1),cross(A2-A1,B2-B1));
	 A0 = A1 + (nA/d)*(A2-A1);
	 B0 = B1 + (nB/d)*(B2-B1);
	 */
	/*�γo��
	http://www.mathworks.com/matlabcentral/newsreader/view_thread/149132
	 U = A2-A1;
	 V = B2-B1;
	 W = cross(U,V);
	 X = A1 + dot(cross(B1-A1,V),W)/dot(W,W)*U;
	 Y = B1 + dot(cross(B1-A1,U),W)/dot(W,W)*V;
	 d = norm(Y-X);

	  A point P belongs to the plane containing line A1A2 and orthogonal to
	line XY if:

	 dot(W,P-A1) = 0.
	*/
	// U = A2-A1;
	Point* U = new Point( endPosition );
	// V = B2-B1;
	Point* V = new Point( line -> endPosition );
	// W = cross(U,V);
	Point* W = new Point(); W -> Cross( U , V );
	if( W -> Absolution() < SMALL_NUMBER )
	{
		delete U;
		delete V;
		delete W;
		return false;
	}
	Point* B1_A1 = new Point(); B1_A1 -> Vector( initPosition , line -> initPosition );
	Point* crossB1_A1_V = new Point(); crossB1_A1_V -> Cross( B1_A1 , V );
	Point* crossB1_A1_U = new Point(); crossB1_A1_U -> Cross( B1_A1 , U );
	// X = A1 + dot(cross(B1-A1,V),W)/dot(W,W)*U;
	Point* X = new Point( U -> position ); 
	X -> Multiply( crossB1_A1_V -> Dot( W ) / W -> Dot( W ) );
	if( X -> Absolution() > 1000 )//�p�G�o���I�|�]�ӷ��A�N�G�n�]���򻷡A�̦h�]1000
	{
		GLfloat multiplier = 1000.0 / X -> Absolution();
		X -> Multiply( multiplier );
	}
	X -> Plus( initPosition );
	//Y = B1 + dot(cross(B1-A1,U),W)/dot(W,W)*V;
	Point* Y = new Point( V -> position ); 
	Y -> Multiply( crossB1_A1_U -> Dot( W ) / W -> Dot( W ) );
	if( Y -> Absolution() > 1000 )//�p�G�o���I�|�]�ӷ��A�N�G�n�]���򻷡A�̦h�]1000
	{
		GLfloat multiplier = 1000.0 / Y -> Absolution();
		Y -> Multiply( multiplier );
	}
	Y -> Plus( line -> initPosition );
	//d = norm(Y-X);
	std::cout << "�Υt�@�Ӥ�k��X�Ӫ��Z���O" << X -> DistanceToPoint( Y -> position ) << std::endl;
	//std::cout << "�s���I��m�O�I1(" << X -> position[0] << "," << X -> position[1] << "," << X -> position[2] << ")" << std::endl;
	//std::cout << "�s���I��m�O�I2(" << Y -> position[0] << "," << Y -> position[1] << "," << Y -> position[2] << ")" << std::endl;
	//�o��ݺ�X�Ӫ���m�|���|�]�ӻ��A�p�G�]�ӻ��N�G�n��
	if( MathHelper::VectorDistance( initPosition , X -> position ) > 1001.0 ||
		MathHelper::VectorDistance( line -> initPosition , Y -> position) > 1001.0 )
	{
		delete U;
		delete V;
		delete W;
		delete crossB1_A1_V;
		delete crossB1_A1_U;
		delete X;
		delete Y;
		std::cout << "Line::TranslateToNearest_2�X���G����u�̬۪��I�����I�ӻ�(�W�L500)" << std::endl;
		return false;
	}
	MathHelper::CopyVector( initPosition , X -> position );
	MathHelper::CopyVector( line -> initPosition , Y -> position );

	delete U;
	delete V;
	delete W;
	delete crossB1_A1_V;
	delete crossB1_A1_U;
	delete X;
	delete Y;

	return true;
}
//====================================================Plane//====================================================

Plane::Plane()
{
}

Plane::Plane( int i )
{
	index = i;
}

Plane::Plane( GLfloat* pos , GLfloat* vec )
{
	MathHelper::CopyVector( position , pos );
	MathHelper::CopyVector( vector , vec );
}

Plane::Plane( Plane* p )
{
	MathHelper::CopyVector( position , p -> position );
	MathHelper::CopyVector( vector , p -> vector );
}

Plane::~Plane()
{
}

bool Plane::SetSplitSurface( GLfloat* pos0 , GLfloat* pos1 )
{
	position[0] = ( pos0[0] + pos1[0] ) / 2.0;
	position[1] = ( pos0[1] + pos1[1] ) / 2.0;
	position[2] = ( pos0[2] + pos1[2] ) / 2.0;

	//std::cout << "Plane::SetSplitSurface���I0(" << position[0] << "," << position[1] << "," << position[2] << ")" << std::endl;

	MathHelper::VectorMinus( vector , pos0 , pos1 );
	MathHelper::VectorNormalize( vector );

	return true;
}

bool Plane::AddToLink( Plane* p )
{
	p -> firstPlane = firstPlane;
	for( Plane* i = this; i != NULL; i = i -> link )
	{
		if( i -> link == NULL )
		{
			i -> link = p;
			break;
		}
	}
	return true;
}

bool Plane::CheckIntersection( GLfloat* initPosition , GLfloat* endPodition )
{
	//http://geomalgorithms.com/a05-_intersect-1.html

	// intersect3D_SegmentPlane(): find the 3D intersection of a segment and a plane
	//    Input:  S = a segment, and Pn = a plane = {Point V0;  Vector n;}
	//    Output: *I0 = the intersect point (when it exists)
	//    Return: 0 = disjoint (no intersection)
	//            1 =  intersection in the unique point *I0
	//            2 = the  segment lies in the plane
	

	//std::cout << "Debug:�bPlane::CheckIntersection" << std::endl;
	Point* u = new Point();		u -> Vector( initPosition , endPodition );
	Point* w = new Point();		w -> Vector( position , initPosition );
	//Vector    w = S.P0 - Pn.V0;

	GLfloat D = MathHelper::VectorDot( vector , u -> position );
	//float     D = dot(Pn.n, u);
	GLfloat N = -MathHelper::VectorDot( vector , w -> position );
	//float     N = -dot(Pn.n, w);

	delete u;
	delete w;

	//GLfloat SMALL_NUM = 0.0001;
	if ( fabs( D ) < SMALL_NUMBER) {           // segment is parallel to plane
		if ( N == 0 )                      // segment lies in plane
			return false;//��{�K�b�����W�ɡA�O�����ۥ�
		else
			return false;                    // no intersection
	}
	// they are not parallel
	// compute intersect param
	float sI = N / D;
	if (sI < -SMALL_NUMBER || sI > 1 + SMALL_NUMBER )
		return false;                        // no intersection


	//*I = S.P0 + sI * u;                  // compute segment intersect point
	return true;
	
	//===================================================================

}

bool Plane::GetIntersectPoint( GLfloat* initPosition , GLfloat* endPodition , GLfloat* intersectPoint )
{
	//http://geomalgorithms.com/a05-_intersect-1.html

	// intersect3D_SegmentPlane(): find the 3D intersection of a segment and a plane
	//    Input:  S = a segment, and Pn = a plane = {Point V0;  Vector n;}
	//    Output: *I0 = the intersect point (when it exists)
	//    Return: 0 = disjoint (no intersection)
	//            1 =  intersection in the unique point *I0
	//            2 = the  segment lies in the plane
	
	Point* u = new Point();		u -> Vector( initPosition , endPodition );
	Point* w = new Point();		w -> Vector( position , initPosition );
	//Vector    w = S.P0 - Pn.V0;

	GLfloat D = MathHelper::VectorDot( vector , u -> position );
	//float     D = dot(Pn.n, u);
	GLfloat N = -MathHelper::VectorDot( vector , w -> position );
	//float     N = -dot(Pn.n, w);
	//GLfloat SMALL_NUM = 0.0001;
	if ( fabs(D) < SMALL_NUMBER) {           // segment is parallel to plane
		if (N == 0)                      // segment lies in plane
			return false;//��{�K�b�����W�ɡA�O�����ۥ�
		else
			return false;                    // no intersection
	}
	// they are not parallel
	// compute intersect param
	float sI = N / D;
	if (sI < -SMALL_NUMBER || sI > 1 + SMALL_NUMBER )
		return false;                        // no intersection


	//*I = S.P0 + sI * u;                  // compute segment intersect point
	if( initPosition == NULL )
	{
		std::cout << "Plane::GetIntersectPoint�X���G�_�l�I���s�b" << std::endl;
	}
	MathHelper::CopyVector( intersectPoint , initPosition );
	GLfloat sICrossu[3] = { 0 , 0 , 0 };
	if( u -> position == NULL )
	{
		std::cout << "Plane::GetIntersectPoint�X���Gu�V�q���s�b" << std::endl;
	}
	MathHelper::CopyVector( sICrossu , u -> position );
	MathHelper::VectorMutiply( sICrossu , sI );
	MathHelper::VectorPlus( intersectPoint , sICrossu );

	return true;
	
	//===================================================================

}

//�@���d�u���u�q�P�������ۥ��I
bool Plane::GetIntersectPointOnRay( Line* line , GLfloat* intersectPoint )
{
	if( MathHelper::VectorAbsolution( line -> endPosition) == 0 )
	{
		std::cout << "Plane::GetIntersectPointOnRay�X���G�ӱ��u�S���V�q" << std::endl;
		return false;
	}

	//�����P�u�����I
	//������V
	//GLfloat normalDirection = line -> endPosition[0] + line -> endPosition[1] + line -> endPosition[2];
	//GLfloat times = MathHelper::VectorDot( line -> endPosition , position ) - MathHelper::VectorDot( line -> endPosition , line -> initPosition );
	//�Nline�����I�u��line����V���A����plane�W
	//������m-
	GLfloat lineInitPosition[3];
	GLfloat lineEndPosition[3];
	//�II = pos + vec * -1000
	MathHelper::CopyVector( lineInitPosition , line -> endPosition );
	MathHelper::VectorMutiply( lineInitPosition , -3000);
	MathHelper::VectorPlus( lineInitPosition , line -> initPosition );
	//�IE = pos + vec * 1000
	MathHelper::CopyVector( lineEndPosition , line -> endPosition );
	MathHelper::VectorMutiply( lineEndPosition , 3000);
	MathHelper::VectorPlus( lineEndPosition , line -> initPosition );

	if( !GetIntersectPoint( lineInitPosition , lineEndPosition , intersectPoint ) )
	{
		std::cout << "Plane::GetIntersectPointOnRay�X���G�ӱ��u�S���I�쥭��" << std::endl;
		return false;
	}

	//MathHelper::CopyVector( intersectPoint , line -> initPosition );
	//intersectPoint[0] += line -> endPosition[0] * times;
	//intersectPoint[1] += line -> endPosition[1] * times;
	//intersectPoint[2] += line -> endPosition[2] * times;

	return true;
}


bool Plane::Get2PlaneIntersectLine_2( Plane* plane , Line* line )
{
	//������V
	Point* lineVector = new Point();
	MathHelper::VectorCorss( lineVector -> position , vector , plane -> vector );
	lineVector -> Normalize();

	if( lineVector -> Absolution() == 0 )
	{
		//�⭱����
		std::cout <<  "Plane::Get2PlaneIntersectLine_2�X���A��ӭ�����" << std::endl;
		delete lineVector;
		return false;
	}
	//�������@�W���Ⱦlline���V�q
	Point* vecticalOnPlane1 = new Point();
	MathHelper::VectorCorss( vecticalOnPlane1 -> position , lineVector -> position , vector );
	vecticalOnPlane1 -> Normalize();
	//�������G�W���Ⱦlline���V�q ���G�S���Ψ�
	Point* vecticalOnPlane2 = new Point();
	MathHelper::VectorCorss( vecticalOnPlane2 -> position , lineVector -> position , plane -> vector );
	vecticalOnPlane2 -> Normalize();
	//������1���I�쥭��2���I���Z�� ��v�쥭��1�W������
	Point* plane1ToPlane2 = new Point();
	plane1ToPlane2 -> Vector( position , plane -> position );
	GLfloat distanceProjectOnPlane1 = plane1ToPlane2 -> Dot( vecticalOnPlane1 );
	//��plane�����I�u�ۧ�v�����סA����line�W
	Point* linePosition = new Point( vecticalOnPlane1 -> position );
	linePosition -> Multiply( distanceProjectOnPlane1 );
	linePosition -> Plus( position );
	//��Ȧs�Dline§
	MathHelper::CopyVector( line -> initPosition , linePosition -> position );
	MathHelper::CopyVector( line -> endPosition , lineVector -> position );

	delete lineVector;
	delete vecticalOnPlane1;
	delete vecticalOnPlane2;
	delete plane1ToPlane2;
	delete linePosition;

	if( MathHelper::VectorAbsolution( line -> endPosition ) == 0 )
	{
		std::cout <<  "Plane::Get2PlaneIntersectLine_2�X���G���G�u�S���V�q" << std::endl;
		return false;
	}

	return true;

}
//���o��ӥ������ۥ�u
bool Plane::Get2PlaneIntersectLine( Plane* plane , Line* line )
{
	//http://geomalgorithms.com/a05-_intersect-1.html
	// intersect3D_2Planes(): find the 3D intersection of two planes
	//    Input:  two planes Pn1 and Pn2
	//    Output: *L = the intersection line (when it exists)
	//    Return: 0 = disjoint (no intersection)
	//            1 = the two  planes coincide
	//            2 =  intersection in the unique line *L

    //Vector   u = Pn1.n * Pn2.n;          // cross product
	Point* u = new Point();
	MathHelper::VectorCorss( u -> position , vector , plane -> vector );

    float    ax = ( u -> position[0] >= 0 ? u -> position[0] : -u -> position[0] );
    float    ay = ( u -> position[1] >= 0 ? u -> position[1] : -u -> position[1] );
    float    az = ( u -> position[2] >= 0 ? u -> position[2] : -u -> position[2] );

    // test if the two planes are parallel
	//float SMALL_NUM = 0.0001;
    if ((ax+ay+az) < SMALL_NUMBER ) {        // Pn1 and Pn2 are near parallel
        // test if disjoint or coincide
        //Vector   v = Pn2.V0 -  Pn1.V0;
		Point* v = new Point( position );
		MathHelper::VectorMinus( v -> position , plane -> position );
        if ( MathHelper::VectorDot( vector , v -> position ) == 0 )          // Pn2.V0 lies in Pn1 //dot(Pn1.n, v) == 0
            return false;                    // Pn1 and Pn2 coincide
        else 
            return false;                    // Pn1 and Pn2 are disjoint
    }

    // Pn1 and Pn2 intersect in a line
    // first determine max abs coordinate of cross product
    int      maxc;                       // max coordinate
    if (ax > ay) {
        if (ax > az)
             maxc =  1;
        else maxc = 3;
    }
    else {
        if (ay > az)
             maxc =  2;
        else maxc = 3;
    }

    // next, to get a point on the intersect line
    // zero the max coord, and solve for the other two
    Point*    iP = new Point();                // intersect point
    float    d1, d2;            // the constants in the 2 plane equations
    //d1 = -dot(Pn1.n, Pn1.V0);  // note: could be pre-stored  with plane
	d1 = -MathHelper::VectorDot( vector , position );
    //d2 = -dot(Pn2.n, Pn2.V0);  // ditto
	d2 = -MathHelper::VectorDot( plane -> vector , plane -> position );

    switch (maxc) {             // select max coordinate
    case 1:                     // intersect with x=0
        iP -> position[0] = 0;
        iP -> position[1] = ( d2 * vector[2] - d1 * plane -> vector[2] ) /  u -> position[0];
        iP -> position[2] = ( d1 * plane -> vector[1] - d2 * vector[1] ) /  u -> position[0];
        break;
    case 2:                     // intersect with y=0
        iP -> position[0] = ( d1 * plane -> vector[2] - d2 * vector[2] ) / u -> position[1];//(d1*Pn2.n.z - d2*Pn1.n.z) /  u.y;
        iP -> position[1] = 0;
        iP -> position[2] = ( d2 * plane -> vector[0] - d1 * plane -> vector[0] ) / u -> position[1];//(d2*Pn1.n.x - d1*Pn2.n.x) /  u.y;
        break;
    case 3:                     // intersect with z=0
        iP -> position[0] = ( d2 * vector[1] - d1 * plane -> vector[2] ) / u -> position[2];//(d2*Pn1.n.y - d1*Pn2.n.y) /  u.z
        iP -> position[1] = ( d1 * plane -> vector[0] - d2 * vector[0] ) / u -> position[2];//(d1*Pn2.n.x - d2*Pn1.n.x) /  u.z;
        iP -> position[2] = 0;
    }
    //L->P0 = iP;
    //L->P1 = iP + u;
	u -> Normalize();
	MathHelper::CopyVector( line -> initPosition , iP -> position );
	MathHelper::CopyVector( line -> endPosition , u -> position );

	delete iP;
	delete u;
    return true;

//===================================================================
}

//�p�G�b�k�V�q�P��V�A�N�Ooutside,���M�N�Oinside
bool Plane::IsInside( GLfloat* pos )
{
	Point* vectorToPoint = new Point();
	vectorToPoint -> Vector( position , pos );

	if( vectorToPoint -> Dot( vector ) > 0 )
	{
		delete vectorToPoint;
		return true;
	}
	else if( vectorToPoint -> Dot( vector ) == 0 )
	{
		std::cout << "Plane::IsInside�X���A�I�b�����W" << std::endl;
	}
	else
	{
		delete vectorToPoint;
		return false;
	}
}

bool Plane::Contain( GLfloat* pos )
{
	GLfloat posToPosition[3];
	MathHelper::CopyVector( posToPosition , pos );
	MathHelper::VectorMinus( posToPosition , position );
	//std::cout << "Plane::Contain��dot�����G�O" << std::setprecision(10) << abs( MathHelper::VectorDot( vector , posToPosition ) ) << std::endl;
	if( abs( MathHelper::VectorDot( vector , posToPosition ) ) < SMALL_NUMBER )//SMALL_NUM
	{
		//std::cout << "Plane::Contain��dot�����G�O" << std::setprecision(10) << abs( MathHelper::VectorDot( vector , posToPosition ) ) << std::endl;
		return true;
	}
	return false;
}