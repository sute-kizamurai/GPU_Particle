
#include "common.hlsl"

// CS�ݒ�
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// In
StructuredBuffer<PARTICLE_LOCAL_CONFIG> BufIn : register(t0);

// Out
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    int index = input.dispatch.x;
        
    //���x���Z�o
    float3 velocity = BufIn[index].ShootDirection * ParticleGlobalConfig.SpeedFactor;
    
    
    //�����x���Z�o
    float3 acceleration = float3(0.0, 0.0, 0.0);
    
    //�d�͎g�p�t���O��true�̏ꍇ
    if (ParticleGlobalConfig.IsEnableGravity == true)
    { //�d�͂�ݒ�
        acceleration.y -= ParticleGlobalConfig.GravityFactor;
    }
    
    
    //���x�Ɖ����x�ɂ��ړ��ʂ��v�Z
    float3 result = velocity + acceleration * BufIn[index].Life;
    
    
    //��R�͂̎g�p��true�̏ꍇ
    if (ParticleGlobalConfig.IsEnableDrag == true)
    {
        //��R�ɂ��ړ��ʂ̌������v�Z
        result = result * pow(1.0 - ParticleGlobalConfig.DragFactor, BufIn[index].Life);
    }
    
    
    if (BufIn[index].Life > ParticleGlobalConfig.MaxLife)
    {
        //�ʒu��������
        BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
        
        //�������Ԃ�������
        BufOut[index].Life = 0.0;
    }
    else
    {
        //�ʒu�����i�[
        BufOut[index].Position.xyz = BufIn[index].Position.xyz + result;
        BufOut[index].Position.w = 1.0;
        
        //���˕������i�[
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //�������Ԃ����Z���Ċi�[
        BufOut[index].Life = BufIn[index].Life + 1.0 / PcInfomation.Fps;
    }
}
