/****************************************************************************/
/*!
\file
   Shader.cpp
\Author
   Ryan Dugie
\brief
	Copyright (c) Ryan Dugie. All rights reserved.
	Licensed under the Licensed under the Apache License 2.0.

	Wrapper around an ID3D11InputLayout
*/
/****************************************************************************/

/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "InputLayout.hpp"

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
  Create an ID3D11InputLayout
  https://gist.github.com/mobius/b678970c61a93c81fffef1936734909f
*/
/****************************************************************************/
DX11::InputLayout::InputLayout(DX11::Device device, DX11::ShaderStage blob)
{
    ShaderReflection reflection;
    if (!SUCCEEDED(D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(reflection.ReleaseAndGetAddressOf()))))
    {
        throw std::runtime_error("DX11: D3DReflect() failed from InputLayout!\n");
    }

    D3D11_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for (UINT32 i = 0; i < shaderDesc.InputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        reflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc = {};
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        if (paramDesc.Mask == 1)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 7)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        else if (paramDesc.Mask <= 15)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        inputLayoutDesc.push_back(elementDesc);
    }

    if (!SUCCEEDED(device->CreateInputLayout(inputLayoutDesc.data(), static_cast<uint32_t>(inputLayoutDesc.size()), blob->GetBufferPointer(), blob->GetBufferSize(), ReleaseAndGetAddressOf())))
    {
        throw std::runtime_error("DX11: CreateInputLayout() failed from InputLayout!\n");
    }

}
