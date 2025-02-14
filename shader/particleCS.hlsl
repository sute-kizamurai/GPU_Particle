
#include "common.hlsl"

// CS�ݒ�
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};


//�^�����������֐�///////////////////////////////////////////
float RandomOrig(float2 v)
{
    uint2 u = asint(v * float2(141421356, 2718281828));
    return float((u.x ^ u.y) * 3141592653) * 2.3283064365386962890625e-10;
}
  
float Random(float4 v)
{
    return RandomOrig(v.xy + v.zw);
}
////////////////////////////////////////////////////////////


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
    
    
    //fps���擾
    float fps = PcInfomation.Fps;

    if (fps < 30.0)
    {
        fps = 30;
    }
    
    //�������Ԃ������𒴂����ꍇ
    if (BufIn[index].Life > ParticleGlobalConfig.MaxLife)
    {
        //���˕����̗������쐬
        float3 shootDirection;
        shootDirection.x = Random(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
        shootDirection.y = Random(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
        shootDirection.z = Random(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
        //���˕�����␳
        shootDirection.x = shootDirection.x * 2.0 - 1.0;
        shootDirection.y = shootDirection.y * ParticleGlobalConfig.ShootingMethod.x - ParticleGlobalConfig.ShootingMethod.y;
        shootDirection.z = shootDirection.z * 2.0 - 1.0;
        
        //���˕����𐳋K��
        shootDirection = normalize(shootDirection);
        
        //���˕�����������
        BufOut[index].ShootDirection = shootDirection;

        //�ʒu��������
        BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
        
        //�������Ԃ�������
        BufOut[index].Life = 0.0;
    }
    //�������Ă���ꍇ
    else
    {
        //�ʒu�����i�[
        BufOut[index].Position.xyz = BufIn[index].Position.xyz + result;
        BufOut[index].Position.w = 1.0;
        
        //���˕������i�[
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //�������Ԃ����Z���Ċi�[
        BufOut[index].Life = BufIn[index].Life + 1.0 / fps;
    }
}
