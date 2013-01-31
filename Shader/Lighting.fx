/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * Deferred lighting shader partly based on the deferred lighting approach presented in 
 * Practical rendering with DirectX 11 (2011) by Jason Zink, Matt Pettineo, Jack Hoxley
 */
#define DIRECTIONALLIGHT 1
#define POINTLIGHT 2
#define SPOTLIGHT 3

Texture2D NormalTexture         : register(t0); 
Texture2D DiffuseAlbedoTexture  : register(t1); 
Texture2D SpecularAlbedoTexture : register(t2); 
Texture2D PositionTexture       : register(t3); 

cbuffer LightParameter 
{ 
	float3 LightPos; 
	float3 LightColor; 
	float3 LightDirection; 
	float2 SpotlightAngles; 
	float4 LightRange; 
};

cbuffer CameraParameter 
{ 
	float3 CameraPos; 
};

void GetGBufferAttributes(in float2 screenPos, out float3 normal, out float3 position, out float3 diffuseAlbedo, out float3 specularAlbedo, out float specularPower) 
{ 
	int3 sampleIndices = int3(screenPos.xy, 0); 
	normal = NormalTexture.Load(sampleIndices).xyz; 
	position = PositionTexture.Load(sampleIndices).xyz; 
	diffuseAlbedo = DiffuseAlbedoTexture.Load(sampleIndices).xyz; 
	float4 spec = SpecularAlbedoTexture.Load(sampleIndices); 
	specularAlbedo = spec.xyz; 
	specularPower = spec.w; 
} 

float3 CalcLighting(in float3 normal, in float3 position, in float3 diffuseAlbedo, in float3 specularAlbedo, in float specularPower, uniform int gLightingMode) 
{  
	float3 L = 0; 
	float attenuation = 1.0f; 
	if (gLightingMode == POINTLIGHT || gLightingMode == SPOTLIGHT)
	{
		L = LightPos - position; 
		float dist = length(L); 
		attenuation = max(0, 1.0f - (dist / LightRange.x)); 
		L /= dist;
	}
	else if (gLightingMode == DIRECTIONALLIGHT)
	{
		L = -LightDirection; 
	}
	if (gLightingMode == SPOTLIGHT)
	{
		float3 L2 = LightDirection; 
		float rho = dot(-L, L2); 
		attenuation *= saturate((rho - SpotlightAngles.y) / (SpotlightAngles.x - SpotlightAngles.y)); 
	}
	float nDotL = saturate(dot(normal, L)); 
	float3 diffuse = nDotL * LightColor * diffuseAlbedo;
	if (gLightingMode == DIRECTIONALLIGHT)
	{
		float3 ambient = 0.18 * LightColor * diffuseAlbedo;
		diffuse = max(diffuse, ambient);
	}
	float3 V = CameraPos - position; 
	float3 H = normalize(L + V); 
	float3 specular = pow(saturate(dot(normal, H)), specularPower) * LightColor * specularAlbedo.xyz * nDotL; 
	return (diffuse + specular) * attenuation; 
} 

float4 VSMain(in float3 Position : POSITION) : SV_Position 
{ 
	return float4(Position, 1.0f); 
}


float4 PSMain(in float4 screenPos : SV_Position, uniform int gLightingMode) : SV_Target0 
{ 
	float3 normal;
	float3 position; 
	float3 diffuseAlbedo; 
	float3 specularAlbedo; 
	float specularPower; 
	GetGBufferAttributes(screenPos.xy, normal, position, diffuseAlbedo, specularAlbedo, specularPower); 
	float3 lighting = CalcLighting(normal, position, diffuseAlbedo,	specularAlbedo, specularPower, gLightingMode); 
	return float4(lighting, 1.0f); 
}

BlendState AdditiveBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend[0] = ONE;
	DestBlend[0] = ONE;
	BlendOp[0] = ADD;
	SrcBlendAlpha[0] = ZERO;
	DestBlendAlpha[0] = ZERO;
	BlendOpAlpha[0] = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

technique11 DirectionalLight
{
    pass P0
    {
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMain(DIRECTIONALLIGHT)));
    }
}

technique11 PointLight
{
    pass P0
    {
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMain(POINTLIGHT)));
    }
}

technique11 SpotLight
{
    pass P0
    {
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMain(SPOTLIGHT)));
    }
}