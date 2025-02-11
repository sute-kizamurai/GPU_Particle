#include "main.h"
#include "manager.h"
#include "ImGui\\imgui.h"
#include "particle.h"
#include "particleColorImgui.h"

void ParticleColorImgui::Init()
{
	//�p�[�e�B�N���̐F�̃f�t�H���g�l���i�[
	m_ParticleColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	//ImGui�J���[�s�b�J�[����擾�ł���l��������
	m_Buf[0] = 1.0f;
	m_Buf[1] = 1.0f;
	m_Buf[2] = 1.0f;
	m_Buf[3] = 1.0f;
}

void ParticleColorImgui::Draw()
{
	//�p�[�e�B�N���̐F��ύX�ł���X���C�_�[��\��
	if (ImGui::ColorEdit4("ParticleColor", m_Buf))
	{
		m_ParticleColor.x = m_Buf[0];
		m_ParticleColor.y = m_Buf[1];
		m_ParticleColor.z = m_Buf[2];
		m_ParticleColor.w = m_Buf[3];

		m_Target->SetParticleColor(m_ParticleColor);
	}
}
