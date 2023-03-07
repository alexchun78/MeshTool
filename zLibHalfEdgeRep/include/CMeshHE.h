#ifndef CMESHHE_H
#define CMESHHE_H
#include "../include/MeshHEData.h"
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CMeshIOData.h"
#include "MeshHEData.h"
#include <vector>
#include <map>

namespace MeshHERepLib
{
    class CMeshHE 
    {
    public:
        CMeshHE(const unsigned long vertexCount, const std::vector<MeshIOLib::Triangle>& vecTriangles);
        ~CMeshHE();
    
        bool Build();

    private:
        bool GenUnOrderedEdgeFromTriangles(std::vector<MeshIOLib::Edge>& edgeList, const std::vector<MeshIOLib::Triangle>& tris);
       // MeshIOLib::index_t AddBlankHalfEdge(MeshHERepLib::HalfEdge& blankHE, std::vector<MeshHERepLib::HalfEdge> heList);
        MeshHERepLib::HalfEdge& AddBlankHalfEdge(MeshIOLib::index_t& id, std::vector<MeshHERepLib::HalfEdge> heList);
        void Clear();



    private:
        unsigned long m_vertexCount;
        std::vector<MeshIOLib::Triangle> m_vecTriangles;
        std::vector<MeshIOLib::Edge> m_edgeList;

        std::vector<MeshHERepLib::HalfEdge> m_halfEdges;
        std::vector< MeshIOLib::index_t > m_vertex_halfedges; // 'halfedges' per vertex
        std::vector< MeshIOLib::index_t > m_face_halfedges; // 'halfedges' per face
        std::vector< MeshIOLib::index_t > m_edge_halfedges;// 'halfedges' per face (unordered pair of vertex indices)
        typedef std::map< std::pair<MeshIOLib::index_t, MeshIOLib::index_t>, MeshIOLib::index_t > Edge2Index_map;
        Edge2Index_map m_directedEdge2HalfEdgeMap;
    };
}
#endif // CMESHHE_H

