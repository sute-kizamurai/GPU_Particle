
#include "common.hlsl"

StructuredBuffer<PARTICLE_LOCAL_CONFIG> BufIn : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = In.Position;
	
    Out.Position.xyz += BufIn[In.InstanceId].Position.xyz;
    
    Out.WorldPosition = mul(Out.Position, World);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
    
    Out.Clip = 0.0;
    
    if (BufIn[In.InstanceId].Life == 0.0) //¶‘¶ŠÔ‚ª0‚Ìê‡•`‰æÈ—ª
    {
        Out.Clip = -1.0;
    }
}

