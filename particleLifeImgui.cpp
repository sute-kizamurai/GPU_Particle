#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleLifeImgui.h"

void ParticleLifeImgui::Init()
{
	//�����ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_LifeSlider = 5.0f;
}

void ParticleLifeImgui::Draw()
{
	//�p�[�e�B�N���̎�����ύX�ł���X���C�_�[��\��
	if (ImGui::SliderFloat("ParticleMaxLife", &m_LifeSlider, 0.1f, 20.0f, "%.1f") == true)
	{
		m_Target->SetMaxLife(m_LifeSlider);
	}
}
