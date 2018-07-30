
////--------------------------------------------------------------//
//// NormalMapping 주석
////--------------------------------------------------------------//
////--------------------------------------------------------------//
//// Pass 0
////--------------------------------------------------------------//
//string NormalMapping_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.x";

//float4x4 gWorldMatrix : World;
//float4x4 gWVPMatrix : WorldViewProjection;

//// cal light value
//float4 gWorldLightPosition = float4( 500.00, 500.00, -500.00, 1.00 );
//float4 gWorldCameraPosition : ViewPosition;
//float3 gLightColor = float3(0.70, 0.70, 1.00);

//struct VS_INPUT 
//{
//    float4 mPosition : POSITION;
//    float3 mNormal : NORMAL;
//    float3 mBinormal : BINORMAL;
//    float3 mTangent : TANGENT;
//    float2 mUV: TEXCOORD0;
//};

//struct VS_OUTPUT 
//{
//    float4 mPosition : POSITION;
//    float2 mUV: TEXCOORD0;
//    float3 mLightDir : TEXCOORD1;
//    float3 mViewDir : TEXCOORD2;
//    float3 T : TEXCOORD3;
//    float3 B : TEXCOORD4;
//    float3 N : TEXCOORD5;
//};

//VS_OUTPUT NormalMapping_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
//{
//    VS_OUTPUT Output;
//    Output.mPosition = mul(Input.mPosition, gWVPMatrix);
//    Output.mUV = Input.mUV;
    
//    // v is world position
//    float4 worldPos = mul(Input.mPosition, gWorldMatrix);
    
//    // v -> light dir
//    float3 lightDir = worldPos.xyz - gWorldLightPosition.xyz;
//    Output.mLightDir = normalize(lightDir);
    
//    float3 viewDir = normalize(worldPos.xyz - gWorldCameraPosition.xyz);
//    Output.mViewDir = viewDir;
    
//    // T B N on World Space // tangent space 검색
//    Output.T = normalize(mul(Input.mTangent, (float3x3) gWorldMatrix));
//    Output.B = normalize(mul(Input.mBinormal, (float3x3) gWorldMatrix));
//    Output.N = normalize(mul(Input.mNormal, (float3x3) gWorldMatrix));
   
//    return Output;
//}

//struct PS_INPUT
//{
//    float2 mUV : TEXCOORD0;
//    float3 mLightDir : TEXCOORD1;
//    float3 mViewDir : TEXCOORD2;
//    float3 T : TEXCOORD3;
//    float3 B : TEXCOORD4;
//    float3 N : TEXCOORD5;
//};

//texture DiffuseMap_Tex;
//sampler2D DiffuseSampler = sampler_state
//{
//   Texture = (DiffuseMap_Tex);
//};

//texture SpecularMap_Tex;
//sampler2D SpecularSampler = sampler_state
//{
//   Texture = (SpecularMap_Tex);
//};

//texture NormalMap_Tex;
//sampler2D NormalSampler = sampler_state
//{
//   Texture = (NormalMap_Tex);
//};


//float4 NormalMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
//{   
//    float3 tangentNormal = tex2D(NormalSampler, Input.mUV).rgb; // equal xyz
//    tangentNormal = normalize(tangentNormal * 2 - 1); // 0~1 -> -1~1 // 울퉁불퉁
//    //tangentNormal = float3(0, 0, 1);// 매끈
    
//    // World -> TBN 공간으로 보내는 행렬 생성
//    // 열 기준의 행렬이 생성됨
//    // 기존 사용 하던 행렬은 행 기준임
//    // ------
//    // ------
//    // ------
//    float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
    
//    // 연산 순서는 이와 같은데
//    // 역행렬을 만들어 TBN -> World 로 보내는 행렬 생성
//    //TBN = transpose(TBN);
//    // 열기준 행렬 이므로 행렬 * 벡터 순으로 mul
//    //float3 worldNormal = mul(TBN, tangentNormal);
   
//    // 결과적으로는 이렇게
//    float3 worldNormal = mul(tangentNormal, TBN);
   
//    float4 albedo = tex2D(DiffuseSampler, Input.mUV);
//    float3 lightDir = normalize(Input.mLightDir);
   
//    float3 diffuse = saturate(dot(worldNormal, -lightDir));
//    float3 specular = 0;
   
//    if ( diffuse.x > 0 )
//    {
//        float3 reflection = reflect(lightDir, worldNormal);
//        float3 viewDir = normalize(Input.mViewDir);
      
//        specular = saturate(dot(reflection, -viewDir));
//        specular = pow(specular, 20.0f);
      
//        float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
//        specular *= specularIntensity.rgb * gLightColor;
//    }
//    diffuse = gLightColor * albedo.rgb * diffuse;
//    float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo.rgb;
   
//    return float4( ambient + diffuse + specular, 1);
//}

////--------------------------------------------------------------//
//// Technique Section for NormalMapping
////--------------------------------------------------------------//
//technique NormalMapping
//{
//    pass Pass_0
//    {
//        VertexShader = compile vs_2_0 NormalMapping_Pass_0_Vertex_Shader_vs_main();
//        PixelShader = compile ps_2_0 NormalMapping_Pass_0_Pixel_Shader_ps_main();
//    }

//}


//--------------------------------------------------------------//
// NormalMapping 2
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string NormalMapping_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.x";

float4x4 gWorldMatrix : World;
float4x4 gWVPMatrix : WorldViewProjection;

// cal light value
float4 gWorldLightPosition = float4(500.00, 500.00, -500.00, 1.00);
float4 gWorldCameraPosition : ViewPosition;
float3 gLightColor = float3(0.70, 0.70, 1.00);

struct VS_INPUT
{
    float4 mPosition : POSITION;
    float3 mNormal : NORMAL;
    float3 mBinormal : BINORMAL;
    float3 mTangent : TANGENT;
    float2 mUV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 mPosition : POSITION;
    float2 mUV : TEXCOORD0;
    float3 mLightDir : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

VS_OUTPUT NormalMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.mPosition = mul(Input.mPosition, gWVPMatrix);
    Output.mUV = Input.mUV;
    
    // v is world position
    float4 worldPos = mul(Input.mPosition, gWorldMatrix);
    
    // v -> light dir
    float3 lightDir = worldPos.xyz - gWorldLightPosition.xyz;

    Output.mLightDir = normalize(lightDir);
    
    float3 viewDir = normalize(worldPos.xyz - gWorldCameraPosition.xyz);
    Output.mViewDir = viewDir;
    
    // T B N on World Space
    Output.T = normalize(mul(Input.mTangent, (float3x3) gWorldMatrix));
    Output.B = normalize(mul(Input.mBinormal, (float3x3) gWorldMatrix));
    Output.N = normalize(mul(Input.mNormal, (float3x3) gWorldMatrix));
   
    return Output;
}

struct PS_INPUT
{
    float2 mUV : TEXCOORD0;
    float3 mLightDir : TEXCOORD1;
    float3 mViewDir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state
{
    Texture = (DiffuseMap_Tex);
};

texture SpecularMap_Tex;
sampler2D SpecularSampler = sampler_state
{
    Texture = (SpecularMap_Tex);
};

texture NormalMap_Tex;
sampler2D NormalSampler = sampler_state
{
    Texture = (NormalMap_Tex);
};


float4 NormalMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
    float3 tangentNormal = tex2D(NormalSampler, Input.mUV).rgb; // equal xyz
    tangentNormal = normalize(tangentNormal * 2 - 1); // 0~1 -> -1~1
    //tangentNormal = float3(0, 0, 1);
   
    float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
    
    float3 worldNormal = mul(tangentNormal, TBN);
   
    float4 albedo = tex2D(DiffuseSampler, Input.mUV);
    float3 lightDir = normalize(Input.mLightDir);
   
    float3 diffuse = saturate(dot(worldNormal, -lightDir));
    float3 specular = 0;
   
    if (diffuse.x > 0)
    {
        float3 reflection = reflect(lightDir, worldNormal);
        float3 viewDir = normalize(Input.mViewDir);
      
        specular = saturate(dot(reflection, -viewDir));
        specular = pow(specular, 20.0f);
      
        float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
        specular *= specularIntensity.rgb * gLightColor;
    }
    diffuse = gLightColor * albedo.rgb * diffuse;
    float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo.rgb;
   
    return float4(ambient + diffuse + specular, 1);
}

//--------------------------------------------------------------//
// Technique Section for NormalMapping
//--------------------------------------------------------------//
technique NormalMapping
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 NormalMapping_Pass_0_Vertex_Shader_vs_main();
        PixelShader = compile ps_2_0 NormalMapping_Pass_0_Pixel_Shader_ps_main();
    }

}
