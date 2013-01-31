/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXTextureCube.h"
#include "DXRenderInfo.h"
#include "../RenderInfo.h"
#include "../../Common.h"

#include <D3DX11tex.h>

CDXTextureCube::CDXTextureCube(void) : m_texture(0)
{

}

CDXTextureCube::~CDXTextureCube(void)
{
	ReleaseCOM(m_texture);
}

void CDXTextureCube::load(std::wstring& filename)
{
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	ID3D11Texture2D* cubeTexture = 0;
	HR(D3DX11CreateTextureFromFile(static_cast<SDXRenderInfo*>(g_RenderInfo)->device, filename.c_str(),	&loadInfo, 0, (ID3D11Resource**)&cubeTexture, 0));

	D3D11_TEXTURE2D_DESC cubeDesc;
	cubeTexture->GetDesc(&cubeDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = cubeDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MipLevels = cubeDesc.MipLevels;
	viewDesc.TextureCube.MostDetailedMip = 0;

	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateShaderResourceView(cubeTexture, &viewDesc, &m_texture));
}

ID3D11ShaderResourceView* CDXTextureCube::get(void) const
{
	return m_texture;
}