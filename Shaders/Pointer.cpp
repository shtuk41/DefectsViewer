#include "GraphObjects.h"
#include "shaders.h"
#include "StlParser.h"
#include "Camera.h"
#include "utility.h"
#include "optionsreader.h"


Pointer::Pointer():numberOfTriangles(0)
{
	shVertex.LoadShader("shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("shader.frag", GL_FRAGMENT_SHADER);

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shVertex);
	spMain.AddShaderToProgram(&shFragment);

	spMain.LinkProgram();
	spMain.UseProgram();

	unsigned int numTriangles = 0;

	output_data d = StlParser::ParseToTriangleMesh(options_reader::OptionsReader::GetPointerOptions()->GetFileName().c_str(), numTriangles, 0, 0, 0);

	v4Vertices	= d.output;
	v3Normal	= d.normal;
	v4AmbientDiffuseColor = new vec4[numTriangles * 3];
	SetNumberOfTriangles(numTriangles);
}

void Pointer::Init(Color::Names e)
{
	for (int nt = 0; nt < (int)GetNumberOfTriangles() * 3; nt++)
	{
		v4AmbientDiffuseColor[nt] = Color::GetForeground(e);
	}
	
	glGenVertexArrays(1, vao); 
	glGenBuffers(3, vbo);

	// Setup whole triangle
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, GetNumberOfTriangles() * 3 * sizeof(vec4), v4Vertices, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(spMain.GetProgramID(),"vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,0,0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, GetNumberOfTriangles() * 3 * sizeof(vec4), v4AmbientDiffuseColor, GL_STATIC_DRAW);
	vAmbientDiffuseColor = glGetAttribLocation(spMain.GetProgramID(),"vColor");
	glEnableVertexAttribArray(vAmbientDiffuseColor);
	glVertexAttribPointer(vAmbientDiffuseColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, GetNumberOfTriangles() * 3 * sizeof(vec3), v3Normal, GL_STATIC_DRAW);
	vNormal = glGetAttribLocation(spMain.GetProgramID(),"vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	model_view = glGetUniformLocation(spMain.GetProgramID(),"model_view");
	projection = glGetUniformLocation(spMain.GetProgramID(),"projection");
}

void Pointer::Draw()
{
	mv =  mv * Translate(-36,-72,0) * RotateX(rx) * RotateY(ry) * RotateZ(rz) * Translate(tx,ty,tz);

	glUniformMatrix4fv(model_view,1,GL_TRUE,mv);
	glUniformMatrix4fv(projection,1,GL_TRUE,p);

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, GetNumberOfTriangles() * 3);
}

void Pointer::InitTransform(float tX = 0, float tY = 0, float tZ = 0, float rX = 0, float rY = 0, float rZ = 0)
{
	tx	=	tX;
	ty	=	tY;
	tz	=	tZ;
	rx	=	rX;
	ry	=	rY;
	rz	=	rZ;
}

Pointer::~Pointer()
{
	shVertex.DeleteShader();
	shFragment.DeleteShader();

	spMain.DeleteProgram();

	delete [] v4Vertices;			
	delete [] v4AmbientDiffuseColor;
}