#pragma once
#include <vector>
#include <Windows.h>

class CMeshIOManager
{
public:
    CMeshIOManager(const HMODULE handle);
    //CMeshIOManager(const char* path);
    ~CMeshIOManager();

    bool LoadOBJ(const char* filename, HMODULE handle);
    bool LoadSTL(const char* filename, HMODULE handle);
    //bool LoadSTL(const char* filename);

   // void TerminateDLL();

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
    //HMODULE m_dll;
};

