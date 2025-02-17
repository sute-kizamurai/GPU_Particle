
#include "common.hlsl"

[maxvertexcount(6)]
void main(point GS_IN input[1], inout TriangleStream<PS_IN> triStream)
{
  // �r���{�[�h�̃T�C�Y (���ƍ���)
    float billboardSize = 5.0f;
    
  //  // �r���{�[�h�̒��_�̃I�t�Z�b�g (�J�������W�Ɋ�Â�)
    float3 camRight = normalize(float3(View._11, View._21, View._31));
    float3 camUp = normalize(float3(View._12, View._22, View._32));
    
    camRight.xyz *= billboardSize;
    camUp.xyz *= billboardSize;
    
    // 4�̒��_���v�Z
    float3 positions[4] =
    {
        input[0].Position.xyz + (-camRight + camUp), // ����
        input[0].Position.xyz + (camRight + camUp), // �E��
        input[0].Position.xyz + (-camRight - camUp), // ����
        input[0].Position.xyz + (camRight - camUp) // �E��
    };

    // �Ή�����e�N�X�`�����W
    float2 texCoords[4] =
    {
        float2(0.0, 0.0), // ����
        float2(1.0, 0.0), // �E��
        float2(0.0, 1.0), // ����
        float2(1.0, 1.0) // �E��
    };
    
    // ���_�f�[�^���O�p�`�X�g���[���ɒǉ�
    for (int i = 0; i < 4; i++)
    {
        PS_IN output;
        matrix wvp;
        wvp = mul(World, View);
        wvp = mul(wvp, Projection);
        float4 worldPos = float4(positions[i], 1.0);
        output.TexCoord = texCoords[i]; // �e�N�X�`�����W
        output.Diffuse = input[0].Diffuse;
        output.WorldPosition.xyz = mul(worldPos, World);
        output.WorldPosition.w = 1.0f;
        
        output.Clip = 0.0;
        output.Clip = input[0].Clip;
        
        output.Position = mul(worldPos, wvp);
        
        triStream.Append(output);
    }

    triStream.RestartStrip(); // ���̃v���~�e�B�u�̏���
}
