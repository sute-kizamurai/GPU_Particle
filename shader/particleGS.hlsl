
#include "common.hlsl"

[maxvertexcount(6)]
void main(point GS_IN input[1], inout TriangleStream<PS_IN> triStream)
{
  // ビルボードのサイズ (幅と高さ)
    float billboardSize = 5.0f;
    
  //  // ビルボードの頂点のオフセット (カメラ座標に基づく)
    float3 camRight = normalize(float3(View._11, View._21, View._31));
    float3 camUp = normalize(float3(View._12, View._22, View._32));
    
    camRight.xyz *= billboardSize;
    camUp.xyz *= billboardSize;
    
    // 4つの頂点を計算
    float3 positions[4] =
    {
        input[0].Position.xyz + (-camRight + camUp), // 左上
        input[0].Position.xyz + (camRight + camUp), // 右上
        input[0].Position.xyz + (-camRight - camUp), // 左下
        input[0].Position.xyz + (camRight - camUp) // 右下
    };

    // 対応するテクスチャ座標
    float2 texCoords[4] =
    {
        float2(0.0, 0.0), // 左上
        float2(1.0, 0.0), // 右上
        float2(0.0, 1.0), // 左下
        float2(1.0, 1.0) // 右下
    };
    
    // 頂点データを三角形ストリームに追加
    for (int i = 0; i < 4; i++)
    {
        PS_IN output;
        matrix wvp;
        wvp = mul(World, View);
        wvp = mul(wvp, Projection);
        float4 worldPos = float4(positions[i], 1.0);
        output.TexCoord = texCoords[i]; // テクスチャ座標
        output.Diffuse = input[0].Diffuse;
        output.WorldPosition.xyz = mul(worldPos, World);
        output.WorldPosition.w = 1.0f;
        
        output.Clip = 0.0;
        output.Clip = input[0].Clip;
        
        output.Position = mul(worldPos, wvp);
        
        triStream.Append(output);
    }

    triStream.RestartStrip(); // 次のプリミティブの準備
}
