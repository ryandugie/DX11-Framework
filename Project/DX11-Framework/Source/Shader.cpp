/****************************************************************************/
/*!
\file
   Shader.cpp
\Author
   Ryan Dugie
\brief
	Copyright (c) Ryan Dugie. All rights reserved.
	Licensed under the Licensed under the Apache License 2.0.

	Creates a shader program from the given file paths
*/
/****************************************************************************/

/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "Shader.hpp"

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
  Create a shader program

\param device
  The ID3D11Device

\param paths
  The files paths for each part of the shader program
*/
/****************************************************************************/
DX11::Shader::Shader(DX11::Device device, DX11::ShaderInfo paths)
{
	if (paths.vertex == "?" || paths.pixel == "?")
	{
		throw std::runtime_error("DX11: ShaderInfo Vertex or Pixel Shader invalid!\n");
	}

	ShaderStage vertexShader = CreateShaderStage(paths.vertex);
	ShaderStage pixelShader = CreateShaderStage(paths.pixel);

	if (!SUCCEEDED(device->CreateVertexShader(vertexShader->GetBufferPointer(), vertexShader->GetBufferSize(), nullptr, pVertexShader.ReleaseAndGetAddressOf())))
	{
		throw std::runtime_error("DX11: CreateVertexShader failed!");
	}

	pInputLayout = DX11::InputLayout(device, vertexShader);

	if (!SUCCEEDED(device->CreatePixelShader(pixelShader->GetBufferPointer(), pixelShader->GetBufferSize(), nullptr, pPixelShader.ReleaseAndGetAddressOf())))
	{
		throw std::runtime_error("DX11: CreatePixelShader failed!");
	}
}

/****************************************************************************/
/*!
\brief
  Set the bound shader program to this

\param device
  The ID3D11DeviceContext
*/
/****************************************************************************/
void DX11::Shader::Bind(DX11::DeviceContext context)
{
	context->VSSetShader(pVertexShader.Get(), nullptr, 0);
	context->PSSetShader(pPixelShader.Get(), nullptr, 0);
}

/****************************************************************************/
/*!
\brief
  Set the bound shader program to null

\param device
  The ID3D11DeviceContext
*/
/****************************************************************************/
void DX11::Shader::Unbind(DX11::DeviceContext context)
{
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

/****************************************************************************/
/*!
\brief
  get the Input layout
*/
/****************************************************************************/
DX11::InputLayout DX11::Shader::InputLayout() const
{
	return pInputLayout;
}

/*============================================================================*\
|| ------------------------- PRIVATE FUNCTIONS ------------------------------ ||
\*============================================================================*/

/****************************************************************************/
/*!
\brief
  load a shader blob

\param path
  The file path of the compiled shader
*/
/****************************************************************************/
DX11::ShaderStage DX11::Shader::CreateShaderStage(std::string path)
{
	DX11::ShaderStage blob;
	if (!SUCCEEDED(D3DReadFileToBlob(utf8ToUtf16(path).c_str(), blob.ReleaseAndGetAddressOf())))
	{
		throw std::runtime_error("DX11: D3DReadFileToBlob failed! Path:\n" + path + "\n");
	}
	return blob;
}
