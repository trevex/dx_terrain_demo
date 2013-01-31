/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXRENDERINFO_H_
#define DXRENDERINFO_H_

#include <d3d11.h>

struct SDXRenderInfo
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
};

#endif