#pragma once

#include "gameObject.h"

class Camera : public GameObject
{
private:
	XMFLOAT3 m_Target{};

	XMFLOAT4X4 m_ViewMatrix{};

	class Audio* m_BGM{};

	float m_Angle{};
	float m_Distance{};

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	XMMATRIX GetViewMatrix()
	{
		return DirectX::XMLoadFloat4x4(&m_ViewMatrix);
	}
};
