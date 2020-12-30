/****************************************************************************/
/*!
\file
   PipelineStates.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Licensed under the Apache License 2.0.

    Wrappers around ID3D11RasterizerState, ID3D11DepthStencilState, and
    ID3D11BlendState because I didnt feel like making multiple files
    and they are are loosely related anyways.
*/
/****************************************************************************/
#ifndef PIPELINESTATES_H
#define PIPELINESTATES_H
#pragma once

#include "DX11PCH.hpp"
#include "Device.hpp"

namespace DX11
{
    class RasterizerState : public DX11::DXPtr<ID3D11RasterizerState>
    {
    public:
        RasterizerState() = default;
        RasterizerState(DX11::Device device, D3D11_RASTERIZER_DESC desc)
        {
            if (!SUCCEEDED(device->CreateRasterizerState(&desc, ReleaseAndGetAddressOf())))
            {
                throw std::runtime_error("DX11: CreateRasterizerState() failed from RasterizerState!\n");
            }
        }

    private:
    };

    class DepthStencilState : public DX11::DXPtr<ID3D11DepthStencilState>
    {
    public:
        DepthStencilState() = default; 
        DepthStencilState(DX11::Device device, D3D11_DEPTH_STENCIL_DESC desc)
        {
            if (!SUCCEEDED(device->CreateDepthStencilState(&desc, ReleaseAndGetAddressOf())))
            {
                throw std::runtime_error("DX11: CreateDepthStencilState() failed from RasterizerState!\n");
            }
        }
    private:
    };

    class BlendState : public DX11::DXPtr<ID3D11BlendState>
    {
    public:
        BlendState() = default; 
        BlendState(DX11::Device device, D3D11_BLEND_DESC desc)
        {
            if (!SUCCEEDED(device->CreateBlendState(&desc, ReleaseAndGetAddressOf())))
            {
                throw std::runtime_error("DX11: CreateBlendState() failed from RasterizerState!\n");
            }
        }
    private:
    };
}

#endif
