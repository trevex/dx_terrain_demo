/*
 * (c) Niklas K. Voss (AbertayID 1004462) 2012-2013
 *
 * Deferred lighting shader partly based on the deferred lighting approach presented in 
 * Practical rendering with DirectX 11 (2011) by Jason Zink, Matt Pettineo, Jack Hoxley
 */

cbuffer Transforms
{
	float4x4 WorldMatrix;
	float4x4 WorldViewProjMatrix;
};

cbuffer MaterialProps
{
	float4 SpecularAlbedoAndPower;
};

Texture2D DiffuseMap;
Texture2D NormalMap;

SamplerState TexureSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct PSOutput // gbuffer
{ 
	float4 Normal           : SV_Target0;
	float4 DiffuseAlbedo    : SV_Target1;
	float4 SpecularAlbedo   : SV_Target2;
	float4 Position         : SV_Target3;
};

/*
 * DiffuseMap-ONLY
 */

struct VSDiffuseInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VSDiffuseOutput
{
	float4 PositionCS : SV_POSITION;
	float2 TexCoord   : TEXCOORD;
	float3 NormalWS   : NORMALWS;
	float3 PositionWS : POSITIONWS;
};

struct PSDiffuseInput
{
	float4 PositionSS : SV_POSITION;
	float2 TexCoord   : TEXCOORD;
	float3 NormalWS   : NORMALWS;
	float3 PositionWS : POSITIONWS;
};

VSDiffuseOutput VSDiffuseMain(in VSDiffuseInput input)
{
	VSDiffuseOutput output;

	// calculate normal and position in world-space
	output.PositionWS = mul(float4(input.Position, 1.0f), WorldMatrix).xyz;
	output.NormalWS = normalize(mul(input.Normal, (float3x3)WorldMatrix));
	// clip-space
	output.PositionCS = mul(float4(input.Position, 1.0f), WorldViewProjMatrix);
	// simply forward texcoord
	output.TexCoord = input.TexCoord;

	return output;
};

PSOutput PSDiffuseMain(in PSDiffuseInput input)
{
	PSOutput output;

	// sample diffuse map
	float3 diffuseAlbedo = DiffuseMap.Sample(TexureSampler, input.TexCoord).rgb;
	// normalize normal after interpolation
	float3 normalWS = normalize(input.NormalWS);

	// output gbuffer
	output.Normal = float4(normalWS, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.SpecularAlbedo = SpecularAlbedoAndPower;
	output.Position = float4(input.PositionWS, 1.0f);

	return output;
};

/*
 * WITH NORMAL MAPPING
 */

struct VSNormalInput
{
	float4 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD;
	float4 Tangent  : TANGENT;
};

struct VSNormalOutput
{
	float4 PositionCS : SV_POSITION;
	float2 TexCoord   : TEXCOORD;
	float3 NormalWS   : NORMALWS;
	float3 PositionWS : POSITIONWS;
	float3 TangentWS  : TANGENTWS;
	float3 BitangentWS: BITANGENTWS;
};

struct PSNormalInput
{
	float4 PositionSS : SV_POSITION;
	float2 TexCoord   : TEXCOORD;
	float3 NormalWS   : NORMALWS;
	float3 PositionWS : POSITIONWS;
	float3 TangentWS  : TANGENTWS;
	float3 BitangentWS: BITANGENTWS;
};

VSNormalOutput VSNormalMain(in VSNormalInput input)
{
	VSNormalOutput output;
	
	// get normal and position in world space
	output.PositionWS = mul(input.Position, WorldMatrix).xyz;
	float3 normalWS = normalize(mul(input.Normal, (float3x3)WorldMatrix));
	output.NormalWS = normalWS;

	// reconstruct tangent frame
	float3 tangentWS = normalize(mul(input.Tangent.xyz, (float3x3)WorldMatrix));
	output.BitangentWS = normalize(cross(normalWS, tangentWS)) * input.Tangent.w;
	output.TangentWS = tangentWS;

	// clip-space position
	output.PositionCS = mul(input.Position, WorldViewProjMatrix);
	
	output.TexCoord = input.TexCoord;

	return output;	
}

PSOutput PSNormalMain(in PSNormalInput input) 
{ 
	PSOutput output; 
	
	// sample diffuse map 
	float3 diffuseAlbedo = DiffuseMap.Sample(TexureSampler, input.TexCoord).rgb;
	 
	// normalize tangent frame 
	float3x3 tangentFrameWS = float3x3(normalize(input.TangentWS), 
									   normalize(input.BitangentWS),
									   normalize(input.NormalWS));

	// sample normal map 
	float3 normalTS = NormalMap.Sample(TexureSampler, input.TexCoord).rgb; 
	normalTS = normalize(normalTS * 2.0f - 1.0f); 
	// convert to world space 
	float3 normalWS = mul(normalTS, tangentFrameWS); 

	// output gbuffer
	output.Normal = float4(normalWS, 1.0f); 
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f); 
	output.SpecularAlbedo = SpecularAlbedoAndPower;
	output.Position = float4(input.PositionWS, 1.0f); 
	return output; 
}

/*
 * TECHNIQUES
 */

BlendState DefaultBlending
{
	BlendEnable[0] = FALSE;
	SrcBlend[0] = ONE;
	DestBlend[0] = ONE;
	BlendOp[0] = ADD;
	SrcBlendAlpha[0] = ZERO;
	DestBlendAlpha[0] = ZERO;
	BlendOpAlpha[0] = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

technique11 TexturedGeometry
{
    pass P0
    {
		SetBlendState(DefaultBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSDiffuseMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSDiffuseMain()));
    }
}

technique11 NormalMappedGeometry
{
	pass P0
	{
		SetBlendState(DefaultBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSNormalMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSNormalMain()));
	}
}