
////�C���N���[�h
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "imguiBase.h"
#include "pcInfoImgui.h"
#include "pcFpsImgui.h"

void PcInfoImgui::Init()
{
	//ImGui�̗v�f��ǉ�
	m_Element.emplace_back(new PcFpsImgui(this, "FPS"));

	//ImGui�̗v�f��������
	for (auto element : m_Element)
	{
		element->Init();
	}

	m_PcInfomation = new PC_INFOMATION();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(PC_INFOMATION);
	bd.StructureByteStride = sizeof(float);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.MiscFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = m_PcInfomation;

	//�p�[�e�B�N���̑S�̐ݒ���͗p�̃o�b�t�@�쐬
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_PcInfoBuffer);

	//�萔�o�b�t�@��ComputeShader�Ƀo�C���h
	Renderer::GetDeviceContext()->CSSetConstantBuffers(7, 1, &m_PcInfoBuffer);
}

void PcInfoImgui::Uninit()
{
	//�������̊J��
	for (auto element : m_Element)
	{
		element->Uninit();
		delete element;
	}

	delete m_PcInfomation;

	m_PcInfoBuffer->Release();
}

void PcInfoImgui::Update()
{
	Renderer::GetDeviceContext()->UpdateSubresource(m_PcInfoBuffer, 0, NULL, m_PcInfomation, 0, 0);
}

void PcInfoImgui::Draw()
{
	//ImGui�ݒ�
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(200.0f, 200.0f));
	ImGui::Begin("PC Infomation");
	for (auto element : m_Element)
	{
		element->Draw();
	}

	ImGui::End();
}
