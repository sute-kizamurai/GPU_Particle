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

#include <DirectXMath.h>
using namespace DirectX;


#include "DirectXTex.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DIrectXTEx_Release.lib")
#endif




#pragma comment (lib, "winmm.lib")

//------------------------------
//�}�N����`
//------------------------------
#define WINDOW_WIDTH	(1280)
#define WINDOW_HEIGHT	(720)


HWND GetWindow();

void Invoke(std::function<void()> Function, int Time);

