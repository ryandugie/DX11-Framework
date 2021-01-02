/****************************************************************************/
/*!
\file
   Renderer.cpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    The core renderer class
*/
/****************************************************************************/
/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "Renderer.hpp"
#include "Factory.hpp"
#include <array>
#include <math.h>

/*============================================================================*\
|| --------------------------- GLOBAL VARIABLES ----------------------------- ||
\*============================================================================*/


/*============================================================================*\
|| -------------------------- STATIC FUNCTIONS ------------------------------ ||
\*============================================================================*/

namespace DX11 {
    /****************************************************************************/
    /*!
    \brief
      Callback for all GLFW errors

    \param error
      The GLFW error code

    \param description
      Error description
    */
    /****************************************************************************/
    static void GLFWErrorCallback(int error, const char* description)
    {
        DEBUG::log.Error("GLFW Error ", error, " : ", description);
    }

    /****************************************************************************/
    /*!
    \brief
      Called when our window gets resized,
      signal the renderer to resize its buffers

    \param window
      The window that got resized

    \param width
      The new width

    \param height
      The new height
    */
    /****************************************************************************/
    static void FramebufferResizeCallback(WindowPtr window, int width, int height)
    {
        DX11::Renderer* renderer = reinterpret_cast<DX11::Renderer*>(glfwGetWindowUserPointer(window));
        renderer->mFramebufferResized = true;
        renderer->mWindowWidth = width;
        renderer->mWindowHeight = height;
    }
}

/*============================================================================*\
|| -------------------------- PUBLIC FUNCTIONS ------------------------------ ||
\*============================================================================*/

/****************************************************************************/
/*!
\brief
  Initialize the renderer
*/
/****************************************************************************/
DX11::Renderer::Renderer()
{
    InitGLFW();
    InitDX11();
}

/****************************************************************************/
/*!
\brief
  Clean up
*/
/****************************************************************************/
DX11::Renderer::~Renderer()
{
    ShutdownGLFW();
    ShutdownDX11();
}

/****************************************************************************/
/*!
\brief
  Render

\param dt
  Delta-Time
*/
/****************************************************************************/
void DX11::Renderer::Draw(float dt)
{
    /* init render pass */
    DX11::DeviceContext context = mDevice.Context();
    context->IASetPrimitiveTopology(mPrimitiveTopology);
    context->IASetInputLayout(mShader.InputLayout().Get());
    context->RSSetState(mRasterizerState.Get());
    context->RSSetViewports(uint32_t(vViewport.size()), vViewport.data());
    context->OMSetBlendState(mBlendState.Get(), mBlendFactors, mBlendSampleMask);
    context->OMSetDepthStencilState(mDepthStencilState.Get(), 1);
    context->ClearDepthStencilView(mDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    std::array<ID3D11RenderTargetView*, 1> renderTargetViews = { mSwapChain.View().Get() };
    context->OMSetRenderTargets(UINT(renderTargetViews.size()), renderTargetViews.data(), mDepthView.Get());

    /* update matricies */

    // calculate
    DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
    mAngle -= dt;
    worldMatrix = DirectX::XMMatrixRotationAxis({0, 1, 0}, mAngle) * worldMatrix;
    worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);

    // update buffer
    std::array<DirectX::XMMATRIX, 3> bufferData = { worldMatrix, mProjectionMatrix, mViewMatrix };
    size_t bufferSize = sizeof(bufferData[0]) * bufferData.size();
    mMatrixBuffer.Update(mDevice, bufferData.data(), 0, uint32_t(bufferSize));
    std::array<ID3D11Buffer*, 1> constantBuffers = { mMatrixBuffer.Get() };
    context->VSSetConstantBuffers(0, uint32_t(constantBuffers.size()), constantBuffers.data());

    /* draw the test object */
    mShader.Bind(context);
    mDisplayMesh.Draw(mDevice);
    mShader.Unbind(context);

    /* present */
    Present();
    glfwPollEvents();
    context->ClearState();
}

/****************************************************************************/
/*!
\brief
  Get the application window

\return
  Pointer to the window

*/
/****************************************************************************/
WindowPtr DX11::Renderer::Window() const
{
    return mWindow;
}

/*============================================================================*\
|| ------------------------- PRIVATE FUNCTIONS ------------------------------ ||
\*============================================================================*/

/****************************************************************************/
/*!
\brief
  Restart DX11
*/
/****************************************************************************/
void DX11::Renderer::ReInitDX11()
{
    ShutdownDX11();
    InitDX11();
}

/****************************************************************************/
/*!
\brief
  Initialize GLFW
*/
/****************************************************************************/
void DX11::Renderer::InitGLFW()
{
    glfwSetErrorCallback(DX11::GLFWErrorCallback);
    if (!glfwInit())
    {
        throw std::runtime_error("GLFW: glfwInit() failed! Check error callback!\n");
    }

    // create the GLFW window
    InitWindow();
}

/****************************************************************************/
/*!
\brief
  Create the application window
*/
/****************************************************************************/
void DX11::Renderer::InitWindow()
{
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "DX11-Framework", nullptr, nullptr);
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetFramebufferSizeCallback(mWindow, DX11::FramebufferResizeCallback);
}


/****************************************************************************/
/*!
\brief
  All DirectX11 related initilaization
*/
/****************************************************************************/
void DX11::Renderer::InitDX11()
{
    // init core DX11
    DX11::Factory factory;
    DX11::Adaptor adaptor(factory);

    mDevice = DX11::Device(adaptor);
    mSwapChain = DX11::SwapChain(factory, mDevice, mWindow, mWindowWidth, mWindowHeight);

    if (!SUCCEEDED(factory->MakeWindowAssociation(glfwGetWin32Window(mWindow), DXGI_MWA_NO_ALT_ENTER)))
    {
        throw std::runtime_error("DX11: MakeWindowAssociation() failed from InitDX11!\n");
    }

    InitPipelineDescription();
    InitDepthResouces();

    /* Temp stuff for this example only and should be moved */

    // view port
    vViewport = { { 0.0f, 0.0f, float(mWindowWidth), float(mWindowHeight), 0.0f, 1.0f } };

    // matrix buffer
    mMatrixBuffer = DX11::Buffer(mDevice, uint32_t(sizeof(DirectX::XMMATRIX) * 3), D3D11_USAGE_DYNAMIC);

    // display shader -- delete this
    DX11::ShaderInfo shaderInfo;
    shaderInfo.vertex = "../Resource/Shaders/Simple.vs.cso";
    shaderInfo.pixel = "../Resource/Shaders/Simple.ps.cso";
    mShader = DX11::Shader(mDevice, shaderInfo);

    // display mesh -- delete this
    mDisplayMesh = DX11::Mesh(mDevice, "../Resource/Models/StanfordBunny.obj");

    // camera -- delete this
    DirectX::XMVECTOR position = { 0, 0.1f, 1 };
    DirectX::XMVECTOR up = { 0, 1, 0 };
    float fov = 0.42173f;
    float pitch = 0;
    float yaw = 0;
    float aspectRatio = float(mWindowWidth) / mWindowHeight;
    float nearPlane = 0.1f;
    float farPlane = 250.f;

    mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);
    DirectX::XMVECTOR front = DirectX::XMVector3Normalize({ std::sin(yaw) * std::cos(pitch), std::sin(pitch),   -std::cos(yaw) * std::cos(pitch) });
    mViewMatrix = DirectX::XMMatrixLookAtLH(position, DirectX::XMVectorAdd(position, front), up);

}

/****************************************************************************/
/*!
\brief
  Initalize the pipeline settings
*/
/****************************************************************************/
void DX11::Renderer::InitPipelineDescription()
{
    mPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_FRONT;
    rasterDesc.FrontCounterClockwise = true;
    mRasterizerState = DX11::RasterizerState(mDevice, rasterDesc);

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    mDepthStencilState = DX11::DepthStencilState(mDevice, depthStencilDesc);

    D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
    renderTargetBlendDesc.BlendEnable = false;
    renderTargetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
    renderTargetBlendDesc.DestBlend = D3D11_BLEND_ZERO;
    renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0] = renderTargetBlendDesc;
    mBlendState = DX11::BlendState(mDevice, blendDesc);
}

/****************************************************************************/
/*!
\brief
  Create the depth texture and other related things
*/
/****************************************************************************/
void DX11::Renderer::InitDepthResouces()
{
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.Height = uint32_t(mWindowHeight);
    depthDesc.Width = uint32_t(mWindowWidth);
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.SampleDesc = { 1, 0 };
    depthDesc.ArraySize = 1;
    mDepthTexture = DX11::Texture2D(mDevice, depthDesc);
    mDepthView = DX11::DepthStencilView(mDevice, mDepthTexture);
}

/****************************************************************************/
/*!
\brief
  all GLFW related cleanup
*/
/****************************************************************************/
void DX11::Renderer::ShutdownGLFW()
{
    ShutdownWindow();
    glfwTerminate();
}

/****************************************************************************/
/*!
\brief
  destroy the application window
*/
/****************************************************************************/
void DX11::Renderer::ShutdownWindow()
{
    glfwDestroyWindow(mWindow);
}

/****************************************************************************/
/*!
\brief
  All DirectX11 related clean up
*/
/****************************************************************************/
void DX11::Renderer::ShutdownDX11()
{
    mSwapChain.View().Reset();
    mSwapChain.Buffer().Reset();
    mSwapChain.Reset();

    DX11::DeviceContext context = mDevice.Context();
    if (context != nullptr)
    {
        context->ClearState();
        context->Flush();
    }
    context.Reset();

#ifdef USE_DEBUG_DEVICE
    DX11::DebugDevice debug = mDevice.Debug();
    if (debug != nullptr)
    {
        // debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        debug.Reset();
    }
    debug.Reset();
#endif
}

/****************************************************************************/
/*!
\brief
  Present the swapchain
*/
/****************************************************************************/
void DX11::Renderer::Present()
{
    ID3D11RenderTargetView* renderTargetViews[] = { mSwapChain.View().Get() };
    DX11::DeviceContext context = mDevice.Context();
    context->OMSetRenderTargets(1, renderTargetViews, nullptr);
    mSwapChain->Present(mVSync, 0);
    const DirectX::XMVECTORF32 actuallyDarkGray = { { { 0.1f, 0.1f, 0.1f, 1.000000000f } } };
    context->ClearRenderTargetView(renderTargetViews[0], actuallyDarkGray);
}