
////インクルード
#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleImgui.h"

#include "particleLifeImgui.h"

void ParticleImgui::Init()
{
	//パーティクルのアドレスを格納
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//ImGuiの要素を追加
	m_Element.emplace_back(new ParticleLifeImgui(m_Target, "Life"));

	//変更可能ステータスの値を初期化
	SetModifiableStatus();

	for (auto element : m_Element)
	{
		element->Init();
	}
}

void ParticleImgui::Uninit()
{
}

void ParticleImgui::Update()
{
}

void ParticleImgui::Draw()
{
	//ImGui設定
	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("ParticleStatus");

	ImGui::Text("Particle count : %d", m_Target->GetParticleAmount());

	for (auto element : m_Element)
	{
		if (ImGui::TreeNode(element->GetImguiTreeName().c_str()))
		{
			element->Draw();
			ImGui::TreePop();
		}
	}

	if (ImGui::SliderFloat("ParticleSpeed", &m_SpeedSlider, 0.1f, 5.0f) == true)
	{
		m_Target->SetSpeedFactor(m_SpeedSlider);
	}

	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_Target->SetIsEnableGravity(m_IsEnableGravity);

		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_Target->SetGravityFactor(m_GravityStrengthSlider);
		}
	}

	ImGui::End();
}


//変更可能ステータスの初期値を設定
void ParticleImgui::SetModifiableStatus()
{
	//ライフの変更用スライダーのデフォルト値
	m_LifeSlider = 120;

	//速度の変更用スレイダーのデフォルト値
	m_SpeedSlider = 1.0f;

	//重力使用フラグのデフォルト値
	m_IsEnableGravity = false;

	//重力の強さの変更用スレイダーのデフォルト値
	m_GravityStrengthSlider = 1.0f;
}
