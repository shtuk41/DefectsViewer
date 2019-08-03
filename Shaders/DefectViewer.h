#ifndef __DEFECTVIEWER_H
#define __DEFECTVIEWER_H

#include "GraphObjects.h"
#include "Camera.h"
#include <vector>

class	Scene
{
public:
	enum	CAMERA {STATIC=0,VIEWPOINT,CHASE};


	void Init();
	//against OP, but seems like true according to Stroustrup
	void Draw(GraphObject *object, bool update);
	void SetMv(mat4 *m) { if (!m) return; else mv = *m;};
	void SetP(mat4 *p) { if (!p) return; else this->p = *p;};
	mat4 GetMv() const {return mv;}
	mat4 GetP() const {return p;}
	void SetWindowSize(GLfloat w, GLfloat h) {fWindowWidth = w; fWindowHeight = h;}
	CAMERA	GetCamera() const {return eCamera;}
	void	SetCameraStatic() {pCameraCurrent = &cCameraStatic;eCamera=STATIC;}
	void	SetCameraViewpoint() {pCameraCurrent = &cCameraViewpoint;eCamera=VIEWPOINT;}
	void	SetCameraChase() {pCameraCurrent = &cCameraChase;eCamera=CHASE;}
	Camera *	GetCurrentCamera() { return pCameraCurrent;}
	CameraStatic *GetStaticCamera() {return &cCameraStatic;}
	void UpdateYRot(float,bool);
	void UpdateXRot(float,bool);
	void SetMultiplier(float tm) { tempMultiplier = tm;}



	static Scene * SceneInstance() { if (scene) return scene; else return scene = new Scene();}
protected:


private:
	static Scene *scene;

	mat4 mv;
	mat4 p;

	CAMERA	eCamera;
	GLfloat	fFovy;

	GLfloat fWindowWidth;
	GLfloat fWindowHeight;

	CameraStatic	cCameraStatic;
	Camera			cCameraViewpoint;
	Camera			cCameraChase;

	Camera	*pCameraCurrent;

	GLfloat		glfRotAngleX;
	GLfloat		glfRotAngleY;
	float		oldYRot;
	float		oldXRot;

	float		tempMultiplier;
};

class	DefectViewer : public Scene
{
public:
	DefectViewer()   { Init();}
	static DefectViewer* Instance() { if (defectViewer) return defectViewer; else return defectViewer =  new DefectViewer();}
	static void Create() {  Instance();}
	Widget * WidgetInstance()  {return widget;}
	//Pointer *PointerInstance() { return pointer;}
	void	DrawObjects();
	void	RepositionBottomPlane(Pointer *p, float width, float height);
	void	RepositionLeftPlane(Pointer *p, float width, float height);
	void	RepositionRightPlane(Pointer *p, float width, float height);
	void	RepositionTopPlane(Pointer *p, float width, float height);
	void	ReloadPointers();

	~DefectViewer();
private:
	void	Init();

	static DefectViewer *defectViewer;

	Background *background;
	Widget *widget;
	std::vector<Pointer *> pointers;
};
#endif