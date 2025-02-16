#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleShotNumImgui.h"

void ParticleShotNumImgui::Init()
{
	//寿命変更用スライダーのデフォルト値を格納
	m_ShotNum = 10;
}

void ParticleShotNumImgui::Draw()
{
	//パーティクルの寿命を変更できるスライダーを表示
	if (ImGui::InputInt("ParticleShotNum", &m_ShotNum, 1, 100) == true)
	{
		if (m_ShotNum <= 0)
		{
			m_ShotNum = 1;
		}
		else if(m_ShotNum >= 1024 * 512)
		{
			m_ShotNum = 1024 * 512;
		}

		m_Target->SetShotNum(m_ShotNum);
	}
}
