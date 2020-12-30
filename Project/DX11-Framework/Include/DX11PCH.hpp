/****************************************************************************/
/*!
\file
   DX11PCH.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    Precompiled Header, all common headers / functionality
*/
/****************************************************************************/

#ifndef DX11RENDERER_PCH
#define DX11RENDERER_PCH
#pragma once
#pragma warning(push, 0)

//System include
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX

// DIRECTX11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl/client.h>

// DX libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dCompiler.lib")
#pragma comment(lib, "dxguid.lib")

// GLFW
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3.h"
#include "glfw3native.h"

// STL Includes
#include <algorithm>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <cstdint>
#include <stdexcept>

// CMath
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef PI
#define PI 3.14159265359f
#endif
#undef _USE_MATH_DEFINES

#pragma warning(pop) // warning(push, 0)

//! SILENCE WARNING (expression, warning)
#define SILENCE_WARNING(x, w)         \
        __pragma(warning(push))       \
        __pragma(warning(disable : w))\
                x                     \
        __pragma(warning(pop))        \

#define UNUSED(x) (void)(x)

// DEBUG
#include "Log.hpp"

// typedefs
namespace DX11
{
    template <typename T>
    using DXPtr = Microsoft::WRL::ComPtr<T>;
}

inline std::wstring utf8ToUtf16(const std::string& utf8Str)
{
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[utf8Str.size()];
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, wstr, wchars_num);
    return std::wstring(wstr);
}

#endif // DX11RENDERER_PCH