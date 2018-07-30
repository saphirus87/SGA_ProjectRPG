//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Lambert
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Lambert_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjMatrix : Projection;

float4x4 mWIT
<
   string UIName = "mWIT";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
> = float4x4(1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00);

float3 vLightDir
<
   string UIName = "vLightDir";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3(-500.00, 500.00, -500.00);

float4 I_a = { 0.3f, 0.3f, 0.3f, 0.0f };
float4 I_d = { 0.7f, 0.7f, 0.7f, 0.0f };

float4 k_a = { 1.0f, 1.0f, 1.0f, 1.0f };
float4 k_d = { 1.0f, 1.0f, 1.0f, 1.0f };

struct VS_INPUT
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position : POSITION0;
    float3 oNormal : TEXCOORD0;
    float4 Color : COLOR0;
    float3 oPos : TEXCOORD1;
};

VS_OUTPUT Lambert_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;
   
    Output.Position = mul(Input.Position, gWorldMatrix);
    Output.Position = mul(Output.Position, gViewMatrix);
    Output.oPos = Output.Position;
    Output.Position = mul(Output.Position, gProjMatrix);
   // vertex color
    float3 L = -vLightDir;
    float3 N = normalize(mul(Input.Normal, (float3x3) mWIT));

    Output.Color = I_a * k_a + I_d * k_d * max(0, dot(N, L));

    Output.oNormal = mul(Input.Normal, (float3x3) gWorldMatrix);
    return (Output);
}




struct PS_INPUT
{
    float4 Color : COLOR0;
    float3 Normal : TEXCOORD0;
    float3 Pos : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 Color : COLOR0;
    float4 Normal : COLOR1;
    float4 Pos : COLOR2;
};

void Lambert_Pass_0_Pixel_Shader_ps_main(
float4 Color : COLOR0,
float3 Normal : TEXCOORD0,
float3 Pos : TEXCOORD1,
out float4 oColor : COLOR0,
out float4 oNormal : COLOR1,
out float4 oPos : COLOR2
)
{
    //PS_OUTPUT Output = (PS_OUTPUT) 0;

    oColor = Color;
    oNormal = float4(normalize(Normal), 1.0f);
    oPos = float4(Pos, 1);

    //return Ouput;
}
//--------------------------------------------------------------//
// Technique Section for Lambert
//--------------------------------------------------------------//
technique Lambert
{
    pass Pass_0
    {
        VertexShader = compile vs_2_0 Lambert_Pass_0_Vertex_Shader_vs_main();
        PixelShader = compile ps_2_0 Lambert_Pass_0_Pixel_Shader_ps_main();
    }

}

