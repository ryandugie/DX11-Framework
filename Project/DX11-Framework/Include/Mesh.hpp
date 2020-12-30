/****************************************************************************/
/*!
\file
   Mesh.hpp
\Author
   Ryan Dugie
\brief
    Copyright (c) Ryan Dugie. All rights reserved.
*/
/****************************************************************************/
#ifndef MESH_H
#define MESH_H
#pragma once

#include "Device.hpp"
#include "Buffer.hpp"

#pragma warning(push)
#pragma warning(disable : 26812 26495 26451)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)

namespace DX11 {

    struct Vertex {
        DirectX::XMVECTOR position = DirectX::XMVECTOR();
        DirectX::XMVECTOR  normal = DirectX::XMVECTOR();
    };

    class Mesh {
    public:
        ~Mesh();
        Mesh() = default;
        Mesh(DX11::Device device, std::string path);

        void Draw(DX11::Device device);

    private:
        void GetMesh(aiMesh* mesh);

        DX11::Buffer VBO;
        DX11::Buffer IBO;

        std::vector<Vertex> Vertices;
        std::vector<unsigned> Indices;
    };
}

#endif
