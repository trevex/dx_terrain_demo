/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXLIGHTINGSHADER_H_
#define DXLIGHTINGSHADER_H_

#include "../Light.h"
#include <d3dx11effect.h>

#define DXLightingShader CDXLightingShader::getInstance()

class CDXLightingShader
{
public:
	CDXLightingShader(void);
	~CDXLightingShader(void);

	static CDXLightingShader* getInstance(void);
	
	void init(ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* specular, ID3D11ShaderResourceView* position);

	void enable(void);
	void disable(void);

	void set(const CLight& light);
	void renderCurrent(void);
protected:
private:
 	static CDXLightingShader* m_Instance;


	LIGHT_TYPE m_currentType;

	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_DirectionalLight;
	ID3DX11EffectTechnique* m_PointLight;
	ID3DX11EffectTechnique* m_SpotLight;
	D3DX11_TECHNIQUE_DESC m_DirectionalLightDesc;
	D3DX11_TECHNIQUE_DESC m_PointLightDesc;
	D3DX11_TECHNIQUE_DESC m_SpotLightDesc;
	ID3D11InputLayout* m_InputLayout;
	
	ID3D11ShaderResourceView* m_NormalView;
	ID3D11ShaderResourceView* m_DiffuseView;
	ID3D11ShaderResourceView* m_SpecularView;
	ID3D11ShaderResourceView* m_PositionView;
	
	ID3DX11EffectVectorVariable* m_fxLightPos; 
	ID3DX11EffectVectorVariable* m_fxLightColor; 
	ID3DX11EffectVectorVariable* m_fxLightDirection; 
	ID3DX11EffectVectorVariable* m_fxSpotlightAngles; 
	ID3DX11EffectVectorVariable* m_fxLightRange;
	ID3DX11EffectVectorVariable* m_fxCameraPos;
	ID3DX11EffectShaderResourceVariable* m_fxNormalMap;
	ID3DX11EffectShaderResourceVariable* m_fxDiffuseMap;
	ID3DX11EffectShaderResourceVariable* m_fxSpecularMap;
	ID3DX11EffectShaderResourceVariable* m_fxPositionMap;

	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
};

#endif