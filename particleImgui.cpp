#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleImgui.h"

void ParticleImgui::Init()
{
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//変更可能ステータスの値を初期化
	SetModifiableStatus();
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

	//階層構造を作成
	ChangeSelfStatus();

	if(ImGui::TreeNode("ExternalStatus"))
	{
		ChangeExternalStatus();
		ImGui::TreePop;
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


//自身のステータス変更用の関数
void ParticleImgui::ChangeSelfStatus()
{
	if (ImGui::TreeNode("SelfStatus"))
	{
		//寿命の変更用ImGui
		if (ImGui::SliderInt("ParticleMaxLife", &m_LifeSlider, 40, 300) == true)
		{
			m_Target->SetMaxLife(m_LifeSlider);
		}

		//発射速度の変更用ImGui
		if (ImGui::SliderFloat("ParticleSpeed", &m_SpeedSlider, 0.1f, 5.0f) == true)
		{
			m_Target->SetSpeedFactor(m_SpeedSlider);
		}
		ImGui::TreePop;
	}
}

//外部ステータス変更用の関数
void ParticleImgui::ChangeExternalStatus()
{
	//重力の使用フラグ変更用ImGui
	if (ImGui::Checkbox("IsEnableGravity", &m_IsEnableGravity) || m_IsEnableGravity)
	{
		m_Target->SetIsEnableGravity(m_IsEnableGravity);


		//重力の強さ変更用ImGui
		if (ImGui::SliderFloat("GravityStrength", &m_GravityStrengthSlider, -10.0f, 10.0) == true)
		{
			m_Target->SetGravityFactor(m_GravityStrengthSlider);
		}
	}
}
