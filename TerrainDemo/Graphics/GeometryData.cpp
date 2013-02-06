/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#include <hash_map>

#include "GeometryData.h"

CGeometryData::CGeometryData(void)
{

}

CGeometryData::~CGeometryData(void)
{

}

CGeometryData::positionVector* CGeometryData::getPositionVector(void)
{
	return &m_positions;
}

CGeometryData::normalVector* CGeometryData::getNormalVector(void)
{
	return &m_normals;
}

CGeometryData::indexVector* CGeometryData::getIndexVector(void)
{
	return &m_indices;
}

CGeometryData::uvVector* CGeometryData::getUVVector(void)
{
	return &m_uvs;
}

int CGeometryData::removeDuplicates(void)
{
	typedef std::hash_map<unsigned int, unsigned int> position_hash_map;
	typedef std::pair<unsigned int, unsigned int> mark_pair;
	typedef std::vector<mark_pair> mark_vector;

	// hash map validating if a the same position already exists and if saving the index of the position
	position_hash_map map;

	// vector of mark pairs: second is duplicate of first index in position vector
	mark_vector marked;

	// iterate over position vector; not using iterators, index is important!
	for(unsigned int i = 0; i < m_positions.size(); i++)
	{
		// temporary representation of vec3 as uint array
		const unsigned int* temp = (const unsigned int*)(&m_positions[i]);

		// hash the uint array using 3 prime number:
		// 1, (actually only coprime...)
		// 11, 
		// 17, (subtracted to bias around 0)
		// and equalize +0 and -0
		unsigned int hash = (temp[0] + temp[1]*11 - temp[2]*17) & 0x7fffffff;

		// improve randomness by mixing exponent and mantissa parts
		hash = (hash>>22) ^ (hash>>12) ^ (hash);

		// try to find hash in map
		position_hash_map::const_iterator find_it = map.find(hash);
		if (find_it == map.end()) 
		{ // if not existing
			map[hash] = i; // create it
		}
		else 
		{ // if it already exists
			marked.push_back(mark_pair(find_it->second, i)); // mark duplicate and association
		}
	}

	/* TODO: CONTINUE :D */

	return marked.size();
}