/****************************************************************************/
/*!
\file
   SwapChain.cpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Licensed under the Apache License 2.0.

    Wrapper around a IDXGISwapChain
*/
/****************************************************************************/

/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "SwapChain.hpp"
#include "Device.hpp"
#include "Factory.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"
#undef GLFW_EXPOSE_NATIVE_WIN32

/*============================================================================*\
|| --------------------------- GLOBAL VARIABLES ----------------------------- ||
\*============================================================================*/

/*============================================================================*\
|| -------------------------- STATIC FUNCTIONS ------------------------------ ||
\*============================================================================*/

/*============================================================================*\
|| -------------------------- PUBLIC FUNCTIONS ------------------------------ ||
\*============================================================================*/

/****************************************************************************/
/*!
\brief
  Constructor, calls DX11 to create the IDXGISwapChain

\param device
  The ID3D11Device

\param window
  Pointer to the glfw window

\param width
  what buffer width to allocate

\param height
  what buffer height to allocate
*/
/****************************************************************************/
DX11::SwapChain::SwapChain(DX11::Factory factory, DX11::Device device, WindowPtr window, uint32_t width, uint32_t height)
{
    DXGI_MODE_DESC bufferDesc{};
    bufferDesc.Width = width;
    bufferDesc.Height = height;
    bufferDesc.RefreshRate.Numerator = 0;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

    //Disable MSAA to allow DXGI_SWAP_EFFECT_FLIP_DISCARD
    DXGI_SAMPLE_DESC msaSampleDesc = { 1, 0 };

    //Prefer triple buffering
    uint32_t backBufferCount = DXGI_MAX_SWAP_CHAIN_BUFFERS < 3 ? 2 : 3;

    DXGI_SWAP_CHAIN_DESC swapDesc{};
    swapDesc.BufferDesc = bufferDesc;
    swapDesc.SampleDesc = msaSampleDesc;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = backBufferCount;
    swapDesc.OutputWindow = glfwGetWin32Window(window);
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr = factory->CreateSwapChain(device.Get(), &swapDesc, ReleaseAndGetAddressOf());
    if (!SUCCEEDED(hr))
    {
        throw std::runtime_error("DX11: CreateSwapChain() failed!\n");
    }

    if (!SUCCEEDED(Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBuffer.GetAddressOf()))))
    {
        throw std::runtime_error("DX11: GetBuffer() failed from SwapChain!\n");
    }

    pRenderTargetView = RenderTargetView(device, pBuffer);
    pRenderTargetView.Clear(device);
}

/****************************************************************************/
/*!
\brief
  Get the swapchain's buffer/texture
*/
/****************************************************************************/
DX11::Texture2D DX11::SwapChain::Buffer() const
{
    return pBuffer;
}

/****************************************************************************/
/*!
\brief
  Get the swapchain's render target view
*/
/****************************************************************************/
DX11::RenderTargetView DX11::SwapChain::View() const
{
    return pRenderTargetView;
}

/****************************************************************************/
/*!
\brief
  resize the swap chain buffer

\param width
  the new width

\param height
  the new height
*/
/****************************************************************************/
void DX11::SwapChain::Resize(uint32_t width, uint32_t height)
{
    // TODO
}
