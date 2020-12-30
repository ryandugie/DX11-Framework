/****************************************************************************/
/*!
\file
   Shader.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Licensed under the Apache License 2.0.

    Creates a shader program from the given file paths
*/
/****************************************************************************/
#ifndef SHADER_H
#define SHADER_H
#pragma once

#include "DX11PCH.hpp"
#include "Device.hpp"
#include "InputLayout.hpp"

namespace DX11
{
    struct ShaderInfo
    {
        std::string vertex = "?";
        std::string pixel = "?";
    };

    class Shader
    {
    public:
        Shader() = default;
        Shader(DX11::Device device, DX11::ShaderInfo paths);
        void Bind(DX11::DeviceContext context);
        void Unbind(DX11::DeviceContext context);

        DX11::InputLayout InputLayout() const;

    private:
        enum class ShaderType
        {
            Vertex,
            Pixel,
        };

        ShaderStage CreateShaderStage(std::string path);

        DX11::DXPtr<ID3D11VertexShader> pVertexShader;
        DX11::DXPtr<ID3D11PixelShader>  pPixelShader;
        DX11::InputLayout  pInputLayout;
    };
}

#endif 
