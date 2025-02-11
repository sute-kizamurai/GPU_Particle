#pragma once

////�C���N���[�h
#include <vector>

#include "imguiBase.h"
#include "component.h"

////�O���錾
class Particle;

class ParticleImgui : public Component
{
private:
	//���g�̒S������I�u�W�F�N�g���i�[
	Particle* m_Target;

	//�p�[�e�B�N���p��ImGui���i�[����ϐ�
	std::vector<ImguiBase*> m_Element;

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};
