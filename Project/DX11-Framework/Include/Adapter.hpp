/****************************************************************************/
/*!
\file
   Adaptor.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0 

    Wrapper around a IDXGIAdaptor1
*/
/****************************************************************************/
#ifndef ADAPTOR_H
#define ADAPTOR_H
#pragma once

#include "DX11PCH.hpp"

namespace DX11
{
    class Factory;

    class Adaptor : public DX11::DXPtr<IDXGIAdapter1>
    {
    public:

        Adaptor(DX11::Factory factory);

    private:

        Adaptor(DX11::Factory factory, uint32_t i);

    };
}

#endif // ADAPTOR_H
