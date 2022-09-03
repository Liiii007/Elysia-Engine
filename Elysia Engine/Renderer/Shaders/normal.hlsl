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

	float3 lightColor;
	float lightPower;
}

cbuffer cbPerMaterial : register(b2)
{
	float4 diffuseAlbedo;

	float3 fresnelR0;
	float  roughness;
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
	float4 PosW  : POSITION;
	float3 ViewDirW : POSITION1;
	float3 NormalW : NORMAL;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	

	float4x4 mvp = mul(gWorld, gViewProj);
	vout.PosH = mul(float4(vin.PosL, 1), mvp);
	vout.PosW = mul(float4(vin.PosL, 1), gWorld);
	vout.NormalW = normalize(mul(float4(vin.Normal, 0), gWorld));
	vout.ViewDirW = normalize(viewPos - vout.PosW);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float lightStrength = lightPower;
	float3 lightPass = max(dot(normalize(-lightDir), pin.NormalW), 0) * lightStrength * lightColor * diffuseAlbedo;

	float3 halfwayNormal = normalize(pin.ViewDirW - normalize(lightDir));

	float m = roughness * 10;

	float3 rf = fresnelR0 + (1 - fresnelR0) * pow(1-dot(normalize(-lightDir), halfwayNormal), 5);
	
	float3 diffuse = lightPass;
	float3 ambient = float3(0.0f, 0.0f, 0.0f);
	float3 specular = lightPass * rf * ((m + 8) / 8) * pow(dot(halfwayNormal, pin.NormalW), m);

	float3 color = 0.7*diffuse + ambient + specular;
	float p = pow(max(dot(halfwayNormal, pin.NormalW), 0), m) * (m+8) / m;
	float d = dot(normalize(-lightDir), halfwayNormal);

	float4 debug = float4(color, 1);
	return debug;
}
