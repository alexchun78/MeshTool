#pragma once
#include <vector>
#include <Windows.h>
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CMeshIOData.h"

class CMeshIOManager
{
public:
    CMeshIOManager();
    CMeshIOManager(const char* path);
    ~CMeshIOManager();

    bool LoadOBJ(const char* filename);
    bool LoadSTL(const char* filename);

    void TerminateDLL();

public:
    inline std::vector<MeshIOLib::Triangle> GetTriangleList() {
        return m_vecTriangles;
    }
    inline std::vector<MeshIOLib::Vertex> GetVertexList() {
        return m_vecVertices;
    }

private:
    std::vector<MeshIOLib::Triangle> m_vecTriangles;
    std::vector<MeshIOLib::Vertex> m_vecVertices;
    MeshIOLib::CMeshIO* m_CMeshIO;
    HMODULE m_dll;
};

