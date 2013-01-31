/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#include "Common.h"
#include "App.h"
#include "State/StateManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Game/MenuState.h"
#include "Base/Timer.h"


/*
 * MAIN
 */
MAINENTRY
{	
	// used to track down an internal assertion memory leak
	//_CrtSetReportMode( _CRT_ASSERT,  _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
	
	
	
	CRenderWindow::Ref pWindow;
	pWindow.reset(new CRenderWindow("DirectX and Networking Coursework - Niklas K. Voss - 1004462"));
	pWindow->init();

	CRenderer::Ref pRenderer;
	pRenderer.reset(new CRenderer());
	if (pRenderer->init(pWindow->getRenderInfo()))
	{
		StateManager::set<MenuState>();
		CTimer* timer = new CTimer();
		float delta = 0.0f;
		while (pWindow->update(delta))
		{
			timer->stop();
			delta = boost::chrono::duration<float, boost::milli>(timer->getStop() - timer->getStart()).count();
			timer->start();
			if (pWindow->isActive())
			{	
				
				if(pWindow->wasResized())
				{
					pRenderer->resize(pWindow->getWidth(), pWindow->getHeight());
					pWindow->setResizedFlag();
				}
				StateManager::update(delta);
				
				Camera->update();

				pRenderer->draw();
			}
		}
	
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
