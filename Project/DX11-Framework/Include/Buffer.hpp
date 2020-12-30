/****************************************************************************/
/*!
\file
   Buffer.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    Wrapper around ID3D11Buffer
*/
/****************************************************************************/
#ifndef BUFFER_H
#define BUFFER_H
#pragma once

#include "DX11PCH.hpp"
#include "Device.hpp"

namespace DX11
{
    class Buffer : public DX11::DXPtr<ID3D11Buffer>
    {
    public:
        Buffer() = default;
        Buffer(DX11::Device device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data);
        Buffer(DX11::Device device, uint32_t size, D3D11_USAGE dynamic = D3D11_USAGE_DEFAULT);

        void Update(DX11::Device device, void* data, uint32_t offset, uint32_t size, D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD);
        void Map(DX11::Device device, D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD);
        void Unmap(DX11::Device device);

        void* Data() const;

    private:
        uint32_t pSize = 0;
        void* pData = nullptr;
    };
}

#endif
