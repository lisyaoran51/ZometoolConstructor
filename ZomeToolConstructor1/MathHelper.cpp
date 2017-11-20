#include "MathHelper.h"
#define T0 0
#define T1 4
#define T2 8
#define T3 12
#define T4 1
#define T5 5
#define T6 9
#define T7 13
#define T8 2
#define T9 6
#define T10 10
#define T11 14
#define T12 3
#define T13 7
#define T14 11
#define T15 15

// This is a column-order matrix multiply of matrices m1 and m2.
void MathHelper::matrixMult4x4Column(GLfloat* result, GLfloat* m1, GLfloat* m2)
{
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[T0] = m1[T0] * m2[T0] + m1[T1] * m2[T4] + m1[T2] * m2[T8] + m1[T3] * m2[T12];
	result[T1] = m1[T0] * m2[T1] + m1[T1] * m2[T5] + m1[T2] * m2[T9] + m1[T3] * m2[T13];
	result[T2] = m1[T0] * m2[T2] + m1[T1] * m2[T6] + m1[T2] * m2[T10] + m1[T3] * m2[T14];
	result[T3] = m1[T0] * m2[T3] + m1[T1] * m2[T7] + m1[T2] * m2[T11] + m1[T3] * m2[T15];

	result[T4] = m1[T4] * m2[T0] + m1[T5] * m2[T4] + m1[T6] * m2[T8] + m1[T7] * m2[T12];
	result[T5] = m1[T4] * m2[T1] + m1[T5] * m2[T5] + m1[T6] * m2[T9] + m1[T7] * m2[T13];
	result[T6] = m1[T4] * m2[T2] + m1[T5] * m2[T6] + m1[T6] * m2[T10] + m1[T7] * m2[T14];
	result[T7] = m1[T4] * m2[T3] + m1[T5] * m2[T7] + m1[T6] * m2[T11] + m1[T7] * m2[T15];

	result[T8] = m1[T8] * m2[T0] + m1[T9] * m2[T4] + m1[T10] * m2[T8] + m1[T11] * m2[T12];
	result[T9] = m1[T8] * m2[T1] + m1[T9] * m2[T5] + m1[T10] * m2[T9] + m1[T11] * m2[T13];
	result[T10] = m1[T8] * m2[T2] + m1[T9] * m2[T6] + m1[T10] * m2[T10] + m1[T11] * m2[T14];
	result[T11] = m1[T8] * m2[T3] + m1[T9] * m2[T7] + m1[T10] * m2[T11] + m1[T11] * m2[T15];

	result[T12] = m1[T12] * m2[T0] + m1[T13] * m2[T4] + m1[T14] * m2[T8] + m1[T15] * m2[T12];
	result[T13] = m1[T12] * m2[T1] + m1[T13] * m2[T5] + m1[T14] * m2[T9] + m1[T15] * m2[T13];
	result[T14] = m1[T12] * m2[T2] + m1[T13] * m2[T6] + m1[T14] * m2[T10] + m1[T15] * m2[T14];
	result[T15] = m1[T12] * m2[T3] + m1[T13] * m2[T7] + m1[T14] * m2[T11] + m1[T15] * m2[T15];
}
void MathHelper::matrixMult4x4(GLfloat* result, GLfloat* m1, GLfloat* m2)
{
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[0] = m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12];
	result[1] = m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13];
	result[2] = m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14];
	result[3] = m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15];

	result[4] = m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12];
	result[5] = m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13];
	result[6] = m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14];
	result[7] = m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15];

	result[8] = m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12];
	result[9] = m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13];
	result[10] = m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14];
	result[11] = m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15];

	result[12] = m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12];
	result[13] = m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13];
	result[14] = m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14];
	result[15] = m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15];
}


// Makes an identity matrix
void MathHelper::makeIdentity(GLfloat* result)
{
	for (int i = 0; i < 16; i++) {
		result[i] = 0.0f;
	}
	result[0] = result[5] = result[10] = result[15] = 1.0f;
}

// Returns a matrix that translates by x, y and z amount
void MathHelper::makeTranslate(GLfloat* result, GLfloat x, GLfloat y, GLfloat z)
{
	makeIdentity(result);
	result[3] = x;
	result[7] = y;
	result[11] = z;
}

// Returns matrices that rotate about the X, Y and Z axes by a rotation amount (radians)
void MathHelper::makeRotateX(GLfloat* result, GLfloat rotation)
{
	makeIdentity(result);
	result[5] = result[10] = cos(rotation);
	result[6] = sin(rotation);
	result[9] = -result[6];
}
void MathHelper::makeRotateY(GLfloat* result, GLfloat rotation)
{
	makeIdentity(result);
	result[0] = result[10] = cos(rotation);
	result[2] = -sin(rotation);
	result[8] = -result[2];
}
void MathHelper::makeRotateZ(GLfloat* result, GLfloat rotation)
{
	makeIdentity(result);
	result[0] = result[5] = cos(rotation);
	result[1] = sin(rotation);
	result[4] = -result[1];
}

void MathHelper::makeQuaternion(GLfloat* result, GLfloat* axis , GLfloat theta )
{
	//x = s * Xa
	//y = s * Ya
	//z = s * Za
	//w = cos(θ/2)
	//s = sin(θ/2)

	GLfloat s[3] = { axis[0] * sin( theta / 2.0 ) ,
					 axis[1] * sin( theta / 2.0 ) ,
					 axis[2] * sin( theta / 2.0 ) };
	GLfloat w = cos( theta / 2.0 );

	result[0] = 1.0 - 2.0 * (s[1]*s[1]+s[2]*s[2]);
	result[1] = 2.0 * (s[0]*s[1]-s[2]*w);
	result[2] = 2.0 * (s[0]*s[2]+s[1]*w);
	result[3] = 0.0;
	result[4] = 2.0 * (s[0]*s[1]+s[2]*w);
	result[5] = 1.0 - 2.0 * (s[0]*s[0]+s[2]*s[2]);
	result[6] = 2.0 * (s[1]*s[2]-s[0]*w);
	result[7] = 0.0;
	result[8] = 2.0 * (s[0]*s[2]-s[1]*w);
	result[9] = 2.0 * (s[1]*s[2]+s[0]*w);
	result[10] = 1.0 - 2.0 * (s[0]*s[0]+s[1]*s[1]);
	result[11] = 0.0;
	result[12] = 0.0;
	result[13] = 0.0;
	result[14] = 0.0;
	result[15] = 1.0;
}

// 2) Create the makeScale method here and fill in the code using the examples above and below
void MathHelper::makeScale(GLfloat* result, GLfloat x, GLfloat y, GLfloat z)
{
	makeIdentity(result);
	result[0] = x;
	result[5] = y;
	result[10] = z;
}


void MathHelper::makePerspectiveMatrix(GLfloat* result, GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane) {
	GLfloat f = 1.0f / tan(fov*3.1415926f / 360.0f);
	makeIdentity(result);
	result[0] = f / aspect;
	result[5] = f;
	result[10] = ((farPlane + nearPlane) / (nearPlane - farPlane));
	result[11] = -1;
	result[14] = (2.0f*farPlane*nearPlane) / (nearPlane - farPlane);
	result[15] = 0;
}

void MathHelper::makePlane( GLfloat* result , GLfloat* vec1 , GLfloat* vec2 , GLfloat* vec3 )
{
	//還沒寫

	GLfloat* edge1 = new GLfloat[3];
	GLfloat* edge2 = new GLfloat[3];
	GLfloat* normal = new GLfloat[3];
	//result = new GLfloat[4];

	VectorMinus( edge1 , vec2 , vec1 );
	VectorMinus( edge2 , vec3 , vec1 );
	VectorCorss( normal , edge1 , edge2 );
	vectorNormalize( normal );

	result[3] = -( vec1[0] * normal[0] + vec1[1] * normal[1] + vec1[2] * normal[2] );
	CopyVector( result , normal );
	delete edge1;
	delete edge2;
	delete normal;
}

void MathHelper::translateMatrixBy(GLfloat* result, GLfloat x, GLfloat y, GLfloat z)
{
	result[3] += x;
	result[7] += y;
	result[11] += z;
}

void MathHelper::copyMatrix(GLfloat* src, GLfloat* dest)
{
	for (int i = 0; i < 16; i++) {
		dest[i] = src[i];
	}
}

void MathHelper::CleanMatrixSmall( GLfloat* result )
{
	GLfloat* newMatrix = new GLfloat[16];

	for( int i = 0; i < 16; i++ )
	{
		if( result[i] < 0.0000001 && result[i] > -0.0000001 )
		{
			newMatrix[i] = 0.0;
		}
		else
		{
			newMatrix[i] = result[i];
		}
	}
	for( int i = 0; i < 16; i++ )
	{
		result[i] =	newMatrix[i];
	}
	delete newMatrix;

}

void MathHelper::print4x4Matrix(GLfloat* mat)
{
	printf("====================================");
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) printf("\n");
		printf("%f\t", mat[i]);
	}
	printf("\n");
}

void MathHelper::vectorMultMatrix(GLfloat* result, GLfloat* v, GLfloat* m)
{
	GLfloat result3 = v[0] * m[12] + v[1] * m[13] + v[2] * m[14] + 1 * m[15];
	if ( result3 == 0 )std::cout<<"矩陣出錯了，除以零"<<std::endl;
	result[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2] + 1 * m[3]; result[0] /= result3;
	result[1] = v[0] * m[4] + v[1] * m[5] + v[2] * m[6] + 1 * m[7]; result[1] /= result3;
	result[2] = v[0] * m[8] + v[1] * m[9] + v[2] * m[10] + 1 * m[11]; result[2] /= result3;
}

void MathHelper::vectorMultMatrix(GLfloat* result , GLfloat* m )
{
	GLfloat result3 = result[0] * m[12] + result[1] * m[13] + result[2] * m[14] + 1 * m[15];
	if ( result3 == 0 )std::cout<<"矩陣出錯了，除以零"<<std::endl;
	GLfloat* v = new GLfloat[3];
	CopyVector( v , result );
	result[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2] + 1 * m[3]; result[0] /= result3;
	result[1] = v[0] * m[4] + v[1] * m[5] + v[2] * m[6] + 1 * m[7]; result[1] /= result3;
	result[2] = v[0] * m[8] + v[1] * m[9] + v[2] * m[10] + 1 * m[11]; result[2] /= result3;
	delete[] v;
}

void MathHelper::vectorNormalize(GLfloat* v)
{
	GLfloat abs = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
	if( abs == 0 )
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}
	else
	{
		v[0] /= abs;
		v[1] /= abs;
		v[2] /= abs;
	}
}

void MathHelper::SetVector( GLfloat* vec , GLfloat x , GLfloat y , GLfloat z )
{
	*vec = x;
	*(vec+1) = y;
	*(vec+2) = z;
}

void MathHelper::SetVector( GLfloat* result , GLfloat* initPosition , GLfloat* endPosition )
{
	result[0] = endPosition[0] - initPosition[0];
	result[1] = endPosition[1] - initPosition[1];
	result[2] = endPosition[2] - initPosition[2];
}

void MathHelper::VectorPlus( GLfloat* result , GLfloat* plused , GLfloat* pluser )
{
	GLfloat x = plused[0] + pluser[0];
	GLfloat y = plused[1] + pluser[1];
	GLfloat z = plused[2] + pluser[2];
	result[0] = x;
	result[1] = y;
	result[2] = z;
}

void MathHelper::VectorPlus( GLfloat* plused , GLfloat* pluser )
{
	VectorPlus( plused , plused , pluser );
}

void MathHelper::VectorPlus( GLfloat* plused , GLfloat x , GLfloat y , GLfloat z )
{
	GLfloat pluser[] = {x,y,z};
	VectorPlus( plused , plused , pluser );
}

void MathHelper::VectorMinus( GLfloat* result , GLfloat* minused , GLfloat* minuser )// 答案，A - B
{
	GLfloat x = minused[0] - minuser[0];
	GLfloat y = minused[1] - minuser[1];
	GLfloat z = minused[2] - minuser[2];
	result[0] = x;
	result[1] = y;
	result[2] = z;
}

void MathHelper::VectorMinus( GLfloat* minused , GLfloat* minuser )// 答案，A - B
{
	VectorMinus( minused , minused , minuser );
}

void MathHelper::VectorMutiply( GLfloat* result , GLfloat* multiplied , GLfloat multiplier )
{
	result[0] = multiplied[0] * multiplier;
	result[1] = multiplied[1] * multiplier;
	result[2] = multiplied[2] * multiplier;
}

void MathHelper::VectorMutiply( GLfloat* multiplied , GLfloat multiplier )
{
	VectorMutiply( multiplied , multiplied , multiplier );
}

void MathHelper::VectorDot( GLfloat* result , GLfloat* vec1 , GLfloat* vec2 ){}

GLfloat MathHelper::VectorDot( GLfloat* vec1 , GLfloat* vec2 )
{
	return vec1[0] * vec2[0] + 
		   vec1[1] * vec2[1] + 
		   vec1[2] * vec2[2]; 
}

void MathHelper::VectorCorss( GLfloat* result , GLfloat* vec1 , GLfloat* vec2 )
{
	//A x B = (a2b3 ' a3b2, a3b1 ' a1b3, a1b2 ' a2b1)
	result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void MathHelper::VectorReverse( GLfloat* result )
{
	GLfloat temp[3];
	temp[0] = -result[0];
	temp[1] = -result[1];
	temp[2] = -result[2];
	CopyVector( result , temp );
}

void MathHelper::VectorNormalize( GLfloat* result )
{
	GLfloat absolute = sqrt( pow( result[0] , 2 ) + pow( result[1] , 2 ) + pow( result[2] , 2 ) );
	if( absolute == 0 ) return;
	result[0] = result[0] / absolute;
	result[1] = result[1] / absolute;
	result[2] = result[2] / absolute;
}

GLfloat MathHelper::VectorDistance( GLfloat* vec1 , GLfloat* vec2 )
{
	GLfloat squareDistance;
	squareDistance = pow( vec1[0] - vec2[0] , 2 ) + 
					 pow( vec1[1] - vec2[1] , 2 ) + 
					 pow( vec1[2] - vec2[2] , 2 );
	return sqrt( squareDistance );
}

GLfloat MathHelper::VectorAbsolution( GLfloat* vec1 )
{
	GLfloat squareDistance;
	squareDistance = pow( vec1[0] , 2 ) + 
					 pow( vec1[1] , 2 ) + 
					 pow( vec1[2] , 2 );
	return sqrt( squareDistance );
}

bool MathHelper::CompareVector( GLfloat* vec1 , GLfloat* vec2 )
{
	if( vec1[0] == vec2[0] &&
		vec1[1] == vec2[1] &&
		vec1[2] == vec2[2] )
		return true;
	else return false;
}

bool MathHelper::CompareVector( GLfloat* vec1 , GLfloat x , GLfloat y , GLfloat z )
{
	if( vec1[0] == x &&
		vec1[1] == y &&
		vec1[2] == z )
		return true;
	else return false;
}

bool MathHelper::CompareDirection( GLfloat* vec1 , GLfloat* vec2 )
{
	GLfloat* tempVec1 = new GLfloat[3];
	GLfloat* tempVec2 = new GLfloat[3];

	CopyVector( tempVec1 , vec1 );
	CopyVector( tempVec2 , vec2 );

	VectorNormalize( tempVec1 );
	VectorNormalize( tempVec2 );

	bool result = VectorApproximation( tempVec1 , tempVec2 );
	delete tempVec1;
	delete tempVec2;

	return result;
}

bool MathHelper::CompareDirection( GLfloat* vec1Initial , GLfloat* vec1End , GLfloat* vec2Initial , GLfloat* vec2End )
{
	GLfloat* tempVec1 = new GLfloat[3];
	GLfloat* tempVec2 = new GLfloat[3];

	VectorMinus( tempVec1 , vec1End , vec1Initial );
	VectorMinus( tempVec2 , vec2End , vec2Initial );

	bool result = CompareDirection( tempVec1 , tempVec2 );
	delete tempVec1;
	delete tempVec2;

	return result;
}

bool MathHelper::CheckIntersectOnTriangle( GLfloat* vecInitial , 
										   GLfloat* vecEnd , 
										   GLfloat* point0 , 
										   GLfloat* point1 , 
										   GLfloat* point2 )
{
	//debug
	GLfloat test[3] = { -131 , 37 , 69 };
	if( MathHelper::VectorDistance( test , point0 ) < 10 || 
		MathHelper::VectorDistance( test , point1 ) < 10 || 
		MathHelper::VectorDistance( test , point2 ) < 10 )
	{
		std::cout << "debug";
	}


	GLfloat d[3];
	GLfloat e1[3];
	GLfloat e2[3];
	//Point* d = new Point();
	//Point* e1 = new Point();
	//Point* e2 = new Point();

	MathHelper::SetVector( d , vecInitial , vecEnd );
	//d -> Vector( linePosition1 , linePosition2 );

	MathHelper::SetVector( e1 , point0 , point1 );
	MathHelper::SetVector( e2 , point0 , point2 );
	//e1 -> Vector( trianglePosition1 , trianglePosition2 );
	//e2 -> Vector( trianglePosition1 , trianglePosition3 );

	GLfloat s1[3];
	//Point* s1 = new Point();
	MathHelper::VectorCorss( s1 , d , e2 );
	//s1 -> Cross( d , e2 );

	GLfloat divisor = MathHelper::VectorDot( s1 , e1 );
	//s1 -> Dot( e1 );
	GLfloat indivisor = 1.0 / divisor;
   
	if ( abs(divisor) <= -SMALL_NUMBER )
	{
		//不相交
		//delete s1;
		//
		//delete d;
		//delete e1;
		//delete e2;
		return false;
	}

	// Compute first barycentric coordinate
	GLfloat s[3];
	//Point* s = new Point();
	MathHelper::SetVector( s , point0 , vecInitial );
	//s -> Vector( trianglePosition1 , linePosition1 );
	GLfloat b1 = MathHelper::VectorDot( s , s1 ) * indivisor;
	//GLfloat b1 = s -> Dot( s1 ) * indivisor;
	if( b1 < -SMALL_NUMBER || b1 > 1 + SMALL_NUMBER )
	{
		//不相交
		//delete s1;
		//delete s;
		//
		//delete d;
		//delete e1;
		//delete e2;
		return false;
	}


				
	// Compute second barycentric coordinate
	GLfloat s2[3];
	//Point* s2 = new Point();
	MathHelper::VectorCorss( s2 , s , e1 );
	//s2 -> Cross( s , e1 );
	GLfloat b2 = MathHelper::VectorDot( d , s2 ) * indivisor;
	//GLfloat b2 = d -> Dot( s2 ) * indivisor;
	if( b2 < -SMALL_NUMBER || b1 + b2 > 1 + SMALL_NUMBER )
	{
		//不相交
		//delete s1;
		//delete s;
		//delete s2;
		//
		//delete d;
		//delete e1;
		//delete e2;
		return false;
	}

				
	// Compute _t_ to intersection point
	GLfloat t = MathHelper::VectorDot( e2 , s2 ) * indivisor;
	//GLfloat t = e2 -> Dot( s2 ) * indivisor;
	if( t < -SMALL_NUMBER || t > 1 + SMALL_NUMBER )
	{
		//delete s1;
		//delete s;
		//delete s2;
		//
		//delete d;
		//delete e1;
		//delete e2;
		//不相交
		return false;
	}
				
				
	//delete s1;
	//delete s;
	//delete s2;
	//
	//delete d;
	//delete e1;
	//delete e2;

	return true;
}

GLfloat MathHelper::CheckTwoLineDistance( GLfloat* vec1Initial , 
										  GLfloat* vec1End , 
										  GLfloat* vec2Initial , 
										  GLfloat* vec2End )
{
	//Point* u = new Point(); u -> Vector( vec1Initial -> position , vec1End -> position );
	GLfloat u[3]; MathHelper::SetVector( u , vec1Initial , vec1End );
	//Point* v = new Point(); v -> Vector( vec2Initial -> position , vec2End -> position );
	GLfloat v[3]; MathHelper::SetVector( v , vec2Initial , vec2End );
	//Point* w = new Point(); w -> Vector( vec2Initial -> position , vec1Initial -> position );
	GLfloat w[3]; MathHelper::SetVector( w , vec2Initial , vec1Initial );


	//GLfloat a = u -> Dot( u -> position );
	GLfloat a = MathHelper::VectorDot( u , u );
	//GLfloat b = u -> Dot( v -> position );
	GLfloat b = MathHelper::VectorDot( u , v );
	//GLfloat c = v -> Dot( v -> position );
	GLfloat c = MathHelper::VectorDot( v , v );
	//GLfloat d = u -> Dot( w -> position );
	GLfloat d = MathHelper::VectorDot( u , w );
	//GLfloat e = v -> Dot( w -> position );
	GLfloat e = MathHelper::VectorDot( v , w );
	GLfloat D = a*c - b*b;
	GLfloat sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    GLfloat tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
	//GLfloat SMALL_NUM = 0.001;

	 // compute the line parameters of the two closest points
	if (D < SMALL_NUMBER) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < SMALL_NUMBER ) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < SMALL_NUMBER ) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < SMALL_NUMBER )
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < SMALL_NUMBER )
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUMBER ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUMBER ? 0.0 : tN / tD);

    // get the difference of the two closest points
   // Vector   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
	//Point* dP = new Point( w -> position[0] + sc * u -> position[0] - tc * v -> position[0] ,
	//					   w -> position[1] + sc * u -> position[1] - tc * v -> position[1] ,
	//					   w -> position[2] + sc * u -> position[2] - tc * v -> position[2] );
	GLfloat dP[3] = { w[0] + sc * u[0] - tc * v[0] ,
					  w[1] + sc * u[1] - tc * v[1] ,
					  w[2] + sc * u[2] - tc * v[2] };

	//return norm(dP);   // return the closest distance
	//GLfloat result = dP -> Absolution();
	GLfloat result = MathHelper::VectorAbsolution( dP );
	return result;
}

GLfloat MathHelper::CheckDistanceToTriangle( GLfloat* position , 
										     GLfloat* point0 , 
										     GLfloat* point1 , 
										     GLfloat* point2 ,
											 GLfloat* normal0 , 
											 GLfloat* normal1 , 
											 GLfloat* normal2 ,
											 GLfloat* nearestPointOnSurface )
{

	//Point* vector1 = new Point(); vector1 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
	GLfloat vector1[3]; SetVector( vector1 , point0 , point2 );
	//Point* vector2 = new Point(); vector2 -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
	GLfloat vector2[3]; SetVector( vector2 , point1 , point2 );
	//算normal
	//Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
	GLfloat normal[3]; VectorCorss( normal , vector1 , vector2 ); MathHelper::VectorNormalize( normal );
	
	GLfloat vertexNormalAverage[3];//vertex normal平均值
	VectorPlus( vertexNormalAverage , normal0 , normal1 );
	VectorPlus( vertexNormalAverage , normal2 );
	VectorMutiply( vertexNormalAverage , 1.0 / 3.0 );
	//( Normal 1 + Normal 2 + Normal 3 ) / 3

	//如果和點法向量相反方向，就把平面normal反轉
	//if( vertexNormalAverage -> Dot( normal -> position ) < 0 ) normal -> Reverse();
	if( VectorDot( vertexNormalAverage , normal ) < 0 ) VectorReverse( normal );


	//從末端到表面 與normal同方向
	//Point* distancePtoV0 = new Point(); 
	GLfloat distancePtoV0[3];
	//distancePtoV0 -> Vector( endPointCenter -> position , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
	SetVector( distancePtoV0 , position , point0 );
	//GLfloat sb = normal -> Dot( distancePtoV0 );//是正的 如果點在模型裡，也就是距離
	GLfloat sb = VectorDot( normal , distancePtoV0 );
	
	if( sb > -0.1 )
	{
		//如果這個面與點的距離小於目前的最小距離，則來看
		//1.這個點投影到面上時，會在三角型三個點之內，則她的距離就試與平面的距離
		//2.這個點投影到面上時，在三角型三個點之外，則她的距離就是與三個點的最小距離(比與平面的距離大)
		//http://www.blackpawn.com/texts/pointinpoly/
			
		bool insideTriangle = false;
		//這邊算投影的點
		//Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
		GLfloat pointProjectOnPlane[3]; CopyVector( pointProjectOnPlane , normal );
		if( abs(sb) > SMALL_NUMBER )
		{
			VectorMutiply( pointProjectOnPlane , sb );
			VectorPlus( pointProjectOnPlane , position );
		}
		
		//normal -> Multiply( sb );
		//pointProjectOnPlane -> Plus( normal );
		//normal -> Multiply( 1.0 / sb );

		//算第一個做標 第一個是三角型總面積x2 第二個是三角型點0到投影點的距離 第三個是投影點和編1所包的面積
		//Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
		GLfloat crossTotalArea[3]; VectorCorss( crossTotalArea , vector1 , vector2 );
		//Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 , pointProjectOnPlane -> position );
		GLfloat vertex0ToPoint[3]; SetVector( vertex0ToPoint , point0 , pointProjectOnPlane );
		//Point* vertex1ToPoint = new Point(); vertex1ToPoint -> Vector( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 , pointProjectOnPlane -> position );
		GLfloat vertex1ToPoint[3]; SetVector( vertex1ToPoint , point1 , pointProjectOnPlane );
		
		//Point* crossArea1 = new Point(); crossArea1 -> Cross( vector1 , vertex0ToPoint );
		GLfloat crossArea1[3]; VectorCorss( crossArea1 , vector1 , vertex0ToPoint );
		//Point* crossArea2 = new Point(); crossArea2 -> Cross( vertex1ToPoint , vector2 );
		GLfloat crossArea2[3]; VectorCorss( crossArea2 , vertex1ToPoint , vector2 );
			
		//算第一個做標漢第二個做標
		//GLfloat TotalArea = crossTotalArea -> Dot( normal );
		GLfloat TotalArea = VectorDot( crossTotalArea , normal );
		//GLfloat Area1 = crossArea1 -> Dot( normal );
		GLfloat Area1 = VectorDot( crossArea1 , normal );
		//GLfloat Area2 = crossArea2 -> Dot( normal );
		GLfloat Area2 = VectorDot( crossArea2 , normal );

		if( ( Area1 >= 0 && Area2 >= 0 ) || ( Area1 <= 0 && Area2 <= 0 ) )//這行代表一個正一個負，就是這個點未在三角形中
		{
			if( abs( TotalArea ) > abs( Area1 ) + abs( Area2 ) )
			{
				//有在三角形上
				insideTriangle = true;
			}
		}

		if( insideTriangle )
		{
			//minDistance = sb;
			//tempNearstPoint -> Copy( pointProjectOnPlane );
			CopyVector( nearestPointOnSurface , pointProjectOnPlane );
			return sb;
		}
		else
		{
			//bool onEdge[3] = { false , false , false };//0~1 1~2 2~0
			//=====================================查看最近點是某在三角形邊上
			//GLfloat VectorPtoP0[3]; SetVector( vector1 , position , point0 );
			//GLfloat VectorPtoP1[3]; SetVector( vector1 , position , point1 );
			//GLfloat VectorPtoP2[3]; SetVector( vector1 , position , point2 );


			//=====================================查看最近點是某在三角形邊上
			GLfloat distance[3];
			//distance[0] = endPointCenter -> DistanceToPoint( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
			distance[0] = VectorDistance( point0 , position );
			//distance[1] = endPointCenter -> DistanceToPoint( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
			distance[1] = VectorDistance( point1 , position );
			//distance[2] = endPointCenter -> DistanceToPoint( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
			distance[2] = VectorDistance( point2 , position );
			int min = 0;

			//找出最近的點
			if( distance[0] < distance[1] )
				if( distance[0] < distance[2] )
				{	CopyVector( nearestPointOnSurface , point0 );
					min = 0;}
				else
				{	CopyVector( nearestPointOnSurface , point2 );
					min = 2;}
			else
				if( distance[1] < distance[2] )
				{	CopyVector( nearestPointOnSurface , point1 );
					min = 1;}
				else
				{	CopyVector( nearestPointOnSurface , point2 );
					min = 2;}

			//把這個點的位置給紀錄下來 寄在tempNearstPoint裡
			//minDistance = distance[ min ];
			//tempNearstPoint -> Copy( *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 + min ] * 3 );
			return distance[ min ];
			
		}

	}
	//std::cout << "MathHelper::CheckDistanceToTriangle出錯：點在三角形外面" << std::endl;
	//std::cout << ".";
	return -1;
	

}

bool MathHelper::VectorApproximation( GLfloat* vec1 , GLfloat* vec2 )
{
	if( VectorDistance( vec1 , vec2 ) < SMALL_NUMBER )
	{
		return true;
	}
	return false;
}

void MathHelper::CopyVector( GLfloat* result , GLfloat* vec )
{
	result[0] = vec[0];
	result[1] = vec[1];
	result[2] = vec[2];
}

void MathHelper::MirrorVector( GLfloat* result , GLfloat* vec , GLfloat* plane )
{
	//工是：t = -( aX0 + bY0 + cZ0 + d ) / ( a平 + b平 + c平 )
	//鏡像點：( X0+2at , Y0+2bt , Z0+2ct )

	//還沒寫
	GLfloat t = -( plane[0] * vec[0] + plane[1] * vec[1] + plane[2] * vec[2] + plane[3] ) / ( plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2] );

	result[0] = vec[0] + 2.0 * plane[0] * t;
	result[1] = vec[1] + 2.0 * plane[1] * t;
	result[2] = vec[2] + 2.0 * plane[2] * t;


}

GLfloat MathHelper::GetDirectionTheta( GLfloat* direction )
{
	GLfloat sinTheta = direction[2] / sqrt( pow( direction[0] , 2 ) + pow( direction[2] , 2 ) );
	GLfloat theta = asin( sinTheta );
	if ( direction[0] < 0 )
	{
		theta = M_PI - theta;
	}
	if ( sqrt( pow( direction[0] , 2 ) + pow( direction[2] , 2 ) ) < SMALL_NUMBER )
	{
		theta = 0;
	}
	return theta;
}
GLfloat MathHelper::GetDirectionPhi( GLfloat* direction )
{
	GLfloat sinPhi = direction[1] / sqrt( pow( direction[0] , 2 ) + pow( direction[1] , 2 ) + pow( direction[2] , 2 ) );
	GLfloat phi = acos( sinPhi );
	return phi;
}
GLfloat MathHelper::GetDirectionYaw( GLfloat* direction )
{
	GLfloat yaw = 0;
	if( direction[0] == 0 && direction[2] == 0 ) return 0;
	GLfloat sinYaw = direction[2] / sqrt( pow( direction[0] , 2 ) + pow( direction[2] , 2 ) );
	yaw = asin( sinYaw );
	return yaw;

}