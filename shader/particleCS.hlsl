
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
        
    //�d�͂̌v�Z(�d�͎g�p�t���O��true�̏ꍇ)
    //if (ParticleGlobalConfig.IsEnableGravity)
    //{
    //    velocity.y += ParticleGlobalConfig.GravityFactor;
    //}
    
    //���x�ɂ��ړ����v�Z
    float3 result = BufIn[index].Position + BufIn[index].Velocity;
    
    if (BufIn[index].Life <= 0)
    {
        BufOut[index].Position = float3(0.0f, 0.0f, 0.0f);
        BufOut[index].Life = BufIn[index].MaxLife;
        
    //���x���Čv�Z�i�����x�j
        BufOut[index].Velocity = BufIn[index].ShootDirection * ParticleGlobalConfig.SpeedFactor;
        
    //�����x���v�Z
        float3 acceleration = BufIn[index].Acceleration;
        //�d�͎g�p�t���O��true�̏ꍇ
        if (ParticleGlobalConfig.IsEnableGravity)
        {//�d�͂�ݒ�
            acceleration.y += ParticleGlobalConfig.GravityFactor * -1.0f;
        }
    }
    else
    {
        BufOut[index].Position = result;
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //�����x�𑬓x�ɉ��Z
        BufOut[index].Velocity = BufIn[index].Velocity + BufIn[index].Acceleration;
        BufOut[index].Acceleration = BufIn[index].Acceleration;
        BufOut[index].MaxLife = BufIn[index].MaxLife;
        
        //�������Ԃ��}�C�i�X
        BufOut[index].Life = BufIn[index].Life - 1.0f;
    }
    
    
    GroupMemoryBarrierWithGroupSync();
}
