
#include "common.hlsl"

// CS設定
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
        
    //重力の計算(重力使用フラグがtrueの場合)
    //if (ParticleGlobalConfig.IsEnableGravity)
    //{
    //    velocity.y += ParticleGlobalConfig.GravityFactor;
    //}
    
    //速度による移動を計算
    float3 result = BufIn[index].Position + BufIn[index].Velocity;
    
    if (BufIn[index].Life <= 0)
    {
        BufOut[index].Position = float3(0.0f, 0.0f, 0.0f);
        BufOut[index].Life = BufIn[index].MaxLife;
        
    //速度を再計算（初速度）
        BufOut[index].Velocity = BufIn[index].ShootDirection * ParticleGlobalConfig.SpeedFactor;
        
    //加速度を計算
        float3 acceleration = BufIn[index].Acceleration;
        //重力使用フラグがtrueの場合
        if (ParticleGlobalConfig.IsEnableGravity)
        {//重力を設定
            acceleration.y += ParticleGlobalConfig.GravityFactor * -1.0f;
        }
    }
    else
    {
        BufOut[index].Position = result;
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        
        //加速度を速度に加算
        BufOut[index].Velocity = BufIn[index].Velocity + BufIn[index].Acceleration;
        BufOut[index].Acceleration = BufIn[index].Acceleration;
        BufOut[index].MaxLife = BufIn[index].MaxLife;
        
        //生存時間をマイナス
        BufOut[index].Life = BufIn[index].Life - 1.0f;
    }
    
    
    GroupMemoryBarrierWithGroupSync();
}
