/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 *
 * TEST GEOMETRY FOR A SIMPLE QUAD WITHOUT ADDITIONAL FUNCTIONALITY!
 */
#ifndef DXQUADGEOMETRY_H_
#define DXQUADGEOMETRY_H_

#include <D3D11.h>

#include "../GeometryBase.h"
#include "DXGeometryInfo.h"

class CDXQuadGeometry : public TGeometry<CDXQuadGeometry>
{
public:
	CDXQuadGeometry(void);
	~CDXQuadGeometry(void);

	static CDXQuadGeometry* getInstance(void);

	void prepare(void);
	void* getInfo(void);
	void checkUV(bool need);
protected:
private:
	static CDXQuadGeometry* m_Instance;

	SDXGeometryInfo* m_Info;
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
};

#endif