#ifndef __CAMERA_H
#define __CAMERA_H

#include "Angel.h"

class	Camera
{
public:
	
	void	Init(vec4 eye, vec4 at, vec4 up, GLfloat fovy, GLfloat aspect, GLfloat fnear, GLfloat ffar)
	{
		this->eye		= eye;
		this->at		= at;
		this->up		= up;
		this->fovy		= fovy;
		this->aspect	= aspect;
		this->fnear		= fnear;
		this->ffar		= ffar;
	}

	void	ZoomInLens();
	void	ZoomOutLens();
	void	ZoomInPosition();
	void	ZoomOutPosition();
	void	Reset() {};
	mat4	GetMv() const {return LookAt(eye,at,up);}
	mat4	GetP()	const {return Perspective(fovy,aspect,fnear,ffar);}
	mat4	GetPFrustrum() const {return Frustum(-50,50,-50,50.0,1,10);}
	void	SetEye(vec4 v) {eye = v;}
	vec4	GetEye() const {return eye;}
	void	SetAt(vec4 v) {at = v;}
	vec4	GetAt() const {return at;}
	void	SetUp(vec4 v) {up = v;}
	vec4	GetUp() const {return up;}
	void	SetFovy(GLfloat f) {fovy = f;}
	GLfloat	GetFovy() const {return fovy;}
	void	SetAspect(GLfloat f) {aspect = f;}
	GLfloat GetAspect() const {return aspect;}
	void	SetNear(GLfloat f) {fnear = f;}
	GLfloat GetNEar() const {return fnear;}
	void	SetFar(GLfloat f) {ffar = f;}
	GLfloat GetFar() const {return ffar;}

private:

	vec4	eye;
	vec4	at;
	vec4	up;

	GLfloat	fovy;
	GLfloat	aspect;
	GLfloat fnear;
	GLfloat ffar;
};

class	CameraStatic : public Camera
{
public:
	enum	FOCUS {CENTER = 0, CAR};

	FOCUS	GetFocus() {return eFocus;}
	void	SetFocus(FOCUS e) {eFocus = e;}
	void	ZoomInLens() { float f = GetFovy() ; if (f > 10.0f) SetFovy(f - 0.5f);  }
	void	ZoomOutLens() { float f = GetFovy() ; if (f < 60.0f) SetFovy(f + 0.5f); }

private:
	FOCUS	eFocus;
	
};

#endif