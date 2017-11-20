#include "Model.h"



Model::Model()
{

}

Model::Model( int vSize , int nSize , int fSize )
{
	vertices = new GLfloat*();
	normals = new GLfloat*();
	indices = new GLuint*();
	verticeSize = vSize;
	normalSize = nSize;
	indiceSize = fSize;
	*vertices = new GLfloat[ vSize ];
	*normals = new GLfloat[ nSize ];
	*indices = new GLuint[ fSize ];
}

Model::Model( char* path )
{
	vertices = new GLfloat*();
	normals = new GLfloat*();
	indices = new GLuint*();

	ObjImporter objImporter;
	objImporter.GetSize( path , &verticeSize , &normalSize , &indiceSize );
	*vertices = new GLfloat[ verticeSize ];
	*normals = new GLfloat[ normalSize ];
	*indices = new GLuint[ indiceSize ];
	objImporter.Import( path , *vertices , &verticeSize , *normals , &normalSize , *indices , &indiceSize );
	//std::cout << "model時法向量數是" << normalSize << std::endl;

}

ModelObject::ModelObject(Model* m, GLfloat* sca , GLfloat* tra, GLfloat* rot , GLuint verOffs , GLuint indOffs )
{
	//std::cout << tra[0] << "," << tra[1] << "," << tra[2] << std::endl;
	vertices = new GLfloat*();
	normals = new GLfloat*();
	indices = new GLuint*();

	model = m;
	*vertices = *model->vertices;
	*normals = *model->normals;
	*indices = *model->indices;
	verticeSize = model->verticeSize;
	normalSize = model->normalSize;
	indiceSize = model->indiceSize;
	
	scale = new GLfloat[3];
	memcpy( scale , sca , sizeof(GLfloat) * 3 );

	translate = new GLfloat[3];
	memcpy( translate , tra , sizeof(GLfloat) * 3 );
	//std::cout << translate[0] << "," << translate[1] << "," << translate[2] << std::endl;
	rotate = new GLfloat[3];
	memcpy( rotate , rot , sizeof(GLfloat) * 3 );

	scaleMatrix = new GLfloat[16];MathHelper::makeIdentity(scaleMatrix);
	MathHelper::makeScale( scaleMatrix, scale[0] , scale[1] , scale[2] );

	translateMatrix = new GLfloat[16];MathHelper::makeIdentity(translateMatrix);
	MathHelper::makeTranslate( translateMatrix, translate[0] , translate[1] , translate[2] );

	rotateThetaMatrix = new GLfloat[16];MathHelper::makeIdentity(rotateThetaMatrix);
	rotatePhiMatrix = new GLfloat[16];MathHelper::makeIdentity(rotatePhiMatrix);
	rotateYawMatrix = new GLfloat[16];MathHelper::makeIdentity(rotateYawMatrix);
	MathHelper::makeRotateY( rotateThetaMatrix , rotate[0] );	
	MathHelper::makeRotateZ( rotatePhiMatrix , rotate[1] );	
	MathHelper::makeRotateY( rotateYawMatrix , rotate[2] );	
	vertexOffset = verOffs;
	indexOffset = indOffs;

	//socket用，拉伸socket
	elongX = 0;
	elongY = 0;
	elongZ = 0;
}

GLuint ModelObject::GetNextVertexOffset()
{
	return vertexOffset + verticeSize;
}
GLuint ModelObject::GetNextIndexOffset()
{
	return indexOffset + indiceSize;
}

