/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXGEOSPHEREGEOMETRY_H_
#define DXGEOSPHEREGEOMETRY_H_

#include <D3D11.h>

#include "../GeometryBase.h"
#include "DXGeometryInfo.h"

class CDXSkySphereGeometry : public TGeometry<CDXSkySphereGeometry>
{
public:
	CDXSkySphereGeometry(float radius, unsigned int subdivisions);
	~CDXSkySphereGeometry(void);

	static CDXSkySphereGeometry* getInstance(void);

	void prepare(void);
	void* getInfo(void);
	void checkUV(bool need);
protected:
private:
	static CDXSkySphereGeometry* m_Instance;

	SDXGeometryInfo* m_Info;
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
};

#endif