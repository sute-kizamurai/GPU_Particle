#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleDragImgui.h"

void ParticleDragImgui::Init()
{
	//��R�͎g�p�t���O�̃f�t�H���g�l���i�[
	m_IsEnableDrag = false;

	//��R�͂̋����ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_DragStrengthSlider = 0.5f;
}

void ParticleDragImgui::Draw()
{
	//�p�[�e�B�N���̒�R�͂�ύX�ł���`�F�b�N�{�b�N�X��\��
	if (ImGui::Checkbox("IsEnableDrag", &m_IsEnableDrag) || m_IsEnableDrag)
	{
		m_Target->SetIsEnableDrag(m_IsEnableDrag);

		//�p�[�e�B�N���̒�R�͂�ύX�ł���X���C�_�[��\��
		if (ImGui::SliderFloat("DragStrength", &m_DragStrengthSlider, 0.0f, 1.0f) == true)
		{
			m_Target->SetDragFactor(m_DragStrengthSlider);
		}
	}
}
