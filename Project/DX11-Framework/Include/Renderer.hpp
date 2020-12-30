/****************************************************************************/
/*!
\file
   Renderer.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    The core renderer class
*/
/****************************************************************************/

#ifndef RENDERER_HPP
#define RENDERER_HPP
#pragma once

#include "Device.hpp"
#include "SwapChain.hpp"
#include "Shader.hpp"
#include "PipelineStates.hpp"
#include "DepthStencilView.hpp"
#include "Buffer.hpp"
#include "Mesh.hpp"

struct GLFWwindow;
typedef GLFWwindow* WindowPtr;

namespace DX11
{

    class Renderer
    {

    public:

        Renderer();
        ~Renderer();
        void Draw(float dt);
        WindowPtr Window() const;


    private:
        /* friends */
        friend static void FramebufferResizeCallback(WindowPtr window, int width, int height);

        /* Functions */
        void InitGLFW();
        void ShutdownGLFW();

        void InitWindow();
        void ShutdownWindow();

        void ReInitDX11();
        void InitDX11();
        void InitPipelineDescription();
        void InitDepthResouces();
        void ShutdownDX11();

        void Present();

        // window
        WindowPtr mWindow = nullptr;
        int mWindowWidth = 800;
        int mWindowHeight = 800;
        int mVSync = 1;

        // Core DX11
        DX11::SwapChain mSwapChain;
        DX11::Device mDevice;
        bool mFramebufferResized = false;

        // This stuff should probably get put in classes
        DX11::RasterizerState mRasterizerState;
        DX11::BlendState mBlendState;
        uint32_t mBlendSampleMask = 0xffffffff;
        float mBlendFactors[4] = { 1, 1, 1, 1 };
        D3D11_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
        std::vector<D3D11_VIEWPORT> vViewport;

        // Depth stuff
        DX11::Texture2D mDepthTexture;
        DX11::DepthStencilView mDepthView;
        DX11::DepthStencilState mDepthStencilState;

        // Test Display Data
        DX11::Shader mShader;
        DX11::Buffer mMatrixBuffer;
        DX11::Mesh mDisplayMesh;
        DirectX::XMMATRIX mViewMatrix;
        DirectX::XMMATRIX mProjectionMatrix;
        float mAngle = 0;

    };
}

#endif // RENDERER_HPP
