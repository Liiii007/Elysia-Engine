cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4 gColor;
};

cbuffer cbPerPass : register(b1)
{
	float4x4 gViewProj;

	float3 viewPos;
	//float pad1;
	float3 viewDir;
	//float pad2;
	float3 lightDir;
	//float pad3;
	float3 lightPos;
	//float pad4;
}

struct VertexIn
{
	float3 PosL  : POSITION;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float4x4 mvp = mul(gWorld, gViewProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), mvp);
	vout.Normal = normalize(viewPos);
	vout.Color = float4(-viewDir, 1);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{

	return pin.Color;
}
