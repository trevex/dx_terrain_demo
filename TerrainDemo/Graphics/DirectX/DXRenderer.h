/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXRENDERER_H_
#define DXRENDERER_H_

#include <d3dx11.h>

struct SDXInfo
{
	int width;
	int height;
	bool enableMsaa;
	HWND hWnd;
};

class CDXRenderer
{
public:
	CDXRenderer(void);
	~CDXRenderer(void);

	bool init(void* options);
	void resize(int width, int height);
	
	void prepareGeometryPass(void);
	void prepareLightPass(void);
	void prepareForward(void);
	void present(void);
protected:
private:
	UINT m_uiMsaaQuality;
	bool m_enableMsaa;

	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dImmediateContext;
	IDXGISwapChain* m_SwapChain;
	
	ID3D11RasterizerState* m_RasterState;

	ID3D11Texture2D* m_DepthStencilBuffer;
	
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11DepthStencilView* m_DepthStencilView;
	
	ID3D11Texture2D* m_NormalBuffer;
	ID3D11Texture2D* m_DiffuseBuffer;
	ID3D11Texture2D* m_SpecularBuffer;
	ID3D11Texture2D* m_PositionBuffer;

	ID3D11ShaderResourceView* m_NormalView;
	ID3D11ShaderResourceView* m_DiffuseView;
	ID3D11ShaderResourceView* m_SpecularView;
	ID3D11ShaderResourceView* m_PositionView;

	ID3D11RenderTargetView* m_gBuffer[4];

	D3D11_VIEWPORT m_Viewport;
	D3D_DRIVER_TYPE m_d3dDriverType;
	
	ID3D11DepthStencilState* m_GeometryDepthStencilState;
	ID3D11DepthStencilState* m_LightingDepthStencilState;
	ID3D11DepthStencilState* m_ForwardDepthStencilState;

	ID3D11Texture2D* m_BackTargetTexture;
	ID3D11RenderTargetView* m_BackTargetView;
	ID3D11ShaderResourceView* m_BackResourceView;
};

#endif