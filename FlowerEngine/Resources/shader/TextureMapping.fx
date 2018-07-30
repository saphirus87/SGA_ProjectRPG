//--------------------------------------------------------------//
// TextureMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string TextureMapping_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjMatrix : Projection;

struct VS_INPUT
{
   float4 Position : POSITION;
   float3 Normal	: NORMAL0;
   float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 Position : POSITION;
   float2 TexCoord : TEXCOORD0;
};

texture DiffuseMap_Tex
<
	string ResourceName = "..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;

sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
};



struct PS_INPUT
{
	float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Input.Position.xyz = Input.Position.xyz + (Input.Normal * 2.0f);
	Output.Position = mul(Input.Position, gWorldMatrix);
	Output.Position = mul(Output.Position, gViewMatrix);
	Output.Position = mul(Output.Position, gProjMatrix);

	Output.TexCoord = Input.TexCoord;

	return Output;
}

float4 PS_MAIN(PS_INPUT Input) : COLOR
{
	
	return float4(1, 0, 0, 1);
}


VS_OUTPUT TextureMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.Position  = mul(Input.Position, gWorldMatrix);
   Output.Position  = mul(Output.Position, gViewMatrix);
   Output.Position  = mul(Output.Position, gProjMatrix);
   
   Output.TexCoord = Input.TexCoord;
   
   return Output;
}



float4 TextureMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float3 albedo = tex2D(DiffuseSampler, Input.TexCoord).rgb;
   
   return float4(albedo, 1);
}
//--------------------------------------------------------------//
// Technique Section for TextureMapping
//--------------------------------------------------------------//
technique TextureMapping
{
	
	pass Pass_0
	{
		VertexShader = compile vs_2_0 VS_MAIN();
		PixelShader = compile ps_2_0 PS_MAIN();
	}
	
   pass Pass_1
   {
      VertexShader = compile vs_2_0 TextureMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 TextureMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

