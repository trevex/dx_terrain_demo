/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef GEOMETRYDATA_H_
#define GEOMETRYDATA_H_

#include <vector>
#include <glm\glm.hpp>

class CGeometryData
{
public:
	CGeometryData(void);
	~CGeometryData(void);

	typedef	std::vector<glm::vec3> positionVector;
	typedef	std::vector<glm::vec3> normalVector;
	typedef	std::vector<unsigned int> indexVector;
	typedef std::vector<glm::vec2> uvVector;

	positionVector* getPositionVector(void);
	normalVector* getNormalVector(void);
	indexVector* getIndexVector(void);
	uvVector* getUVVector(void);

	int removeDuplicates(void);
protected:
private:
	positionVector m_positions;
	normalVector m_normals;
	indexVector m_indices;
	uvVector m_uvs;
};

#endif