#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "particle.h"
#include "particleImgui.h"


void Particle::Init()
{
	//パーティクルサイズを決定
	m_Size = { 5.0f, 5.0f };

	//パーティクルの色情報を初期化
	m_ParticleColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_Component.emplace_back(new ParticleImgui(this));
	m_Component[0]->Init();

	//頂点資料をいれる
	VERTEX_3D vertex;
	vertex.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &vertex;

	//頂点バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	

	//パーティクルの全体設定を生成
	CreateParticleGlobal();

	//パーティクルの最大生成数を設定
	CreateParticleMaxCapacity(1024 * 512);

	//パーティクルの内容の変更がないためfalse
	m_ChangeParticle = false;


	//構造体バッファ生成
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bd.ByteWidth = sizeof(PARTICLE_LOCAL_CONFIG) * m_ParticleAmount;
	bd.StructureByteStride = sizeof(PARTICLE_LOCAL_CONFIG);
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_ParticleLocal;

	//パーティクルの個別設定入力用のバッファ作成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleLocalBuffer);

	//パーティクルの個別設定出力用兼VertexShader入力用のバッファ作成
	Renderer::GetDevice()->CreateBuffer(&bd, nullptr, &m_ResultBuffer);


	bd.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bd.ByteWidth = sizeof(PARTICLE_GLOBAL_CONFIG_RW);
	bd.StructureByteStride = sizeof(PARTICLE_GLOBAL_CONFIG_RW);

	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_ParticleGlobalReadWrite;

	//パーティクル全体設定の一部をGPUで読み書き可能で作る
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleGlobalReadWriteBuffer);


	//定数バッファ
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PARTICLE_GLOBAL_CONFIG_R);
	bd.StructureByteStride = sizeof(float);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.MiscFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_ParticleGlobalRead;

	//パーティクルの全体設定入力用のバッファ作成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleGlobalReadBuffer);

	//定数バッファをComputeShaderにバインド
	Renderer::GetDeviceContext()->CSSetConstantBuffers(6, 1, &m_ParticleGlobalReadBuffer);


	//SRV生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_UNKNOWN; // 構造体バッファの場合はUNKNOWN
	srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv.Buffer.FirstElement = 0;
	srv.Buffer.NumElements = m_ParticleAmount;
	Renderer::GetDevice()->CreateShaderResourceView(m_ParticleLocalBuffer, &srv, &m_ParticleLocalSRV);
	Renderer::GetDevice()->CreateShaderResourceView(m_ResultBuffer, &srv, &m_ResultSRV);

	//UAV生成
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
	uav.Format = DXGI_FORMAT_UNKNOWN;
	uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav.Buffer.FirstElement = 0;
	uav.Buffer.NumElements = m_ParticleAmount;
	Renderer::GetDevice()->CreateUnorderedAccessView(m_ParticleLocalBuffer, &uav, &m_ParticleLocalUAV);
	Renderer::GetDevice()->CreateUnorderedAccessView(m_ResultBuffer, &uav, &m_ResultUAV);

	uav.Buffer.NumElements = 1;
	Renderer::GetDevice()->CreateUnorderedAccessView(m_ParticleGlobalReadWriteBuffer, &uav, &m_m_ParticleGlobalReadWriteUAV);


	//パーティクル更新用コンピュートシェーダ―作成
	Renderer::CreateComputeShader(&m_ComputeShader, "shader\\particleCS.cso");
	//パーティクル初期化用コンピュートシェーダー作成
	Renderer::CreateComputeShader(&m_ParticleInitialShader, "shader\\particleInitialCS.cso");
	//バッファ間のデータ入れ替え用のコンピュートシェーダ作成
	Renderer::CreateComputeShader(&m_PingPongShader, "shader\\pingPongCS.cso");


	//パーティクル初期化
	Renderer::GetDeviceContext()->CSSetShader(m_ParticleInitialShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleLocalUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	// リソースを解除
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);


	//ジオメトリシェーダ作成
	Renderer::CreateGeometryShader(&m_GeometryShader, "shader\\particleGS.cso");


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
	//メモリ解放
	for (auto element : m_Component)
	{
		element->Uninit();
		delete element;
	}

	m_ComputeShader->Release();
	m_ParticleInitialShader->Release();
	m_PingPongShader->Release();

	m_GeometryShader->Release();

	delete[] m_ParticleLocal;
	delete m_ParticleGlobalRead;
	delete m_ParticleGlobalReadWrite;

	m_VertexBuffer->Release();
	m_ParticleLocalBuffer->Release();
	m_ParticleGlobalReadBuffer->Release();
	m_ResultBuffer->Release();

	m_ParticleLocalSRV->Release();
	m_ResultSRV->Release();

	m_ParticleLocalUAV->Release();
	m_ResultUAV->Release();

	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Particle::Update()
{
	//パーティクル用コンピュートシェーダー実行
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_ParticleLocalSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV, nullptr);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(1, 1, &m_m_ParticleGlobalReadWriteUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	//GPU側と同期を取る
	Renderer::GetDeviceContext()->Flush();

	// リソースを解除
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, nullSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

	//ComputeShaderを使用し出力用バッファから入力用バッファに値を受け渡し
	Renderer::GetDeviceContext()->CSSetShader(m_PingPongShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_ResultSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleLocalUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	// リソースを解除
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, nullSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
}

void Particle::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->GSSetShader(m_GeometryShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));

	//マテリアルを暗めの白に変更
	material.Diffuse = m_ParticleColor;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_Component[0]->Draw();

	if (m_ChangeParticle)
	{//パーティクルの全体設定に変更があった場合にバッファの値を更新
		Renderer::GetDeviceContext()->UpdateSubresource(m_ParticleGlobalReadBuffer, 0, NULL, m_ParticleGlobalRead, 0, 0);
		m_ChangeParticle = false;
	}

	//パーティクルの発射数を更新
	Renderer::GetDeviceContext()->UpdateSubresource(m_ParticleGlobalReadWriteBuffer, 0, NULL, m_ParticleGlobalReadWrite, 0, 0);


	//Zバッファ無効
	Renderer::SetDepthEnable(false);

	//加算合成有効
	Renderer::SetBlendAddEnable(true);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x / 10, m_Scale.y / 10, m_Scale.z / 10);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//VSに送る座標情報
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_ResultSRV);

	//ポリゴン描画
	Renderer::GetDeviceContext()->DrawInstanced(4, m_ParticleAmount, 0, 0);

	//Zバッファ有効
	Renderer::SetDepthEnable(true);

	//加算合成無効
	Renderer::SetBlendAddEnable(false);

	// リソースを解除
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, nullSRV);
}


//パーティクルの最大生成数を設定する
void Particle::CreateParticleMaxCapacity(int ParticleAmount)
{
	//パーティクルの最大生成枠を作成
	m_ParticleAmount = ParticleAmount;
	m_ParticleLocal = new PARTICLE_LOCAL_CONFIG[m_ParticleAmount];
}

//パーティクルの全体設定を生成する
void Particle::CreateParticleGlobal()
{
	//読み込み専用のパーティクル全体設定の枠を生成
	m_ParticleGlobalRead = new PARTICLE_GLOBAL_CONFIG_R();

	//パーティクルの全体設定を作成
	//パーティクルの発射方法を決定する補正値を設定
	m_ParticleGlobalRead->ShootingMethod = { 2.0f, 1.0f };

	//最大寿命を設定
	m_ParticleGlobalRead->MaxLife = 5.0f;

	//速度の初期値を設定
	m_ParticleGlobalRead->SpeedFactor = 1.0f;

	//重力の使用フラグの初期値を設定
	m_ParticleGlobalRead->IsEnableGravity = false;

	//重力の初期値を設定
	m_ParticleGlobalRead->GravityFactor = 1.0f;

	//抵抗力の使用フラグの初期値を設定
	m_ParticleGlobalRead->IsEnableDrag = false;

	//抵抗力の初期値を設定
	m_ParticleGlobalRead->DragFactor = 0.5f;


	//読み書き用のパーティクル全体設定の枠を生成
	m_ParticleGlobalReadWrite = new PARTICLE_GLOBAL_CONFIG_RW();

	//一度に発射できるパーティクルの数を設定
	m_ParticleGlobalReadWrite->ShotNum = 10;

	//前回発射からの経過時間を初期化
	m_ParticleGlobalReadWrite->ElapsedTime = 0.0f;
}


