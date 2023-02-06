#pragma once
#include <string>
#include <vector>


//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include "assimp/include/assimp/Importer.hpp"
#include "assimp/include/assimp/scene.h"
#include "assimp/include/assimp/postprocess.h"
#include "Affine.h"
#include "glm/glm.hpp"


void ReadAssimpFile(const std::string& path, const glm::mat4& M, std::vector<AssimpMeshData*>& meshDatas);
void recurseModelNodes(const  aiScene* aiscene,
    const  aiNode* node,
    const aiMatrix4x4& parentTr,
    const int level,
    std::vector<AssimpMeshData*>& meshes);