#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleShotNumImgui.h"

void ParticleShotNumImgui::Init()
{
	//�����ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_ShotNum = 10;
}

void ParticleShotNumImgui::Draw()
{
	//�p�[�e�B�N���̎�����ύX�ł���X���C�_�[��\��
	if (ImGui::InputInt("ParticleShotNum", &m_ShotNum, 1, 100) == true)
	{
		if (m_ShotNum <= 0)
		{
			m_ShotNum = 1;
		}
		else if(m_ShotNum >= 1024 * 512)
		{
			m_ShotNum = 1024 * 512;
		}

		m_Target->SetShotNum(m_ShotNum);
	}
}
