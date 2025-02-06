#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleImgui.h"

void ParticleImgui::Init()
{
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//�ύX�\�X�e�[�^�X�̒l��������
	SetModifiableStatus();
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

	//�K�w�\�����쐬
	ChangeSelfStatus();

	if(ImGui::TreeNode("ExternalStatus"))
	{
		ChangeExternalStatus();
		ImGui::TreePop;
	}

	ImGui::End();
}


//�ύX�\�X�e�[�^�X�̏����l��ݒ�
void ParticleImgui::SetModifiableStatus()
{
	//���C�t�̕ύX�p�X���C�_�[�̃f�t�H���g�l
	m_LifeSlider = 120;

	//���x�̕ύX�p�X���C�_�[�̃f�t�H���g�l
	m_SpeedSlider = 1.0f;

	//�d�͎g�p�t���O�̃f�t�H���g�l
	m_IsEnableGravity = false;

	//�d�͂̋����̕ύX�p�X���C�_�[�̃f�t�H���g�l
	m_GravityStrengthSlider = 1.0f;
}


//���g�̃X�e�[�^�X�ύX�p�̊֐�
void ParticleImgui::ChangeSelfStatus()
{
	if (ImGui::TreeNode("SelfStatus"))
	{
		//�����̕ύX�pImGui
		if (ImGui::SliderInt("ParticleMaxLife", &m_LifeSlider, 40, 300) == true)
		{
			m_Target->SetMaxLife(m_LifeSlider);
		}

		//���ˑ��x�̕ύX�pImGui
		if (ImGui::SliderFloat("ParticleSpeed", &m_SpeedSlider, 0.1f, 5.0f) == true)
		{
			m_Target->SetSpeedFactor(m_SpeedSlider);
		}
		ImGui::TreePop;
	}
}

//�O���X�e�[�^�X�ύX�p�̊֐�
void ParticleImgui::ChangeExternalStatus()
{
	//�d�͂̎g�p�t���O�ύX�pImGui
	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_Target->SetIsEnableGravity(m_IsEnableGravity);


		//�d�͂̋����ύX�pImGui
		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_Target->SetGravityFactor(m_GravityStrengthSlider);
		}
	}
}
