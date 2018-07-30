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
// Position
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Position_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

// use GPU world, view, proj Matrix
float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjMatrix : Projection;
float4 gColor
<
   string UIName = "gColor";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 0.00, 1.00 );

struct VS_INPUT
{
   float4 Position : POSITION;    // vertex position
   float4 Color : COLOR0;         // vertex color
};

struct VS_OUTPUT
{
   float4 Position : POSITION;
   float4 Color : COLOR0;
};

VS_OUTPUT Position_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   Output.Position = mul(Input.Position, gWorldMatrix);
   Output.Position = mul(Output.Position, gViewMatrix);
   Output.Position = mul(Output.Position, gProjMatrix);
   Output.Color = gColor;
   
   return Output;
}
struct PS_INPUT
{
   float4 Color : COLOR0;         // vertex color
};

float4 Position_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   return Input.Color;
}
//--------------------------------------------------------------//
// Technique Section for Position
//--------------------------------------------------------------//
technique Position
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Position_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Position_Pass_0_Pixel_Shader_ps_main();
   }

}

