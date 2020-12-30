/****************************************************************************/
/*!
\file
   SwapChain.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Licensed under the Apache License 2.0.

    Wrapper around a IDXGISwapChain
*/
/****************************************************************************/
#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H
#pragma once

struct GLFWwindow;
typedef GLFWwindow* WindowPtr; // typdefing this here since this file gets included in everthing its needed in

#include "Texture2D.hpp"
#include "RenderTargetView.hpp"

namespace DX11
{
    class Device;
    class Factory;

    class SwapChain : public DX11::DXPtr<IDXGISwapChain>
    {
    public:

        SwapChain() = default;
        SwapChain(DX11::Factory factory, DX11::Device device, WindowPtr window, uint32_t width, uint32_t height);

        DX11::Texture2D Buffer() const;
        DX11::RenderTargetView View() const;

        void Resize(uint32_t width, uint32_t height);

    private:

        DX11::Texture2D pBuffer;
        DX11::RenderTargetView pRenderTargetView;

    };
}

#endif // SWAPCHAIN_H
