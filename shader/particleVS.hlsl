
#include "common.hlsl"

StructuredBuffer<PARTICLE_LOCAL_CONFIG> BufIn : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = mul(In.Position, World);
	
    Out.Position.xyz += BufIn[In.InstanceId].Position;
    
    //ワールド変換した頂点座標を出力
    Out.WorldPosition = mul(Out.Position, World);

    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

