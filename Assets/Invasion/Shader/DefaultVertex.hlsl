

cbuffer MatrixBuffer : register(b0)
{
    matrix projectionMatrix;
    matrix viewMatrix;
    matrix worldMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
    float3 color : COLOR;
    float3 normal : NORMAL;
    float2 textureCoordinates : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 textureCoordinates : TEXCOORD;
};

PixelInputType Main(VertexInputType input)
{
    PixelInputType output;
    
    float4 worldPosition = float4(input.position, 1.0f);
    
    worldPosition = mul(worldPosition, worldMatrix);
    worldPosition = mul(worldPosition, viewMatrix);
    worldPosition = mul(worldPosition, projectionMatrix);
    
    output.position = worldPosition;
    output.color = float4(input.color, 1.0f);
    output.normal = input.normal;
    output.textureCoordinates = input.textureCoordinates;

    return output;
}