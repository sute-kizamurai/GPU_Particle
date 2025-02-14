#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "particle.h"
#include "particleImgui.h"


void Particle::Init()
{
	//�p�[�e�B�N���T�C�Y������
	m_Size = { 5.0f, 5.0f };

	//�p�[�e�B�N���̐F����������
	m_ParticleColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_Component.emplace_back(new ParticleImgui(this));
	m_Component[0]->Init();

	//���_�����������
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

	//���_�o�b�t�@����
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	

	//�p�[�e�B�N���̑S�̐ݒ�𐶐�
	CreateParticleGlobal();

	//�p�[�e�B�N���̍ő吶������ݒ�
	CreateParticleMaxCapacity(1024 * 216);

	//�p�[�e�B�N���̓��e�̕ύX���Ȃ�����false
	m_ChangeParticle = false;


	//�\���̃o�b�t�@����
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bd.ByteWidth = sizeof(PARTICLE_LOCAL_CONFIG) * m_ParticleAmount;
	bd.StructureByteStride = sizeof(PARTICLE_LOCAL_CONFIG);
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_ParticleLocal;

	//�p�[�e�B�N���̌ʐݒ���͗p�̃o�b�t�@�쐬
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleLocalBuffer);

	//�p�[�e�B�N���̌ʐݒ�o�͗p��VertexShader���͗p�̃o�b�t�@�쐬
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

	//�p�[�e�B�N���̑S�̐ݒ���͗p�̃o�b�t�@�쐬
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_ParticleGlobalBuffer);

	//�萔�o�b�t�@��ComputeShader�Ƀo�C���h
	Renderer::GetDeviceContext()->CSSetConstantBuffers(6, 1, &m_ParticleGlobalBuffer);


	//SRV����
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_UNKNOWN; // �\���̃o�b�t�@�̏ꍇ��UNKNOWN
	srv.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv.Buffer.FirstElement = 0;
	srv.Buffer.NumElements = m_ParticleAmount;
	Renderer::GetDevice()->CreateShaderResourceView(m_ParticleLocalBuffer, &srv, &m_ParticleLocalSRV);
	Renderer::GetDevice()->CreateShaderResourceView(m_ResultBuffer, &srv, &m_ResultSRV);

	//UAV����
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav = {};
	uav.Format = DXGI_FORMAT_UNKNOWN;
	uav.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav.Buffer.FirstElement = 0;
	uav.Buffer.NumElements = m_ParticleAmount;
	Renderer::GetDevice()->CreateUnorderedAccessView(m_ParticleLocalBuffer, &uav, &m_ParticleLocalUAV);
	Renderer::GetDevice()->CreateUnorderedAccessView(m_ResultBuffer, &uav, &m_ResultUAV);


	//�p�[�e�B�N���X�V�p�R���s���[�g�V�F�[�_�\�쐬
	Renderer::CreateComputeShader(&m_ComputeShader, "shader\\particleCS.cso");
	//�p�[�e�B�N���������p�R���s���[�g�V�F�[�_�[�쐬
	Renderer::CreateComputeShader(&m_ParticleInitialShader, "shader\\particleInitialCS.cso");
	//�o�b�t�@�Ԃ̃f�[�^����ւ��p�̃R���s���[�g�V�F�[�_�쐬
	Renderer::CreateComputeShader(&m_PingPongShader, "shader\\pingPongCS.cso");


	//�p�[�e�B�N��������
	Renderer::GetDeviceContext()->CSSetShader(m_ParticleInitialShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleLocalUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	// ���\�[�X������
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);


	//�W�I���g���V�F�[�_�쐬
	Renderer::CreateGeometryShader(&m_GeometryShader, "shader\\particleGS.cso");


	//�e�N�X�`���ǂݍ���
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
	//���������
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
	delete m_ParticleGlobal;

	m_VertexBuffer->Release();
	m_ParticleLocalBuffer->Release();
	m_ParticleGlobalBuffer->Release();
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
	//�p�[�e�B�N���p�R���s���[�g�V�F�[�_�[���s
	Renderer::GetDeviceContext()->CSSetShader(m_ComputeShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_ParticleLocalSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ResultUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	//GPU���Ɠ��������
	Renderer::GetDeviceContext()->Flush();

	// ���\�[�X������
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	ID3D11UnorderedAccessView* nullUAV[1] = { nullptr };
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, nullSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

	//ComputeShader���g�p���o�͗p�o�b�t�@������͗p�o�b�t�@�ɒl���󂯓n��
	Renderer::GetDeviceContext()->CSSetShader(m_PingPongShader, nullptr, 0);
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, &m_ResultSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_ParticleLocalUAV, nullptr);
	Renderer::GetDeviceContext()->Dispatch(m_ParticleAmount / 1024, 1, 1);

	// ���\�[�X������
	Renderer::GetDeviceContext()->CSSetShaderResources(0, 1, nullSRV);
	Renderer::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
}

void Particle::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->GSSetShader(m_GeometryShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	//�J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	XMMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//�t�s��
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));

	//�}�e���A�����Â߂̔��ɕύX
	material.Diffuse = m_ParticleColor;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_Component[0]->Draw();

	if (m_ChangeParticle)
	{
		Renderer::GetDeviceContext()->UpdateSubresource(m_ParticleGlobalBuffer, 0, NULL, m_ParticleGlobal, 0, 0);
		m_ChangeParticle = false;
	}

	//Z�o�b�t�@����
	Renderer::SetDepthEnable(false);

	//���Z�����L��
	Renderer::SetBlendAddEnable(true);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x / 10, m_Scale.y / 10, m_Scale.z / 10);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(world);

	//VS�ɑ�����W���
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_ResultSRV);

	//�|���S���`��
	Renderer::GetDeviceContext()->DrawInstanced(4, m_ParticleAmount, 0, 0);

	//Z�o�b�t�@�L��
	Renderer::SetDepthEnable(true);

	//���Z��������
	Renderer::SetBlendAddEnable(false);

	// ���\�[�X������
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, nullSRV);
}


//�p�[�e�B�N���̍ő吶������ݒ肷��
void Particle::CreateParticleMaxCapacity(int ParticleAmount)
{
	//�p�[�e�B�N���̍ő吶���g���쐬
	m_ParticleAmount = ParticleAmount;
	m_ParticleLocal = new PARTICLE_LOCAL_CONFIG[m_ParticleAmount];
}

//�p�[�e�B�N���̑S�̐ݒ�𐶐�����
void Particle::CreateParticleGlobal()
{
	//�p�[�e�B�N���̑S�̐ݒ�̘g�𐶐�
	m_ParticleGlobal = new PARTICLE_GLOBAL_CONFIG();

	//�p�[�e�B�N���̑S�̐ݒ���쐬
	//�p�[�e�B�N���̔��˕��@�����肷��␳�l��ݒ�
	m_ParticleGlobal->ShootingMethod = { 2.0f, 1.0f };

	//�ő������ݒ�
	m_ParticleGlobal->MaxLife = 5.0f;

	//���x�̏����l��ݒ�
	m_ParticleGlobal->SpeedFactor = 1.0f;

	//�d�͂̎g�p�t���O�̏����l��ݒ�
	m_ParticleGlobal->IsEnableGravity = false;

	//�d�͂̏����l��ݒ�
	m_ParticleGlobal->GravityFactor = 1.0f;

	//��R�͂̎g�p�t���O�̏����l��ݒ�
	m_ParticleGlobal->IsEnableDrag = false;

	//��R�͂̏����l��ݒ�
	m_ParticleGlobal->DragFactor = 0.5f;
}


