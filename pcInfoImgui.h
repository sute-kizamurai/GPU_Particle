#pragma once

////�C���N���[�h
#include <vector>

#include "imguiBase.h"
#include "gameObject.h"

////�\���̐錾
struct PC_INFOMATION
{
	float Fps;

};

class PcInfoImgui : public GameObject
{
private:
	//�p�[�e�B�N���p��ImGui���i�[����ϐ�
	std::vector<ImguiBase*> m_Element;

	//�R���s���[�g�V�F�[�_��PC�̊�{���𑗐M���邽�߂̍\����
	PC_INFOMATION* m_PcInfomation;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

