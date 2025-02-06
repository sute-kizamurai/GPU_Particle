#pragma once

#include "component.h"

//�O���錾
class Particle;

class ParticleImgui : public Component
{
private:
	//���g�̒S������I�u�W�F�N�g���i�[
	Particle* m_Target;


	//�p�[�e�B�N���̕ύX�\�X�e�[�^�X/////////////////////////
	////���g�̃X�e�[�^�X////
	//���C�t
	int m_LifeSlider{};
	//���x
	float m_SpeedSlider{};
	//�p�[�e�B�N���̐F
	XMFLOAT4 m_Color{};

	////�O���X�e�[�^�X////
	//�d�͂̎g�p�t���O
	bool m_IsEnableGravity{};
	//�d�͂̋���
	float m_GravityStrengthSlider{};
	///////////////////////////////////////////////////////////

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:
	//�ύX�\�X�e�[�^�X�̏����l��ݒ�
	void SetModifiableStatus();


	//���g�̃X�e�[�^�X�ύX�p�̊֐�
	void ChangeSelfStatus();

	//�O���X�e�[�^�X�ύX�p�̊֐�
	void ChangeExternalStatus();

};
