/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef APP_H_
#define APP_H_

#include "Common.h"

class CApp
{
public:
	CApp(void);
	~CApp(void);

	void run(void);
protected:
private:
	CRenderWindow::Ref m_pWindow;
	CRenderer::Ref m_pRenderer;
};

#endif