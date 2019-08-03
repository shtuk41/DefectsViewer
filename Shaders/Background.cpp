#include "GraphObjects.h"
#include "shaders.h"
#include "TextureOpenIL.h"
#include "optionsreader.h"

Background::Background()
{
	shVertex.LoadShader("shader.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("shader.frag", GL_FRAGMENT_SHADER);

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shVertex);
	spMain.AddShaderToProgram(&shFragment);

	spMain.LinkProgram();
	spMain.UseProgram();
}

void Background::Init(Color::Names e)
{
	v2Texture[0] = vec2(0,1);
	v2Texture[1] = vec2(0,0);
	v2Texture[2] = vec2(1,1);
	v2Texture[3] = vec2(1,0);
	v2Texture[4] = vec2(1,1);
	v2Texture[5] = vec2(0,0);

	GLfloat legendX = 70;
	GLfloat legendY = legendX * 0.8f;

	v4Vertices[0] = vec4(legendX,legendY,0,1);
	v4Vertices[1] = vec4(legendX,0,0,1);
	v4Vertices[2] = vec4(0,legendY,0,1);
	v4Vertices[3] = vec4(0,0,0,1);
	v4Vertices[4] = vec4(0,legendY,0,1);
	v4Vertices[5] = vec4(legendX,0,0,1);

	glGenVertexArrays(1, vao); 
	glGenBuffers(2, vbo);

	// Setup whole triangle
	glBindVertexArray(vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(v4Vertices),v4Vertices,GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(spMain.GetProgramID(),"vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData( GL_ARRAY_BUFFER, sizeof(v2Texture), v2Texture, GL_STATIC_DRAW);
	vTexture = glGetAttribLocation(spMain.GetProgramID(),"vTexture");
	glEnableVertexAttribArray(vTexture);
	glVertexAttribPointer(vTexture,2,GL_FLOAT,GL_FALSE,0,0);
	
	ILuint ilTexID[1]; /* ILuint is a 32bit unsigned integer.
    //Variable texid will be used to store image name. */

	ilInit(); /* Initialization of OpenIL */
	ilGenImages(1, ilTexID); /* Generation of ONE FOR NOW image names for OpenIL image loading */
	glGenTextures(1, gluiTexName); //and we eventually want the data in an OpenGL texture

	gluiTexMap[0] = glGetUniformLocation(spMain.GetProgramID(), "textureBackground");
	glUniform1i(gluiTexMap[0], 0);//assign this one to texture unit 0

	ilBindImage(ilTexID[0]); /* Binding of IL image name */
	TextureOpenIL::loadTexFile(options_reader::OptionsReader::GetLegendsOptions()->GetFileName().c_str());
	glBindTexture(GL_TEXTURE_2D, gluiTexName[0]); //bind OpenGL texture name

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

   //Note how we depend on OpenIL to supply information about the file we just loaded in
   glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),0,
			ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

   glGenerateMipmap(GL_TEXTURE_2D);

   ilDeleteImages(1, ilTexID); //we're done with OpenIL, so free up the memory

   	model_view = glGetUniformLocation(spMain.GetProgramID(),"model_view");
	projection = glGetUniformLocation(spMain.GetProgramID(),"projection");

	gluiMapMode = glGetUniformLocation(spMain.GetProgramID(),"mapMode");
}

void Background::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gluiTexName[0]); 

	glUniform1i(gluiMapMode,0);

	glUniformMatrix4fv(model_view,1,GL_TRUE,mv);
	glUniformMatrix4fv(projection,1,GL_TRUE,p);

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES,0,6);
}

Background::~Background()
{
	shVertex.DeleteShader();
	shFragment.DeleteShader();

	spMain.DeleteProgram();
}