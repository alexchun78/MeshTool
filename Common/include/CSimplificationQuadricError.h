#pragma once
#ifdef USEHALFEDGEDLL 
#include "../manager/CDLLManager.h"
#include "../manager/CHEManager.h"
#else
#include <vector>
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CHalfEdgeRep.h"
#endif
namespace MeshSimplificationQELib
{
    class CSimplificationQuadricError
    {
    public:
        CSimplificationQuadricError(const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
        //CSimplificationQuadricError(const std::string dllPath, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
        ~CSimplificationQuadricError();

        bool DoSimplification(const float reductionRate, const float agressive);
        void GetSimplificationOutputData(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris)
        {
            verts = m_outVertices;
            tris = m_outTriangles;
        }

    private:
        bool LoadMeshData(const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris);
        bool Internal_Simplification(const int targetCount, const double agressive);
        void UpdateMesh(const int iter);
        void UpdateNeighborTris();
        void ComposeSymetricMatrix(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris);
        void UpdateTriangles(int& deletedTriCount, const int vid, const std::vector<size_t>& neighborTriangles, const std::vector<size_t> vidList, const std::vector<int>& vecDeleted);
        double CalVertexError(MeshIOLib::SymetricMatrix q, Vec3 v);
        void CompactMesh();

    private:
        bool Internal_Simplification_FQM(const int targetCount, const double agressive);
        void UpdateMesh_FQM(const int iter);
        void Internal_SetInitData_FQM(std::vector<MeshIOLib::Vertex>& vertices, std::vector<MeshIOLib::Triangle>& tris, std::vector<MeshIOLib::Ref>& refs);
        void Internal_UpdateRefData_FQM(std::vector<MeshIOLib::Vertex>& vertices, std::vector<MeshIOLib::Triangle>& tris, std::vector<MeshIOLib::Ref>& refs);
        double CalQuadricError_FQM(Vec3& p, const size_t v1, const size_t v2); // ũ�γ�Ŀ ��Ÿ (�ټ�) ���ϴ� ����    
        bool IsFlipped_FQM(std::vector<int>& vecDeleted, const Vec3& p, const MeshIOLib::index_t vid0, const MeshIOLib::index_t vid1, const MeshIOLib::Vertex& v0);
        void UpdateTriangles_FQM(int& deletedTriCount, const int vid, const MeshIOLib::Vertex& v, const std::vector<int>& vecDeleted);
        void CompactMesh_FQM();

    private: // ���ذ� �ȵǴ� ��ɵ��� �Ʒ� ���� ����Ʈ�� ���� �ڵ� �����.
        // ����: https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification 
        double CalQuadricError(Vec3& p, const size_t v1, const size_t v2); // ũ�γ�Ŀ ��Ÿ (�ټ�) ���ϴ� ����    
        bool IsFlipped(std::vector<int>& vecDeleted, std::vector<size_t>& neighborTriangles, const Vec3& p, const MeshIOLib::index_t vid0, const MeshIOLib::index_t vid1, const MeshIOLib::Vertex& v0);

    private:
        // input
        std::vector<MeshIOLib::Vertex> m_vertices;
        std::vector<MeshIOLib::Triangle> m_triangles;

        // output
        std::vector<MeshIOLib::Vertex> m_outVertices;
        std::vector<MeshIOLib::Triangle> m_outTriangles;
        std::vector<MeshIOLib::Ref> m_refs;
#ifdef USEHALFEDGEDLL 
    private:
        CDLLManager* m_dllManager;
        CHEManager* m_halfEdgeManager;
#endif
    };
};
