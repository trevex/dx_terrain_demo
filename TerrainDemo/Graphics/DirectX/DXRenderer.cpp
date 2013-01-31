/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include <assert.h>
#include <glm\glm.hpp>

#include "DXRenderer.h"
#include "DXRenderInfo.h"
#include "../RenderInfo.h"
#include "../../Common.h"
#include "../Camera.h"
#include "DXLightingShader.h"
#include "DXPostProcessShader.h"


CDXRenderer::CDXRenderer(void) : m_d3dDriverType(D3D_DRIVER_TYPE_HARDWARE), m_d3dDevice(0), m_d3dImmediateContext(0),
	m_SwapChain(0),	m_DepthStencilBuffer(0), m_RenderTargetView(0),	m_DepthStencilView(0), m_uiMsaaQuality(0), m_enableMsaa(0),
	m_NormalBuffer(0), m_DiffuseBuffer(0), m_SpecularBuffer(0), m_PositionBuffer(0), m_NormalView(0), m_DiffuseView(0), m_SpecularView(0), m_PositionView(0),
	m_GeometryDepthStencilState(0), m_LightingDepthStencilState(0), m_RasterState(0), m_BackTargetTexture(0), m_BackTargetView(0), m_BackResourceView(0)
{
	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
	m_gBuffer[0] = 0;
	m_gBuffer[1] = 0;
	m_gBuffer[2] = 0;
	m_gBuffer[3] = 0;
}

CDXRenderer::~CDXRenderer(void)
{
	g_RenderInfo = 0;
	ReleaseCOM(m_BackResourceView);
	ReleaseCOM(m_BackTargetView);
	ReleaseCOM(m_BackTargetTexture);
	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_GeometryDepthStencilState);
	ReleaseCOM(m_LightingDepthStencilState);
	ReleaseCOM(m_NormalView);
	ReleaseCOM(m_DiffuseView);
	ReleaseCOM(m_SpecularView);
	ReleaseCOM(m_PositionView);
	ReleaseCOM(m_gBuffer[0]);
	ReleaseCOM(m_gBuffer[1]);
	ReleaseCOM(m_gBuffer[2]);
	ReleaseCOM(m_gBuffer[3]);
	ReleaseCOM(m_SwapChain);
	ReleaseCOM(m_DepthStencilBuffer);
	ReleaseCOM(m_NormalBuffer);
	ReleaseCOM(m_DiffuseBuffer);
	ReleaseCOM(m_SpecularBuffer);
	ReleaseCOM(m_PositionBuffer);
	ReleaseCOM(m_RasterState);
	if(m_d3dImmediateContext) m_d3dImmediateContext->ClearState();
	ReleaseCOM(m_d3dImmediateContext);
	ReleaseCOM(m_d3dDevice);
}

bool CDXRenderer::init(void* options)
{ /* this basic initialization follows frank lunas initDirect3D method! */
	SDXInfo* info = (SDXInfo*)options;
	m_enableMsaa = info->enableMsaa;

	UINT createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(
			0,                 // default adapter
			m_d3dDriverType,
			0,                 // no software device
			createDeviceFlags, 
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			&featureLevel,
			&m_d3dImmediateContext);

	if(FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if(featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	HR(m_d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_uiMsaaQuality));
	assert(m_uiMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = info->width;
	sd.BufferDesc.Height = info->height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if(info->enableMsaa)
	{
		sd.SampleDesc.Count   = 4;
		sd.SampleDesc.Quality = m_uiMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = info->hWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	IDXGIDevice* dxgiDevice = 0;
	HR(m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
	      
	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_d3dDevice, &sd, &m_SwapChain));
	
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	SDXRenderInfo* renderinfo = new SDXRenderInfo();
	renderinfo->context = m_d3dImmediateContext;
	renderinfo->device = m_d3dDevice;
	g_RenderInfo = (void*)renderinfo;
	

	resize(info->width, info->height);

	delete info;

	D3D11_RASTERIZER_DESC rasterState;
	rasterState.FillMode = D3D11_FILL_SOLID;
	rasterState.CullMode = D3D11_CULL_BACK;
	rasterState.FrontCounterClockwise = false;
	rasterState.DepthBias = 0;
	rasterState.SlopeScaledDepthBias = 0.0f;
	rasterState.DepthBiasClamp = 0.0f;
	rasterState.DepthClipEnable	= true;
	rasterState.ScissorEnable = false;
	rasterState.MultisampleEnable = false;
	rasterState.AntialiasedLineEnable = false;

	HR(m_d3dDevice->CreateRasterizerState(&rasterState, &m_RasterState));

	D3D11_DEPTH_STENCIL_DESC GeometryDepthStencilDesc;
	GeometryDepthStencilDesc.DepthEnable = true;
	GeometryDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	GeometryDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	GeometryDepthStencilDesc.StencilEnable = false;
	GeometryDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	GeometryDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	GeometryDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	GeometryDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	GeometryDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	GeometryDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	GeometryDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	GeometryDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	GeometryDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	GeometryDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_d3dDevice->CreateDepthStencilState(&GeometryDepthStencilDesc, &m_GeometryDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC LightingDepthStencilDesc;
	LightingDepthStencilDesc.DepthEnable = false;
	LightingDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	LightingDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	LightingDepthStencilDesc.StencilEnable = false;
	LightingDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	LightingDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	LightingDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	LightingDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	LightingDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	LightingDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	LightingDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	LightingDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	LightingDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	LightingDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_d3dDevice->CreateDepthStencilState(&LightingDepthStencilDesc, &m_LightingDepthStencilState);

	D3D11_DEPTH_STENCIL_DESC ForwardDepthStencilDesc;
	ZeroMemory(&ForwardDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	ForwardDepthStencilDesc.DepthEnable = true;
	ForwardDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ForwardDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	m_d3dDevice->CreateDepthStencilState(&ForwardDepthStencilDesc, &m_ForwardDepthStencilState);

	m_d3dImmediateContext->RSSetState(m_RasterState);
	m_d3dImmediateContext->OMSetDepthStencilState(m_GeometryDepthStencilState, 1);

	return true;
}

void CDXRenderer::resize(int width, int height)
{
	assert(m_d3dImmediateContext);
	assert(m_d3dDevice);
	assert(m_SwapChain);

	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_DepthStencilView);
	ReleaseCOM(m_DepthStencilBuffer);

	ReleaseCOM(m_NormalView);
	ReleaseCOM(m_DiffuseView);
	ReleaseCOM(m_SpecularView);
	ReleaseCOM(m_PositionView);
	ReleaseCOM(m_gBuffer[0]);
	ReleaseCOM(m_gBuffer[1]);
	ReleaseCOM(m_gBuffer[2]);
	ReleaseCOM(m_gBuffer[3]);
	ReleaseCOM(m_NormalBuffer);
	ReleaseCOM(m_DiffuseBuffer);
	ReleaseCOM(m_SpecularBuffer);
	ReleaseCOM(m_PositionBuffer);

	ReleaseCOM(m_BackResourceView);
	ReleaseCOM(m_BackTargetView);
	ReleaseCOM(m_BackTargetTexture);


	HR(m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_RenderTargetView));
	ReleaseCOM(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width     = width;
	depthStencilDesc.Height    = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if(m_enableMsaa)
	{
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = m_uiMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	HR(m_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer));
	HR(m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));
	
	D3D11_TEXTURE2D_DESC gBufferDesc;
	gBufferDesc.Width = width;
	gBufferDesc.Height = height;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.ArraySize = 1;
	gBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.SampleDesc.Quality = 0;
	gBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;
	HR(m_d3dDevice->CreateTexture2D(&gBufferDesc, 0, &m_NormalBuffer));
	HR(m_d3dDevice->CreateTexture2D(&gBufferDesc, 0, &m_DiffuseBuffer));
	HR(m_d3dDevice->CreateTexture2D(&gBufferDesc, 0, &m_SpecularBuffer));
	HR(m_d3dDevice->CreateTexture2D(&gBufferDesc, 0, &m_PositionBuffer));
	HR(m_d3dDevice->CreateRenderTargetView(m_NormalBuffer, 0, &m_gBuffer[0]));
	HR(m_d3dDevice->CreateRenderTargetView(m_DiffuseBuffer, 0, &m_gBuffer[1]));
	HR(m_d3dDevice->CreateRenderTargetView(m_SpecularBuffer, 0, &m_gBuffer[2]));
	HR(m_d3dDevice->CreateRenderTargetView(m_PositionBuffer, 0, &m_gBuffer[3]));

	D3D11_SHADER_RESOURCE_VIEW_DESC gBufferViewDesc;
	ZeroMemory(&gBufferViewDesc, sizeof(gBufferViewDesc));
	gBufferViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gBufferViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	gBufferViewDesc.Texture2D.MipLevels = 1;
	gBufferViewDesc.Texture2D.MostDetailedMip = 0;
	HR(m_d3dDevice->CreateShaderResourceView(m_NormalBuffer, &gBufferViewDesc, &m_NormalView));
	HR(m_d3dDevice->CreateShaderResourceView(m_DiffuseBuffer, &gBufferViewDesc, &m_DiffuseView));
	HR(m_d3dDevice->CreateShaderResourceView(m_SpecularBuffer, &gBufferViewDesc, &m_SpecularView));
	HR(m_d3dDevice->CreateShaderResourceView(m_PositionBuffer, &gBufferViewDesc, &m_PositionView));
	DXLightingShader->init(m_NormalView, m_DiffuseView, m_SpecularView, m_PositionView);

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width    = static_cast<float>(width);
	m_Viewport.Height   = static_cast<float>(height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_d3dImmediateContext->RSSetViewports(1, &m_Viewport);

	Camera->setAspectRatio(static_cast<float>(width)/static_cast<float>(height));

	D3D11_TEXTURE2D_DESC backTextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC backTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC backResourceViewDesc;


	ZeroMemory(&backTextureDesc, sizeof(backTextureDesc));
	backTextureDesc.Width = width;
	backTextureDesc.Height = height;
	backTextureDesc.MipLevels = 1;
	backTextureDesc.ArraySize = 1;
	backTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	backTextureDesc.SampleDesc.Count = 1;
	backTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	backTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	backTextureDesc.CPUAccessFlags = 0;
	backTextureDesc.MiscFlags = 0;

	HR(m_d3dDevice->CreateTexture2D(&backTextureDesc, NULL, &m_BackTargetTexture));
	
	backTargetViewDesc.Format = backTextureDesc.Format;
	backTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	backTargetViewDesc.Texture2D.MipSlice = 0;

	HR(m_d3dDevice->CreateRenderTargetView(m_BackTargetTexture, &backTargetViewDesc, &m_BackTargetView));

	backResourceViewDesc.Format = backTextureDesc.Format;
	backResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	backResourceViewDesc.Texture2D.MostDetailedMip = 0;
	backResourceViewDesc.Texture2D.MipLevels = 1;

	HR(m_d3dDevice->CreateShaderResourceView(m_BackTargetTexture, &backResourceViewDesc, &m_BackResourceView));

	DXPostProcessShader->init(m_BackResourceView, m_PositionView); 
}


const glm::vec4 clearColor(0.0f, 0.0f, 0.0f, 0.0f);

void CDXRenderer::prepareGeometryPass(void)
{
	m_d3dImmediateContext->RSSetState(m_RasterState);
	m_d3dImmediateContext->OMSetDepthStencilState(m_GeometryDepthStencilState, 1);
	m_d3dImmediateContext->ClearRenderTargetView(m_BackTargetView, reinterpret_cast<const float*>(&clearColor));
	m_d3dImmediateContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&clearColor));
	m_d3dImmediateContext->ClearRenderTargetView(m_gBuffer[0], reinterpret_cast<const float*>(&clearColor));
	m_d3dImmediateContext->ClearRenderTargetView(m_gBuffer[1], reinterpret_cast<const float*>(&clearColor));
	m_d3dImmediateContext->ClearRenderTargetView(m_gBuffer[2], reinterpret_cast<const float*>(&clearColor));
	m_d3dImmediateContext->ClearRenderTargetView(m_gBuffer[3], reinterpret_cast<const float*>(&clearColor));
	m_d3dImmediateContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_d3dImmediateContext->OMSetRenderTargets(4, m_gBuffer, m_DepthStencilView);
}

void CDXRenderer::prepareLightPass(void)
{
	m_d3dImmediateContext->OMSetDepthStencilState(m_LightingDepthStencilState, 1);
	m_d3dImmediateContext->OMSetRenderTargets(1, &m_BackTargetView, m_DepthStencilView);
	DXLightingShader->enable();
}

void CDXRenderer::prepareForward(void)
{
	DXLightingShader->disable();
	m_d3dImmediateContext->OMSetDepthStencilState(m_ForwardDepthStencilState, 1);
}

void CDXRenderer::present(void)
{
	m_d3dImmediateContext->OMSetDepthStencilState(m_LightingDepthStencilState, 1);
	m_d3dImmediateContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	DXPostProcessShader->render();
	HR(m_SwapChain->Present(0, 0));
}