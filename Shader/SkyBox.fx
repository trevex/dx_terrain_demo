cbuffer Transforms
{
	float4x4 WorldViewProjMatrix;
};

TextureCube SkyMap;
SamplerState TexureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};


struct VSInput
{
	float3 Position : POSITION;
};

struct VSOutput
{
	float4 PositionCS : SV_POSITION;
	float3 TexCoord   : TEXCOORD;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output;

	// xyww so that z will always be 1 (furthest away)
	output.PositionCS = mul(float4(input.Position, 1.0f), WorldViewProjMatrix).xyww;

	output.TexCoord = input.Position;

	return output;
}

float4 PSMain(VSOutput input) : SV_Target0
{
	return SkyMap.Sample(TexureSampler, input.TexCoord);
}

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

technique11 SkyBox
{
    pass P0
    {
		SetBlendState(DefaultBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMain()));
    }
}