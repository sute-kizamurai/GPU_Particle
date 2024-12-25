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
	//回転
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
	//ビューマトリクス設定
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&up));

	Renderer::SetViewMatrix(viewMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	//カメラの逆行列を作成
	XMMATRIX invView = XMMatrixInverse(nullptr, viewMatrix);

	//カメラの右方向ベクトルを作成
	XMFLOAT3 right;
	XMStoreFloat3(&right, invView.r[0]);

	//カメラの上方向ベクトルを作成
	XMStoreFloat3(&up, invView.r[1]);

	//カメラの前方向ベクトルを作成
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, invView.r[2] * -1.0f);

	CAMERA camera = { m_Position, forward, right, up };
	Renderer::SetCamera(camera);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(projectionMatrix);
}
