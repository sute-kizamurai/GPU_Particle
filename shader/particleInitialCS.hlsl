
//======================================
// �p�[�e�B�N���̏���������
// Author:���R�D
//======================================

#include "common.hlsl"

// CS�ݒ�
struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

// �o�͗p�o�b�t�@
RWStructuredBuffer<PARTICLE_LOCAL_CONFIG> BufOut : register(u0);


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


#define size_x    1024
#define size_y       1
#define size_z       1

[numthreads(size_x, size_y, size_z)]
void main(const CSInput input)
{
    //�f�B�X�p�b�`���ł̈�ӂ̒l���i�[
    int index = input.dispatch.x;
    
    //�ʒu�̏�����
    BufOut[index].Position = float4(0.0, 0.0, 0.0, 1.0);
    
    //���˕����̗������쐬
    float3 shootDirection;
    shootDirection.x = Random(float4(input.dispatch.x, input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x % 33.3));
    shootDirection.y = Random(float4(input.groupThread.x, input.dispatch.x % 33.3, input.dispatch.x / 33.3, input.dispatch.x));
    shootDirection.z = Random(float4(input.groupThread.x, input.dispatch.x / 33.3, input.dispatch.x, input.dispatch.x % 33.3));
    
    //�ł��グ�悤��x,z�̒l���}�C�i�X�����ɂ��g��
    shootDirection.x = shootDirection.x * 2.0 - 1.0;
    shootDirection.y = shootDirection.y * 2.0 - 1.0;
    shootDirection.z = shootDirection.z * 2.0 - 1.0;
    
    //���˕����𐳋K��
    shootDirection = normalize(shootDirection);
        
    BufOut[index].ShootDirection = shootDirection;
    
    //���C�t��������
    BufOut[index].Life = 0.0;

}
