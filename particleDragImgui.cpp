#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleDragImgui.h"

void ParticleDragImgui::Init()
{
	//�d�͎g�p�t���O�̃f�t�H���g�l���i�[
	m_IsEnableDrag = false;

	//�d�͂̋����ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_DragStrengthSlider = 0.5f;
}

void ParticleDragImgui::Draw()
{
	//�p�[�e�B�N���̏d�͂�ύX�ł���X���C�_�[��\��
	if (ImGui::Checkbox("IsEnableDrag", &m_IsEnableDrag) || m_IsEnableDrag)
	{
		m_Target->SetIsEnableDrag(m_IsEnableDrag);

		if (ImGui::SliderFloat("DragStrength", &m_DragStrengthSlider, 0.0f, 1.0f) == true)
		{
			m_Target->SetDragFactor(m_DragStrengthSlider);
		}
	}
}
