
////�C���N���[�h
#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleImgui.h"

#include "particleLifeImgui.h"
#include "particleSpeedImgui.h"
#include "particleGravityImgui.h"

void ParticleImgui::Init()
{
	//�p�[�e�B�N���̃A�h���X���i�[
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//ImGui�̗v�f��ǉ�
	m_Element.emplace_back(new ParticleLifeImgui(m_Target, "Life"));
	m_Element.emplace_back(new ParticleSpeedImgui(m_Target, "ShootingSpeed"));
	m_Element.emplace_back(new ParticleGravityImgui(m_Target, "Gravity"));

	//ImGui�̗v�f��������
	for (auto element : m_Element)
	{
		element->Init();
	}
}

void ParticleImgui::Uninit()
{
}

void ParticleImgui::Update()
{
}

void ParticleImgui::Draw()
{
	//ImGui�ݒ�
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("ParticleStatus");

	ImGui::Text("Particle count : %d", m_Target->GetParticleAmount());

	for (auto element : m_Element)
	{
		if (ImGui::TreeNode(element->GetImguiTreeName().c_str()))
		{
			element->Draw();
			ImGui::TreePop();
		}
	}

	ImGui::End();
}
