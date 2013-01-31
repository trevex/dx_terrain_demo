/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 *
 * TEST GEOMETRY FOR A SIMPLE CUBE WITHOUT ADDITIONAL FUNCTIONALITY!
 */
#ifndef DXCUBEGEOMETRY_H_
#define DXCUBEGEOMETRY_H_

#include <D3D11.h>

#include "../GeometryBase.h"
#include "DXGeometryInfo.h"

class CDXCubeGeometry : public TGeometry<CDXCubeGeometry>
{
public:
	CDXCubeGeometry(void);
	~CDXCubeGeometry(void);

	static CDXCubeGeometry* getInstance(void);

	void prepare(void);
	void* getInfo(void);
	void checkUV(bool need);
protected:
private:
	static CDXCubeGeometry* m_Instance;

	SDXGeometryInfo* m_Info;
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
};

#endif