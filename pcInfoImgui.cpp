
////�C���N���[�h
#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "pcInfoImgui.h"
#include "pcFpsImgui.h"

void PcInfoImgui::Init()
{
	//ImGui�̗v�f��ǉ�
	m_Element.emplace_back(new PcFpsImgui("FPS"));

	//ImGui�̗v�f��������
	for (auto element : m_Element)
	{
		element->Init();
	}
}

void PcInfoImgui::Uninit()
{
	//�������̊J��
	for (auto element : m_Element)
	{
		element->Uninit();
		delete element;
	}
}

void PcInfoImgui::Update()
{
}

void PcInfoImgui::Draw()
{
	//ImGui�ݒ�
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("PC Infomation");
	for (auto element : m_Element)
	{
		element->Draw();
	}

	ImGui::End();
}
