/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXLightingShader.h"
#include "DXShaderHelper.h"
#include "../RenderInfo.h"
#include "DXRenderInfo.h"
#include "../Camera.h"

#include <glm\gtc\type_ptr.hpp>

CDXLightingShader* CDXLightingShader::m_Instance = 0;


CDXLightingShader::CDXLightingShader(void) : m_FX(0), m_DirectionalLight(0),	m_PointLight(0), m_SpotLight(0), m_InputLayout(0),
m_NormalView(0), m_DiffuseView(0), m_SpecularView(0), m_PositionView(0), m_fxLightPos(0), m_fxLightColor(0), m_fxLightDirection(0), m_fxSpotlightAngles(0), m_fxLightRange(0),
m_fxCameraPos(0), m_fxNormalMap(0), m_fxDiffuseMap(0), m_fxSpecularMap(0), m_fxPositionMap(0), m_VertexBuffer(0), m_IndexBuffer(0), m_currentType(DIRECTIONALLIGHT)
{
	std::wstring filename = L"../Shader/Lighting.fxo";
	LoadPrecompiledShader(filename, m_FX);
	
	m_DirectionalLight = m_FX->GetTechniqueByName("DirectionalLight");
	m_PointLight = m_FX->GetTechniqueByName("PointLight");
	m_SpotLight = m_FX->GetTechniqueByName("SpotLight");

	m_fxLightPos = m_FX->GetVariableByName("LightPos")->AsVector(); 
	m_fxLightColor = m_FX->GetVariableByName("LightColor")->AsVector();
	m_fxLightDirection = m_FX->GetVariableByName("LightDirection")->AsVector();
	m_fxSpotlightAngles = m_FX->GetVariableByName("SpotlightAngles")->AsVector();
	m_fxLightRange = m_FX->GetVariableByName("LightRange")->AsVector();
	m_fxCameraPos = m_FX->GetVariableByName("CameraPos")->AsVector();

	m_fxNormalMap = m_FX->GetVariableByName("NormalTexture")->AsShaderResource();
	m_fxDiffuseMap = m_FX->GetVariableByName("DiffuseAlbedoTexture")->AsShaderResource();
	m_fxSpecularMap = m_FX->GetVariableByName("SpecularAlbedoTexture")->AsShaderResource();
	m_fxPositionMap = m_FX->GetVariableByName("PositionTexture")->AsShaderResource();

	D3D11_INPUT_ELEMENT_DESC lightingInput[] = 
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};


    D3DX11_PASS_DESC passDesc;
    m_DirectionalLight->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateInputLayout(lightingInput, 1, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_InputLayout));
	
	m_DirectionalLight->GetDesc(&m_DirectionalLightDesc);
	m_PointLight->GetDesc(&m_PointLightDesc);
	m_SpotLight->GetDesc(&m_SpotLightDesc);


	glm::vec3 vertices[] =
    {
		glm::vec3(-1.0f, -1.0f,  0.0f),
		glm::vec3(-1.0f,  1.0f,  0.0f),
		glm::vec3( 1.0f, -1.0f,  0.0f),
		glm::vec3( 1.0f,  1.0f,  0.0f),
    };																		
    
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(glm::vec3) * 4;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateBuffer(&vbd, &vinitData, &m_VertexBuffer));


	UINT indices[] = { 1, 3, 2, 2, 0, 1 };

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * 6;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer));
}

CDXLightingShader::~CDXLightingShader(void)
{
	ReleaseCOM(m_FX);
	ReleaseCOM(m_InputLayout);
	ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
}

CDXLightingShader* CDXLightingShader::getInstance(void)
{
	if (!m_Instance) m_Instance = new CDXLightingShader();
	return m_Instance;
}

void CDXLightingShader::init(ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* specular, ID3D11ShaderResourceView* position)
{
	m_NormalView = normal;
	m_DiffuseView = diffuse;
	m_SpecularView = specular;
	m_PositionView = position;
}
	
void CDXLightingShader::enable(void)
{
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetInputLayout(m_InputLayout);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_fxNormalMap->SetResource(m_NormalView);
	m_fxDiffuseMap->SetResource(m_DiffuseView);
	m_fxSpecularMap->SetResource(m_SpecularView);
	m_fxPositionMap->SetResource(m_PositionView);
	m_fxCameraPos->SetFloatVector(Camera->getPosition());

	const UINT stride = sizeof(glm::vec3);
	const UINT offset = 0;
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void CDXLightingShader::disable(void)
{
	m_fxNormalMap->SetResource(NULL);
	m_fxDiffuseMap->SetResource(NULL);
	m_fxSpecularMap->SetResource(NULL);
	m_fxPositionMap->SetResource(NULL);
	m_DirectionalLight->GetPassByIndex(0)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
}

void CDXLightingShader::set(const CLight& light)
{
	if (light.Type == DIRECTIONALLIGHT)
	{
		m_fxLightColor->SetFloatVector(glm::value_ptr(light.LightColor)); 
		m_fxLightDirection->SetFloatVector(glm::value_ptr(light.LightDirection));
	}
	else if (light.Type == POINTLIGHT)
	{
		m_fxLightPos->SetFloatVector(glm::value_ptr(light.LightPosition));
		m_fxLightColor->SetFloatVector(glm::value_ptr(light.LightColor));
		m_fxLightRange->SetFloatVector(glm::value_ptr(light.LightRange));
	}
	else if (light.Type == SPOTLIGHT)
	{
		m_fxLightPos->SetFloatVector(glm::value_ptr(light.LightPosition));
		m_fxLightColor->SetFloatVector(glm::value_ptr(light.LightColor)); 
		m_fxLightDirection->SetFloatVector(glm::value_ptr(light.LightDirection)); 
		m_fxSpotlightAngles->SetFloatVector(glm::value_ptr(light.SpotlightAngles));
		m_fxLightRange->SetFloatVector(glm::value_ptr(light.LightRange));
	}
	m_currentType = light.Type;
}

void CDXLightingShader::renderCurrent(void)
{
	if (m_currentType == DIRECTIONALLIGHT)
	{
		for(UINT p = 0; p < m_DirectionalLightDesc.Passes; ++p)
		{
			m_DirectionalLight->GetPassByIndex(p)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
			static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(6, 0, 0);
		}
	}
	else if (m_currentType == POINTLIGHT)
	{
		for(UINT p = 0; p < m_PointLightDesc.Passes; ++p)
		{
			m_PointLight->GetPassByIndex(p)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
			static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(6, 0, 0);
		}
	}
	else if (m_currentType == SPOTLIGHT)
	{
		for(UINT p = 0; p < m_SpotLightDesc.Passes; ++p)
		{
			m_SpotLight->GetPassByIndex(p)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
			static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(6, 0, 0);
		}
	}
}