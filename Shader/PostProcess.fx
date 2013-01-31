/* 
   This Technique is based on the dx9 shader by Wojciech Toman, 
   but was updated and changed to work as directx11 technique 
   in this small engine by Niklas K. Voss (AbertayID 1004462)

	http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/rendering-water-as-a-post-process-effect-r2642
   Copyright (C) Wojciech Toman 2009
 */

Texture2D HeightMap		: register(t0);
Texture2D BackBufferMap	: register(t1);
Texture2D PositionMap	: register(t2);
Texture2D NormalMap		: register(t3);
Texture2D FoamMap		: register(t4);
Texture2D ReflectionMap	: register(t5);

//  needed restore position in world space
cbuffer Transforms
{
	float4x4 ViewProjMatrix;
	float4x4 ViewInverseMatrix;
};

cbuffer CameraParameter 
{ 
	float3 CameraPos; 
};

cbuffer LightParameter 
{ 
	float3 LightColor; 
	float3 LightDirection;
};

SamplerState TexureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Level at which water surface begins
float waterLevel = 0.0f;

// How fast will colours fade out. You can also think about this
// values as how clear water is. Therefore use smaller values (eg. 0.05f)
// to have crystal clear water and bigger to achieve "muddy" water.
float fadeSpeed = 0.15f;

// Normals scaling factor
float normalScale = 1.0f;

// R0 is a constant related to the index of refraction (IOR).
// It should be computed on the CPU and passed to the shader.
//float R0 =  pow(1.0-1.33, 2.0) / pow(1.0+1.33, 2.0);
float R0 = 0.1892484266;

// Maximum waves amplitude
float maxAmplitude = 1.0f;

// The smaller this value is, the more soft the transition between
// shore and water. If you want hard edges use very big value.
// Default is 1.0f.
float shoreHardness = 1.0f;

// This value modifies current fresnel term. If you want to weaken
// reflections use bigger value. If you want to empasize them use
// value smaller then 0. Default is 0.0f.
float refractionStrength = 0.0f;
//float refractionStrength = -0.3f;

// Modifies 4 sampled normals. Increase first values to have more
// smaller "waves" or last to have more bigger "waves"
float4 normalModifier = {1.0f, 2.0f, 4.0f, 8.0f};

// Strength of displacement along normal.
float displace = 1.7f;

// Describes at what depth foam starts to fade out and
// at what it is completely invisible. The fird value is at
// what height foam for waves appear (+ waterLevel).
float3 foamExistence = {0.65f, 1.35f, 0.5f};

float lightScale = 3.0f;

float4x4 reflectionMatrix =
{
	{0.5f, 0.0f, 0.0f, 0.5f},
	{0.0f, 0.5f, 0.0f, 0.5f},
	{0.0f, 0.0f, 0.0f, 0.5f},
	{0.0f, 0.0f, 0.0f, 1.0f}
};


float shininess = 0.7f;
float specularIntensity = 0.32;

// Colour of the water surface
float3 depthColour = {0.0078f, 0.5176f, 0.7f};
// Colour of the water depth
float3 bigDepthColour = {0.0039f, 0.00196f, 0.145f};
float3 extinction = {7.0f, 30.0f, 40.0f};	// Horizontal

// Water transparency along eye vector.
float visibility = 4.0f;

// Increase this value to have more smaller waves.
float2 scale = {0.005f, 0.005f};
float refractionScale = 0.005f;

// Wind force in x and z axes.
float2 wind = {-0.3f, 0.7f};

// Timer
float timer = 0.2;

// VertexShader results
struct VertexOutput
{
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
};

float3x3 compute_tangent_frame(float3 N, float3 P, float2 UV)
{
	float3 dp1 = ddx(P);
	float3 dp2 = ddy(P);
	float2 duv1 = ddx(UV);
	float2 duv2 = ddy(UV);
	
	float3x3 M = float3x3(dp1, dp2, cross(dp1, dp2));
	float2x3 inverseM = float2x3( cross( M[1], M[2] ), cross( M[2], M[0] ) );
	float3 T = mul(float2(duv1.x, duv2.x), inverseM);
	float3 B = mul(float2(duv1.y, duv2.y), inverseM);
	
	return float3x3(normalize(T), normalize(B), N);
}

// Function calculating fresnel term.
// - normal - normalized normal vector
// - eyeVec - normalized eye vector
float fresnelTerm(float3 normal, float3 eyeVec)
{
		float angle = 1.0f - saturate(dot(normal, eyeVec));
		float fresnel = angle * angle;
		fresnel = fresnel * fresnel;
		fresnel = fresnel * angle;
		return saturate(fresnel * (1.0f - saturate(R0)) + R0 - refractionStrength);
}

float4 VSMain(in float3 Position : POSITION) : SV_Position 
{ 
	return float4(Position, 1.0f); 
}

float4 PSDisabledMain(in float4 screenPos : SV_Position) : SV_Target0 
{ 
	return BackBufferMap.Load(int3(screenPos.xy, 0));
}

float4 PSMain(in float4 screenPos : SV_Position) : SV_Target0 
{ 
	int3 sampleIndices = int3(screenPos.xy, 0); 
	float3 color = BackBufferMap.Load(sampleIndices).xyz;
	
	//float3 position = mul(float4(PositionMap.Load(sampleIndices).xyz, 1.0f), ViewInverseMatrix).xyz;
	float3 position = PositionMap.Load(sampleIndices).xyz;
	float level = waterLevel;
	
	if(position.y <= level + maxAmplitude)
	{
		float3 eyeVec = position - CameraPos;
		float diff = level - position.y;
		float cameraDepth = CameraPos.y - position.y;
		
		// Find intersection with water surface
		float3 eyeVecNorm = normalize(eyeVec);
		float t = (level - CameraPos.y) / eyeVecNorm.y;
		float3 surfacePoint = CameraPos + eyeVecNorm * t;
		
				
		/*
			Sample the height-map at the point defined by S and in the direction of the vector E. The result is bias B.
			Multiply B by the maximum wave height H: B = B * H
			Set new L as: L = L + B
			Find new S on the basis of new L value
		 */
		float2 texCoord;
		for(int i = 0; i < 10; ++i)
		{
			texCoord = (surfacePoint.xz + eyeVecNorm.xz * 0.1f) * scale + timer * 0.000005f * wind;
			
			float bias = HeightMap.Load(int3(texCoord, 0)).r;
	
			bias *= 0.1f;
			level += bias * maxAmplitude;
			t = (level - CameraPos.y) / eyeVecNorm.y;
			surfacePoint = CameraPos + eyeVecNorm * t;
		}
		
		float depth = length(position - surfacePoint);
		float depth2 = surfacePoint.y - position.y;
		
		eyeVecNorm = normalize(CameraPos - surfacePoint);
		
		float normal1 = HeightMap.Load(int3(texCoord + float2(-1, 0) / 256, 0)).r;
		float normal2 = HeightMap.Load(int3(texCoord + float2(1, 0) / 256, 0)).r;
		float normal3 = HeightMap.Load(int3(texCoord + float2(0, -1) / 256, 0)).r;
		float normal4 = HeightMap.Load(int3(texCoord + float2(0, 1) / 256, 0)).r;
		
		float3 myNormal = normalize(float3((normal1 - normal2) * maxAmplitude,
										    normalScale,
										   (normal3 - normal4) * maxAmplitude));   
		
		texCoord = surfacePoint.xz * 1.6 + wind * timer * 0.00016;
		float3x3 tangentFrame = compute_tangent_frame(myNormal, eyeVecNorm, texCoord);
		float3 normal0a = normalize(mul(2.0f * NormalMap.Load(int3(texCoord, 0)).xyz - 1.0f, tangentFrame));

		texCoord = surfacePoint.xz * 0.8 + wind * timer * 0.00008;
		tangentFrame = compute_tangent_frame(myNormal, eyeVecNorm, texCoord);
		float3 normal1a = normalize(mul(2.0f * NormalMap.Load(int3(texCoord, 0)).xyz - 1.0f, tangentFrame));
		
		texCoord = surfacePoint.xz * 0.4 + wind * timer * 0.00004;
		tangentFrame = compute_tangent_frame(myNormal, eyeVecNorm, texCoord);
		float3 normal2a = normalize(mul(2.0f * NormalMap.Load(int3(texCoord, 0)).xyz - 1.0f, tangentFrame));
		
		texCoord = surfacePoint.xz * 0.1 + wind * timer * 0.00002;
		tangentFrame = compute_tangent_frame(myNormal, eyeVecNorm, texCoord);
		float3 normal3a = normalize(mul(2.0f * NormalMap.Load(int3(texCoord, 0)).xyz - 1.0f, tangentFrame));
		
		float3 normal = normalize(normal0a * normalModifier.x + normal1a * normalModifier.y +
								  normal2a * normalModifier.z + normal3a * normalModifier.w);
		
		texCoord = screenPos.xy;
		texCoord.x += sin(timer * 0.002f + 3.0f * abs(position.y)) * (refractionScale * min(depth2, 1.0f));
		float3 refraction = BackBufferMap.Load(int3(texCoord, 0)).rgb;
		if(PositionMap.Load(int3(texCoord, 0)).y > level)
			refraction = color;

		float4x4 matTextureProj = mul(ViewProjMatrix, reflectionMatrix);
				
		float3 waterPosition = surfacePoint.xyz;
		waterPosition.y -= (level - waterLevel);
		float4 texCoordProj = mul(float4(waterPosition, 1.0f), matTextureProj);
		
		float4 dPos;
		dPos.x = texCoordProj.x + displace * normal.x;
		dPos.z = texCoordProj.z + displace * normal.z;
		dPos.yw = texCoordProj.yw;
		texCoordProj = dPos;		
		
		float3 reflect = ReflectionMap.Sample(TexureSampler, texCoordProj.xy / texCoordProj.w).xyz;
		
		float fresnel = fresnelTerm(normal, eyeVecNorm);
		
		float3 depthN = depth * fadeSpeed;
		float3 waterCol = saturate(length(LightColor) / lightScale);
		refraction = lerp(lerp(refraction, depthColour * waterCol, saturate(depthN / visibility)),
						  bigDepthColour * waterCol, saturate(depth2 / extinction));

		float3 foam;		

		texCoord = (surfacePoint.xz + eyeVecNorm.xz * 0.1) * 0.05 + timer * 0.00001f * wind + sin(timer * 0.001 + position.x) * 0.005;
		float2 texCoord2 = (surfacePoint.xz + eyeVecNorm.xz * 0.1) * 0.05 + timer * 0.00002f * wind + sin(timer * 0.001 + position.z) * 0.005;
		
		if(depth2 < foamExistence.x)
			foam = (FoamMap.Load(int3(texCoord, 0)).xyz + FoamMap.Load(int3(texCoord2, 0)).xyz) * 0.5f;
		else if(depth2 < foamExistence.y)
		{
			foam = lerp((FoamMap.Load(int3(texCoord, 0)).xyz + FoamMap.Load(int3(texCoord2, 0)).xyz) * 0.5f, 0.0f,
						 (depth2 - foamExistence.x) / (foamExistence.y - foamExistence.x));
			
		}
		
		if(maxAmplitude - foamExistence.z > 0.0001f)
		{
			foam += (FoamMap.Load(int3(texCoord, 0)) + FoamMap.Load(int3(texCoord2, 0))) * 0.5f * 
				saturate((level - (waterLevel + foamExistence.z)) / (maxAmplitude - foamExistence.z));
		}


		half3 specular = 0.0f;

		half3 mirrorEye = (2.0f * dot(eyeVecNorm, normal) * normal - eyeVecNorm);
		half dotSpec = saturate(dot(mirrorEye.xyz, -LightDirection) * 0.5f + 0.5f);
		specular = (1.0f - fresnel) * saturate(-LightDirection.y) * ((pow(dotSpec, 512.0f)) * (shininess * 1.8f + 0.2f))* LightColor;
		specular += specular * 25 * saturate(shininess - 0.05f) * LightColor;		

		color = lerp(refraction, reflect, fresnel);
		color = saturate(color + max(specular, foam * LightColor));
		
		color = lerp(refraction, color, saturate(depth * shoreHardness));
	}

	return float4(color, 1.0f);
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

technique11 PostProcess
{
    pass P0
    {
		SetBlendState(DefaultBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSMain()));
    }
}

technique11 PostProcessDisabled
{
    pass P0
    {
		SetBlendState(DefaultBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
        SetVertexShader(CompileShader(vs_5_0, VSMain()));
		SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSDisabledMain()));
    }
}