
#include "common.hlsl"

struct Particle
{
    float3 position; //���W
    float3 shootDirection; //���˕���
    float speedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����
    float maxLife; //�ő����
    float life; //����
    float dummy[3]; //�T�C�Y�����p�_�~�[
};

StructuredBuffer<Particle> particle : register(t2);

void main(in VS_IN In, out GS_IN Out)
{
    Out.Position = mul(In.Position, World);
	
    Out.Position.xyz += particle[In.InstanceId].position;
    
    //���[���h�ϊ��������_���W���o��
    Out.WorldPosition = mul(Out.Position, World);

    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

