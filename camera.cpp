#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "camera.h"


void Camera::Init()
{
	m_Position = XMFLOAT3(0.0f, 10.0f, -30.0f);
	m_Target = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Distance = 100.0f;
}
 
void Camera::Uninit()
{
}

void Camera::Update()
{
	//��]
	if (Input::GetKeyPress(VK_LEFT))
	{
		m_Angle += 0.05f;
	}
	if (Input::GetKeyPress(VK_RIGHT))
	{
		m_Angle -= 0.05f;
	}
	if (Input::GetKeyPress(VK_UP))
	{
		m_Distance -= 1.0f;
	}
	if (Input::GetKeyPress(VK_DOWN))
	{
		m_Distance += 1.0f;
	}

	m_Position.x = sinf(m_Angle) * m_Distance;
	m_Position.z = cosf(m_Angle) * m_Distance;

}

void Camera::Draw()
{
	//�r���[�}�g���N�X�ݒ�
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//�J�����̋t�s����쐬
	XMMATRIX invView = XMMatrixInverse(nullptr, viewMatrix);

	//�J�����̉E�����x�N�g�����쐬
	XMFLOAT3 right;
	XMStoreFloat3(&right, invView.r[0]);

	//�J�����̏�����x�N�g�����쐬
	XMStoreFloat3(&up, invView.r[1]);

	//�J�����̑O�����x�N�g�����쐬
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, invView.r[2] * -1.0f);

	CAMERA camera = { m_Position, forward, right, up };
	Renderer::SetCamera(camera);

	//�v���W�F�N�V�����}�g���N�X�ݒ�
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);
}
