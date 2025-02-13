#pragma once

////�C���N���[�h
#include <vector>

#include "gameObject.h"

////�\���̐錾
struct PC_INFOMATION
{
	float Fps;
	XMFLOAT3 Dummy;
};


////�O���錾
class ImguiBase;

class PcInfoImgui : public GameObject
{
private:
	//�p�[�e�B�N���p��ImGui���i�[����ϐ�
	std::vector<ImguiBase*> m_Element;

	//�R���s���[�g�V�F�[�_��PC�̊�{���𑗐M���邽�߂̍\����
	PC_INFOMATION* m_PcInfomation;

	//�R���s���[�g�V�F�[�_��PC�̊�{���𑗐M���邽�߂̃o�b�t�@
	ID3D11Buffer* m_PcInfoBuffer;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//�Z�b�^�[���Q�b�^�[
	void SetFps(float Fps) { m_PcInfomation->Fps = Fps; }
};

