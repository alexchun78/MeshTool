#pragma once
#include "../manager/CDLLManager.h"
#include "../manager/CHEManager.h"
class CSimplificationQuadricError
{
public:
    CSimplificationQuadricError(const std::string dllPath, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
    ~CSimplificationQuadricError();

    bool DoSimplification(const float reductionRate, const float agressive);

public:
    void GetSimplificationOuputData(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris)
    {
        verts = m_outVertices;
        tris = m_outTriangles;
    }

private:
    bool LoadMeshData(const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
    bool Internal_Simplification(const int targetCount, const double agressive);
    void UpdateMesh(const int iter);
    void ComposeSymetricMatrix(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris);

private: // 이해가 안되는 기능들은 아래 참고 사이트의 구현 코드 사용함.
    // 참고: https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification 
    double CalQuadricError(Vec3& p, const size_t v1, const size_t v2); // 크로네커 델타 (텐서) 구하는 공식
    double CalVertexError(MeshIOLib::SymetricMatrix q, Vec3 v);
    bool IsFlipped(std::vector<int>& vecDeleted, std::vector<MeshIOLib::index_t>& neighborTriangles, const Vec3& p, const MeshIOLib::index_t vid0, const MeshIOLib::index_t vid1, const MeshIOLib::Vertex& v0);
    void UpdateTriangles(int& deletedTriCount, const int vid, const std::vector<MeshIOLib::index_t>& neighborTriangles, const std::vector<size_t> vidList, const std::vector<int>& vecDeleted);
    void FindVidListFromFaceList(std::vector<size_t>& vidList, const std::vector<MeshIOLib::index_t>& neighborTriangles, const MeshIOLib::index_t vertexIndex);
    void CompactMesh();

private:
    // input
    std::vector<MeshIOLib::Vertex> m_vertices;
    std::vector<MeshIOLib::Triangle> m_triangles;

    // output
    std::vector<MeshIOLib::Vertex> m_outVertices;
    std::vector<MeshIOLib::Triangle> m_outTriangles;

private:
    CDLLManager* m_dllManager;
    CHEManager* m_halfEdgeManager;
};

