#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleGravityImgui.h"

void ParticleGravityImgui::Init()
{
	//�d�͎g�p�t���O�̃f�t�H���g�l���i�[
	m_IsEnableGravity = false;

	//�d�͂̋����ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_GravityStrengthSlider = 1.0f;
}

void ParticleGravityImgui::Draw()
{
	//�p�[�e�B�N���̏d�͂�ύX�ł���X���C�_�[��\��
	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_Target->SetIsEnableGravity(m_IsEnableGravity);

		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_Target->SetGravityFactor(m_GravityStrengthSlider);
		}
	}
}
