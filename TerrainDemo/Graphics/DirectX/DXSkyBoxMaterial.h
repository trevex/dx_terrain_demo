/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXSKYBOXMATERIAL_H_
#define DXSKYBOXMATERIAL_H_

#include <d3dx11effect.h>

#include "../MaterialBase.h"
#include "DXTextureCube.h"

class CDXSkyBoxMaterial : public TMaterial<CDXSkyBoxMaterial>
{
public:
	CDXSkyBoxMaterial(CDXTextureCube* texture);
	~CDXSkyBoxMaterial(void);

	static bool isTextured(void);

	void prepare(void);
	void render(const void* geom_info, const SMeshInfo* mesh_info);
protected:
private:
	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_Tech;
	D3DX11_TECHNIQUE_DESC m_TechDesc;
	ID3D11InputLayout* m_InputLayout;
	CDXTextureCube* m_CubeTexture;

	ID3DX11EffectMatrixVariable* m_fxWorldViewProj;
	ID3DX11EffectShaderResourceVariable* m_fxSkyMap;
};

#endif