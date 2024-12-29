
#include "common.hlsl"

// CSê›íË
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
    
    float3 velocity = BufIn[index].ShootDirection * ParticleGlobalConfig.SpeedFactor;
    
    float3 result = BufIn[index].Position + velocity;
            
    if (BufIn[index].Life <= 0)
    {
        BufOut[index].Position = float3(0.0f, 0.0f, 0.0f);
        BufOut[index].Life = BufIn[index].MaxLife;
    }
    else
    {
        BufOut[index].Position = result;
        BufOut[index].ShootDirection = BufIn[index].ShootDirection;
        BufOut[index].SpeedFactor = BufIn[index].SpeedFactor;
        BufOut[index].MaxLife = BufIn[index].MaxLife;
        BufOut[index].Life = BufIn[index].Life - 1.0f;
    }
    
    GroupMemoryBarrierWithGroupSync();
}
