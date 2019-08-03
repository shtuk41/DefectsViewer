#pragma once

#include <vcclr.h>
#include "openGLControl.h"


class COpenGLWinApp
{
public:
	HWND hWnd; // Handle to application window
	COpenGLControl oglControl; // OpenGL Control

	void ResetTimer();
	void UpdateTimer();
	void UpdateControls();
	float sof(float fVal);

	bool InitializeApp(string a_sAppName);
	void RegisterAppClass(HINSTANCE hAppInstance);
	bool CreateAppWindow(LPCWSTR sTitle);
	
	void AppBody();
	void Shutdown();

	HINSTANCE GetInstance();

	LRESULT CALLBACK MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance; // Application's instance
	string sAppName;
	HANDLE hMutex;

	bool bAppActive; // To check if application is active (not minimized)
	DWORD dwLastFrame;
	float fFrameInterval;

	gcroot<System::Windows::Controls::TextBox ^> textBoxMouseX;
	gcroot<System::Windows::Controls::Label ^> labelMouseX;
	gcroot<System::Windows::Controls::TextBox ^> textBoxMouseY;
	gcroot<System::Windows::Controls::Label ^> labelMouseY;
};

namespace Keys
{
	int Key(int iKey);
	int Onekey(int iKey);
	extern char kp[256];
}

//extern COpenGLWinApp appMain;

void InitScene(LPVOID), RenderScene(LPVOID), ReleaseScene(LPVOID), ReloadScene(LPVOID);