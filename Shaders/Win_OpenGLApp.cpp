

#include "common_header.h"

#include "win_OpenGLApp.h"

#include "optionsreader.h"
#include "reportreader.h"

#include <msclr\marshal_cppstd.h>

COpenGLWinApp appMain;

char Keys::kp[256] = {0};

#define ID_FILE_EXIT 9001
#define ID_FILE_OPEN_REPORT 9002

/*-----------------------------------------------

Name:	Key

Params:	iKey - virtual key code

Result:	Return true if key is pressed.

/*---------------------------------------------*/

int Keys::Key(int iKey)
{
	return (GetAsyncKeyState(iKey)>>15)&1;
}

/*-----------------------------------------------

Name:	Onekey

Params:	iKey - virtual key code

Result:	Return true if key was pressed, but only
		once (the key must be released).

/*---------------------------------------------*/

int Keys::Onekey(int iKey)
{
	if(Key(iKey) && !kp[iKey]){kp[iKey] = 1; return 1;}
	if(!Key(iKey))kp[iKey] = 0;
	return 0;
}

/*-----------------------------------------------

Name:	ResetTimer

Params:	none

Result:	Resets application timer (for example
		after re-activation of application).

/*---------------------------------------------*/

void COpenGLWinApp::ResetTimer()
{
	dwLastFrame = GetTickCount();
	fFrameInterval = 0.0f;
}

/*-----------------------------------------------

Name:	UpdateTimer

Params:	none

Result:	Updates application timer.

/*---------------------------------------------*/

void COpenGLWinApp::UpdateTimer()
{
	DWORD dwCur = GetTickCount();
	fFrameInterval = float(dwCur-dwLastFrame)*0.001f;
	dwLastFrame = dwCur;
}

/*-----------------------------------------------

Name:	sof

Params:	fVal

Result:	Stands for speed optimized float.

/*---------------------------------------------*/

float COpenGLWinApp::sof(float fVal)
{
	return fVal*fFrameInterval;
}

/*-----------------------------------------------

Name:	InitializeApp

Params:	a_sAppName

Result:	Initializes app with specified (unique)
		application identifier.

/*---------------------------------------------*/

bool COpenGLWinApp::InitializeApp(string a_sAppName)
{
	sAppName = a_sAppName;
	hMutex = CreateMutex(NULL, 1, (LPCWSTR)sAppName.c_str());
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, L"This application already runs!", L"Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
	return 1;
}

/*-----------------------------------------------

Name:	RegisterAppClass

Params:	a_hInstance - application instance handle

Result:	Registers application window class.

/*---------------------------------------------*/

LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return appMain.MsgHandlerMain(hWnd, uiMsg, wParam, lParam);
}

void COpenGLWinApp::RegisterAppClass(HINSTANCE a_hInstance)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;

	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wcex.hIcon = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hInstance = hInstance;

	wcex.lpfnWndProc = GlobalMessageHandler;
	wcex.lpszClassName = (LPCWSTR)sAppName.c_str();

	wcex.lpszMenuName = NULL;

	RegisterClassEx(&wcex);
}

/*-----------------------------------------------

Name:	CreateAppWindow

Params:	sTitle - title of created window

Result:	Creates main application window.

/*---------------------------------------------*/
bool COpenGLWinApp::CreateAppWindow(LPCWSTR  sTitle)
{
	hWnd = CreateWindowEx(0, (LPCWSTR)sAppName.c_str(),  L"DEFECT VIEWER", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		options_reader::OptionsReader::GetApplicationWindowOptions()->GetX(), options_reader::OptionsReader::GetApplicationWindowOptions()->GetX(), options_reader::OptionsReader::GetApplicationWindowOptions()->GetWidth(), options_reader::OptionsReader::GetApplicationWindowOptions()->GetHeight(), NULL,
		NULL, hInstance, NULL);

	if(!oglControl.InitOpenGL(hInstance, &hWnd, 3, 1, InitScene, RenderScene, ReleaseScene, ReloadScene, &oglControl))return false;

	// Just to send WM_SIZE message again
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}

/*-----------------------------------------------

Name:	AppBody

Params:	none

Result:	Main application body infinite loop.

/*---------------------------------------------*/

void COpenGLWinApp::AppBody()
{
	MSG msg;
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)break; // If the message was WM_QUIT then exit application
			else
			{
				TranslateMessage(&msg); // Otherwise send message to appropriate window
				DispatchMessage(&msg);
			}
		}
		else if(bAppActive)
		{
			UpdateTimer();
			UpdateControls();
			oglControl.Render(&oglControl);
		}
		else 
		{
			Sleep(200); // Do not consume processor power if application isn't active	
		}
	}
}
/*-----------------------------------------------

Name:	Shutdown

Params:	none

Result:	Shuts down application and releases used
		memory.

/*---------------------------------------------*/
void COpenGLWinApp::UpdateControls()
{

}

/*-----------------------------------------------

Name:	Shutdown

Params:	none

Result:	Shuts down application and releases used
		memory.

/*---------------------------------------------*/

void COpenGLWinApp::Shutdown()
{
	oglControl.ReleaseOpenGLControl(&oglControl);

	DestroyWindow(hWnd);
	UnregisterClass((LPCWSTR)sAppName.c_str(), hInstance);
	COpenGLControl::UnregisterSimpleOpenGLClass(hInstance);
	ReleaseMutex(hMutex);
}

/*-----------------------------------------------

Name:	MsgHandlerMain

Params:	windows message stuff

Result:	Application messages handler.

/*---------------------------------------------*/
LRESULT CALLBACK COpenGLWinApp::MsgHandlerMain(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	int xPos = (int)GET_X_LPARAM(lParam);
	int yPos = (int)GET_Y_LPARAM(lParam);
	int zDelta = (int)GET_WHEEL_DELTA_WPARAM(wParam);

	oglControl.inputDevice.mouse.UpdatePosition(xPos,yPos);
	oglControl.inputDevice.mouse.SetWheelDelta(zDelta);

	switch(uiMsg)
	{
		case WM_CREATE:
			HMENU hMenu, hSubMenu;

			hMenu = CreateMenu();
			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN_REPORT, L"Open Report");
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, L"E&xit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, L"&File");
			SetMenu(hWnd, hMenu);
		break;

		case WM_PAINT:
			BeginPaint(hWnd, &ps);		
			EndPaint(hWnd, &ps);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_ACTIVATE:
		{
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					bAppActive = true;
					break;
				case WA_INACTIVE:
					bAppActive = false;
					break;
			}
			break;
		}
		case WM_SIZE:
			oglControl.ResizeOpenGLViewportFull();
			break;
		case WM_LBUTTONDOWN:
			oglControl.inputDevice.mouse.SetLeftButton(Mouse::BUTTON_DN);
			break;
		case WM_LBUTTONUP:
			oglControl.inputDevice.mouse.SetLeftButton(Mouse::BUTTON_UP);
			break;
		case WM_RBUTTONDOWN:
			oglControl.inputDevice.mouse.SetRightButton(Mouse::BUTTON_DN);
			break;
		case WM_RBUTTONUP:
			oglControl.inputDevice.mouse.SetRightButton(Mouse::BUTTON_UP);
			break;
		case WM_MOUSEWHEEL:
			oglControl.inputDevice.mouse.SetWheelActive(true);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_FILE_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				case ID_FILE_OPEN_REPORT:
					System::Windows::Forms::OpenFileDialog ^ openFileDialog1 = gcnew System::Windows::Forms::OpenFileDialog();
					openFileDialog1->Filter = "Cursor Files|*.csv";
					openFileDialog1->Title = "Select a Report File";

					// Show the Dialog.
					// If the user clicked OK in the dialog and
					if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					{
						 //pin_ptr<const wchar_t> wname = PtrToStringChars(openFileDialog1->FileName);
						 //MessageBox(NULL, wname, L"Multiple Instances Found.", MB_ICONINFORMATION | MB_OK);
						 string str = msclr::interop::marshal_as<std::string>(openFileDialog1->FileName);
						 report_reader::ReportReader::LoadFile(str);
						 oglControl.Reload(&oglControl);
					}

					
				break;
			}
			break;
		  case WM_KEYDOWN:
			  if ( wParam == 'S' )
			  {
				oglControl.inputDevice.keyboard.SetSKeyPressed(true);
			  }
			  else if ( wParam == 'W' )
			  {
				oglControl.inputDevice.keyboard.SetWKeyPressed(true);
			  }
			  break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}

/*-----------------------------------------------

Name:	GetInstance

Params:	none

Result:	Returns application instance.

/*---------------------------------------------*/

HINSTANCE COpenGLWinApp::GetInstance()
{
	return hInstance;
}

/*-----------------------------------------------

Name:	WinMain

Params:	whatever

Result:	Windows entry point for application.

/*---------------------------------------------*/
[System::STAThread] 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	if(!appMain.InitializeApp("03_opengl_3_3"))
		return 0;
	appMain.RegisterAppClass(hInstance);

	if(!appMain.CreateAppWindow(L"Application"))
		return 0;
	appMain.ResetTimer();

	appMain.AppBody();
	appMain.Shutdown();

	return 0;
}

