
#include "common.hlsl"

[maxvertexcount(6)]
void main(point GS_IN input[1], inout TriangleStream<PS_IN> triStream)
{
  // ビルボードのサイズ (幅と高さ)
    float billboardSize = 5.0f;
    
  //  // ビルボードの頂点のオフセット (カメラ座標に基づく)
    //float4 right = { Camera.RightVector.xyz * billboardSize, 1.0f };
    //float4 up = { Camera.UpVector.xyz * billboardSize, 1.0f };
    float3 right = { 1.0f, 0.0f, 0.0f };
    float3 up = { 0.0f, 1.0f, 0.0f };
    
    right.xyz *= billboardSize;
    up.xyz *= billboardSize;
    
    // 4つの頂点を計算
    float3 corners[4] =
    {
        input[0].WorldPosition.xyz + (-right + up), // 左上
        input[0].WorldPosition.xyz + (right + up), // 右上
        input[0].WorldPosition.xyz + (-right - up), // 左下
        input[0].WorldPosition.xyz + (right - up) // 右下
    };

    // 対応するテクスチャ座標
    float2 texCoords[4] =
    {
        float2(0.0, 0.0), // 左上
        float2(1.0, 0.0), // 右上
        float2(0.0, 1.0), // 左下
        float2(1.0, 1.0) // 右下
    };

    bool rollback = true;
    
    // 頂点データを三角形ストリームに追加
    for (int i = 0; i < 4; i++)
    {
        PS_IN output;
        matrix wvp;
        wvp = mul(World, View);
        wvp = mul(wvp, Projection);
        output.Position = input[0].Position;
        output.TexCoord = texCoords[i]; // テクスチャ座標
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

    triStream.RestartStrip(); // 次のプリミティブの準備
}
