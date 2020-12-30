/*/****************************************************************************/
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

/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "Device.hpp"

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
  Constructor, calls DX11 to create the ID3D11Device

\param adaptor
  What adaptor to create this device from / on
*/
/****************************************************************************/
DX11::Device::Device(DX11::Adaptor adaptor)
{ 
    uint32_t deviceFlags = 0;

#ifdef USE_DEBUG_DEVICE
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // USE_DEBUG_DEVICE

    std::vector<D3D_FEATURE_LEVEL> featureLevels = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    if (!SUCCEEDED(D3D11CreateDevice(adaptor.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr,
        deviceFlags, featureLevels.data(), uint32_t(featureLevels.size()), D3D11_SDK_VERSION,
        ReleaseAndGetAddressOf(),  nullptr, mContext.ReleaseAndGetAddressOf())))
    {
        throw std::runtime_error("DX11: D3D11CreateDevice() failed!\n");
    }

    //initialize debug device
#ifdef USE_DEBUG_DEVICE
    if (!SUCCEEDED(As(&mDebug)))
    {
        throw std::runtime_error("DX11: Failed to initialize Debug Device!\n");
    }
#endif // USE_DEBUG_DEVICE
}

/****************************************************************************/
/*!
\brief
  Get the Device context

\return
  Returns the comptr to the D3DDeviceContext
*/
/****************************************************************************/
DX11::DeviceContext DX11::Device::Context() const
{
    return mContext;
}

#ifdef USE_DEBUG_DEVICE
/****************************************************************************/
/*!
\brief
  Get the Debug Device

\return
  Returns the comptr to the ID3D11Debug
*/
/****************************************************************************/
DX11::DebugDevice DX11::Device::Debug() const
{
    return mDebug;
}
#endif