
#include "common.hlsl"

// CSİ’è
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

#define size_x    256
#define size_y      1
#define size_z      1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    uint index = input.dispatch.x;
    
    BufOut[index] = BufIn[index]; // ’l‚ğ‚»‚Ì‚Ü‚Ü‘‚«‚Ş
}
