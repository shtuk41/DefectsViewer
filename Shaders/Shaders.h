#pragma once

#include <string>

/********************************

Class:	CShader

Purpose:	Wraps OpenGL shader loading
			and compiling.

********************************/

class CShader
{
public:
	bool LoadShader(std::string sFile, int a_iType);
	void DeleteShader();

	bool IsLoaded();
	unsigned int GetShaderID();

	CShader();

private:
	unsigned int uiShader; // ID of shader
	int iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool bLoaded; // Whether shader was loaded and compiled
};

/********************************

Class:	CShaderProgram

Purpose: Wraps OpenGL shader program
		and make its usage easy.

********************************/

class CShaderProgram
{
public:
	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(CShader* shShader);
	bool LinkProgram();

	void UseProgram();

	unsigned int GetProgramID() {return uiProgram;};

	CShaderProgram();

private:
	unsigned int uiProgram; // ID of program
	bool bLinked; // Whether program was linked and is ready to use
};