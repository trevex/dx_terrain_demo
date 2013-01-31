/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * TODO:
 * Add capability for multiple render-windows and child-windows
 */
#include "RenderWindow.h"
#include "InputHandler.h"
#include "../State/StateManager.h"

#ifdef _WIN32
	#ifndef WIN32_WITH_OPENGL
		#include "../Graphics/DirectX/DXRenderer.h"
	#else
		#include "../Graphics/OpenGL/GLRenderer.h"
	#endif
#else
	#include "../Graphics/OpenGL/GLRenderer.h"
#endif

#include <WindowsX.h>

CRenderWindow* MainWindowHandle = NULL;
MSG gMsg;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// if window is being destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// if window is being closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// all other messages
		default:
		{
			return MainWindowHandle->handleMessage(hWnd, uMsg, wParam, lParam);
		}
	}
}

CRenderWindow::CRenderWindow(std::string title, int width, int height, bool cursor, bool fullScreen) : m_bFullScreen(fullScreen), m_bCursor(cursor), m_iWidth(width), m_iHeight(height), m_bInactive(false), m_bResized(false), m_bResizing(false)
{
	/* 
	 * convert string to LPCWSTR depending on decoding,
	 * discussion on how to do this: http://social.msdn.microsoft.com/Forums/en-US/Vsexpressvc/thread/0f749fd8-8a43-4580-b54b-fbf964d68375/ 
	 */
	#ifdef UNICODE
		int titleLength = (int)title.length()+1;
		int bufferLength = MultiByteToWideChar(CP_ACP, 0, title.c_str(), titleLength, 0, 0);
		wchar_t* buffer = new wchar_t[bufferLength];
		MultiByteToWideChar(CP_ACP, 0, title.c_str(), titleLength, buffer, bufferLength);
		std::wstring result(buffer);
		delete[] buffer;
		m_wstrTitle = result;
	#else
		m_wstrTitle = title;
	#endif

	// create input handler for this window
	m_pInput = new CInputHandler();
}

CRenderWindow::~CRenderWindow(void)
{
	// show the cursor again if it was hidden
	ShowCursor(true);

	// if fullscreen, get out of fullscreen mode
	if(m_bFullScreen) ChangeDisplaySettings(NULL, 0);

	// get rid of window
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// get rid of class instance
	UnregisterClass(m_wstrTitle.c_str(), m_hInstance);
	m_hInstance = NULL;

	// release pointer to class
	MainWindowHandle = NULL;

	// get rid of input handler
	delete m_pInput;
}

bool CRenderWindow::init(void)
{
	// init main window handle for message loop
	MainWindowHandle = this;

	// get instance of this application
	m_hInstance = GetModuleHandle(NULL);

	// setup window class
	WNDCLASSEX wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_wstrTitle.c_str();
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// register window class
	RegisterClassEx(&wc);

	// display settings
	DEVMODE dmDisplaySettings;

	if(m_bFullScreen)
	{ // if fullscreen, do
		
		// if width and height weren't set, set them to current desktop resolution
		if (!m_iWidth) m_iWidth = GetSystemMetrics(SM_CXSCREEN);
		if (!m_iHeight) m_iHeight = GetSystemMetrics(SM_CYSCREEN);
		memset(&dmDisplaySettings, 0, sizeof(dmDisplaySettings));
		dmDisplaySettings.dmSize = sizeof(dmDisplaySettings);
		dmDisplaySettings.dmPelsWidth = (unsigned long)m_iWidth;
		dmDisplaySettings.dmPelsHeight = (unsigned long)m_iHeight;
		dmDisplaySettings.dmBitsPerPel = 32;			
		dmDisplaySettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// enable fullscreen mode
		ChangeDisplaySettings(&dmDisplaySettings, CDS_FULLSCREEN);

		// create borderless window with the specified settings
		m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_wstrTitle.c_str(), m_wstrTitle.c_str(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 0, 0, m_iWidth, m_iHeight, NULL, NULL, m_hInstance, NULL);
	}
	else
	{ // if windowed, do

		// if width and height weren't set, set them to default values
		if (!m_iWidth) m_iWidth = 800;
		if (!m_iHeight) m_iHeight = 600;
		
		// create default window with the specified settings
		m_hWnd = CreateWindow(m_wstrTitle.c_str(), m_wstrTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_iWidth, m_iHeight, NULL, NULL, m_hInstance, NULL); 
	}
		
	// show window and bring it to front
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// hide mouse cursor
	ShowCursor(m_bCursor);

	// init msg memory
	ZeroMemory(&gMsg, sizeof(MSG));

	return true;
}

bool CRenderWindow::update(const float &delta)
{
	// handle messages
	if(PeekMessage(&gMsg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&gMsg);
		DispatchMessage(&gMsg);
	}
	// if message is a quit message
	if(gMsg.message == WM_QUIT)
	{
		return false;
	}
	else
	{
		bool result = StateManager::update(delta);
		m_pInput->cleanKeyMap();
		return result;
	}
}

void* CRenderWindow::getRenderInfo(void)
{
	#ifndef WIN32_WITH_OPENGL
		SDXInfo* pInfo = new SDXInfo;
		pInfo->enableMsaa = false; // currently no support in this revision for msaa
		pInfo->height = m_iHeight;
		pInfo->width = m_iWidth;
		pInfo->hWnd = m_hWnd;
		return (void*) pInfo;
	#else
		/* ... */
	#endif
}

bool CRenderWindow::isActive(void)
{
	return !m_bInactive;
}

bool CRenderWindow::wasResized(void)
{
	return m_bResized;
}

int CRenderWindow::getWidth(void)
{
	return m_iWidth;
}

int CRenderWindow::getHeight(void)
{
	return m_iHeight;
}

void CRenderWindow::setResizedFlag(void)
{
	m_bResized = false;
}

LRESULT CALLBACK CRenderWindow::handleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		static char keyCode;
		static WPARAM mouseBtn;
		case WM_KEYDOWN:
			keyCode = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
			m_pInput->processKeyDown(keyCode);
			return 0;
		case WM_KEYUP:
			keyCode = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
			m_pInput->processKeyUp(keyCode);
			return 0;
		case WM_ACTIVATE:
			if(LOWORD(wParam) == WA_INACTIVE)
			{
				m_bInactive = true;
			}
			else
			{
				m_bInactive = false;
			}
			return 0;
		case WM_SIZE:
			m_iWidth  = LOWORD(lParam);
			m_iHeight = HIWORD(lParam);
			if(wParam == SIZE_MINIMIZED)
			{
				m_bInactive = true;
			}
			else if(wParam == SIZE_MAXIMIZED)
			{
				m_bInactive = false;
				m_bResized = true;
			}
			else if(wParam == SIZE_RESTORED)
			{
				if (m_bResizing)
				{
					// wait...
				}
				else
				{
					m_bInactive = false;
					m_bResized = true;
				}
			}
			return 0;
		case WM_ENTERSIZEMOVE:
			m_bInactive = true;
			m_bResizing  = true;
			return 0;
		case WM_EXITSIZEMOVE:
			m_bInactive = false;
			m_bResizing = false;
			m_bResized = true;
			return 0;
		case WM_MENUCHAR:
		    return MAKELRESULT(0, MNC_CLOSE);
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
			return 0;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			m_pInput->processMouseDown(wParam);
			return 0;
		case WM_LBUTTONUP:
			mouseBtn = MK_LBUTTON;
			m_pInput->processMouseUp(mouseBtn);
			return 0;
		case WM_MBUTTONUP:
			mouseBtn = MK_MBUTTON;
			m_pInput->processMouseUp(mouseBtn);
			return 0;
		case WM_RBUTTONUP:
			mouseBtn = MK_RBUTTON;
			m_pInput->processMouseUp(mouseBtn);
			return 0;
		case WM_MOUSEMOVE:
			m_pInput->processMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

