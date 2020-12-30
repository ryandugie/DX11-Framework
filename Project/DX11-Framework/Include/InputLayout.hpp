/****************************************************************************/
/*!
\file
   Shader.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.  
    Licensed under the Apache License 2.0
    
    Wrapper around an ID3D11InputLayout
*/
/****************************************************************************/
#ifndef INPUTLAYOUT_H
#define INPUTLAYOUT_H
#pragma once

#include "DX11PCH.hpp"
#include "Device.hpp"

namespace DX11
{

    typedef DX11::DXPtr<ID3DBlob> ShaderStage;
    typedef DX11::DXPtr<ID3D11ShaderReflection> ShaderReflection;

    class InputLayout : public DX11::DXPtr<ID3D11InputLayout>
    {
    public:
        InputLayout() = default;
        InputLayout(DX11::Device device, DX11::ShaderStage blob);

    private:
    };
}
#endif 
