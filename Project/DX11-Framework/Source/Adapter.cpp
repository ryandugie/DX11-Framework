/****************************************************************************/
/*!
\file
   Adapter.hpp
\Author
   Ryan Dugie
\brief 
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0
    
    Wrapper around a IDXGIAdapter1
*/
/****************************************************************************/

/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "Adapter.hpp"
#include "Factory.hpp"

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
  Constructor, calls DX11 to create the IDXGIAdapter1

\param factory
  The factory the adaptor is built from
*/
/****************************************************************************/
DX11::Adaptor::Adaptor(DX11::Factory factory)
{
#define PCI_VENDOR_NVIDIA 0x10de  //PCI VENDOR ID
#define ADAPTER_QUERY_LIMIT 5

    // get the index zero adaptor
    *this = DX11::Adaptor(factory, 0);
    {
        DXGI_ADAPTER_DESC1 adaptorDesc;
        Get()->GetDesc1(&adaptorDesc);

        // early break, if the first device is NVIDIA
        if (adaptorDesc.VendorId == PCI_VENDOR_NVIDIA)
        {
            return;
        }
    }

    try
    {
        // loop over adaptors, prefering NVIDIA
        for (uint32_t i = 1; i < ADAPTER_QUERY_LIMIT; ++i)
        {
            try
            {

                DX11::Adaptor adaptor(factory, i);

            DXGI_ADAPTER_DESC1 adaptorDesc;
            adaptor->GetDesc1(&adaptorDesc);

            if (adaptorDesc.VendorId == PCI_VENDOR_NVIDIA)
            {
                *this = adaptor;
                break;
            }

            }
            catch (std::runtime_error e)
            {

            }
        }
    }
    catch (std::runtime_error e)
    {
        DEBUG::log.Info("No NVIDIA adaptor found, using default Adaptor.");
    }
}

/****************************************************************************/
/*!
\brief
  Constructor, calls DX11 to create the IDXGIAdapter1 for a specific index

\param factory
  The factory the adaptor is built from

\param i
  The index of the adaptor to enumerate
*/
/****************************************************************************/
DX11::Adaptor::Adaptor(DX11::Factory factory, uint32_t i)
{
    HRESULT r = factory->EnumAdapters1(i, ReleaseAndGetAddressOf());
    if (!SUCCEEDED(r))
    {
        throw std::runtime_error("DX11: EnumAdapters1() failed!\n");
    }
}
