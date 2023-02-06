#pragma once
#include "Shader.h"

class NewMesh {
public:

    struct Face {
        int index1, index2, index3;
        Face(int i = -1, int j = -1, int k = -1)
            : index1(i), index2(j), index3(k) {}
    };

    struct Edge {
        int index1, index2;
        Edge(int i = -1, int j = -1)
            : index1(i), index2(j) {}
    };

    NewMesh(const char* vertexShaderPath, const char* fragmentShaderPath);

private:
    Shader* shader;
};