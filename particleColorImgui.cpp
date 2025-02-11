#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleColorImgui.h"

void ParticleColorImgui::Init()
{
	//パーティクルの色のデフォルト値を格納
	m_ParticleColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	//ImGuiカラーピッカーから取得できる値を初期化
	m_Buf[0] = 1.0f;
	m_Buf[1] = 1.0f;
	m_Buf[2] = 1.0f;
	m_Buf[3] = 1.0f;
}

void ParticleColorImgui::Draw()
{
	//パーティクルの色を変更できるスライダーを表示
	if (ImGui::ColorEdit4("ParticleColor", m_Buf))
	{
		m_ParticleColor.x = m_Buf[0];
		m_ParticleColor.y = m_Buf[1];
		m_ParticleColor.z = m_Buf[2];
		m_ParticleColor.w = m_Buf[3];

		m_Target->SetParticleColor(m_ParticleColor);
	}
}
