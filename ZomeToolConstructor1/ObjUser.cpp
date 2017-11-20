#include "ObjUser.h"

void ObjExporter::Export_Old(GLfloat Vertices[], GLuint VerticeSize, GLfloat Normals[], GLuint NormalsSize, GLuint Faces[], GLuint FacesSize)
{
	FILE* ObjFile = fopen("Output.obj", "w");
	for (int i = 0; i < VerticeSize / 3; i++)
	{
		fprintf(ObjFile, "v %f %f %f \n", (float)Vertices[i * 3 + 0], (float)Vertices[i * 3 + 1], (float)Vertices[i * 3 + 2]);
	}

	for (int i = 0; i < NormalsSize / 3; i++)
	{
		fprintf(ObjFile, "vn %f %f %f \n", (float)Normals[i * 3 + 0], (float)Normals[i * 3 + 1], (float)Normals[i * 3 + 2]);
	}

	for (int i = 0; i < FacesSize / 3; i++)
	{
		fprintf(ObjFile, "f %d//%d %d//%d %d//%d \n", (int)Faces[i * 3 + 0] + 1, (int)Faces[i * 3 + 0] + 1
			, (int)Faces[i * 3 + 1] + 1, (int)Faces[i * 3 + 1] + 1
			, (int)Faces[i * 3 + 2] + 1, (int)Faces[i * 3 + 2] + 1);

	}
}


void ObjExporter::Export(GLfloat Vertices[], GLuint VerticeSize, GLfloat Normals[], GLuint NormalsSize, GLuint Faces[], GLuint FacesSize , char* filePath )
{
	//FILE* ObjFile = fopen("Output2.obj", "w");
	FILE* ObjFile = fopen(filePath,"w");

	int debugCount = 0;
	for (int i = 0; i < VerticeSize / 3; i++)
	{
		fprintf(ObjFile, "v %f %f %f \n", (float)Vertices[i * 3 + 0], (float)Vertices[i * 3 + 1], (float)Vertices[i * 3 + 2]);
		fprintf(ObjFile, "vn %f %f %f \n", (float)Normals[i * 3 + 0], (float)Normals[i * 3 + 1], (float)Normals[i * 3 + 2]);
	}

	for (int i = 0; i < NormalsSize / 3; i++)
	{
		
	}

	for (int i = 0; i < FacesSize / 3; i++)
	{
		debugCount++;
		fprintf(ObjFile, "f %d//%d %d//%d %d//%d \n", (int)Faces[i * 3 + 0] + 1, (int)Faces[i * 3 + 0] + 1
			, (int)Faces[i * 3 + 1] + 1, (int)Faces[i * 3 + 1] + 1
			, (int)Faces[i * 3 + 2] + 1, (int)Faces[i * 3 + 2] + 1);

	}
	if ( debugCount < FacesSize / 3 ) std::cout << "face輸出出錯" << std::endl;

	fclose(ObjFile);
}

void ObjExporter::Export(GLfloat Vertices[], 
						 GLuint VerticeSize, 
						 GLfloat Normals[], 
						 GLuint NormalsSize, 
						 GLuint Faces[], 
						 GLuint FacesSize , 
						 char* filePath ,
						 std::map<int,int>* material )
{
	//FILE* ObjFile = fopen("Output2.obj", "w");
	FILE* ObjFile = fopen(filePath,"w");

	fprintf( ObjFile , "mtllib color.mtl \n" , filePath );

	int debugCount = 0;
	int faceCount = 0;
	for (int i = 0; i < VerticeSize / 3; i++)
	{
		fprintf(ObjFile, "v %f %f %f \n", (float)Vertices[i * 3 + 0], (float)Vertices[i * 3 + 1], (float)Vertices[i * 3 + 2]);
		fprintf(ObjFile, "vn %f %f %f \n", (float)Normals[i * 3 + 0], (float)Normals[i * 3 + 1], (float)Normals[i * 3 + 2]);
	}

	for (int i = 0; i < NormalsSize / 3; i++)
	{
		
	}

	for (int i = 0; i < FacesSize / 3; i++)
	{
		std::map<int, int>::iterator m;
		m = material -> find( faceCount );

		if( m != material -> end() )
		{
			switch( m -> second )
			{
				case COLOR_BLUE:
					fprintf( ObjFile , "usemtl blue \n" );
					break;
				case COLOR_RED:
					fprintf( ObjFile , "usemtl red \n" );
					break;
				case COLOR_YELLOW:
					fprintf( ObjFile , "usemtl yellow \n" );
					break;
				case COLOR_WHITE:
					fprintf( ObjFile , "usemtl white \n" );
					break;
			}
		}
		debugCount++;
		fprintf(ObjFile, "f %d//%d %d//%d %d//%d \n", (int)Faces[i * 3 + 0] + 1, (int)Faces[i * 3 + 0] + 1
			, (int)Faces[i * 3 + 1] + 1, (int)Faces[i * 3 + 1] + 1
			, (int)Faces[i * 3 + 2] + 1, (int)Faces[i * 3 + 2] + 1);

		faceCount++;
	}
	if ( debugCount < FacesSize / 3 ) std::cout << "face輸出出錯" << std::endl;

	fclose(ObjFile);
}

void ObjExporter::Export(GLfloat Vertices[], 
						 GLuint VerticeSize, 
						 GLfloat Normals[], 
						 GLuint NormalsSize, 
						 GLuint Faces[], 
						 GLuint FacesSize , 
						 char* filePath ,
						 std::map<int,float>* matForce )
{
	//FILE* ObjFile = fopen("Output2.obj", "w");
	FILE* ObjFile = fopen(filePath,"w");

	fprintf( ObjFile , "mtllib force.mtl \n" , filePath );

	int debugCount = 0;
	int faceCount = 0;
	for (int i = 0; i < VerticeSize / 3; i++)
	{
		fprintf(ObjFile, "v %f %f %f \n", (float)Vertices[i * 3 + 0], (float)Vertices[i * 3 + 1], (float)Vertices[i * 3 + 2]);
		fprintf(ObjFile, "vn %f %f %f \n", (float)Normals[i * 3 + 0], (float)Normals[i * 3 + 1], (float)Normals[i * 3 + 2]);
	}

	for (int i = 0; i < NormalsSize / 3; i++)
	{
		
	}

	for (int i = 0; i < FacesSize / 3; i++)
	{
		std::map<int, float>::iterator m;
		m = matForce -> find( faceCount );

		
		if( m != matForce -> end() )//11/8寫到這邊
		{
			int tempForce = (int)m -> second;//這邊做測試
			if( tempForce < 0 )
			{
				if( tempForce < -1000 )//如果受力過大，就只顯示到1000
				{
					tempForce = -1000;
				}
				//int debuggg = tempForce / 100;
				fprintf( ObjFile , "usemtl n%d \n" , -tempForce );
				//printf( "在第%d面寫入usemtl n%d \n" , faceCount , -tempForce );
			}
			
			if( tempForce >= 0 )
			{
				if( tempForce > 1000 )//如果受力過大，就只顯示到1000
				{
					tempForce = 1000;
				}
				//int debuggg = tempForce / 100;

				fprintf( ObjFile , "usemtl p%d \n" , tempForce );
				//printf( "在第%d面寫入usemtl n%d \n" , faceCount , tempForce );
			}
			/*
			if( m -> second < -500 )//藍到藍綠 受力是-1000~-500
			{
				if( m -> second < -1000 )
				{
					tempForce = -1000;
				}
				int color = ( tempForce - (-1000) ) * 2;
				fprintf( ObjFile , "usemtl blue2aquamarine%d \n" , color );
			}
			else if( m -> second < 0 )//藍綠到綠 受力是-500~0
			{
				int color = ( tempForce - (-500) ) * 2;
				fprintf( ObjFile , "usemtl aquamarine2green%d \n" , color );
			}
			else if( m -> second < 500 )//綠到黃 受力是0~500
			{
				int color = ( tempForce ) * 2;
				fprintf( ObjFile , "usemtl green2yellow%d \n" , color );
			}
			else if(  m -> second < 1000 )//黃到紅 受力是500~1000
			{
				int color = ( tempForce - 500 ) * 2;
				fprintf( ObjFile , "usemtl yellow2red%d \n" , color );
			}
			else
			{
				fprintf( ObjFile , "usemtl yellow2red1000 \n" );
			}
			*/
			/*
			switch( m -> second )
			{
				case COLOR_BLUE:
					fprintf( ObjFile , "usemtl blue \n" );
					break;
				case COLOR_RED:
					fprintf( ObjFile , "usemtl red \n" );
					break;
				case COLOR_YELLOW:
					fprintf( ObjFile , "usemtl yellow \n" );
					break;
				case COLOR_WHITE:
					fprintf( ObjFile , "usemtl white \n" );
					break;
			}*/
		}
		
		debugCount++;
		fprintf(ObjFile, "f %d//%d %d//%d %d//%d \n", (int)Faces[i * 3 + 0] + 1, (int)Faces[i * 3 + 0] + 1
			, (int)Faces[i * 3 + 1] + 1, (int)Faces[i * 3 + 1] + 1
			, (int)Faces[i * 3 + 2] + 1, (int)Faces[i * 3 + 2] + 1);

		faceCount++;
	}
	if ( debugCount < FacesSize / 3 ) std::cout << "face輸出出錯" << std::endl;

	fclose(ObjFile);
}

void ObjImporter::GetSize(char* FilePath, GLuint* VerticeSize, GLuint* NormalSize, GLuint* FaceSize)
{
	FILE* ObjFile = fopen(FilePath, "r");
	if (!ObjFile){
		std::cout << "無法讀取 \"" << FilePath << "\" !" << std::endl;
	}
	GLfloat Temp;
	GLuint VerticeNumber = 0, NormalNumber = 0, FaceNumber = 0;
	char* TempLine = new char[1000];
	while (fscanf(ObjFile, "%s", TempLine) != EOF)
	{
		if (strcmp(TempLine, "v") == 0)
		{
			VerticeNumber += 3;
		}
		else if (strcmp(TempLine, "vn") == 0)
		{
			NormalNumber += 3;
		}
		else if (strcmp(TempLine, "f") == 0)
		{
			FaceNumber += 3;
		}
	}
	fseek(ObjFile, 0, SEEK_SET);
	*VerticeSize = VerticeNumber;
	*NormalSize = NormalNumber;
	*FaceSize = FaceNumber;
	fclose( ObjFile );
	//std::cout << VerticeNumber << " , " << NormalNumber << " , " << FaceNumber << std::endl;
}

void ObjImporter::Import(char* FilePath, GLfloat* Vertices, GLuint* VerticeSize, GLfloat* Normals, GLuint* NormalSize, GLuint* Faces, GLuint* FaceSize)
{
	FILE* ObjFile = fopen(FilePath, "r");
	if (!ObjFile){
		std::cout << "無法讀取 \"" << FilePath << "\" !" << std::endl;
	}
	GLfloat Temp;
	GLuint VerticeCount = 0, NormalCount = 0, FaceCount = 0;
	char* TempLine = new char[1000];
	while (fscanf(ObjFile, "%s", TempLine) != EOF)
	{
		if (strcmp(TempLine, "v") != 0 && strcmp(TempLine, "v") == 0 && strcmp(TempLine, "f") == 0)
		{
			if (TempLine[strlen(TempLine) - 4] == '\\' && TempLine[strlen(TempLine) - 3] == 'n')
			{
				if (strcmp(TempLine, "vn") == 0)
				{
					fscanf(ObjFile, "%f %f %f\n", &Normals[NormalCount], &Normals[NormalCount + 1], &Normals[NormalCount + 2]);
					std::cout << Normals[NormalCount] << " , " << Normals[NormalCount + 1] << " , " << Normals[NormalCount + 2] << std::endl;
					NormalCount += 3;
				}
			}
			else if (TempLine[strlen(TempLine) - 3] == '\\' && TempLine[strlen(TempLine) - 2] == 'n')
			{
				if (strcmp(TempLine, "v") == 0)
				{
					fscanf(ObjFile, "%f %f %f\n", &Vertices[VerticeCount ], &Vertices[VerticeCount + 1], &Vertices[VerticeCount + 2]);
					//std::cout << Vertices[ VerticeCount * 3 ] << " , " << Vertices[ VerticeCount * 3 + 1 ] << " , " << Vertices[ VerticeCount * 3 + 2 ] << std::endl;
					VerticeCount += 3;

				}
				else if (strcmp(TempLine, "vn") == 0)
				{
					fscanf(ObjFile, "%f %f %f\n", &Normals[NormalCount], &Normals[NormalCount + 1], &Normals[NormalCount + 2]);
					//std::cout << Normals[ NormalCount * 3 ] << " , " << Normals[ NormalCount * 3 + 1 ] << " , " << Normals[ NormalCount * 3 + 2 ] << std::endl;
					NormalCount += 3;
				}
				else if (strcmp(TempLine, "f") == 0)
				{
					fscanf(ObjFile, "%s", TempLine);
					//char *DoubleSlashPosition , *SlashPosition;
					char *DoubleSlashPosition = strstr(TempLine, "//");
					char *SlashPosition = strstr(TempLine, "/");
					char* FacesSegment;
					if (DoubleSlashPosition != NULL)
					{
						FacesSegment = strtok(TempLine, "//");
						Faces[FaceCount] = atoi(FacesSegment);
						fscanf(ObjFile, "%u//%u %u//%u\n", &Faces[FaceCount + 1], &Faces[FaceCount + 1], &Faces[FaceCount + 2], &Faces[FaceCount + 2]);
						FaceCount += 3;
					}
					else if (SlashPosition != NULL)
					{
						FacesSegment = strtok(TempLine, "/");
						Faces[FaceCount] = atoi(FacesSegment);
						int VertexTexture;
						fscanf(ObjFile, "%u/%u/%u %u/%u/%u\n", &Faces[FaceCount + 1], &VertexTexture, &Faces[FaceCount + 1], &Faces[FaceCount + 2], &VertexTexture, &Faces[FaceCount + 2]);
						FaceCount += 3;
					}
					else
					{
						fscanf(ObjFile, "%u %u %u\n", &Faces[FaceCount], &Faces[FaceCount + 1], &Faces[FaceCount + 2]);
						FaceCount += 3;
					}
				}
			}
		}
		if (strcmp(TempLine, "v") == 0)
		{
			fscanf(ObjFile, "%f %f %f\n", &Vertices[VerticeCount], &Vertices[VerticeCount + 1], &Vertices[VerticeCount + 2]);
			//std::cout << Vertices[ VerticeCount] << " , " << Vertices[ VerticeCount+ 1 ] << " , " << Vertices[ VerticeCount+ 2 ] << std::endl;
			VerticeCount += 3;

		}
		else if (strcmp(TempLine, "vn") == 0)
		{
			fscanf(ObjFile, "%f %f %f\n", &Normals[NormalCount], &Normals[NormalCount + 1], &Normals[NormalCount + 2]);
			//std::cout << Normals[ NormalCount * 3 ] << " , " << Normals[ NormalCount * 3 + 1 ] << " , " << Normals[ NormalCount * 3 + 2 ] << std::endl;
			NormalCount += 3;
			//std::cout << Normals[ NormalCount * 3 ] << "," << Normals[ NormalCount * 3 + 1] << "," << Normals[ NormalCount * 3 + 2 ] << std::endl;

		}
		else if (strcmp(TempLine, "f") == 0)
		{
			fscanf(ObjFile, "%s", TempLine);
			//char *DoubleSlashPosition , *SlashPosition;
			char *DoubleSlashPosition = strstr(TempLine, "//");
			char *SlashPosition = strstr(TempLine, "/");
			char* FacesSegment;
			int VertexTexture , FaceNormal;
			if (DoubleSlashPosition != NULL)
			{
				FacesSegment = strtok(TempLine, "//");
				Faces[FaceCount] = atoi(FacesSegment);
				fscanf(ObjFile, "%u//%u %u//%u\n", &Faces[FaceCount + 1], &FaceNormal, &Faces[FaceCount + 2], &FaceNormal);
				FaceCount += 3;
			}
			else if (SlashPosition != NULL)
			{
				FacesSegment = strtok(TempLine, "/");
				Faces[FaceCount] = atoi(FacesSegment);
				fscanf(ObjFile, "%u/%u/%u %u/%u/%u\n", &Faces[FaceCount + 1], &VertexTexture, &FaceNormal, &Faces[FaceCount + 2], &VertexTexture,&FaceNormal);
				FaceCount += 3;
			}
			else
			{
				Faces[FaceCount] = atoi(TempLine);
				fscanf(ObjFile, "%u %u\n", &Faces[FaceCount + 1], &Faces[FaceCount + 2]);
				FaceCount += 3;
			}
			//std::cout << Faces[ (FaceCount-1) * 3  ] << "," << Faces[ (FaceCount-1) * 3 + 1 ] << "," << Faces[ (FaceCount-1) * 3 + 2 ] << std::endl;
		}
	}


	for (int i = 0; i < FaceCount; i++)
	{
		Faces[i]--;
	}

	fclose(ObjFile);
}
