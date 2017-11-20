#include "stdafx.h"
//#include <GL/glew.h>
//#include <GL/freeglut.h>
//#include <stdio.h>	
#include "buddha.h"	// NEW! - look at this file if you haven't!  
// It has the vertices[], normals[] and indices[] in it


#include "ObjUser.h"
//#include "Model.h"
//#include "ZomeDirection.h"
#include "Constructor.h"
#include "ObjFunction.h"
#include "ZomeGenerator.h"
#include "Segmenter.h"



// From http://www.opengl.org/registry/specs/EXT/pixel_buffer_object.txt 


ModelObject* currentModelObject;
int* currentModelObjectSize;
ModelObject* originalModelLink;
//GLuint *currentModelMatrixID;

GLfloat *MakeVector3(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat Vector3[3];
	Vector3[0] = x;
	Vector3[1] = y;
	Vector3[2] = z;

	return Vector3;
}


void ExportObj_Discarded( ModelObject* modelObject , int modelObjSize )//這個不用了
{
	GLuint totalVerticeSize = 0 , 
		   totalNormalSize = 0 , 
		   totalIndiceSize = 0;
	for (int i = 0; i < modelObjSize; i++)
	{
		totalVerticeSize += modelObject[i].verticeSize;
		totalNormalSize += modelObject[i].normalSize;
		totalIndiceSize += modelObject[i].indiceSize;
		//std::cout << "Export時點數是" << modelObject[i].verticeSize << ",法向量數是" << modelObject[i].normalSize << ",面數是" << modelObject[i].indiceSize << std::endl;
	}
	GLfloat* scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	GLfloat* resultVertice = new GLfloat[totalVerticeSize];
	GLfloat* resultNormal = new GLfloat[totalNormalSize];
	GLuint* resultIndice = new GLuint[totalIndiceSize];
	std::cout << "Export2時點數是" << totalVerticeSize/3 << ",法向量數是" << totalNormalSize/3 << ",面數是" << totalIndiceSize/3 << std::endl;

	for (int i = 0; i < modelObjSize; i++)
	{
		for ( int j = 0; j < modelObject[i].verticeSize; j += 3 )
		{
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *(modelObject[i].vertices) + j , modelObject[i].rotateThetaMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , modelObject[i].rotatePhiMatrix );
			MathHelper::vectorMultMatrix( &resultVertice[modelObject[i].vertexOffset + j] , tempVector2 , modelObject[i].translateMatrix );
		}

		for ( int j = 0; j < modelObject[i].normalSize; j += 3 )
		{
			GLfloat* tempVector = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *(modelObject[i].normals) + j , modelObject[i].rotateThetaMatrix );
			MathHelper::vectorMultMatrix( &resultNormal[modelObject[i].vertexOffset + j] , tempVector , modelObject[i].rotatePhiMatrix );
			MathHelper::vectorNormalize( &resultNormal[modelObject[i].vertexOffset + j] );
			//std::cout << "法向量" << modelObject[i].offset + j << ":" << resultNormal[modelObject[i].offset + j] 
			//	      << "," << resultNormal[modelObject[i].offset + j + 1] << "," << resultNormal[modelObject[i].offset + j + 2] << std::endl;
		}

		for ( int j = 0; j < modelObject[i].indiceSize; j ++ )
		{
			GLuint a = *( *(modelObject[i].indices) + j );
			GLuint b = modelObject[i].indexOffset;
			resultIndice[ modelObject[i].indexOffset + j ] = *( *(modelObject[i].indices) + j ) + modelObject[i].indexOffset / 3;

		}
		//std::cout << "initial時法向量數是" << modelObject[i].verticeSize << std::endl;
	}

	ObjExporter objExporter;
	char* filePath = "output_Old";
	objExporter.Export( resultVertice , totalVerticeSize , resultNormal , totalNormalSize , resultIndice , totalIndiceSize , filePath );

}



void ExportObj( ModelObject* modelObject , char* filePath )
{
	GLuint totalVerticeSize = 0 , 
		   totalNormalSize = 0 , 
		   totalIndiceSize = 0;
	for ( ModelObject* i = originalModelLink; i != NULL; i = i -> link )
	{
		totalVerticeSize += i -> verticeSize;
		totalNormalSize += i -> normalSize;
		totalIndiceSize += i -> indiceSize;
	}

	GLfloat* scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	GLfloat* resultVertice = new GLfloat[totalVerticeSize];
	GLfloat* resultNormal = new GLfloat[totalNormalSize];
	GLuint* resultIndice = new GLuint[totalIndiceSize];
	std::cout << "Export時點數是" << totalVerticeSize/3 << ",法向量數是" << totalNormalSize/3 << ",面數是" << totalIndiceSize/3 << std::endl;

	for ( ModelObject* i = originalModelLink; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < i -> verticeSize; j += 3 )
		{

			//socket拉伸用
			GLfloat* tempVertice = new GLfloat[3]; MathHelper::CopyVector( tempVertice , *( i -> vertices) + j );
			if( i -> elongY != 0 )
			{
				if( tempVertice[1] > 0 )
				{
					tempVertice[1] += i -> elongY / 2.0;
				}
				else
				{
					tempVertice[1] -= i -> elongY / 2.0;
				}
			}
			//std::cout<<1;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , tempVertice , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( tempVector , tempVector2 , i -> rotateThetaMatrix );
			MathHelper::vectorMultMatrix( &resultVertice[ i -> vertexOffset + j] , tempVector , i -> translateMatrix );

			delete[] tempVertice;
			delete[] tempVector;
			delete[] tempVector2;
		}

		for ( int j = 0; j < i -> normalSize; j += 3 )
		{
			//std::cout<<2;
			//if ( *( *( i -> normals) + j )==0 && *( *( i -> normals) + j+1 )==0 && *( *( i -> normals) + j+2 )==0 )std::cout << "第" << i -> vertexOffset + j << "點本來就是無限"<< std::endl;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *( i -> normals) + j , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( &resultNormal[ i -> vertexOffset + j ] , tempVector2 , i -> rotateThetaMatrix );
			
			MathHelper::vectorNormalize( &resultNormal[ i -> vertexOffset + j ] );
			//std::cout << "法向量" << modelObject[i].offset + j << ":" << resultNormal[modelObject[i].offset + j] 
			//	      << "," << resultNormal[modelObject[i].offset + j + 1] << "," << resultNormal[modelObject[i].offset + j + 2] << std::endl;
			delete[] tempVector;
			delete[] tempVector2;
		}

		for ( int j = 0; j <  i -> indiceSize; j ++ )
		{

			GLuint a = *( *( i -> indices) + j );
			GLuint b =  i -> indexOffset;
			resultIndice[ i -> indexOffset + j ] = *( *( i -> indices) + j ) +  i -> vertexOffset / 3;

		}
		//std::cout << "initial時法向量數是" << modelObject[i].verticeSize << std::endl;

		
	}
	
	ObjExporter objExporter;
	objExporter.Export( resultVertice , totalVerticeSize , resultNormal , totalNormalSize , resultIndice , totalIndiceSize , filePath );
	delete[] scaleMatrix;
	delete[] resultVertice;
	delete[] resultNormal;
	delete[] resultIndice;
}
//可輸出顏色的方法
void ExportObj( ModelObject* modelObject , std::map<int,int>* material , char* filePath )
{
	GLuint totalVerticeSize = 0 , 
		   totalNormalSize = 0 , 
		   totalIndiceSize = 0;
	for ( ModelObject* i = originalModelLink; i != NULL; i = i -> link )
	{
		totalVerticeSize += i -> verticeSize;
		totalNormalSize += i -> normalSize;
		totalIndiceSize += i -> indiceSize;
	}

	GLfloat* scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	GLfloat* resultVertice = new GLfloat[totalVerticeSize];
	GLfloat* resultNormal = new GLfloat[totalNormalSize];
	GLuint* resultIndice = new GLuint[totalIndiceSize];
	std::cout << "Export時點數是" << totalVerticeSize/3 << ",法向量數是" << totalNormalSize/3 << ",面數是" << totalIndiceSize/3 << std::endl;

	for ( ModelObject* i = originalModelLink; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < i -> verticeSize; j += 3 )
		{

			//socket拉伸用
			GLfloat* tempVertice = new GLfloat[3]; MathHelper::CopyVector( tempVertice , *( i -> vertices) + j );
			if( i -> elongY != 0 )
			{
				if( tempVertice[1] > 0 )
				{
					tempVertice[1] += i -> elongY / 2.0;
				}
				else
				{
					tempVertice[1] -= i -> elongY / 2.0;
				}
			}
			//std::cout<<1;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , tempVertice , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( tempVector , tempVector2 , i -> rotateThetaMatrix );
			MathHelper::vectorMultMatrix( &resultVertice[ i -> vertexOffset + j] , tempVector , i -> translateMatrix );

			delete[] tempVertice;
			delete[] tempVector;
			delete[] tempVector2;
		}

		for ( int j = 0; j < i -> normalSize; j += 3 )
		{
			//std::cout<<2;
			//if ( *( *( i -> normals) + j )==0 && *( *( i -> normals) + j+1 )==0 && *( *( i -> normals) + j+2 )==0 )std::cout << "第" << i -> vertexOffset + j << "點本來就是無限"<< std::endl;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *( i -> normals) + j , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( &resultNormal[ i -> vertexOffset + j ] , tempVector2 , i -> rotateThetaMatrix );
			
			MathHelper::vectorNormalize( &resultNormal[ i -> vertexOffset + j ] );
			//std::cout << "法向量" << modelObject[i].offset + j << ":" << resultNormal[modelObject[i].offset + j] 
			//	      << "," << resultNormal[modelObject[i].offset + j + 1] << "," << resultNormal[modelObject[i].offset + j + 2] << std::endl;
			delete[] tempVector;
			delete[] tempVector2;
		}

		for ( int j = 0; j <  i -> indiceSize; j ++ )
		{

			GLuint a = *( *( i -> indices) + j );
			GLuint b =  i -> indexOffset;
			resultIndice[ i -> indexOffset + j ] = *( *( i -> indices) + j ) +  i -> vertexOffset / 3;

		}
		//std::cout << "initial時法向量數是" << modelObject[i].verticeSize << std::endl;

		
	}
	
	ObjExporter objExporter;
	objExporter.Export( resultVertice , 
						totalVerticeSize , 
						resultNormal , 
						totalNormalSize , 
						resultIndice , 
						totalIndiceSize , 
						filePath ,
						material );
	delete[] scaleMatrix;
	delete[] resultVertice;
	delete[] resultNormal;
	delete[] resultIndice;
}
//輸出桿件所受不同壓力的方法
void ExportObj( ModelObject* modelObject , std::map<int,float>* matForce , char* filePath )
{
	GLuint totalVerticeSize = 0 , 
		   totalNormalSize = 0 , 
		   totalIndiceSize = 0;
	for ( ModelObject* i = originalModelLink; i != NULL; i = i -> link )
	{
		totalVerticeSize += i -> verticeSize;
		totalNormalSize += i -> normalSize;
		totalIndiceSize += i -> indiceSize;
	}

	GLfloat* scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	GLfloat* resultVertice = new GLfloat[totalVerticeSize];
	GLfloat* resultNormal = new GLfloat[totalNormalSize];
	GLuint* resultIndice = new GLuint[totalIndiceSize];
	std::cout << "Export時點數是" << totalVerticeSize/3 << ",法向量數是" << totalNormalSize/3 << ",面數是" << totalIndiceSize/3 << std::endl;

	for ( ModelObject* i = originalModelLink; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < i -> verticeSize; j += 3 )
		{

			//socket拉伸用
			GLfloat* tempVertice = new GLfloat[3]; MathHelper::CopyVector( tempVertice , *( i -> vertices) + j );
			if( i -> elongY != 0 )
			{
				if( tempVertice[1] > 0 )
				{
					tempVertice[1] += i -> elongY / 2.0;
				}
				else
				{
					tempVertice[1] -= i -> elongY / 2.0;
				}
			}
			//std::cout<<1;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , tempVertice , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( tempVector , tempVector2 , i -> rotateThetaMatrix );
			MathHelper::vectorMultMatrix( &resultVertice[ i -> vertexOffset + j] , tempVector , i -> translateMatrix );

			delete[] tempVertice;
			delete[] tempVector;
			delete[] tempVector2;
		}

		for ( int j = 0; j < i -> normalSize; j += 3 )
		{
			//std::cout<<2;
			//if ( *( *( i -> normals) + j )==0 && *( *( i -> normals) + j+1 )==0 && *( *( i -> normals) + j+2 )==0 )std::cout << "第" << i -> vertexOffset + j << "點本來就是無限"<< std::endl;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *( i -> normals) + j , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( &resultNormal[ i -> vertexOffset + j ] , tempVector2 , i -> rotateThetaMatrix );
			
			MathHelper::vectorNormalize( &resultNormal[ i -> vertexOffset + j ] );
			//std::cout << "法向量" << modelObject[i].offset + j << ":" << resultNormal[modelObject[i].offset + j] 
			//	      << "," << resultNormal[modelObject[i].offset + j + 1] << "," << resultNormal[modelObject[i].offset + j + 2] << std::endl;
			delete[] tempVector;
			delete[] tempVector2;
		}

		for ( int j = 0; j <  i -> indiceSize; j ++ )
		{

			GLuint a = *( *( i -> indices) + j );
			GLuint b =  i -> indexOffset;
			resultIndice[ i -> indexOffset + j ] = *( *( i -> indices) + j ) +  i -> vertexOffset / 3;

		}
		//std::cout << "initial時法向量數是" << modelObject[i].verticeSize << std::endl;

		
	}
	
	ObjExporter objExporter;
	objExporter.Export( resultVertice , 
						totalVerticeSize , 
						resultNormal , 
						totalNormalSize , 
						resultIndice , 
						totalIndiceSize , 
						filePath ,
						matForce );
	delete[] scaleMatrix;
	delete[] resultVertice;
	delete[] resultNormal;
	delete[] resultIndice;
}

void ExportSingleObj( ModelObject* modelObject , char* filePath )
{
	GLuint totalVerticeSize = 0 , 
		   totalNormalSize = 0 , 
		   totalIndiceSize = 0;
	for ( ModelObject* i = modelObject; i != NULL; i = i -> link )
	{
		totalVerticeSize += i -> verticeSize;
		totalNormalSize += i -> normalSize;
		totalIndiceSize += i -> indiceSize;
	}

	GLfloat* scaleMatrix = new GLfloat[16];	MathHelper::makeIdentity(scaleMatrix);
	GLfloat* resultVertice = new GLfloat[totalVerticeSize];
	GLfloat* resultNormal = new GLfloat[totalNormalSize];
	GLuint* resultIndice = new GLuint[totalIndiceSize];
	std::cout << "Export時點數是" << totalVerticeSize/3 << ",法向量數是" << totalNormalSize/3 << ",面數是" << totalIndiceSize/3 << std::endl;

	for ( ModelObject* i = modelObject; i != NULL; i = i -> link )
	{
		for ( int j = 0; j < i -> verticeSize; j += 3 )
		{
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *( i -> vertices) + j , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( tempVector , tempVector2 , i -> rotateThetaMatrix );
			MathHelper::vectorMultMatrix( &resultVertice[ i -> vertexOffset + j] , tempVector , i -> translateMatrix );
		}

		for ( int j = 0; j < i -> normalSize; j += 3 )
		{
			//if ( *( *( i -> normals) + j )==0 && *( *( i -> normals) + j+1 )==0 && *( *( i -> normals) + j+2 )==0 )std::cout << "第" << i -> vertexOffset + j << "點本來就是無限"<< std::endl;
			GLfloat* tempVector = new GLfloat[3];
			GLfloat* tempVector2 = new GLfloat[3];
			MathHelper::vectorMultMatrix( tempVector , *( i -> normals) + j , i -> rotateYawMatrix );
			MathHelper::vectorMultMatrix( tempVector2 , tempVector , i -> rotatePhiMatrix );
			MathHelper::vectorMultMatrix( &resultNormal[ i -> vertexOffset + j ] , tempVector2 , i -> rotateThetaMatrix );
			
			MathHelper::vectorNormalize( &resultNormal[ i -> vertexOffset + j ] );
			//std::cout << "法向量" << modelObject[i].offset + j << ":" << resultNormal[modelObject[i].offset + j] 
			//	      << "," << resultNormal[modelObject[i].offset + j + 1] << "," << resultNormal[modelObject[i].offset + j + 2] << std::endl;
		}

		for ( int j = 0; j <  i -> indiceSize; j ++ )
		{
			GLuint a = *( *( i -> indices) + j );
			GLuint b =  i -> indexOffset;
			resultIndice[ i -> indexOffset + j ] = *( *( i -> indices) + j ) +  i -> vertexOffset / 3;

		}
		//std::cout << "initial時法向量數是" << modelObject[i].verticeSize << std::endl;
	}

	ObjExporter objExporter;
	objExporter.Export( resultVertice , totalVerticeSize , resultNormal , totalNormalSize , resultIndice , totalIndiceSize , filePath );

}


void SetVector( GLfloat* vec , GLfloat x , GLfloat y , GLfloat z )
{
	*vec = x;
	*(vec+1) = y;
	*(vec+2) = z;
}

void Debug0308()
{
	/*是一下算是
	//case 21: face1
	GLfloat* rotateZMatrix = new GLfloat[16];
	GLfloat* rotateXMatrix = new GLfloat[16];
	MathHelper::makeRotateZ( rotateZMatrix , M_PI / 2.0 );//繞z軸正向旋轉90度
	//MathHelper::makeRotateX( rotateXMatrix , -M_PI / 2.0 );//繞x軸正向旋轉90度
	GLfloat* tempDirection = new GLfloat[3];
	GLfloat* baseDirection = new GLfloat[3];
	ZomeDirection* tempZ = new ZomeDirection();
	
	MathHelper::vectorMultMatrix( tempDirection , &(tempZ -> faceDirection[ 58 * 3 ] ) , rotateZMatrix );
	//MathHelper::vectorMultMatrix( tempDirection , rotateXMatrix );

	std::cout<< "原本面sourceCorner[1]是58，結果做標" << tempDirection[0] << "," << tempDirection[1] << "," << tempDirection[2] << std::endl;
	std::cout << "結果面應該是" << tempZ -> GetDirectionFace( tempDirection );
	int sadga = 0;*/
	/*
	Point* ppp = new Point(0,0,0);
	Grid* ggg = new Grid(ppp);
	Point* a = new Point( -62.88 , 98.97 , -89.51 );
	Point* b = new Point( -148.76 , -15.166 , -58.924 );
	Point* c = new Point( 201.91 , 4.825 , 93.003 );
	if ( ggg -> CheckIntersectGridOnTri( a -> position , b -> position , c -> position ) )
		std::cout<<"有香蕉";
	//if ( ggg -> CheckIntersectTriOnGrid( a -> position , c -> position ) )
		//std::cout<<"有香蕉2";
		*/


	//去查每個outercone到外表的距離
	/*
	float dist_Point_to_Plane( Point P, Plane PL, Point* B)
	{
	    //http://geomalgorithms.com/a04-_planes.html
		//    Input:  P  = a 3D point
		//            PL = a  plane with point V0 and normal n
		//    Output: *B = base point on PL of perpendicular from P
		//    Return: the distance from P to the plane PL
		//    d norm 距離
		float    sb, sn, sd;

		sn = -dot( PL.n, (P - PL.V0));
		sd = dot(PL.n, PL.n);
		sb = sn / sd;

		*B = P + sb * PL.n;
		return d(P, *B);
	}*/
	//算出三個末端的中心
	/*
	GLfloat* endPointCenter = new GLfloat[3];
	endPointCenter[0] = ( outerCone -> cornerEnd[0] -> position[0] + 
						  outerCone -> cornerEnd[1] -> position[0] + 
						  outerCone -> cornerEnd[2] -> position[0] ) / 3.0;
	endPointCenter[1] = ( outerCone -> cornerEnd[0] -> position[1] + 
						  outerCone -> cornerEnd[1] -> position[1] + 
						  outerCone -> cornerEnd[2] -> position[1] ) / 3.0;
	endPointCenter[2] = ( outerCone -> cornerEnd[0] -> position[2] + 
						  outerCone -> cornerEnd[1] -> position[2] + 
						  outerCone -> cornerEnd[2] -> position[2] ) / 3.0;*/
	Point* endPointCenter = new Point( 0 , 0 , 0 );

	//紀錄最小距離
	GLfloat minDistance = 9999999.9;
	//for( int i = 0; i < model -> indiceSize / 3; i++ ) //出錯
	//{
		Point* p0 = new Point(1,1,0);
		Point* p1 = new Point(1,-1,1);
		Point* p2 = new Point(1,-1,0.5);
		Point* vector1 = new Point(); vector1 ->Vector(p0->position,p1->position);
		Point* vector2 = new Point(); vector2 ->Vector(p0->position,p2->position);
		//算normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();
		
		//從末端到表面 與normal同方向
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position , p0->position );
		GLfloat sb = normal -> Dot( distancePtoV0 );//是正的 如果點在模型裡，也就是距離
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//是正的 如果點在模型裡，也就是距離

		if( sb < minDistance && sb > 0 )
		{
			//如果這個面與點的距離小於目前的最小距離，則來看
			//1.這個點投影到面上時，會在三角型三個點之內，則她的距離就試與平面的距離
			//2.這個點投影到面上時，在三角型三個點之外，則她的距離就是與三個點的最小距離(比與平面的距離大)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//這邊算投影的點
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			normal -> Multiply( sb );
			pointProjectOnPlane -> Plus( normal );
			normal -> Multiply( 1.0 / sb );

			//算第一個做標
			Point* crossTotalArea = new Point(); crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); vertex0ToPoint -> Vector( p0 -> position , pointProjectOnPlane -> position );
			Point* crossArea1 = new Point(); crossArea1 -> Cross( vector1 , vertex0ToPoint );

			if( crossTotalArea -> Dot( crossArea1 ) > 0 )
			{
				Point* crossArea2 = new Point(); crossArea2 -> Cross( vertex0ToPoint , vector2 );
				if( crossTotalArea -> Dot( crossArea2 ) > 0 )
				{
					if( crossTotalArea -> Absolution() > crossArea1 -> Absolution() + crossArea2 -> Absolution() )
					{
						//有在三角形上
						insideTriangle = true;
					}
				}
				delete crossArea2;
			}

			if( insideTriangle )
			{
				minDistance = sb;
			}
			else
			{
				GLfloat* distance = new GLfloat[3];
				distance[0] = endPointCenter -> DistanceToPoint( p0->position );
				distance[1] = endPointCenter -> DistanceToPoint( p1->position );
				distance[2] = endPointCenter -> DistanceToPoint( p2->position );
				GLfloat min = ( distance[0] < distance[1] ? distance[0] : distance[1] ) < distance[2] ? 
							  ( distance[0] < distance[1] ? distance[0] : distance[1] ) : distance[2];
				if( min < minDistance )
				{
					minDistance = min;
				}
				delete[] distance;
			}
			delete crossTotalArea;
			delete vertex0ToPoint;
			delete crossArea1;
			delete pointProjectOnPlane;
		}




	//}
	
	//outerCone -> distanceToSurface[ outerCone -> type ] = minDistance;
	//return true;
}

void Debug0309()
{
		//http://geomalgorithms.com/a07-_distance.html
	/*
	// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
	//    Input:  two 3D line segments S1 and S2
	//    Return: the shortest distance between S1 and S2
	float
	dist3D_Segment_to_Segment( Segment S1, Segment S2)
	{
		Vector   u = S1.P1 - S1.P0;
		Vector   v = S2.P1 - S2.P0;
		Vector   w = S1.P0 - S2.P0;
		float    a = dot(u,u);         // always >= 0
		float    b = dot(u,v);
		float    c = dot(v,v);         // always >= 0
		float    d = dot(u,w);
		float    e = dot(v,w);
		float    D = a*c - b*b;        // always >= 0
		float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
		float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
    
	}*/

	Point* vec1Initial = new Point(1,1,0);
	Point* vec1End = new Point(1,-1,0);
	Point* vec2Initial = new Point(0,0,11);
	Point* vec2End = new Point(0,0,10);
	Point* u = new Point(); u -> Vector( vec1Initial -> position , vec1End -> position );
	Point* v = new Point(); v -> Vector( vec2Initial -> position , vec2End -> position );
	Point* w = new Point(); w -> Vector( vec2Initial -> position , vec1Initial -> position );

	GLfloat a = u -> Dot( u -> position );
	GLfloat b = u -> Dot( v -> position );
	GLfloat c = v -> Dot( v -> position );
	GLfloat d = u -> Dot( w -> position );
	GLfloat e = v -> Dot( w -> position );
	GLfloat D = a*c - b*b;
	GLfloat sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    GLfloat tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
	GLfloat SMALL_NUM = 0.001;

	 // compute the line parameters of the two closest points
	if (D < SMALL_NUM) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                 // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
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

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
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
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

    // get the difference of the two closest points
   // Vector   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)
	Point* dP = new Point( w -> position[0] + sc * u -> position[0] - tc * v -> position[0] ,
						   w -> position[1] + sc * u -> position[1] - tc * v -> position[1] ,
						   w -> position[2] + sc * u -> position[2] - tc * v -> position[2] );

	//return norm(dP);   // return the closest distance
	GLfloat result = dP -> Absolution();
	delete u;
	delete v;
	delete w;
	delete dP;
	//return result;
}

void Debug0316()
{
	//Point* endPointCenter = new Point( outerCone -> cornerEnd[0] -> position );
	//endPointCenter -> Plus( outerCone -> cornerEnd[1] );
	//endPointCenter -> Plus( outerCone -> cornerEnd[2] );
	//endPointCenter -> Multiply( 1.0 / 3.0 );
	Point* endPointCenter = new Point( 0 , 0 , 0 );

	Point* tempNearstPoint = new Point();

	//紀錄最小距離
	GLfloat minDistance = 9999999.9;
	for( int i = 0; i < 1; i++ ) //出錯
	{
		GLfloat p0[] = { 50 , 50 , 0 };
		GLfloat p1[] = { 0 , 50 , 50 };
		GLfloat p2[] = { 50 , 0 , 50 };


		Point* vector1 = new Point(); vector1 -> Vector( p0 , p2 );
		Point* vector2 = new Point(); vector2 -> Vector( p1 , p2 );
		//算normal
		Point* normal = new Point(); normal -> Cross( vector1 , vector2 ); normal -> Normalize();

		std::cout << "法向量方向是：" << normal -> position[0] << "," << normal -> position[1] << "," << normal -> position[2] << std::endl;
		/*
		GLfloat* vertexNormalAverage = new GLfloat[3];//vertex normal平均值
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
		MathHelper::VectorPlus( vertexNormalAverage , *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		MathHelper::VectorMutiply( vertexNormalAverage , 1.0 / 3.0 );*/
		//( Normal 1 + Normal 2 + Normal 3 ) / 3
		//Point* vertexNormalAverage = new Point( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//vertexNormalAverage -> Plus( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 1 ] * 3 );
		//vertexNormalAverage -> Plus( *( model -> normals ) + ( *( model -> indices ) )[ i * 3 + 2 ] * 3 );
		//vertexNormalAverage -> Multiply( 1.0 / 3.0 );
		////如果和點法向量相反方向，就把平面normal反轉
		//if( vertexNormalAverage -> Dot( normal -> position ) < 0 ) normal -> Reverse();
		
		//從末端到表面 與normal同方向
		Point* distancePtoV0 = new Point(); 
		distancePtoV0 -> Vector( endPointCenter -> position , p0 );
		GLfloat sb = normal -> Dot( distancePtoV0 );//是正的 如果點在模型裡，也就是距離
		//GLfloat* distancePtoV0 = new GLfloat[3]; MathHelper::VectorMinus( distancePtoV0 , endPointCenter , *( model -> vertices ) + ( *( model -> indices ) )[ i * 3 ] * 3 );
		//GLfloat sb = MathHelper::VectorDot( normal -> position , distancePtoV0 );//是正的 如果點在模型裡，也就是距離

		std::cout << "法向量距離試" << sb << std::endl;

		if( sb < minDistance && sb > 0 )
		{
			//如果這個面與點的距離小於目前的最小距離，則來看
			//1.這個點投影到面上時，會在三角型三個點之內，則她的距離就試與平面的距離
			//2.這個點投影到面上時，在三角型三個點之外，則她的距離就是與三個點的最小距離(比與平面的距離大)
			//http://www.blackpawn.com/texts/pointinpoly/
			
			bool insideTriangle = false;
			//這邊算投影的點
			Point* pointProjectOnPlane = new Point(); pointProjectOnPlane -> Copy( endPointCenter );
			normal -> Multiply( sb );
			pointProjectOnPlane -> Plus( normal );
			normal -> Multiply( 1.0 / sb );


			std::cout << "投影在平面上的做標為:" << pointProjectOnPlane -> position[0] << ","
				<< pointProjectOnPlane -> position[1] << ","
				<< pointProjectOnPlane -> position[2] << std::endl;
			//算第一個做標 第一個是三角型總面積x2 第二個是三角型點0到投影點的距離 第三個是投影點和編1所包的面積
			Point* crossTotalArea = new Point(); 
				   crossTotalArea -> Cross( vector1 , vector2 );
			Point* vertex0ToPoint = new Point(); 
				   vertex0ToPoint -> Vector( p0 , pointProjectOnPlane -> position );
			Point* vertex1ToPoint = new Point(); 
				   vertex1ToPoint -> Vector( p1 , pointProjectOnPlane -> position );
			Point* crossArea1 = new Point(); 
				   crossArea1 -> Cross( vector1 , vertex0ToPoint ); 
			Point* crossArea2 = new Point(); 
				   crossArea2 -> Cross( vertex1ToPoint , vector2 );
			std::cout << "總面積為:" << crossTotalArea -> Absolution() << ",第一塊面積為:" << crossArea1 -> Absolution() << std::endl;
			


			std::cout << "做標一:" << crossTotalArea -> Dot( crossArea1 ) << std::endl;
			//算第一個做標漢第二個做標
			GLfloat TotalArea = crossTotalArea -> Dot( normal );
			GLfloat Area1 = crossArea1 -> Dot( normal );
			GLfloat Area2 = crossArea2 -> Dot( normal );
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
				minDistance = sb;
				tempNearstPoint -> Copy( pointProjectOnPlane );
			}
			else
			{
				GLfloat* distance = new GLfloat[3];
				distance[0] = endPointCenter -> DistanceToPoint( p0 );
				distance[1] = endPointCenter -> DistanceToPoint( p1 );
				distance[2] = endPointCenter -> DistanceToPoint( p2 );
				int min = 0;

				//找出最近的點
				if( distance[0] < distance[1] )
					if( distance[0] < distance[2] )
						min = 0;
					else
						min = 2;
				else
					if( distance[1] < distance[2] )
						min = 1;
					else
						min = 2;

				//把這個點的位置給紀錄下來 寄在tempNearstPoint裡
				if( distance[ min ] < minDistance )
				{
					minDistance = distance[ min ];
					tempNearstPoint -> Copy( p0 );//p0應該是p?
				}
				delete[] distance;
			}
			delete crossTotalArea;
			delete vertex0ToPoint;
			delete crossArea1;
			delete pointProjectOnPlane;
		}
	}

	std::cout << "距離結果是：" << minDistance << std::endl;
}

void Debug0330()
{
	GLfloat pos[3] = {0,0,0};
	GLfloat vec[3] = {5,5,5};
	MathHelper::VectorNormalize( vec );
	Plane* p = new Plane( pos , vec );

	GLfloat p1[3] = {0,0,0};
	GLfloat p2[3] = {2,2,1};

	std::cout << p -> CheckIntersection(p1,p2);
}

void Debug0406()
{
	GLfloat pos0[3] = {1,0,0};
	GLfloat vec0[3] = {0,1,0};
	Line* l1 = new Line( pos0 , vec0 );
	GLfloat pos1[3] = {2,0,0};
	GLfloat vec1[3] = {0,0,-1};
	Line* l2 = new Line( pos1 , vec1 );
	l1 -> TranslateToNearest_2( l2 );
	l1 -> TranslateToNearest( l2 );
	std::cout << "邊線Line0原點：(" << l1 -> initPosition[0] << "," << l1 -> initPosition[1] << "," << l1 -> initPosition[2] 
				<< ")，方向：("  << l1 -> endPosition[0] << "," << l1 -> endPosition[1] << "," << l1 -> endPosition[2] << ")" << std::endl;
	std::cout << "邊線Line1原點：(" << l2 -> initPosition[0] << "," << l2 -> initPosition[1] << "," << l2 -> initPosition[2] 
				<< ")，方向：("  << l2 -> endPosition[0] << "," << l2 -> endPosition[1] << "," << l2 -> endPosition[2] << ")" << std::endl;


	GLfloat pos[3] = {0,0,0};
	GLfloat vec[3] = {5,3,5};
	MathHelper::VectorNormalize( vec );
	Plane* p1 = new Plane( pos , vec );

	MathHelper::SetVector( vec , 9 , -5 , 5 );
	MathHelper::VectorNormalize( vec );
	Plane* p2 = new Plane( pos , vec );

	Line* line = new Line();
	p1 -> Get2PlaneIntersectLine( p2 , line );
	std::cout << "先取面法向量：(" << p1 -> vector[0] << "," << p1 -> vector[1] << "," << p1 -> vector[2] 
			  << ")和面法向量：(" << vec[0] << "," << vec[1] << "," << vec[2] << ")作相交，得到線段原點" << std::endl;
	std::cout << "(" << line -> initPosition[0] << "," << line -> initPosition[1] << "," << line -> initPosition[2] << ")，方向" 
			  << "(" << line -> endPosition[0] << "," << line -> endPosition[1] << "," << line -> endPosition[2] << ")" << std::endl;

}

void Debug0426()
{
	Constructor* constructor = new Constructor(  );
	int ball [32];
	ball[0] = 0;
	for( int i = 0 ; i < 12; i++ )
	{
		ball[i+1] = constructor->Construct( ball[0] , 30+i , SIZE_L , 1 );
	}
	for( int j = 0; j <11; j++ )
	for( int i = 0 ; i < 12; i++ )
	{
		constructor->Construct( ball[1+j] , 30+i , SIZE_L , 1 );
	}


	originalModelLink = constructor -> originalModel;

	ExportObj( originalModelLink , "Try.obj" );
}