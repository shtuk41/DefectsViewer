#include "common_header.h"

#include "win_OpenGLApp.h"
#include "GraphObjects.h"
#include "DefectViewer.h"

/*-----------------------------------------------

Name:		initScene

Params:	lpParam - Pointer to anything you want.

Result:	Initializes OpenGL features that will
			be used.

/*---------------------------------------------*/
void InitScene(LPVOID lpParam)
{
	DefectViewer::Create();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);

	glEnable(GL_DEPTH_TEST);


}

/*-----------------------------------------------

Name:	RenderScene

Params:	lpParam - Pointer to anything you want.

Result:	Renders whole scene.

/*---------------------------------------------*/

void RenderScene(LPVOID lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;

	// We just clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	float xPosition = oglControl->inputDevice.mouse.GetXPosition();
	float yPosition = oglControl->inputDevice.mouse.GetYPosition();

	int	 wheelPos = oglControl->inputDevice.mouse.GetWheelDelta();
	bool wheelActive = oglControl->inputDevice.mouse.GetWheelActive();

	if (wheelActive)
	{
		if (wheelPos > 1)
			DefectViewer::Instance()->SceneInstance()->GetStaticCamera()->ZoomOutLens();
		else if (wheelPos < -1)
			DefectViewer::Instance()->SceneInstance()->GetStaticCamera()->ZoomInLens();
	}

	bool bSKeyPressed = oglControl->inputDevice.keyboard.GetSKeyPressed();
	bool bWKeyPressed = oglControl->inputDevice.keyboard.GetWKeyPressed();

	if (bSKeyPressed)
	{
		DefectViewer::Instance()->SceneInstance()->GetStaticCamera()->ZoomOutLens();
	}
	else if (bWKeyPressed)
	{
		DefectViewer::Instance()->SceneInstance()->GetStaticCamera()->ZoomInLens();
	}

	DefectViewer::Instance()->SceneInstance()->UpdateYRot(xPosition,oglControl->inputDevice.mouse.IsLeftButtonDown());
	DefectViewer::Instance()->SceneInstance()->UpdateXRot(yPosition,oglControl->inputDevice.mouse.IsLeftButtonDown());

	DefectViewer::Instance()->DrawObjects();

	oglControl->SwapBuffersM();

	oglControl->inputDevice.mouse.SetWheelActive(false);
	oglControl->inputDevice.keyboard.SetSKeyPressed(false);
	oglControl->inputDevice.keyboard.SetWKeyPressed(false);
}

void ReloadScene(LPVOID lpParam)
{
	DefectViewer::Instance()->ReloadPointers();
}
/*-----------------------------------------------

Name:	ReleaseScene

Params:	lpParam - Pointer to anything you want.

Result:	Releases OpenGL scene.

/*---------------------------------------------*/

void ReleaseScene(LPVOID lpParam)
{

}