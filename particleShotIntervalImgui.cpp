#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleShotIntervalImgui.h"

void ParticleShotIntervalImgui::Init()
{
	//発射許可変数の初期値を格納
	m_Fireable = 1; //発射許可
	m_Target->SetFireable(m_Fireable);

	//前回発射からの経過時間を格納
	m_ElapsedTime = 0.0f;

	//発射間隔変更用スライダーのデフォルト値を格納
	m_ShotInterval = 4.0f;
}

void ParticleShotIntervalImgui::Draw()
{
	//パーティクルの発射間隔を変更できるスライダーを表示
	ImGui::SliderFloat("ParticleMaxLife", &m_ShotInterval, 0.0f, 20.0f, "%.1f");


	if (m_ElapsedTime == 0.0f)
	{
		m_Fireable = 0; //発射不許可
		m_Target->SetFireable(m_Fireable);
	}

	//エミッターの前回発射からの経過時間を更新
	float fps = GetFps();
	if (fps < 30.0f)
		fps = 30.0f;
	m_ElapsedTime += 1.0f / fps;


	if (m_ElapsedTime >= m_ShotInterval)
	{
		m_Fireable = 1; //発射許可
		m_ElapsedTime = 0.0f;
		m_Target->SetFireable(m_Fireable);
	}


}
