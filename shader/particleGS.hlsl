
#include "common.hlsl"

[maxvertexcount(6)]
void main(point GS_IN input[1], inout TriangleStream<PS_IN> triStream)
{
  // �r���{�[�h�̃T�C�Y (���ƍ���)
    float billboardSize = 5.0f;
    
  //  // �r���{�[�h�̒��_�̃I�t�Z�b�g (�J�������W�Ɋ�Â�)
    //float4 right = { Camera.RightVector.xyz * billboardSize, 1.0f };
    //float4 up = { Camera.UpVector.xyz * billboardSize, 1.0f };
    float3 right = { 1.0f, 0.0f, 0.0f };
    float3 up = { 0.0f, 1.0f, 0.0f };
    
    right.xyz *= billboardSize;
    up.xyz *= billboardSize;
    
    // 4�̒��_���v�Z
    float3 corners[4] =
    {
        input[0].WorldPosition.xyz + (-right + up), // ����
        input[0].WorldPosition.xyz + (right + up), // �E��
        input[0].WorldPosition.xyz + (-right - up), // ����
        input[0].WorldPosition.xyz + (right - up) // �E��
    };

    // �Ή�����e�N�X�`�����W
    float2 texCoords[4] =
    {
        float2(0.0, 0.0), // ����
        float2(1.0, 0.0), // �E��
        float2(0.0, 1.0), // ����
        float2(1.0, 1.0) // �E��
    };

    bool rollback = true;
    
    // ���_�f�[�^���O�p�`�X�g���[���ɒǉ�
    for (int i = 0; i < 4; i++)
    {
        PS_IN output;
        matrix wvp;
        wvp = mul(World, View);
        wvp = mul(wvp, Projection);
        output.Position = input[0].Position;
        output.TexCoord = texCoords[i]; // �e�N�X�`�����W
        output.Diffuse = input[0].Diffuse;
        output.WorldPosition.xyz = corners[i];
        output.WorldPosition.w = 1.0f;
        
        output.Position = mul(output.Position, wvp);
        
        triStream.Append(output);
        
        if (i == 2 && rollback == true)
        {
            i -= 1;
            rollback = false;
        }
    }

    triStream.RestartStrip(); // ���̃v���~�e�B�u�̏���
}
