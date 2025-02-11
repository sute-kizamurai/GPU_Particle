
////�C���N���[�h
#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleImgui.h"

#include "particleLifeImgui.h"
#include "particleSpeedImgui.h"

void ParticleImgui::Init()
{
	//�p�[�e�B�N���̃A�h���X���i�[
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//ImGui�̗v�f��ǉ�
	m_Element.emplace_back(new ParticleLifeImgui(m_Target, "Life"));
	m_Element.emplace_back(new ParticleSpeedImgui(m_Target, "ShootingSpeed"));

	//�ύX�\�X�e�[�^�X�̒l��������
	SetModifiableStatus();

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


	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_Target->SetIsEnableGravity(m_IsEnableGravity);

		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_Target->SetGravityFactor(m_GravityStrengthSlider);
		}
	}

	ImGui::End();
}


//�ύX�\�X�e�[�^�X�̏����l��ݒ�
void ParticleImgui::SetModifiableStatus()
{
	//�d�͎g�p�t���O�̃f�t�H���g�l
	m_IsEnableGravity = false;

	//�d�͂̋����̕ύX�p�X���C�_�[�̃f�t�H���g�l
	m_GravityStrengthSlider = 1.0f;
}
