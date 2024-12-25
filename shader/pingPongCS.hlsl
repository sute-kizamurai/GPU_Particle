// �p�[�e�B�N���\����
struct ParticleCompute
{
    float3 position; //���W
    float3 shootDirection; //���˕���
    float speedFactor; //���x�W���A���K���������˕����ɏ�Z���邱�Ƃő��x���쐬����
    float maxLife; //�ő����
    float life; //����
    float dummy[3]; //�T�C�Y�����p�_�~�[
};

// CS�ݒ�
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
    
    BufOut[index] = BufIn[index]; // �l�����̂܂܏�������
    
    GroupMemoryBarrierWithGroupSync();
}
