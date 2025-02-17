#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleLifeImgui.h"

void ParticleLifeImgui::Init()
{
	//寿命変更用スライダーのデフォルト値を格納
	m_LifeSlider = 5.0f;
}

void ParticleLifeImgui::Draw()
{
	//パーティクルの寿命を変更できるスライダーを表示
	if (ImGui::SliderFloat("ParticleMaxLife", &m_LifeSlider, 0.1f, 20.0f, "%.1f") == true)
	{
		m_Target->SetMaxLife(m_LifeSlider);
	}
}
