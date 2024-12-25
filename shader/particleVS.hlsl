
#include "common.hlsl"

StructuredBuffer<float3> Position : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = mul(In.Position, World);
	
    Out.Position.xyz += Position[In.InstanceId];

    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
    
    //���[���h�ϊ��������_���W���o��
    Out.WorldPosition = mul(In.Position, World);
}

