cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld;
	float4 gColor;
};

cbuffer cbPerPass : register(b1)
{
	float4x4 gView;
	float4x4 gProj;
	float4x4 gViewProj;
}

struct VertexIn
{
	float3 PosL  : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	// Transform to homogeneous clip space.
	float4x4 mvp = mul(gWorld, gViewProj);
	vout.PosH = mul(float4(vin.PosL, 1.0f), mvp);
	vout.Color = gColor;
	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Just pass vertex color into the pixel shader.
	vout.Color = float4(vin.PosL, 1);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}
