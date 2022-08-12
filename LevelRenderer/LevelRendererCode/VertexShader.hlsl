#define MAX_SUBMESH_PER_DRAW 1024

#pragma pack_matrix(row_major)
// TODO: 2i
// an ultra simple hlsl vertex shader
// TODO: Part 2b
// TODO: Part 4g
// TODO: Part 2i
// TODO: Part 3e
// TODO: Part 4a
// TODO: Part 1f
// TODO: Part 4b

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
    float4 pos : POSITION;
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
    //uint MATERIAL_ID;
};

OBJ_VERT_OUT main(float4 position : POSITION, float3 UV : TEXCOORD0, float3 Normal : NORMAL)
{
    OBJ_VERT_OUT vertexOutput;
    float4 pos = position;

    // TODO: Part 1h
	//vertexOutput.pos[1] -= 0.75f;
	//vertexOutput.pos[2] += 0.75f;

    pos = mul(pos, SceneData[0].matricies[MESH_ID]);
    pos = mul(pos, SceneData[0].viewMatrix);
    pos = mul(pos, SceneData[0].projectionMatrix);
    vertexOutput.nrm = mul(Normal, SceneData[0].matricies[MESH_ID]);
    vertexOutput.worldPos = mul(position, SceneData[0].matricies[MESH_ID]);

    vertexOutput.pos = pos;
    vertexOutput.uvw = UV;
    vertexOutput.nrm = normalize(vertexOutput.nrm);

    return vertexOutput;
	// TODO: Part 2i
		// TODO: Part 4e
	// TODO: Part 4b
		// TODO: Part 4e
}