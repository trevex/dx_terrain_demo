/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXQuadGeometry.h"
#include "DXRenderInfo.h"
#include "../RenderInfo.h"
#include "../../Common.h"

#include <glm\glm.hpp>

CDXQuadGeometry* CDXQuadGeometry::m_Instance = 0;

struct VertexType
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

CDXQuadGeometry::CDXQuadGeometry(void) : m_Info(new SDXGeometryInfo())
{
	float w2 = 0.5f*40.0f;
	float d2 = 0.5f*40.0f;

	VertexType vertices[] =
    {
		{ glm::vec3(-w2, 0.0f, -d2), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-w2, 0.0f, +d2), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(+w2, 0.0f, +d2), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(+w2, 0.0f, -d2), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 1.0f) }									
    };																		
    
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(VertexType) * 24;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateBuffer(&vbd, &vinitData, &m_VertexBuffer));


	UINT indices[] = { 0, 1, 2, 0, 2, 3 };

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

	m_Info->index_count = 6;
	m_Info->index_start = 0;
}

CDXQuadGeometry::~CDXQuadGeometry(void)
{
	ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
}

CDXQuadGeometry* CDXQuadGeometry::getInstance(void)
{
	if (!m_Instance)
	{
		m_Instance = new CDXQuadGeometry();
	}
	return m_Instance;
}

void CDXQuadGeometry::prepare(void)
{
	const UINT stride = sizeof(VertexType);
	const UINT offset = 0;
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void* CDXQuadGeometry::getInfo()
{
	return (void*)m_Info;
}	

void CDXQuadGeometry::checkUV(bool need)
{
	assert(need == true);
}