
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
//ibukiHash
float ibuki(float4 v)
{
    const uint4 mult =
        uint4(0xae3cc725, 0x9fe72885, 0xae36bfb5, 0x82c1fcad);
  
    uint4 u = uint4(v);
    u = u * mult;
    u ^= u.wxyz ^ u >> 13;
      
    uint r = dot(u, mult);
  
    r ^= r >> 11;
    r = (r * r) ^ r;
          
    return r * 2.3283064365386962890625e-10;
}

//PCG4D
uint4 pcg4d_orig(uint4 v)
{
    v = v * 1664525 + 1013904223;
  
    v.x += v.y * v.w;
    v.y += v.z * v.x;
    v.z += v.x * v.y;
    v.w += v.y * v.z;
  
    v = v ^ v >> 16;
  
    v.x += v.y * v.w;
    v.y += v.z * v.x;
    v.z += v.x * v.y;
    v.w += v.y * v.z;
  
    return v;
}
  
float pcg4d(float4 v)
{
    uint4 a = pcg4d_orig(v);
    return dot(a, 1) * 2.3283064365386962890625e-10;
}

//fihash
float fihash_orig(float2 v)
{
    uint2 u = asint(v * float2(141421356, 2718281828));
    return float((u.x ^ u.y) * 3141592653) * 2.3283064365386962890625e-10;
}
  
float fihash(float4 v)
{
    return fihash_orig(v.xy + v.zw);
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
    shootDirection.x = fihash(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
    shootDirection.y = fihash(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
    shootDirection.z = fihash(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
    //打ち上げようにx,zの値をマイナス方向にも拡大
    shootDirection.x = shootDirection.x * 2.0 - 1.0;
    shootDirection.y = shootDirection.y / 2.0 + 0.5;
    shootDirection.z = shootDirection.z * 2.0 - 1.0;
    
    //発射方向を正規化
    shootDirection = normalize(shootDirection);
        
    BufOut[index].ShootDirection = shootDirection;
    
    //ライフを初期化
    BufOut[index].Life = 0.0;

}
