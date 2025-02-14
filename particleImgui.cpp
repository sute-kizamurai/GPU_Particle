
////�C���N���[�h
#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleImgui.h"

#include "particleShootingMethodImgui.h"
#include "particleLifeImgui.h"
#include "particleSpeedImgui.h"
#include "particleGravityImgui.h"
#include "particleColorImgui.h"
#include "particleDragImgui.h"

void ParticleImgui::Init()
{
	//�p�[�e�B�N���̃A�h���X���i�[
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//ImGui�̗v�f��ǉ�
	m_Element.emplace_back(new ParticleShootingMethodImgui(m_Target, "ShootingMethod"));
	m_Element.emplace_back(new ParticleLifeImgui(m_Target, "Life(second)"));
	m_Element.emplace_back(new ParticleSpeedImgui(m_Target, "ShootingSpeed"));
	m_Element.emplace_back(new ParticleColorImgui(m_Target, "ParticleColor"));
	m_Element.emplace_back(new ParticleGravityImgui(m_Target, "Gravity"));
	m_Element.emplace_back(new ParticleDragImgui(m_Target, "Drag"));

	//ImGui�̗v�f��������
	for (auto element : m_Element)
	{
		element->Init();
	}
}

void ParticleImgui::Uninit()
{
	//�������̊J��
	for (auto element : m_Element)
	{
		element->Uninit();
		delete element;
	}
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
