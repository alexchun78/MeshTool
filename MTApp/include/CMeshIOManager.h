#pragma once
#include <vector>

class CMeshIOManager
{
public:
    CMeshIOManager(const HMODULE handle);
    ~CMeshIOManager();

    bool LoadOBJ(const char* filepath);
    bool LoadSTL(const char* filepath);
    bool WriteOBJ(const char* filepath);
    bool WriteSTL(const char* filepath);
    bool WriteOBJwithData(const char* filepath, const std::vector<MeshIOLib::Vertex>& vecVerts, const std::vector<MeshIOLib::Triangle>& vecTris);
    bool WriteSTLwithData(const char* filepath, const std::vector<MeshIOLib::Vertex>& vecVerts, const std::vector<MeshIOLib::Triangle>& vecTris);
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
    HMODULE m_handle;
};

