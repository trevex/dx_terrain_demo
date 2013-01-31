/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef MESH_H_
#define MESH_H_


#include "GeometryBase.h"
#include "MaterialBase.h"
#include "MeshInfo.h"

#include <glm/gtc/matrix_transform.hpp>

template <class G, class M>
class CMesh
{
public:
	CMesh(void) {}
	CMesh(TGeometry<G> &geometry, TMaterial<M> &material) : m_geom(geometry), m_mat(material), m_info(new SMeshInfo()), m_bMeshInfoChanged(false)
	{
		m_geom->checkUV(M::isTextured());
		m_info->world = glm::mat4(1.0f);
		m_info->position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_info->rotation = glm::mat4(1.0f);
	}
	CMesh(TGeometry<G>* geometry, TMaterial<M>* material) : m_geom(geometry), m_mat(material), m_info(new SMeshInfo()), m_bMeshInfoChanged(false)
	{
		m_geom->checkUV(M::isTextured());
		m_info->world = glm::mat4(1.0f);
		m_info->position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_info->rotation = glm::mat4(1.0f);
	}

	void translate(const glm::vec3 &v)
	{
		m_info->position += v;
		m_bMeshInfoChanged = true;
	}

	void setPosition(const glm::vec3 &v)
	{
		m_info->position = v;
		m_bMeshInfoChanged = true;
	}

	glm::vec3* getPosition(void) const
	{
		return &(m_info->position);
	}

	void rotate(const float &angle, const glm::vec3 &vector)
	{
		m_info->rotation = glm::rotate(m_info->rotation, angle, vector);
		m_bMeshInfoChanged = true;
	}

	void rotateAbs(const float &angle, const glm::vec3 &vector)
	{
		m_info->rotation = glm::rotate(glm::mat4(1.0f), angle, vector);
		m_bMeshInfoChanged = true;
	}

	void render(void)
	{
		if(m_bMeshInfoChanged)
		{
			m_info->world = glm::translate(glm::mat4(1.0f), m_info->position) * m_info->rotation;
			m_bMeshInfoChanged = false;
		}

		m_mat->prepare();
		
		m_geom->prepare();
		
		m_mat->render(m_geom->getInfo(), m_info);
	}
protected:
private:
	TGeometry<G>* m_geom;
	TMaterial<M>* m_mat;

	SMeshInfo* m_info;
	bool m_bMeshInfoChanged;
};

#endif