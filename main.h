#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <io.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DXGI.lib")

#include <DirectXMath.h>
using namespace DirectX;


#include "DirectXTex.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DIrectXTEx_Release.lib")
#endif


//ImGuiインクルード
#include "ImGui\\imgui.h"
#include "ImGui\\imgui_impl_win32.h"
#include "ImGui\\imgui_impl_dx11.h"


#pragma comment (lib, "winmm.lib")

//------------------------------
//マクロ定義
//------------------------------
#define WINDOW_WIDTH	(1280)
#define WINDOW_HEIGHT	(720)


HWND GetWindow();

float GetFps();
