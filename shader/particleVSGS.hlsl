
#include "common.hlsl"

struct Particle
{
    float3 pos;
    float3 vel;
    float life;
    float maxLife;
};

StructuredBuffer<Particle> particle : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = mul(In.Position, World);
	
    Out.Position.xyz += particle[In.InstanceId].pos;
    
    //ワールド変換した頂点座標を出力
    Out.WorldPosition = mul(Out.Position, World);

    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

