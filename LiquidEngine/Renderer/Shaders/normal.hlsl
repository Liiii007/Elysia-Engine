cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4 gColor;
};

cbuffer cbPerPass : register(b1)
{
	float4x4 gViewProj;

	float3 viewPos;
	float pad1;

	float3 viewDir;
	float pad2;

	float3 lightDir;
	float pad3;

	float3 lightPos;
	float pad4;
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
	float4 normalWorld = mul(float4(vin.Normal, 1), gWorld);
	vout.Normal = normalize(normalWorld);
	vout.Color = float4(-viewDir, 1);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float c = max(dot(pin.Normal, normalize(-lightDir)), 0);
	c = min(c, 1);
	c = pow(c, 5);
	float strength = 5;
	float4 color =  0.9 * float4(strength * c, strength * c, strength * c, 1) + 0.1;
	return color;
}
