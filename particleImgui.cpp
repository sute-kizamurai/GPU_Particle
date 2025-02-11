
////インクルード
#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleImgui.h"

#include "particleLifeImgui.h"
#include "particleSpeedImgui.h"
#include "particleGravityImgui.h"

void ParticleImgui::Init()
{
	//パーティクルのアドレスを格納
	m_Target = dynamic_cast<Particle*>(m_GameObject);

	//ImGuiの要素を追加
	m_Element.emplace_back(new ParticleLifeImgui(m_Target, "Life"));
	m_Element.emplace_back(new ParticleSpeedImgui(m_Target, "ShootingSpeed"));
	m_Element.emplace_back(new ParticleGravityImgui(m_Target, "Gravity"));

	//ImGuiの要素を初期化
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

	ImGui::End();
}
