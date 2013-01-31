/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef WINRENDERWINDOW_H_
#define WINRENDERWINDOW_H_

#include <Windows.h>
#include <string>
#include <memory>

class CInputHandler;
class CApp;

class CRenderWindow {
public:
	CRenderWindow(std::string title, int width = 0, int height = 0, bool cursor = true, bool fullScreen = false);
	~CRenderWindow(void);

	bool init(void);
	bool update(const float &delta);
	void* getRenderInfo(void);
	bool isActive(void);
	bool wasResized(void);
	int getWidth(void);
	int getHeight(void);
	void setResizedFlag(void);

	LRESULT CALLBACK handleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	typedef std::shared_ptr<CRenderWindow> Ref;
protected:
private:
	HINSTANCE		m_hInstance;
	std::wstring	m_wstrTitle;
	HWND			m_hWnd;

	CInputHandler* m_pInput;

	bool m_bFullScreen;
	bool m_bCursor;
	bool m_bInactive;
	bool m_bResizing;
	bool m_bResized;
	int  m_iWidth;
	int  m_iHeight;

	friend class CApp;
};

#endif