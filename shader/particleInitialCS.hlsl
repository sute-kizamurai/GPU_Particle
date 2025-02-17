
//======================================
// パーティクルの情報を初期化
// Author:中山颯
//======================================

#include "common.hlsl"

// CS設定
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// 出力用バッファ
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);


//疑似乱数生成関数///////////////////////////////////////////
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


#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    //ディスパッチ内での一意の値を格納
    int index = input.dispatch.x;
    
    //位置の初期化
    BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
    
    //発射方向の乱数を作成
    float3 shootDirection;
    shootDirection.x = Random(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
    shootDirection.y = Random(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
    shootDirection.z = Random(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
    //打ち上げようにx,zの値をマイナス方向にも拡大
    shootDirection.x = shootDirection.x * 2.0 - 1.0;
    shootDirection.y = shootDirection.y * 2.0 - 1.0;
    shootDirection.z = shootDirection.z * 2.0 - 1.0;
    
    //発射方向を正規化
    shootDirection = normalize(shootDirection);
        
    BufOut[index].ShootDirection = shootDirection;
    
    //ライフを初期化
    BufOut[index].Life = 0.0;

}
