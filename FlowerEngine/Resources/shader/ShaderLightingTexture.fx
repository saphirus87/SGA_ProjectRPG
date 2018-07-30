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
// Lighting
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Lighting_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjMatrix : Projection;
float4 gWorldLightPos
<
   string UIName = "gWorldLightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( -500.00, 500.00, -500.00, 1.00 ); // World light pos, for use cimentic
float4 gWorldCameraPos : ViewPosition; // my eye pos

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
   float Diffuse : TEXCOORD1;
   float3 ViewDir : TEXCOORD2;
   float3 Reflection : TEXCOORD3;   
};

VS_OUTPUT Lighting_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   Output.TexCoord = Input.TexCoord;

   Output.Position = mul(Input.Position, gWorldMatrix);
   
   // light src -> pos dir
   float3 lightDir = Output.Position.xyz - gWorldLightPos.xyz;
   // This is dir
   lightDir = normalize(lightDir);
   // my eye -> pos dir
   float3 viewDir = normalize(Output.Position.xyz - gWorldCameraPos.xyz);
   Output.ViewDir = viewDir;
   
   Output.Position = mul(Output.Position, gViewMatrix);
   Output.Position = mul(Output.Position, gProjMatrix);
   
   // diffuse, normal
   float3 worldNormal = mul(Input.Normal, (float3x3)gWorldMatrix);
   worldNormal = normalize(worldNormal);
   
   // dot product
   Output.Diffuse = dot(-lightDir, worldNormal);
   Output.Reflection = reflect(lightDir, worldNormal);
   
   return  Output;
}
texture DiffuseMap_Tex
<
   string ResourceName = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\EarthNight.jpg";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

struct PS_INPUT 
{
   float2 texCoord : TEXCOORD0;
   float Diffuse : TEXCOORD1;
   float3 ViewDir : TEXCOORD2;
   float3 Reflection : TEXCOORD3;   
};

float4 Lighting_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{
   float3 albedo = tex2D(DiffuseSampler, Input.texCoord).rgb;
   
   float3 diffuse = saturate(Input.Diffuse); // saturate : value must be 0~1
   float3 viewDir = normalize(Input.ViewDir);
   float3 reflection = normalize(Input.Reflection);
   float3 specular = 0;
   
   // jomeng bat nen sang tae
   if (diffuse.x > 0)
   {
      specular = saturate(dot(reflection, -viewDir));
      specular = pow(specular, 500.0f); // 500 is small, 20 is big
   }
   
   float3 ambient = float3(0.15f, 0.15f, 0.15f) * albedo.rgb;
   
   return float4(albedo + diffuse + specular + ambient, 1);
}




//--------------------------------------------------------------//
// Technique Section for Lighting
//--------------------------------------------------------------//
technique Lighting
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Lighting_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Lighting_Pass_0_Pixel_Shader_ps_main();
   }

}

