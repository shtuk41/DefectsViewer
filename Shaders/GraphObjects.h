#ifndef __GRAPH_OBJECTS
#define __GRAPH_OBJECTS

#include "Angel.h"
#include <vector>
#include "utility.h"
#include "Shaders.h"
#include "OpenGLControl.h"
#include "Color.h"

using namespace std;

struct LightStruct
{
	GLuint position;	
	GLuint color;		
	GLuint direction;	
	GLuint cutoffangle;
};

class	GraphObject
{
public:

	GraphObject() { 
	tx=
	ty=
	tz=
	rx=
	ry=
	rz= 0.0;

	}

	virtual void Init(Color::Names) = 0;
	virtual void Draw() = 0;

	void SetMv(mat4 mat) {mv = mat;}
	mat4 GetMv() const {return mv;}

	void SetP(mat4 mat) {p = mat;}
	mat4 GetP() const {return p;}

	void SetTx(GLfloat val) {tx = val;}
	GLfloat GetTx() const {return tx;}

	void SetTy(GLfloat val) {ty = val;}
	GLfloat GetTy() const {return ty;}

	void SetTz(GLfloat val) {tz = val;}
	GLfloat GetTz() const {return tz;}

	void SetRx(GLfloat val) {rx = val;}
	GLfloat GetRx() const {return tx;}

	void SetRy(GLfloat val) {ry = val;}
	GLfloat GetRy() const {return ty;}

	void SetRz(GLfloat val) {rz = val;}
	GLfloat GetRz() const {return tz;}

	vec4	GetSpecularColor() const {return vec4(1.0,1.0,1.0,1.0);}
	GLfloat	GetSpecularExponent() const {return 100;}

protected:
	mat4	mv,p;

	GLfloat		tx;
	GLfloat		ty;
	GLfloat		tz;

	GLfloat		rx;
	GLfloat		ry;
	GLfloat		rz;

	GLuint vao[1];
	GLuint vbo[3];

	GLuint vPosition;
	GLuint vAmbientDiffuseColor;
	GLuint vNormal;
	GLuint vTexture;
	GLuint vTangent;
	
	GLuint vSpecularColor;
	GLuint vSpecularExponent;
	GLuint model_view;
	GLuint projection;

	CShader shVertex, shFragment;
	CShaderProgram spMain;

	LightStruct		lightSun;

	GLuint		gluiMapMode;

};

class Widget : public GraphObject
{
public:

	Widget();
	~Widget();
	void Init(Color::Names);
	void Draw();
	void SetNumberOfTriangles(unsigned int t) {numberOfTriangles = t;}
	unsigned int GetNumberOfTriangles() const {return numberOfTriangles;}
private:

 	vec4	*v4Vertices;
	vec3	*v3Normal;
	vec4	*v4AmbientDiffuseColor;
	unsigned int numberOfTriangles;
};

class Pointer : public GraphObject
{
public:

	Pointer();
	~Pointer();
	void Init(Color::Names);
	void Draw();
	void SetNumberOfTriangles(unsigned int t) {numberOfTriangles = t;}
	unsigned int GetNumberOfTriangles() const {return numberOfTriangles;}
	void InitTransform(float tX, float tY, float tZ, float rX, float rY, float rZ);

private:

	vec4	*v4Vertices;
	vec3	*v3Normal;
	vec4	*v4AmbientDiffuseColor;
	unsigned int numberOfTriangles;
	float tx;
	float ty;
	float tz;
	float rx;
	float ry;
	float rz;
};


class Background : public GraphObject
{
public:

	Background();
	~Background();
	void Init(Color::Names);
	void Draw();
private:

	GLuint		gluiTexName[1];
	GLuint		gluiTexMap[1];
	vec2	v2Texture[6];
	vec4	v4Vertices[6];
};







#endif