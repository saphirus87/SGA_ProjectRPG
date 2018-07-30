//
// Skinned Mesh Effect file 
// Copyright (c) 2000-2002 Microsoft Corporation. All rights reserved.
//

// Matrix Pallette
static const int MAX_MATRICES = 26;
float4x3 mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4 mView : VIEW;
float4x4 mProj : PROJECTION;

struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 BlendWeights : BLENDWEIGHT;
    float4 BlendIndices : BLENDINDICES;
    float4 Normal : NORMAL;
    float3 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 TexPos : TEXCOORD2;
};

VS_OUTPUT VShade(VS_INPUT i, uniform int NumBones)
{
    VS_OUTPUT o;
    float3 Pos = 0.0f;
    float3 Normal = 0.0f;
    float LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.BlendIndices);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4]) i.BlendWeights;
    int IndexArray[4] = (int[4]) IndexVector;
    
    // calculate the pos/normal using the "normal" weights and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones - 1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight;

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones - 1]]) * LastWeight);
    Normal += (mul(i.Normal, mWorldMatrixArray[IndexArray[NumBones - 1]]) * LastWeight);
    
    // transform position from world space into view and then projection space
    o.Position = mul(float4(Pos.xyz, 1.0f), mView);
    o.TexPos = (float3)o.Position;
    o.Position = mul(o.Position, mProj);

    // normalize normals
    o.Normal = normalize(Normal);
    // copy the input texture coordinate through
    o.TexCoord = i.TexCoord.xy;

    return o;
}

texture DiffuseMap_Tex;

sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMap_Tex);
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
    MIPFILTER = LINEAR;
    //ADDRESSU = WRAP;
    //ADDRESSV = WRAP;
};

struct PS_INPUT
{
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Pos : TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0; // Pixel color    
    float4 Normal : COLOR1;
    float4 Pos : COLOR2;
};

PS_OUTPUT ps_main(PS_INPUT Input)
{
    PS_OUTPUT OutPS;
    OutPS.RGBColor = tex2D(DiffuseSampler, Input.TexCoord);
    OutPS.Normal = float4(Input.Normal, 1.0f);
    OutPS.Pos = float4(Input.Pos, 1.0f);

    return OutPS;
}

int CurNumBones = 2;
VertexShader vsArray[4] =
{
    compile vs_2_0 VShade(1), compile vs_2_0 VShade(2), compile vs_2_0 VShade(3), compile vs_2_0 VShade(4)
};

//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
        PixelShader = compile ps_2_0 ps_main();
    }
}