#pragma once

#include "gameObject.h"
#include "particle.h"

class ParticleImgui : public GameObject
{
private:
	//���g�̒S������I�u�W�F�N�g���i�[
	Particle* m_Parent;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};