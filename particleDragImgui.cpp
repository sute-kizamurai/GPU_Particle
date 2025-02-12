#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleDragImgui.h"

void ParticleDragImgui::Init()
{
	//重力使用フラグのデフォルト値を格納
	m_IsEnableDrag = false;

	//重力の強さ変更用スレイダーのデフォルト値を格納
	m_DragStrengthSlider = 0.5f;
}

void ParticleDragImgui::Draw()
{
	//パーティクルの重力を変更できるスライダーを表示
	if (ImGui::Checkbox("IsEnableDrag", &m_IsEnableDrag) || m_IsEnableDrag)
	{
		m_Target->SetIsEnableDrag(m_IsEnableDrag);

		if (ImGui::SliderFloat("DragStrength", &m_DragStrengthSlider, 0.0f, 1.0f) == true)
		{
			m_Target->SetDragFactor(m_DragStrengthSlider);
		}
	}
}
