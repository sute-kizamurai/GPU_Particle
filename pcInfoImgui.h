#pragma once

////�C���N���[�h
#include <vector>

#include "imguiBase.h"
#include "gameObject.h"

class PcInfoImgui : public GameObject
{
private:
	//�p�[�e�B�N���p��ImGui���i�[����ϐ�
	std::vector<ImguiBase*> m_Element;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
};

