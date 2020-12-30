/****************************************************************************/
/*!
\file
   Factory.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0
    
    Wrapper around a IDXGIFactory1
*/
/****************************************************************************/
#ifndef FACTORY_H
#define FACTORY_H
#pragma once

namespace DX11
{
    class Factory : public DX11::DXPtr<IDXGIFactory1>
    {
    public:

        Factory();

    private:

    };
}

#endif 
