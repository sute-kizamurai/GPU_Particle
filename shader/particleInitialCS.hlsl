
//======================================
// �p�[�e�B�N���̏���������
// Author:���R�D
//======================================

#include "common.hlsl"

// CS�ݒ�
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// �o�͗p�o�b�t�@
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    //�f�B�X�p�b�`���ł̈�ӂ̒l���i�[
    int index = input.dispatch.x;
    
    //�ʒu�̏�����
    BufOut[index].Position = float3(0.0, 0.0, 0.0);
    
    //���˕����̗������쐬
    
    //���x��������
    BufOut[index].Velocity = float3(0.0, 1.0, 0.0) * ParticleGlobalConfig.SpeedFactor;
    
    //�����x��������
    BufOut[index].Acceleration = float3(0.0, 0.0, 0.0);
    
    //���C�t��������
    BufOut[index].Life = ParticleGlobalConfig.MaxLife;

}
