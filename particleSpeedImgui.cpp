#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleSpeedImgui.h"

void ParticleSpeedImgui::Init()
{
	//���x�ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_SpeedSlider = 1.0f;
}

void ParticleSpeedImgui::Draw()
{
	//�p�[�e�B�N���̑��x��ύX�ł���X���C�_�[��\��
	if (ImGui::SliderFloat("ParticleSpeed", &m_SpeedSlider, 0.1f, 5.0f) == true)
	{
		m_Target->SetSpeedFactor(m_SpeedSlider);
	}
}
