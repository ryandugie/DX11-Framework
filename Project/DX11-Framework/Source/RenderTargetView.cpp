/****************************************************************************/
/*!
\file
   RenderTargetView.cpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0
    
    Wrapper around a ID3D11RenderTargetView
*/
/****************************************************************************/

/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "RenderTargetView.hpp"

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
  Constructor, calls DX11 to create the ID3D11RenderTargetView

\param device 
  The ID3D11Device

\param texture
  The buffer to save the render data to
*/
/****************************************************************************/
DX11::RenderTargetView::RenderTargetView(DX11::Device device, DX11::Texture2D texture)
{
    if (!SUCCEEDED(device->CreateRenderTargetView(texture.Get(), nullptr, GetAddressOf())))
    {
        throw std::runtime_error("DX11: CreateRenderTargetView() failed!\n");
    }
}

/****************************************************************************/
/*!
\brief
  Clear the buffer

\param device 
  The ID3D11Device
*/
/****************************************************************************/
void DX11::RenderTargetView::Clear(DX11::Device device)
{
    device.Context()->ClearRenderTargetView(Get(), DirectX::Colors::DarkGray);
}
