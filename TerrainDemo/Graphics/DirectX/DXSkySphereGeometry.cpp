/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include "DXSkySphereGeometry.h"
#include "DXRenderInfo.h"
#include "../RenderInfo.h"
#include "../../Common.h"

#include <glm\glm.hpp>
#include <algorithm>
#include <vector>

CDXSkySphereGeometry* CDXSkySphereGeometry::m_Instance = 0;

CDXSkySphereGeometry::CDXSkySphereGeometry(float radius, unsigned int subdivisions) : m_Info(new SDXGeometryInfo())
{
	// based on frank luna's geosphere algorithm but simplified for sky sphere

	subdivisions = std::min(subdivisions, 3u);

	static const float x = 0.525731f;
	static const float z = 0.850651f;
	static const glm::vec3 position_data[12] =
	{
		glm::vec3( -x, .0f,   z), glm::vec3(  x, .0f,   z),
		glm::vec3( -x, .0f,  -z), glm::vec3(  x, .0f,  -z),
		glm::vec3(.0f,   z,   x), glm::vec3(.0f,   z,  -x),
		glm::vec3(.0f,  -z,   x), glm::vec3(.0f,  -z,  -x),
		glm::vec3(  z,   x, .0f), glm::vec3( -z,   x, .0f),
		glm::vec3(  z,  -x, .0f), glm::vec3( -z,  -x, .0f)
	};
	/*static const UINT index_data[60] = 
	{ 
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};*/
	static const UINT index_data[60] = 
	{ 
		1,0,4,  4,0,9,  4,9,5,  8,4,5,  1,4,8,
		1,8,10, 10,8,3, 8,5,3,  3,5,2,  3,2,7,
		3,7,10, 10,7,6, 6,7,11, 6,11,0, 6,0,1,
		10,6,1, 11,9,0, 2,9,11, 5,9,2,  11,7,2
	};

	std::vector<glm::vec3> vertices(position_data, position_data + sizeof(position_data) / sizeof(position_data[0]));
	std::vector<UINT> indices(index_data, index_data + sizeof(index_data) / sizeof(index_data[0]));

	for (unsigned int s = 0; s < subdivisions; s++)
	{
		std::vector<glm::vec3> temp_v = vertices;
		std::vector<UINT> temp_i = indices;

		vertices.resize(0);
		indices.resize(0);

		// use frank luna's subdivision... quite inefficient memory but it is not an algorithm executed while running
		//       v1
		//       *
		//      / \
		//     /   \
		//  m0*-----*m1
		//   / \   / \
		//  /   \ /   \
		// *-----*-----*
		// v0    m2     v2

		UINT numTris = temp_i.size()/3;
		for(UINT i = 0; i < numTris; ++i)
		{
			glm::vec3 v0 = temp_v[temp_i[i*3+0]];
			glm::vec3 v1 = temp_v[temp_i[i*3+1]];
			glm::vec3 v2 = temp_v[temp_i[i*3+2]];

			glm::vec3 m0( 0.5f*(v0.x + v1.x),
						  0.5f*(v0.y + v1.y),
						  0.5f*(v0.z + v1.z)),
					  m1( 0.5f*(v1.x + v2.x),
						  0.5f*(v1.y + v2.y),
						  0.5f*(v1.z + v2.z)),
					  m2( 0.5f*(v0.x + v2.x),
						  0.5f*(v0.y + v2.y),
						  0.5f*(v0.z + v2.z));

			vertices.push_back(v0);
			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(m0);
			vertices.push_back(m1);
			vertices.push_back(m2);
 
			indices.push_back(i*6+0);
			indices.push_back(i*6+3);
			indices.push_back(i*6+5);
			indices.push_back(i*6+3);
			indices.push_back(i*6+4);
			indices.push_back(i*6+5);
			indices.push_back(i*6+5);
			indices.push_back(i*6+4);
			indices.push_back(i*6+2);
			indices.push_back(i*6+3);
			indices.push_back(i*6+1);
			indices.push_back(i*6+4);
		}
	}

	// project onto sphere and scale according to radius
	for(std::vector<glm::vec3>::size_type i = 0; i != vertices.size(); i++) 
	{
		vertices[i] = glm::normalize(vertices[i]) * radius;	
	}

	D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(glm::vec3) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
	HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateBuffer(&vbd, &vinitData, &m_VertexBuffer));

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(static_cast<SDXRenderInfo*>(g_RenderInfo)->device->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer));

	m_Info->index_count = indices.size();
	m_Info->index_start = 0;
}

CDXSkySphereGeometry::~CDXSkySphereGeometry(void)
{
	ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
}

void CDXSkySphereGeometry::prepare(void)
{
	const UINT stride = sizeof(glm::vec3);
	const UINT offset = 0;
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	static_cast<SDXRenderInfo*>(g_RenderInfo)->context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void* CDXSkySphereGeometry::getInfo()
{
	return (void*)m_Info;
}	

void CDXSkySphereGeometry::checkUV(bool need)
{
	assert(need == false);
}