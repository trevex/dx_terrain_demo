/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXSkyBoxMaterial.h"
#include "DXShaderHelper.h"
#include "../RenderInfo.h"
#include "DXRenderInfo.h"
#include "DXGeometryInfo.h"
#include "../Camera.h"

#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

CDXSkyBoxMaterial::CDXSkyBoxMaterial(CDXTextureCube* texture) : m_CubeTexture(texture), m_InputLayout(0)
{
	std::wstring filename = L"../Shader/SkyBox.fxo";
	LoadPrecompiledShader(filename, m_FX);

	m_Tech = m_FX->GetTechniqueByName("SkyBox");
	m_fxWorldViewProj = m_FX->GetVariableByName("WorldViewProjMatrix")->AsMatrix();
	m_fxSkyMap = m_FX->GetVariableByName("SkyMap")->AsShaderResource();

	D3D11_INPUT_ELEMENT_DESC skyInput[] = 
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};


    D3DX11_PASS_DESC passDesc;
    m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateInputLayout(skyInput, 1, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_InputLayout));

	m_Tech->GetDesc(&m_TechDesc);
}

CDXSkyBoxMaterial::~CDXSkyBoxMaterial(void)
{
	ReleaseCOM(m_FX);
	ReleaseCOM(m_InputLayout);
}

void CDXSkyBoxMaterial::prepare(void)
{
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetInputLayout(m_InputLayout);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CDXSkyBoxMaterial::render(const void* geom_info, const SMeshInfo* mesh_info)
{	
	m_fxWorldViewProj->SetMatrix(Camera->getWorldViewProj(glm::translate(mesh_info->world, Camera->getPositionVector())));
	m_fxSkyMap->SetResource(m_CubeTexture->get());

	for(UINT p = 0; p < m_TechDesc.Passes; ++p)
	{
		m_Tech->GetPassByIndex(p)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
		static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(static_cast<const SDXGeometryInfo*>(geom_info)->index_count, static_cast<const SDXGeometryInfo*>(geom_info)->index_start, 0);
	}
}	

bool CDXSkyBoxMaterial::isTextured(void)
{
	return false;
}