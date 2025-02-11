#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleGravityImgui.h"

void ParticleGravityImgui::Init()
{
	//重力使用フラグのデフォルト値を格納
	m_IsEnableGravity = false;

	//重力の強さ変更用スレイダーのデフォルト値を格納
	m_GravityStrengthSlider = 1.0f;
}

void ParticleGravityImgui::Draw()
{
	//パーティクルの重力を変更できるスライダーを表示
	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_Target->SetIsEnableGravity(m_IsEnableGravity);

		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_Target->SetGravityFactor(m_GravityStrengthSlider);
		}
	}
}
