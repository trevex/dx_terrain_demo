/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXBasicMaterial.h"
#include "DXShaderHelper.h"
#include "../RenderInfo.h"
#include "DXRenderInfo.h"
#include "DXGeometryInfo.h"
#include "../Camera.h"

#include <glm\gtc\type_ptr.hpp>

CDXBasicMaterial::CDXBasicMaterial(CDXTexture* texture) : m_Texture(texture), m_InputLayout(0)
{
	std::wstring filename = L"../Shader/Geometry.fxo";
	LoadPrecompiledShader(filename, m_FX);

	m_Tech = m_FX->GetTechniqueByName("TexturedGeometry");
	m_fxWorldViewProj = m_FX->GetVariableByName("WorldViewProjMatrix")->AsMatrix();
	m_fxWorld = m_FX->GetVariableByName("WorldMatrix")->AsMatrix();
	m_fxSpecularAlbedoAndPower = m_FX->GetVariableByName("SpecularAlbedoAndPower")->AsVector();
	m_fxDiffuseMap = m_FX->GetVariableByName("DiffuseMap")->AsShaderResource();

	D3D11_INPUT_ELEMENT_DESC basicInput[] = 
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}	
	};


    D3DX11_PASS_DESC passDesc;
    m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateInputLayout(basicInput, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_InputLayout));

	m_Tech->GetDesc(&m_TechDesc);
}

CDXBasicMaterial::~CDXBasicMaterial(void)
{
	ReleaseCOM(m_FX);
	ReleaseCOM(m_InputLayout);
}

void CDXBasicMaterial::prepare(void)
{
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetInputLayout(m_InputLayout);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CDXBasicMaterial::render(const void* geom_info, const SMeshInfo* mesh_info)
{	
	m_fxWorldViewProj->SetMatrix(Camera->getWorldViewProj(mesh_info->world));
	m_fxWorld->SetMatrix(glm::value_ptr(glm::transpose(mesh_info->world)));
	m_fxSpecularAlbedoAndPower->SetFloatVector(glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 32.0f)));
	m_fxDiffuseMap->SetResource(m_Texture->get());

	for(UINT p = 0; p < m_TechDesc.Passes; ++p)
	{
		m_Tech->GetPassByIndex(p)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
		static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(static_cast<const SDXGeometryInfo*>(geom_info)->index_count, static_cast<const SDXGeometryInfo*>(geom_info)->index_start, 0);
	}
}	

bool CDXBasicMaterial::isTextured(void)
{
	return true;
}