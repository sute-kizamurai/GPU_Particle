#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "computeShader.h"

void ComputeShader::Init()
{
	//パーティクルサイズを決定
	m_Size = { 5.0f, 5.0f };

	//頂点資料をいれる
	VERTEX_3D_PARTICLE vertex[4];
	vertex[0].Positon = { -m_Size.x, m_Size.y, 0.0f };
	vertex[0].TexCoord = { 0.0f, 0.0f };
	vertex[1].Positon = { m_Size.x, m_Size.y, 0.0f };
	vertex[1].TexCoord = { 1.0f, 0.0f };
	vertex[2].Positon = { -m_Size.x, -m_Size.y, 0.0f };
	vertex[2].TexCoord = { 0.0f, 1.0f };
	vertex[3].Positon = { m_Size.x, -m_Size.y, 0.0f };
	vertex[3].TexCoord = { 1.0f, 1.0f };

	//パーティクル資料生成（Amountは最大数）
	m_ParticleAmount = 100;
	m_Particle = new ParticleCompute[m_ParticleAmount];

	//パーティクルの資料をいれる（固定かランダムとか自分で作る）
	for (int i = 0; i < m_ParticleAmount; i++)
	{
		m_Particle[i].Vel = { (float)(rand() % 100), 1.0f, 0.0f }; //速度
		m_Particle[i].life = 300.0f; //ライフ（フレーム）
		m_Particle[i].Pos = { 0.0f, 0.0f, 0.0f };
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D_PARTICLE) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	//ID3D11Device
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.ByteWidth = sizeof(ParticleCompute) * m_ParticleAmount;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.StructureByteStride = sizeof(ParticleCompute);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_ParticleBuffer);
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_PositionBuffer);
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_ResultBuffer);

	//バッファ生成 
	//Renderer::CreateStructuredBuffer(sizeof(ParticleCompute), m_ParticleAmount, nullptr, &m_ParticleBuffer);
	//Renderer::CreateStructuredBuffer(sizeof(XMFLOAT3), m_ParticleAmount, nullptr, &m_PositionBuffer);
	//Renderer::CreateStructuredBuffer(sizeof(ParticleCompute), m_ParticleAmount, nullptr, &m_ResultBuffer);

	//SRV生成
	Renderer::CreateBufferSRV(m_ParticleBuffer, m_ParticleSRV);
	Renderer::CreateBufferSRV(m_PositionBuffer, m_PositionSRV);

	//UAV生成
	Renderer::CreateBufferUAV(m_ResultBuffer, m_ResultUAV);

	//コンピュートシェーダ―作成
	{
		FILE* file{};
		long int fsize{};

		file = fopen("shader\\ParticleCS.cso", "rb");
		fsize = _filelength(_fileno(file));
		unsigned char* buffer = new unsigned char[fsize];
		fread(buffer, fsize, 1, file);
		fclose(file);

		Renderer::GetDevice()->CreateComputeShader(buffer, fsize, nullptr, &m_ComputeShader);

		delete[] buffer;
	}

	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\particle.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void ComputeShader::Uninit()
{
	m_ComputeShader->Release();

	delete m_Particle;

	m_VertexBuffer->Release();
	m_ParticleBuffer->Release();
	m_ResultBuffer->Release();
	m_PositionBuffer->Release();

	m_ParticleSRV->Release();
	m_PositionSRV->Release();
	m_ResultUAV->Release();

	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void ComputeShader::Update()
{
	//パーティクルの資料をバッファに入れる
	D3D11_MAPPED_SUBRESOURCE subRes;
	Renderer::GetDeviceContext()->Map(m_ParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	memcpy(subRes.pData, m_Particle, sizeof(ParticleCompute) * m_ParticleAmount);
	Renderer::GetDeviceContext()->Unmap(m_ParticleBuffer, 0);

	//コンピュートシェーダー実行
	ID3D11ShaderResourceView* pSRVs[1] = { m_ParticleSRV };
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, pSRVs);
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV, 0);
	Renderer::GetDeviceContext()->Dispatch(256, 1, 1);

	// 戻った計算結果をバッファに入れる
	ID3D11Buffer* pBufDbg = m_ResultBuffer;
	Renderer::GetDeviceContext()->Map(pBufDbg, 0, D3D11_MAP_READ, 0, &subRes);
	pBufType = (ParticleCompute*)subRes.pData;
	memcpy(m_Particle, pBufType, sizeof(ParticleCompute) * m_ParticleAmount);
	Renderer::GetDeviceContext()->Unmap(pBufDbg, 0);
	pBufDbg->Release();

	//座標を座標バッファにいれる
	Renderer::GetDeviceContext()->Map(m_PositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	XMFLOAT3* pBuf = (XMFLOAT3*)subRes.pData;
	for (int i = 0; i < m_ParticleAmount; i++)
	{
		pBuf[i] = m_Particle[i].Pos;
	}
	Renderer::GetDeviceContext()->Unmap(m_PositionBuffer, 0);
}

void ComputeShader::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	//ビューの逆行列
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//逆行列
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	//マテリアルを暗めの赤に変更
	material.Diffuse = XMFLOAT4(0.5f, 0.05f, 0.05f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//VSに入れる座標設定
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Zバッファ無効
	Renderer::SetDepthEnable(false);

	//加算合成有効
	Renderer::SetBlendAddEnable(true);

	for (int i = 0; i < m_ParticleAmount; i++)
	{
		//ワールドマトリクス設定
		XMMATRIX world, scale, trans;
		scale = XMMatrixScaling(m_Scale.x / 10, m_Scale.y / 10, m_Scale.z / 10);
		trans = XMMatrixTranslation(m_Particle[i].Pos.x, m_Particle[i].Pos.y, m_Particle[i].Pos.z);
		world = scale * invView * trans;
		Renderer::SetWorldMatrix(world);

		//ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

	//Zバッファ有効
	Renderer::SetDepthEnable(true);

	//加算合成無効
	Renderer::SetBlendAddEnable(false);
}
