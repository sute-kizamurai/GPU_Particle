#include "main.h"
#include "manager.h"
#include "particle.h"
#include "particleShotIntervalImgui.h"

void ParticleShotIntervalImgui::Init()
{
	//���ˋ��ϐ��̏����l���i�[
	m_Fireable = 1; //���ˋ���
	m_Target->SetFireable(m_Fireable);

	//�O�񔭎˂���̌o�ߎ��Ԃ��i�[
	m_ElapsedTime = 0.0f;

	//���ˊԊu�ύX�p�X���C�_�[�̃f�t�H���g�l���i�[
	m_ShotInterval = 4.0f;
}

void ParticleShotIntervalImgui::Draw()
{
	//�p�[�e�B�N���̔��ˊԊu��ύX�ł���X���C�_�[��\��
	ImGui::SliderFloat("ParticleMaxLife", &m_ShotInterval, 0.0f, 20.0f, "%.1f");


	if (m_ElapsedTime == 0.0f)
	{
		m_Fireable = 0; //���˕s����
		m_Target->SetFireable(m_Fireable);
	}

	//�G�~�b�^�[�̑O�񔭎˂���̌o�ߎ��Ԃ��X�V
	float fps = GetFps();
	if (fps < 30.0f)
		fps = 30.0f;
	m_ElapsedTime += 1.0f / fps;


	if (m_ElapsedTime >= m_ShotInterval)
	{
		m_Fireable = 1; //���ˋ���
		m_ElapsedTime = 0.0f;
		m_Target->SetFireable(m_Fireable);
	}


}
