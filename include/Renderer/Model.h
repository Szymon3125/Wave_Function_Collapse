#pragma once

#include "Renderer/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

class Model
{
public:
    Model(const std::string& path);
    void Draw(const std::shared_ptr<Shader>& shader, const glm::mat4& transform);

private:
    std::vector<std::shared_ptr<Mesh>> _meshes;
    std::string _directory;

    void Load(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
};