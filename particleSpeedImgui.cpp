#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleSpeedImgui.h"

void ParticleSpeedImgui::Init()
{
	//速度変更用スライダーのデフォルト値を格納
	m_SpeedSlider = 1.0f;
}

void ParticleSpeedImgui::Draw()
{
	//パーティクルの速度を変更できるスライダーを表示
	if (ImGui::SliderFloat("ParticleSpeed", &m_SpeedSlider, 0.1f, 5.0f) == true)
	{
		m_Target->SetSpeedFactor(m_SpeedSlider);
	}
}
