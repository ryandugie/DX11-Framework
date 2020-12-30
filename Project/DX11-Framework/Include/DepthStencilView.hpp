/****************************************************************************/
/*!
\file
   DepthStencilView.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    Wrapper around ID3D11DepthStencilView
*/
/****************************************************************************/
#ifndef DEPTHSTENCILVIEW_H
#define DEPTHSTENCILVIEW_H
#pragma once

#include "DX11PCH.hpp"
#include "Device.hpp"
#include "Texture2D.hpp"

namespace DX11
{
    class DepthStencilView : public DX11::DXPtr<ID3D11DepthStencilView>
    {
    public:
        DepthStencilView() = default;
        DepthStencilView(DX11::Device device, DX11::Texture2D buffer);

    private:
    };
}

#endif 
