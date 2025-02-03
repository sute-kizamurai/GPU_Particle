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
	//VERTEX_3D vertex;
	//vertex.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//vertex.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//vertex.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);

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


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	//頂点バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	

	//パーティクルの全体設定を生成
	CreateParticleGlobal();

	//パーティクルの個別設定を生成
	CreateParticleLocal(1024 * 730);

	//変更可能ステータスを設定
	SetModifiableStatus();

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


	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PARTICLE_GLOBAL_CONFIG);
	bd.StructureByteStride = sizeof(float);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.MiscFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_ParticleGlobal;

	//パーティクルの全体設定入力用のバッファ作成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleGlobalBuffer);

	//定数バッファをComputeShaderにバインド
	Renderer::GetDeviceContext()->CSSetConstantBuffers(6, 1, &m_ParticleGlobalBuffer);


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
	m_ComputeShader->Release();
	m_ParticleInitialShader->Release();
	m_PingPongShader->Release();

	m_GeometryShader->Release();

	delete[] m_ParticleLocal;

	m_VertexBuffer->Release();
	m_ParticleLocalBuffer->Release();
	m_ParticleGlobalBuffer->Release();

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
	//Renderer::GetDeviceContext()->GSSetShader(m_GeometryShader, NULL, 0);
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


	//ImGui設定
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("ParticleStatus");

	ImGui::Text("Particle count : %d", m_ParticleAmount);

	if (ImGui::SliderInt("ParticleMaxLife", &m_LifeSlider, 40, 300) == true)
	{
		m_ParticleGlobal->MaxLife = m_LifeSlider;

		m_ChangeParticle = true;
	}

	if (ImGui::SliderFloat("ParticleSpeed", &m_SpeedSlider, 0.1f, 5.0f) == true)
	{
		m_ParticleGlobal->SpeedFactor = m_SpeedSlider;

		m_ChangeParticle = true;
	}

	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_ParticleGlobal->IsEnableGravity = m_IsEnableGravity;

		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_ParticleGlobal->GravityFactor = m_GravityStrengthSlider * 1.0f / 60.0f;

			m_ChangeParticle = true;
		}

		m_ChangeParticle = true;
	}

	ImGui::End();

	if (m_ChangeParticle)
	{
		Renderer::GetDeviceContext()->UpdateSubresource(m_ParticleGlobalBuffer, 0, NULL, m_ParticleGlobal, 0, 0);
		m_ChangeParticle = false;
	}

	//Zバッファ無効
	Renderer::SetDepthEnable(false);

	//加算合成有効
	Renderer::SetBlendAddEnable(true);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x / 10, m_Scale.y / 10, m_Scale.z / 10);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
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


//パーティクルの個別設定を生成する
void Particle::CreateParticleLocal(int ParticleAmount)
{
	//パーティクルの個別設定の枠を生成（m_ParticleAmountは最大数）
	m_ParticleAmount = ParticleAmount;
	m_ParticleLocal = new PARTICLE_LOCAL_CONFIG[m_ParticleAmount];

	//パーティクルの個別設定を作成
	for (int i = 0; i < m_ParticleAmount; i++)
	{
		//原点に位置を設定
		m_ParticleLocal[i].Position = { 0.0f, 0.0f, 0.0f };

		//発射方向をランダムで設定(球)
		//m_ParticleLocal[i].ShootDirection = { (float)(rand() % 100 - 50) / 100.0f, (float)(rand() % 100 - 50) / 100.0f, (float)(rand() % 100 - 50) / 100.0f }; //速度

		//発射方向をランダムで設定(打ち上げ)
		m_ParticleLocal[i].ShootDirection = { (float)(rand() % 100 - 50) / 100.0f, 0.5f, (float)(rand() % 100 - 50) / 100.0f }; //速度
		//m_ParticleLocal[i].ShootDirection = { 0.0f, 0.5f, 0.0f }; //速度

		////発射方向が0の場合他の値を設定
		//if (m_ParticleLocal[i].ShootDirection.x == 0.0f && m_ParticleLocal[i].ShootDirection.y == 0.0f && m_ParticleLocal[i].ShootDirection.z == 0.0f)
		//{
		//	m_ParticleLocal[i].ShootDirection = { 0.6f, 0.3f, 0.6f };
		//}

		//発射方向を正規化
		XMStoreFloat3(&m_ParticleLocal[i].ShootDirection, XMVector3Normalize(XMLoadFloat3(&m_ParticleLocal[i].ShootDirection)));


		//初速度を設定
		XMStoreFloat3(&m_ParticleLocal[i].Velocity, XMLoadFloat3(&m_ParticleLocal[i].ShootDirection) * m_ParticleGlobal->SpeedFactor);
		
		//加速度を設定
		m_ParticleLocal[i].Acceleration = {};


		//現在ライフ設定
		m_ParticleLocal[i].Life = m_ParticleGlobal->MaxLife;
	}
}


//パーティクルの全体設定を生成する
void Particle::CreateParticleGlobal()
{
	//パーティクルの全体設定の枠を生成
	m_ParticleGlobal = new PARTICLE_GLOBAL_CONFIG();

	//パーティクルの全体設定を作成
	//最大寿命を設定
	m_ParticleGlobal->MaxLife = 120.0f;

	//速度の初期値を設定
	m_ParticleGlobal->SpeedFactor = 1.0f;

	//重力の使用フラグの初期値を設定
	m_ParticleGlobal->IsEnableGravity = false;

	//重力の初期値を設定
	m_ParticleGlobal->GravityFactor = 1.0f * 1.0f / 60.0f;
}


//変更可能ステータスを設定
void Particle::SetModifiableStatus()
{
	//ライフの変更用スライダーのデフォルト値
	m_LifeSlider = 120;

	//速度の変更用スレイダーのデフォルト値
	m_SpeedSlider = 1.0f;

	//重力の強さの変更用スレイダーのデフォルト値
	m_GravityStrengthSlider = 1.0f;
}


