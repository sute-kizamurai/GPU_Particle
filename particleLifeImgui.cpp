#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleLifeImgui.h"

void ParticleLifeImgui::Init()
{
	//寿命変更用スライダーのデフォルト値を格納
	m_LifeSlider = 120;
}

void ParticleLifeImgui::Draw()
{
	//パーティクルの寿命を変更できるスライダーを表示
	if (ImGui::SliderInt("ParticleMaxLife", &m_LifeSlider, 40, 300) == true)
	{
		m_Target->SetMaxLife(m_LifeSlider);
	}
}
