#pragma once

#include "component.h"
#include "particle.h"

class ParticleImgui : public Component
{
private:
	//���g�̒S������I�u�W�F�N�g���i�[
	Particle* m_Target;

public:
	using Component::Component;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

};