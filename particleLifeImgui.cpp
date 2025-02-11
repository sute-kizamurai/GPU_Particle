#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleLifeImgui.h"

void ParticleLifeImgui::Init()
{
	//�����̕ύX�p�X���C�_�[��������
	m_LifeSlider = 120;
}

void ParticleLifeImgui::Draw()
{
	if (ImGui::SliderInt("ParticleMaxLife", &m_LifeSlider, 40, 300) == true)
	{
		m_Target->SetMaxLife(m_LifeSlider);
	}
}
