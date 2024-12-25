#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "particle.h"
#include "ImGui\\imgui.h"


void Particle::Init()
{
	//パーティクルサイズを決定
	m_Size = { 5.0f, 5.0f };
	

	//頂点資料をいれる
	VERTEX_3D vertex[4];
	vertex[0].Position = XMFLOAT3(-m_Size.x, m_Size.y, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(m_Size.x, m_Size.y, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-m_Size.x, -m_Size.y, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(m_Size.x, -m_Size.y, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//パーティクル資料生成（Amountは最大数）
	m_ParticleAmount = 1024 * 16;
	m_Particle = new ParticleCompute[m_ParticleAmount];

	//パーティクルの資料をいれる
	for (int i = 0; i < m_ParticleAmount; i++)
	{
		//m_Particle[i].Pos = { 0.0f, 0.0f, 0.0f };
		//m_Particle[i].Vel = { (float)(rand() % 100 - 50) / 100.0f, (float)(rand() % 100 - 50) / 100.0f, (float)(rand() % 100 - 50) / 100.0f }; //速度
		m_Particle[i].MaxLife = 120.0f;
		m_Particle[i].Life = m_Particle[i].MaxLife; //ライフ（フレーム）
	}
	m_SliderValue = 120.0f;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	//頂点バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	//構造体バッファ生成 
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.ByteWidth = sizeof(ParticleCompute) * m_ParticleAmount;
	bd.StructureByteStride = sizeof(ParticleCompute);
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_ParticleBuffer);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_ResultBuffer);

	// 結果取得用のスタギングバッファ
	bd.Usage = D3D11_USAGE_STAGING;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	bd.BindFlags = 0;
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_StagingBuffer);

	//ポジション格納用構造体バッファ
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.ByteWidth = sizeof(XMFLOAT3) * m_ParticleAmount;
	bd.StructureByteStride = sizeof(XMFLOAT3);
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_PositionBuffer);


	//SRV生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_UNKNOWN; // 構造体バッファの場合はUNKNOWN
	srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv.Buffer.FirstElement = 0;
	srv.Buffer.NumElements = m_ParticleAmount;
	Renderer::GetDevice()->CreateShaderResourceView(m_ParticleBuffer, &srv, &m_ParticleSRV);
	Renderer::GetDevice()->CreateShaderResourceView(m_PositionBuffer, &srv, &m_PositionSRV);

	//UAV生成
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
	uav.Format = DXGI_FORMAT_UNKNOWN; // 構造体バッファの場合はUNKNOWN
	uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav.Buffer.FirstElement = 0;
	uav.Buffer.NumElements = m_ParticleAmount;
	HRESULT hr = Renderer::GetDevice()->CreateUnorderedAccessView(m_ResultBuffer, &uav, &m_ResultUAV);

	//コンピュートシェーダ―作成
	Renderer::CreateComputeShader(&m_ComputeShader, "shader\\particleCS.cso");


	//テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\texture\\particle.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\particleVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\particlePS.cso");
}

void Particle::Uninit()
{
	m_ComputeShader->Release();

	delete m_Particle;

	m_VertexBuffer->Release();
	m_ParticleBuffer->Release();
	m_StagingBuffer->Release();

	m_ParticleSRV->Release();
	m_PositionSRV->Release();
	//m_ResultUAV->Release();

	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Particle::Update()
{
	//パーティクルの資料をバッファに入れる
	D3D11_MAPPED_SUBRESOURCE subRes{};
	HRESULT hr = Renderer::GetDeviceContext()->Map(m_ParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	memcpy(subRes.pData, m_Particle, sizeof(ParticleCompute) * m_ParticleAmount);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	Renderer::GetDeviceContext()->Unmap(m_ParticleBuffer, 0);

	//コンピュートシェーダー実行
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_ParticleSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	// 結果をステージングバッファにコピー
	Renderer::GetDeviceContext()->CopyResource(m_StagingBuffer, m_ResultBuffer);

	// マップしてデータを読み取る
	D3D11_MAPPED_SUBRESOURCE subResResult{};
	hr = Renderer::GetDeviceContext()->Map(m_StagingBuffer, 0, D3D11_MAP_READ, 0, &subResResult);
	pBufType = static_cast<ParticleCompute*>(subResResult.pData);
	if (SUCCEEDED(hr))
	{
		memcpy(m_Particle, pBufType, sizeof(ParticleCompute) * m_ParticleAmount);
	}
	Renderer::GetDeviceContext()->Unmap(m_StagingBuffer, 0);

	//各パーティクルの座標を頂点シェーダで使用するため座標バッファにいれる
	Renderer::GetDeviceContext()->Map(m_PositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	XMFLOAT3* pBufType2 = (XMFLOAT3*)subRes.pData;
	for (int i = 0; i < m_ParticleAmount; i++)
	{
		//pBufType2[i] = m_Particle[i].Pos;
	}
	Renderer::GetDeviceContext()->Unmap(m_PositionBuffer, 0);
}

void Particle::Draw()
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
	//マテリアルを暗めの白に変更
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Zバッファ無効
	Renderer::SetDepthEnable(false);

	//加算合成有効
	Renderer::SetBlendAddEnable(true);

	//ワールドマトリクス設定
	XMMATRIX world, scale, trans;
	scale = XMMatrixScaling(m_Scale.x / 10, m_Scale.y / 10, m_Scale.z / 10);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(world);

	//VSに送る座標情報
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_PositionSRV);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawInstanced(4, m_ParticleAmount, 0, 0);

	//Zバッファ有効
	Renderer::SetDepthEnable(true);

	//加算合成無効
	Renderer::SetBlendAddEnable(false);


	//ImGui設定
	ImGui::SetNextWindowSize(ImVec2(400, 70));
	ImGui::Begin("ParticleStatus");

	ImGui::Text("Particle count : %d", m_ParticleAmount);

	if (ImGui::SliderFloat("ParticleMaxLife", &m_SliderValue, 1.0f, 300.0f) == true)
	{
		int a = 0;

		for (int i = 0; i < m_ParticleAmount; i++)
		{
			m_Particle[i].MaxLife = m_SliderValue;
		}
	}

	ImGui::End();
}
