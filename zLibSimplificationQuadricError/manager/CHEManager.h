//#pragma once
//#include <vector>
//#include "../../Common/include/CMeshIO.h"
//#include "../../Common/include/CHalfEdgeRep.h"
//
//namespace MeshSimplificationQELib
//{
//    class CHEManager
//    {
//    public:
//        CHEManager(const HMODULE handle);
//        ~CHEManager();
//
//    public:
//        MeshIOLib::index_t Build(IN const unsigned long vertexListCount, IN const std::vector<MeshIOLib::Triangle>& vecTriangles);
//        const MeshHERepLib::HalfEdge& GetHalfEdge(IN const MeshIOLib::index_t id);
//        bool GetEdgeFromHalfEdge(std::pair<MeshIOLib::index_t, MeshIOLib::index_t>& edge, const MeshIOLib::index_t id);
//        MeshIOLib::index_t GetHalfEdgeIndexFromEdge(IN const MeshIOLib::index_t& v1, IN const MeshIOLib::index_t& v2);
//        bool FindVertexNeighborsFromVertex(OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid);
//        bool FindFaceNeighborsFromVertex(OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid);
//        bool FindFaceNeighborsFromFace(OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t fid);
//        MeshIOLib::index_t IsBoundaryVertex(OUT const MeshIOLib::index_t vid);
//        bool GetBoundaryVertices(OUT std::vector<MeshIOLib::index_t>& boundaryVertices);
//        bool GetBoundaryEdges(OUT std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>>& boundaryEdges);
//
//    private:
//        MeshHERepLib::CHalfEdgeRep* m_CHalfEdgeRep;
//        HMODULE m_handle;
//    };
//};
