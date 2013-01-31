/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef RENDERER_H_
#define RENDERER_H_

/* WINDOWS  (32bit & 64bit) */
#ifdef _WIN32 
	#ifndef WIN32_WITH_OPENGL
		#include "DirectX\DXRenderer.h"
		#define CRenderer TRenderer<CDXRenderer>
	#else
		#include "OpenGL\GLRenderer.h"
		#define CRenderer TRenderer<CGLRenderer>
	#endif
	
/* LINUX */
#elif __linux__
	#include "OpenGL\GLRenderer.h"
	#define CRenderer TRenderer<CGLRenderer>

/* MAC */
#elif __APPLE__ & __MACH__
	#include "OpenGL\GLRenderer.h"
	#define CRenderer TRenderer<CGLRenderer>

#endif

#include <memory>

#include "../State/StateManager.h"
template <class MixinRenderer>
class TRenderer : MixinRenderer
{
public:
	TRenderer(void) : MixinRenderer()
	{

	}
	~TRenderer(void) {}

	bool init(void* options)
	{
		return MixinRenderer::init(options);
	}
	void resize(int width, int height)
	{
		MixinRenderer::resize(width, height);
	}
	void draw(void)
	{
		MixinRenderer::prepareGeometryPass();
		StateManager::renderGeometry();
		MixinRenderer::prepareLightPass();
		StateManager::renderLight();
		MixinRenderer::prepareForward();
		StateManager::renderForward();
		//MixinRenderer::preparePost();
		StateManager::renderPost();
		MixinRenderer::present();
	}

	typedef std::shared_ptr<TRenderer<MixinRenderer>> Ref;
protected:
private:
};

#endif