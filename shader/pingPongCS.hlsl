// パーティクル構造体
struct ParticleCompute
{
    float3 position; //座標
    float3 shootDirection; //発射方向
    float speedFactor; //速度係数、正規化した発射方向に乗算することで速度を作成する
    float maxLife; //最大寿命
    float life; //寿命
    float dummy[3]; //サイズ調整用ダミー
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
