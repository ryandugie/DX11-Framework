/****************************************************************************/
/*!
\file
   Texture2D.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Licensed under the Apache License 2.0.
    
    Wrapper around a ID3D11Texture2D
*/
/****************************************************************************/
#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#pragma once

#include "Device.hpp"

namespace DX11
{
    class Texture2D : public DX11::DXPtr<ID3D11Texture2D>
    {
    public:

        Texture2D() = default;
        Texture2D(DX11::Device device, D3D11_TEXTURE2D_DESC desc);

    private:

    };
}

#endif // TEXTURE2D_H
