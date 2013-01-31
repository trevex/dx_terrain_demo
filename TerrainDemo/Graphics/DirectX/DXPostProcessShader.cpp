/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */

#include "DXPostProcessShader.h"
#include "DXShaderHelper.h"
#include "../RenderInfo.h"
#include "DXRenderInfo.h"
#include "../Camera.h"

#include <glm\gtc\type_ptr.hpp>


CDXPostProcessShader* CDXPostProcessShader::m_Instance = 0;

CDXPostProcessShader::CDXPostProcessShader(void) : m_FX(0), m_PostProcessTechnique(0), m_DisabledTechnique(0), m_InputLayout(0), m_BackBufferMap(0), m_PositionMap(0), m_HeightTexture(0), 
												   m_NormalTexture(0), m_FoamTexture(0), m_ReflectionTexture(0), m_fxViewProjMatrix(0), m_fxViewInverseMatrix(0), m_fxLightColor(0),
												   m_fxLightDirection(0), m_fxCameraPos(0), m_fxHeightMap(0), m_fxBackBufferMap(0), m_fxPositionMap(0), m_fxNormalMap(0),
												   m_fxFoamMap(0), m_fxReflectionMap(0), m_VertexBuffer(0), m_IndexBuffer(0), m_bEnabled(true)
{
	m_HeightTexture	= new CDXTexture();
	m_NormalTexture	= new CDXTexture();	
	m_FoamTexture = new CDXTexture();
	m_ReflectionTexture	= new CDXTexture();
	std::wstring heightfile = L"../GameData/Textures/waterheight.dds";
	std::wstring normalfile = L"../GameData/Textures/waternormal.dds";
	std::wstring foamfile = L"../GameData/Textures/waterfoam.dds";
	std::wstring reflectionfile = L"../GameData/Textures/waterreflection.dds";
	m_HeightTexture->load(heightfile);
	m_NormalTexture->load(normalfile);
	m_FoamTexture->load(foamfile);
	m_ReflectionTexture->load(reflectionfile);


	std::wstring filename = L"../Shader/PostProcess.fxo";
	LoadPrecompiledShader(filename, m_FX);
	
	m_PostProcessTechnique= m_FX->GetTechniqueByName("PostProcess");
	m_DisabledTechnique = m_FX->GetTechniqueByName("PostProcessDisabled");


	m_fxLightColor = m_FX->GetVariableByName("LightColor")->AsVector();
	m_fxLightDirection = m_FX->GetVariableByName("LightDirection")->AsVector();
	m_fxCameraPos = m_FX->GetVariableByName("CameraPos")->AsVector();

	m_fxViewProjMatrix = m_FX->GetVariableByName("ViewProjMatrix")->AsMatrix();
	m_fxViewInverseMatrix = m_FX->GetVariableByName("ViewInverseMatrix")->AsMatrix();

	m_fxHeightMap = m_FX->GetVariableByName("HeightMap")->AsShaderResource();
	m_fxBackBufferMap = m_FX->GetVariableByName("BackBufferMap")->AsShaderResource();
	m_fxPositionMap = m_FX->GetVariableByName("PositionMap")->AsShaderResource();
	m_fxNormalMap = m_FX->GetVariableByName("NormalMap")->AsShaderResource();
	m_fxFoamMap = m_FX->GetVariableByName("FoamMap")->AsShaderResource();
	m_fxReflectionMap = m_FX->GetVariableByName("ReflectionMap")->AsShaderResource();

	D3D11_INPUT_ELEMENT_DESC postInput[] = 
	{
		{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

    D3DX11_PASS_DESC passDesc;
    m_PostProcessTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateInputLayout(postInput, 1, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_InputLayout));
	
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

	m_PostProcessTechnique->GetDesc(&m_PostProcessDesc);
}

CDXPostProcessShader* CDXPostProcessShader::getInstance(void)
{
	if (!m_Instance) m_Instance = new CDXPostProcessShader();
	return m_Instance;
}

void CDXPostProcessShader::init(ID3D11ShaderResourceView* back, ID3D11ShaderResourceView* position)
{
	m_PositionMap = position;
	m_BackBufferMap = back;
}

void CDXPostProcessShader::render(void)
{
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetInputLayout(m_InputLayout);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_fxHeightMap->SetResource(m_HeightTexture->get());		
	m_fxBackBufferMap->SetResource(m_BackBufferMap);
	m_fxPositionMap->SetResource(m_PositionMap);
	m_fxNormalMap->SetResource(m_NormalTexture->get());
	m_fxFoamMap->SetResource(m_FoamTexture->get());
	m_fxReflectionMap->SetResource(m_ReflectionTexture->get());
	m_fxCameraPos->SetFloatVector(Camera->getPosition());
	//TODO: UNHARDCODE!
	const glm::mat4 postWorld;
	m_fxViewProjMatrix->SetMatrix(Camera->getWorldViewProj(postWorld));
	const glm::vec3 lightColor(0.5f, 0.5f, 0.5f);
	m_fxLightColor->SetFloatVector(glm::value_ptr(lightColor));
	const glm::vec3 lightDirection(-1.0f, -2.0f, 1.0f);
	m_fxLightDirection->SetFloatVector(glm::value_ptr(lightDirection)); 


	const UINT stride = sizeof(glm::vec3);
	const UINT offset = 0;
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_bEnabled = false;

	if(m_bEnabled)
	{
		for(UINT p = 0; p < m_PostProcessDesc.Passes; ++p)
		{
			m_PostProcessTechnique->GetPassByIndex(p)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
			static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(6, 0, 0);
		}
	}
	else
	{
		m_DisabledTechnique->GetPassByIndex(0)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
		static_cast<SDXRenderInfo*>(g_RenderInfo)->context->DrawIndexed(6, 0, 0);
	}

	m_fxHeightMap->SetResource(NULL);		
	m_fxBackBufferMap->SetResource(NULL);
	m_fxPositionMap->SetResource(NULL);
	m_fxNormalMap->SetResource(NULL);
	m_fxFoamMap->SetResource(NULL);
	m_fxReflectionMap->SetResource(NULL);
	m_PostProcessTechnique->GetPassByIndex(0)->Apply(0, static_cast<SDXRenderInfo*>(g_RenderInfo)->context);
}