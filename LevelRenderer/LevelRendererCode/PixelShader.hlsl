#define MAX_SUBMESH_PER_DRAW 1024

struct OBJ_ATTRIBUTES
{
    float3 Kd; // diffuse reflectivity
    float d; // dissolve (transparency) 
    float3 Ks; // specular reflectivity
    float Ns; // specular exponent
    float3 Ka; // ambient reflectivity
    float sharpness; // local reflection map sharpness
    float3 Tf; // transmission filter
    float Ni; // optical density (index of refraction)
    float3 Ke; // emissive reflectivity
    uint illum; // illumination model
};

struct OBJ_VERT
{
    float3 pos : SV_POSITION;
    float3 uvw : DEPTH;
    float3 nrm : NORMAL;
};

struct SHADER_MODEL_DATA
{
	//globally shared model data
    
    int DirectionalLightCount, PointLightCount;
    int padding[2];
    
    float4 DirectionalLightDirection[MAX_SUBMESH_PER_DRAW];
    float4 DirectionalLightColor[MAX_SUBMESH_PER_DRAW]; //lighting info
    
    float4 PointLightPosition[MAX_SUBMESH_PER_DRAW];
    float4 PointLightColor[MAX_SUBMESH_PER_DRAW];
    float PointLightFalloffs[MAX_SUBMESH_PER_DRAW];
    
    float4 camEye, sunAmbient;
    float4x4 viewMatrix, projectionMatrix; //viewing info
    
	//per sub-mesh transform and material data
    float4x4 matricies[MAX_SUBMESH_PER_DRAW];
    OBJ_ATTRIBUTES materials[MAX_SUBMESH_PER_DRAW];
};

struct OBJ_VERT_OUT
{
    float4 pos : SV_POSITION;
    float3 uvw : DEPTH;
    float3 nrm : NORMAL;
    float4 worldPos : WORLDPOSITION;
};

StructuredBuffer<SHADER_MODEL_DATA> SceneData;

[[vk::push_constant]]
cbuffer MESH_INDEX
{
    uint MESH_ID;
    uint MATERIAL_ID;
};

// TODO: Part 2b
// TODO: Part 4g
// TODO: Part 2i
// TODO: Part 3e
// an ultra simple hlsl pixel shader
// TODO: Part 4b
float4 main(OBJ_VERT_OUT inputVertex) : SV_TARGET
{
    float4 ReturnObject = 0;
    for (int i = 0; i < SceneData[0].DirectionalLightCount; i++)
    {
        inputVertex.nrm = normalize(inputVertex.nrm);
        float3 sundirectionCopy = -normalize(SceneData[0].DirectionalLightDirection[i].xyz);
        float LightRatio = saturate(dot(sundirectionCopy, inputVertex.nrm));
	//LightRatio = saturate(LightRatio + SceneData[0].sunAmbient);
        float4 colorset = float4(SceneData[0].materials[MATERIAL_ID].Kd, 1);
        float4 ambientResult = SceneData[0].sunAmbient * colorset;
        float4 LightDirectResult = LightRatio * SceneData[0].DirectionalLightColor[i] * colorset;
	//LightDirectResult = LightDirectResult * SceneData[0].sunColor;
        float4 DiffuseResult = LightDirectResult;


        float3 VIEWDIR = normalize(SceneData[0].camEye.xyz - inputVertex.worldPos.xyz);
        float3 HALFVECTOR = normalize(sundirectionCopy + VIEWDIR);
        float INTENSITY = pow(saturate(dot(inputVertex.nrm.xyz, HALFVECTOR)), SceneData[0].materials[MATERIAL_ID].Ns);
        float4 REFLECTEDLIGHT = SceneData[0].DirectionalLightColor[i] * float4(SceneData[0].materials[MATERIAL_ID].Ks, 0) * INTENSITY;
        ReturnObject += saturate(REFLECTEDLIGHT + DiffuseResult + ambientResult);
    }
    
    for (int j = 0; j < SceneData[0].PointLightCount; j++)
    {
        float3 PointLightDirection = normalize(SceneData[0].PointLightPosition[j] - inputVertex.worldPos);
        float LightRatio = saturate(dot(PointLightDirection, inputVertex.nrm));
        float attenuation = 1 - saturate(length(SceneData[0].PointLightPosition[j] - inputVertex.worldPos) / SceneData[0].PointLightFalloffs[j]);
        ReturnObject += (LightRatio * pow(attenuation, 2) * SceneData[0].PointLightColor[j]);
    }
    
    return ReturnObject;
	// TODO: Part 1a
	// TODO: Part 3a
	// TODO: Part 4c
	// TODO: Part 4g (half-vector or reflect method your choice)
}