/****************************************************************************/
/*!
\file
   Device.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0

    Wrapper around a ID3D11Device
*/
/****************************************************************************/
#ifndef DEVICE_H
#define DEVICE_H
#pragma once

#include "Adapter.hpp"

#ifdef _DEBUG
#define USE_DEBUG_DEVICE
#endif

namespace DX11
{
    typedef DX11::DXPtr<ID3D11DeviceContext> DeviceContext;

#ifdef USE_DEBUG_DEVICE
    typedef Microsoft::WRL::ComPtr<ID3D11Debug> DebugDevice;
#endif

    class Device : public DX11::DXPtr<ID3D11Device>
    {
    public:

        Device() = default;
        Device(DX11::Adaptor adaptor);
        DeviceContext Context() const;

#ifdef USE_DEBUG_DEVICE
        DebugDevice Debug() const;
#endif

    private:
        DeviceContext mContext = nullptr;

#ifdef USE_DEBUG_DEVICE
        DebugDevice mDebug = nullptr;
#endif
    };
}

#endif // DEVICE_H
