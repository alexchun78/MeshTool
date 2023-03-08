#ifndef CMESHHE_H
#define CMESHHE_H
#include "../include/MeshHEData.h"
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CMeshIOData.h"
#include "MeshHEData.h"
#include <vector>
#include <map>
typedef std::map< std::pair<MeshIOLib::index_t, MeshIOLib::index_t>, MeshIOLib::index_t > Edge2Index_map;

namespace MeshHERepLib
{
    class CMeshHE 
    {
    public:
        CMeshHE(const unsigned long vertexCount, const std::vector<MeshIOLib::Triangle>& vecTriangles);
        ~CMeshHE();
    
        bool Build(const unsigned long vertexListCount, const std::vector<MeshIOLib::Triangle>& vecTriangles);

    private:
        bool Internal_GenUnOrderedEdgeFromTriangles(std::vector<MeshIOLib::Edge>& edgeList, const std::vector<MeshIOLib::Triangle>& tris);
        MeshHERepLib::HalfEdge& Internal_AddBlankHalfEdge(MeshIOLib::index_t& id, std::vector<MeshHERepLib::HalfEdge>& heList);
        MeshIOLib::index_t Internal_FindEdge2FaceIndex(const Edge2Index_map& map, const MeshIOLib::index_t v0, const MeshIOLib::index_t v1);
        void Internal_Clear();
        MeshIOLib::index_t Internal_AddNextHalfEdge(const HalfEdge& he, const std::vector<MeshIOLib::Triangle>& tris, Edge2Index_map& halfedgeMap);
        void Internal_HandlingButterFlyVertices(std::vector<MeshHERepLib::HalfEdge>& halfEdges, const std::vector<MeshIOLib::index_t>& boundaryHalfEdgeIndices);
    
    private:
        // 입력 받아야 하는 데이터들
        unsigned long m_vertexCount;
        std::vector<MeshIOLib::Triangle> m_vecTriangles;

    private:
        // 생성되는 데이터들
        std::vector<MeshIOLib::Edge> m_edgeList;
        std::vector<MeshHERepLib::HalfEdge> m_halfEdges;
        std::vector< MeshIOLib::index_t > m_vertex_halfedges; // 'halfedges' per vertex
        std::vector< MeshIOLib::index_t > m_face_halfedges; // 'halfedges' per face
        std::vector< MeshIOLib::index_t > m_edge_halfedges;// 'halfedges' per face (unordered pair of vertex indices)
        Edge2Index_map m_edge2HalfEdgeMap;
    };
}
#endif // CMESHHE_H

