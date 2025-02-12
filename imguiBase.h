#pragma once

////�C���N���[�h
#include <string>

#include "particle.h"


class ImguiBase
{
protected:
	//�p�[�e�B�N���̃A�h���X���i�[
	Particle* m_Target;

	//ImGui�̊K�w�\���ɕt���閼�O
	std::string m_ImguiTreeName;

public:
	ImguiBase() = delete;
	ImguiBase(Particle* Target, std::string TreeName) 
	{
		m_Target = Target;
		m_ImguiTreeName = TreeName;
	}
	ImguiBase(std::string TreeName) { m_ImguiTreeName = TreeName; }


	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() = 0;

	std::string GetImguiTreeName() { return m_ImguiTreeName; }
};
