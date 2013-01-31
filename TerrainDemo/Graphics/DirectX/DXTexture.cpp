/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXTexture.h"
#include "DXRenderInfo.h"
#include "../RenderInfo.h"
#include "../../Common.h"

#include <D3DX11tex.h>

CDXTexture::CDXTexture(void) : m_texture(0)
{

}

CDXTexture::~CDXTexture(void)
{
	ReleaseCOM(m_texture);
}

void CDXTexture::load(std::wstring& filename)
{
	HR(D3DX11CreateShaderResourceViewFromFile(static_cast<SDXRenderInfo*>(g_RenderInfo)->device, filename.c_str(), NULL, NULL, &m_texture, NULL));
}

ID3D11ShaderResourceView* CDXTexture::get(void) const
{
	return m_texture;
}