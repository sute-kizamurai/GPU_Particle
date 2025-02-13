#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleLifeImgui.h"

void ParticleLifeImgui::Init()
{
	//�����ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_LifeSlider = 120;
}

void ParticleLifeImgui::Draw()
{
	//�p�[�e�B�N���̎�����ύX�ł���X���C�_�[��\��
	if (ImGui::SliderInt("ParticleMaxLife", &m_LifeSlider, 40, 300) == true)
	{
		m_Target->SetMaxLife(m_LifeSlider);
	}
}
