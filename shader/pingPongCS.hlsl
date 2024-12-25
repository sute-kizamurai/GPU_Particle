// パーティクル構造体
struct ParticleCompute
{
    float3 pos;
    float3 vel;
    float life;
    float maxLife;
};

// CS設定
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// In
StructuredBuffer<ParticleCompute> BufIn : register(t0);

// Out
RWStructuredBuffer<ParticleCompute> BufOut : register(u0);

#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    uint index = input.dispatch.x;
    BufOut[index] = BufIn[index]; // 値をそのまま書き込む
    
    GroupMemoryBarrierWithGroupSync();
}
