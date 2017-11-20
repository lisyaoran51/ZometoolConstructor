#include "main.h"
#include <time.h>


int main(int argc, char** argv) {
	time_t startTime = time(NULL); // 開始時間
	//Debug0316();
	//Debug0330();
	//Debug0406();
	//Debug0426();
	

	if( CONSTRUCTOR_MODE == SELFDEFINE_STRUCTURE )
	{
		ZomeGenerator* zomeGenerator = new ZomeGenerator();
		// 右
		Cone* c0 = zomeGenerator -> firstCone;
		Cone* c1 = zomeGenerator -> GrowCone( c0 , 1 ); // 角
		Cone* c2 = zomeGenerator -> GrowCone( c1 , 2 );
		Cone* c3 = zomeGenerator -> GrowCone( c2 , 2 );
		Cone* c4 = zomeGenerator -> GrowCone( c3 , 2 );
		Cone* c5 = zomeGenerator -> GrowCone( c3 , 0 ); // 角

		Cone* c6 = zomeGenerator -> GrowCone( c2 , 1 );
		Cone* c7 = zomeGenerator -> GrowCone( c6 , 2 );
		Cone* c8 = zomeGenerator -> GrowCone( c7 , 2 );
		Cone* c9 = zomeGenerator -> GrowCone( c8 , 2 );

		// 中右
		Cone* c10 = zomeGenerator -> GrowCone( c7 , 1 );
		Cone* c11 = zomeGenerator -> GrowCone( c10 , 2 );
		Cone* c12 = zomeGenerator -> GrowCone( c11 , 2 );
		Cone* c13 = zomeGenerator -> GrowCone( c12 , 2 );

		Cone* c14 = zomeGenerator -> GrowCone( c9 , 1 );
		Cone* c15 = zomeGenerator -> GrowCone( c14 , 2 );
		Cone* c16 = zomeGenerator -> GrowCone( c15 , 2 );
		Cone* c17 = zomeGenerator -> GrowCone( c16 , 2 );
		// 中中
		Cone* c51 = zomeGenerator -> GrowCone( c16 , 0 );
		Cone* c52 = zomeGenerator -> GrowCone( c51 , 2 );
		Cone* c53 = zomeGenerator -> GrowCone( c52 , 2 );
		Cone* c54 = zomeGenerator -> GrowCone( c53 , 2 );
		// 中左
		Cone* c55 = zomeGenerator -> GrowCone( c53 , 0 );
		Cone* c56 = zomeGenerator -> GrowCone( c55 , 2 );
		Cone* c57 = zomeGenerator -> GrowCone( c56 , 2 );
		Cone* c58 = zomeGenerator -> GrowCone( c57 , 2 );
		
		Cone* c59 = zomeGenerator -> GrowCone( c51 , 0 );
		Cone* c60 = zomeGenerator -> GrowCone( c59 , 2 );
		Cone* c61 = zomeGenerator -> GrowCone( c60 , 2 );
		Cone* c62 = zomeGenerator -> GrowCone( c61 , 2 );
		
		
		// 左
		Cone* c18 = zomeGenerator -> GrowCone( c61 , 1 );
		Cone* c19 = zomeGenerator -> GrowCone( c18 , 2 );
		Cone* c20 = zomeGenerator -> GrowCone( c19 , 2 );
		Cone* c21 = zomeGenerator -> GrowCone( c20 , 2 );
		
		Cone* c22 = zomeGenerator -> GrowCone( c19 , 1 );
		Cone* c23 = zomeGenerator -> GrowCone( c22 , 2 );
		Cone* c24 = zomeGenerator -> GrowCone( c23 , 2 );
		Cone* c25 = zomeGenerator -> GrowCone( c24 , 2 );
		
		Cone* c26 = zomeGenerator -> GrowCone( c23 , 1 ); // 角
		Cone* c27 = zomeGenerator -> GrowCone( c25 , 1 ); // 角

		zomeGenerator -> GrowOuterCone( c26 , 2 , 1 );
		zomeGenerator -> GrowOuterCone( c26 , 3 , 1 );
		
		zomeGenerator -> GrowOuterCone( c27 , 2 , 1 );
		zomeGenerator -> GrowOuterCone( c27 , 3 , 1 );
		
		zomeGenerator -> GrowOuterCone( c0 , 2 , 1 );
		zomeGenerator -> GrowOuterCone( c0 , 3 , 1 );
		
		zomeGenerator -> GrowOuterCone( c5 , 2 , 1 );
		zomeGenerator -> GrowOuterCone( c5 , 3 , 1 );
		zomeGenerator -> ActivateAllCone();
		zomeGenerator -> ConstructStructure();

		//zomeGenerator -> ConstructCusStruc();
		originalModelLink = zomeGenerator -> constructor -> originalModel;
		ExportObj( originalModelLink , zomeGenerator -> constructor -> material , "InnerZometoolOutput.obj" );

		zomeGenerator -> CustomizedAnalysis();
		
		ExportObj( originalModelLink , &(zomeGenerator -> structureAnalyzer -> matForce) , "ForceOutput.obj" );
	}

	//Model *modelToHollow2 = new Model("mrhumpty4Std.obj");
	//Model *modelToHollow4 = new Model("mrhumpty5Std.obj");
	//Model *modelToHollow9 = new Model("bunny0522.obj");
	//Model *modelToHollow10 = new Model("buddha0421Std2.obj");
	//Model *modelToHollow11 = new Model("ateneav0525.obj");//z-10 s0.75
	//Model *modelToHollow12 = new Model("dragon0522.obj");
	//Model *modelToHollow13 = new Model("bear.obj");
	//Model *modelToHollow14 = new Model("kitten.obj");
	//Model *modelToHollow15 = new Model("Column7.obj");
	//Model *modelToHollow16 = new Model("TestBall.obj");
	//Model *modelToHollow17 = new Model("donut1024.obj");
	//Model *modelToHollow18 = new Model("BendTube1201.obj");
	Model *modelToHollow19 = new Model("TestBall1208.obj");
	Model *modelToHollow20 = new Model("TestBall0208.obj");

	Model *tempModel = modelToHollow19;
	//int mode = 0;
	//for( int i = 0; i < modelToHollow10 -> indiceSize / 3; i++ ) 
	//{
	//	GLfloat v1[3] , v2[3] , v3[3];
	//	MathHelper::CopyVector( v1 , *( modelToHollow10 -> vertices ) + ( *( modelToHollow10 -> indices ) )[ i * 3 ] * 3 );
	//	MathHelper::CopyVector( v2 , *( modelToHollow10 -> vertices ) + ( *( modelToHollow10 -> indices ) )[ i * 3 + 1 ] * 3 );
	//	MathHelper::CopyVector( v3 , *( modelToHollow10 -> vertices ) + ( *( modelToHollow10 -> indices ) )[ i * 3 + 2 ] * 3 );
	//	std::cout << "(" << v1[0] << "," << v1[1] << "," << v1[2] << "),";
	//	std::cout << "(" << v2[0] << "," << v2[1] << "," << v2[2] << ")," ;
	//	std::cout << "(" << v3[0] << "," << v3[1] << "," << v3[2] << ")" << std::endl; 
	//}
	GLfloat sca[3] = {1,1,1};
	GLfloat tra[3] = {0,0.0f,-10.0f};
	GLfloat rot[3] = { 0.0f , 0.0f , 0.0f };

	
	
	

	//這邊開始生架子
	if( CONSTRUCTOR_MODE == MAKE_CONE || 
		CONSTRUCTOR_MODE == MAKE_OUTERCONE || 
		CONSTRUCTOR_MODE == MAKE_BRANCH ||
		CONSTRUCTOR_MODE == MAKE_BRANCH_END )
	{
		ZomeGenerator* zomeGenerator = new ZomeGenerator( tempModel );

		//originalModelLink = zomeGenerator -> constructor -> originalModel;

		//ExportObj( originalModelLink , "InnerZometoolOutput.obj" );

		//把前面抓到的街頭 紀錄到這個串列裡面
		Point* nearestPointOnSurface = new Point();
		nearestPointOnSurface -> firstPoint = nearestPointOnSurface;

		zomeGenerator -> GetNearestPointOnSurface( nearestPointOnSurface );
		//zomeGenerator -> ExportNearestPointOnSurface( nearestPointOnSurface );
		if( CONSTRUCTOR_MODE == MAKE_CONE || 
			CONSTRUCTOR_MODE == MAKE_OUTERCONE )
		{
			originalModelLink = zomeGenerator -> constructor -> originalModel;
			ExportObj( originalModelLink , zomeGenerator -> constructor -> material , "InnerZometoolOutput.obj" );
			zomeGenerator -> constructor -> PrintTotalComponents();

			if( CONSTRUCTOR_MODE == MAKE_OUTERCONE )
			{
				if( STRUCTURAL_ANALYSIS )
				{
					ExportObj( originalModelLink , &(zomeGenerator -> structureAnalyzer -> matForce) , "ForceOutput.obj" );
					//originalModelLink = zomeGenerator -> AnalyzeDisplay -> originalModel;
					//ExportObj( zomeGenerator -> AnalyzeDisplay -> originalModel , "AnalyzeOutput.obj" );
				}
			}
			time_t endTime = time(NULL); // 開始時間
			std::cout << "最後共用" << endTime - startTime << "秒" << std::endl;
			system("pause");
			return 0;
		}
		

		Polyhedron* polyhedron = new Polyhedron( tempModel );
		Segmenter* segmenter = new Segmenter( polyhedron , nearestPointOnSurface , IDENTIFIER );
		
		zomeGenerator -> MakeBranch( polyhedron , segmenter );

		originalModelLink = zomeGenerator -> constructor -> originalModel;

		//ExportObj( originalModelLink , "InnerZometoolOutput.obj" );
		ExportObj( originalModelLink , zomeGenerator -> constructor -> material , "InnerZometoolOutput.obj" );//這個輸出結構
		if( STRUCTURAL_ANALYSIS ) //如果有要算結構分析
			ExportObj( originalModelLink , (&zomeGenerator -> structureAnalyzer -> matForce) , "ForceOutput.obj" );//這個輸出受力
		

		originalModelLink = zomeGenerator -> socketConstructor -> originalModel;
		ExportObj( zomeGenerator -> socketConstructor -> originalModel , "SocketOutput.obj" );
		
		// debug用 站時不寫了
		// ExportObj( zomeGenerator -> socketConstructor -> originalModel , "DebugSocket.obj" );

		
		//這邊先把原本的點給清掉，然後重新抓一次表面點，抓完就輸出
		zomeGenerator -> Delete( nearestPointOnSurface );
		nearestPointOnSurface = new Point(); 
		nearestPointOnSurface -> firstPoint = nearestPointOnSurface;
		zomeGenerator -> GetNearestPointOnSurface( nearestPointOnSurface );
		zomeGenerator -> ExportNearestPointOnSurface( nearestPointOnSurface );

		zomeGenerator -> constructor -> PrintTotalComponents();
		delete zomeGenerator;
	}

	if( CONSTRUCTOR_MODE == MAKE_SEGMENT )
	{
		Polyhedron* polyhedron = new Polyhedron( tempModel );

		Point* nearestPointOnSurface = new Point( "NearestPointOnSurface.txt" );
		
		Segmenter* segmenter = new Segmenter( polyhedron , nearestPointOnSurface );

		originalModelLink = segmenter -> splitSurfaceConstructor[0] -> originalModel;

		//趙順序把每一個碎片的分隔建出來
		for( int i = 0; i < segmenter -> segmentAmount; i++ )
		{
			char filePath[80] =  "splitSurfaceOutput";
			
			char segmentNumber[4];
			sprintf( segmentNumber, "%i", i );
			strcat(filePath, segmentNumber);
			strcat(filePath, ".obj");

			originalModelLink = segmenter -> splitSurfaceConstructor[i] -> originalModel;
			ExportObj( segmenter -> splitSurfaceConstructor[i] -> originalModel , filePath );
		}
		

		//ExportObj( segmenter -> splitSurfaceConstructor[0] -> originalModel , "splitSurfaceOutput.obj" );
	}

	time_t endTime = time(NULL); // 開始時間
	std::cout << "最後共用" << endTime - startTime << "秒" << std::endl;
	system("pause");
	return 0;
}