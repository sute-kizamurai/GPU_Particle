
#include "common.hlsl"

StructuredBuffer<PARTICLE_LOCAL_CONFIG> BufIn : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = In.Position;
	
    Out.Position.xyz += BufIn[In.InstanceId].Position.xyz;

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

