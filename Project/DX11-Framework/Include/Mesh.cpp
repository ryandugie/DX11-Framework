/****************************************************************************/
/*!
\file
   Mesh.cpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
    Licensed under the Apache License 2.0
*/
/****************************************************************************/
/*============================================================================*\
|| ------------------------------ INCLUDES ---------------------------------- ||
\*============================================================================*/

#include "DX11PCH.hpp"
#include "Mesh.hpp"

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
  its a desctructor, it cleans up everything
*/
/****************************************************************************/
DX11::Mesh::~Mesh() 
{
    VBO.ReleaseAndGetAddressOf();
    IBO.ReleaseAndGetAddressOf();
}

/****************************************************************************/
/*!
\brief
  Constructor, creates a buffer from the device

\param device
  The ID3D11Device

\param path
  Path of the file to load
*/
/****************************************************************************/
DX11::Mesh::Mesh(DX11::Device device, std::string path) 
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::runtime_error(importer.GetErrorString());
    }

    // store data
    for (unsigned i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[i];
        GetMesh(mesh);
    }

    // VBO
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = uint32_t(Vertices.size() * sizeof(Vertex));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA resourceData = {};
    resourceData.pSysMem = static_cast<void*>(Vertices.data());
    VBO = DX11::Buffer(device, bufferDesc, resourceData);

    // IBO
    bufferDesc = {};
    bufferDesc.ByteWidth = uint32_t(Indices.size() * sizeof(uint32_t));
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    resourceData = {};
    resourceData.pSysMem = static_cast<void*>(Indices.data());
    IBO = DX11::Buffer(device, bufferDesc, resourceData);
}

/****************************************************************************/
/*!
\brief
  Render this mesh

\param device
  The ID3D11Device
*/
/****************************************************************************/
void DX11::Mesh::Draw(DX11::Device device) 
{
    DX11::DeviceContext context = device.Context();
    uint32_t offset[] = { 0 };
    static const uint32_t stride = sizeof(Vertex);
    context->IASetVertexBuffers(0, 1, VBO.GetAddressOf(), &stride, offset);
    context->IASetIndexBuffer(IBO.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(UINT(Indices.size()), 0, 0);
}

/*============================================================================*\
|| ------------------------- PRIVATE FUNCTIONS ------------------------------ ||
\*============================================================================*/

/****************************************************************************/
/*!
\brief
  Get the vertex and index data from assimp

\param mesh
  The ASSIMP type mesh
*/
/****************************************************************************/
void DX11::Mesh::GetMesh(aiMesh* mesh) 
{
    // verticies
    for (unsigned i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        // position
        vertex.position = DirectX::XMVectorSet(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1);

        // normals
        if (mesh->mNormals != nullptr)
        {
            vertex.normal = DirectX::XMVectorSet(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1);
            vertex.normal = DirectX::XMVector4Normalize(vertex.normal);
        }

        Vertices.push_back(vertex);
    }

    // indicies
    for (unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; ++j)
        {
            Indices.push_back(face.mIndices[j]);
        }
    }
}
