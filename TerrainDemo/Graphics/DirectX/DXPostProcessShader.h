/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 */
#ifndef DXPOSTPROCESSSHADER_H_
#define DXPOSTPROCESSSHADER_H_

#include <d3dx11effect.h>
#include "DXTexture.h"

#define DXPostProcessShader CDXPostProcessShader::getInstance()

class CDXPostProcessShader
{
public:
	CDXPostProcessShader(void);
	~CDXPostProcessShader(void);

	static CDXPostProcessShader* getInstance(void);
	
	void init(ID3D11ShaderResourceView* back, ID3D11ShaderResourceView* position);
	
	void render(void);
protected:
private:
 	static CDXPostProcessShader* m_Instance;

	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_PostProcessTechnique;
	ID3DX11EffectTechnique* m_DisabledTechnique;
	ID3D11InputLayout* m_InputLayout;
	D3DX11_TECHNIQUE_DESC m_PostProcessDesc;
	
	ID3D11ShaderResourceView* m_BackBufferMap;	
	ID3D11ShaderResourceView* m_PositionMap;	
	CDXTexture* m_HeightTexture;	
	CDXTexture* m_NormalTexture;		
	CDXTexture* m_FoamTexture;		
	CDXTexture* m_ReflectionTexture;	
	
	ID3DX11EffectMatrixVariable* m_fxViewProjMatrix;
	ID3DX11EffectMatrixVariable* m_fxViewInverseMatrix;
	ID3DX11EffectVectorVariable* m_fxLightColor; 
	ID3DX11EffectVectorVariable* m_fxLightDirection; 
	ID3DX11EffectVectorVariable* m_fxCameraPos;
	ID3DX11EffectShaderResourceVariable* m_fxHeightMap;		
	ID3DX11EffectShaderResourceVariable* m_fxBackBufferMap;	
	ID3DX11EffectShaderResourceVariable* m_fxPositionMap;	
	ID3DX11EffectShaderResourceVariable* m_fxNormalMap;		
	ID3DX11EffectShaderResourceVariable* m_fxFoamMap;		
	ID3DX11EffectShaderResourceVariable* m_fxReflectionMap;	

	ID3D11Buffer* m_VertexBuffer;			 
	ID3D11Buffer* m_IndexBuffer;

	bool m_bEnabled;
};

#endif