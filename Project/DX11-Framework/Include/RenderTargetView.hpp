/****************************************************************************/
/*!
\file
   RenderTargetView.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    Wrapper around a ID3D11RenderTargetView
*/
/****************************************************************************/
#ifndef RENDERTARGETVIEW_H
#define RENDERTARGETVIEW_H
#pragma once

#include "Device.hpp"
#include "Texture2D.hpp"

namespace DX11
{
    class RenderTargetView : public DX11::DXPtr<ID3D11RenderTargetView>
    {
    public:
        RenderTargetView() = default;
        RenderTargetView(DX11::Device device, DX11::Texture2D texture);
        void Clear(DX11::Device device);

    private:

    };
}

#endif // RENDERTARGETVIEW_H
