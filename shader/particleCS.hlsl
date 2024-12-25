
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
StructuredBuffer<PARTICLE> BufIn : register(t0);

// Out
RWStructuredBuffer<PARTICLE> BufOut : register(u0);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    int index = input.dispatch.x;
    
    float3 velocity = BufIn[index].shootDirection * BufIn[index].speedFactor;
    
    float3 result = BufIn[index].position + velocity;
            
    if (BufIn[index].life <= 0)
    {
        BufOut[index].position = float3(0.0f, 0.0f, 0.0f);
        BufOut[index].life = BufIn[index].maxLife;
    }
    else
    {
        BufOut[index].position = result;
        BufOut[index].shootDirection = BufIn[index].shootDirection;
        BufOut[index].speedFactor = BufIn[index].speedFactor;
        BufOut[index].maxLife = BufIn[index].maxLife;
        BufOut[index].life = BufIn[index].life - 1.0f;
    }
    
    GroupMemoryBarrierWithGroupSync();
}
