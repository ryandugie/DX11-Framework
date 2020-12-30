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
/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "Buffer.hpp"

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
  Constructor, creates a buffer from the device

\param device
  The ID3D11Device

\param desc
  The buffer description struct

\param data
  Initilization data
*/
/****************************************************************************/
DX11::Buffer::Buffer(DX11::Device device, D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data)
{
    pSize = desc.ByteWidth;
    if (!SUCCEEDED(device->CreateBuffer(&desc, &data, ReleaseAndGetAddressOf())))
    {
        throw std::runtime_error("DX11: CreateBuffer() failed from Buffer!\n");
    }
}

/****************************************************************************/
/*!
\brief
  Constructor, creates a buffer from the device (for constant buffers)

\param device
  The ID3D11Device

\param size
  The size of the buffer

\param usage
  D3D11_USAGE flag
*/
/****************************************************************************/
DX11::Buffer::Buffer(DX11::Device device, uint32_t size, D3D11_USAGE usage)
{
    pSize = size;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = size;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.Usage = usage;

    HRESULT result = device->CreateBuffer(&bufferDesc, nullptr, ReleaseAndGetAddressOf());
    if (!SUCCEEDED(result))
    {
        throw std::runtime_error("DX11: CreateBuffer() failed from Buffer!\n");
    }
}

/****************************************************************************/
/*!
\brief
  Update the contents of the buffer

\param device
  The ID3D11Device

\param data
  The data to fill the buffer with

\param offset
  The offset into the buffer to start filling from

\param size
  The memory size of the data being updated

\param mapType
  D3D11_MAP, specifies the read and write permissions.
*/
/****************************************************************************/
void DX11::Buffer::Update(DX11::Device device, void* data, uint32_t offset, uint32_t size, D3D11_MAP mapType)
{
    Map(device, mapType);
    std::memcpy(static_cast<char*>(pData) + offset, data, size);
    Unmap(device);
}

/****************************************************************************/
/*!
\brief
  Deny GPU acess to this buffer

\param device
  The ID3D11Device

\param mapType
  D3D11_MAP, specifies the read and write permissions.
*/
/****************************************************************************/
void DX11::Buffer::Map(DX11::Device device, D3D11_MAP mapType)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    HRESULT result = device.Context()->Map(Get(), 0, mapType, 0, &mappedResource);
    if (!SUCCEEDED(result) || mappedResource.pData == nullptr)
    {
        throw std::runtime_error("DX11: Map() failed from Buffer::Map!\n");
    }

    pData = mappedResource.pData;
}

/****************************************************************************/
/*!
\brief
  allow GPU acess to this buffer

\param device
  The ID3D11Device
*/
/****************************************************************************/
void DX11::Buffer::Unmap(DX11::Device device)
{;
    device.Context()->Unmap(Get(), 0);
    pData = nullptr;
}

/****************************************************************************/
/*!
\brief
  Get the data this buffer contains
*/
/****************************************************************************/
void* DX11::Buffer::Data() const
{
	return pData;
}
