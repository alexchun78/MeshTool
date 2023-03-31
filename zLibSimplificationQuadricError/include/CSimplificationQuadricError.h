#pragma once
#include "../manager/CDLLManager.h"
#include "../manager/CHEManager.h"
class CSimplificationQuadricError
{
public:
    CSimplificationQuadricError(const std::string dllPath, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
    ~CSimplificationQuadricError();

    bool DoSimplification(const float reductionRate, const float agressive);

private:
    bool LoadMeshData(const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
    bool Internal_Simplification(const int targetCount, const double agressive);
    void UpdateMesh(const int iter);
    void ComposeSymetricMatrix(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris);

private: // ���ذ� �ȵǴ� ��ɵ��� �Ʒ� ���� ����Ʈ�� ���� �ڵ� �����.
    // ����: https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification 
    double CalQuadricError(Vec3& p, const size_t v1, const size_t v2); // ũ�γ�Ŀ ��Ÿ (�ټ�) ���ϴ� ����
    double CalVertexError(MeshIOLib::SymetricMatrix q, Vec3 v);

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

