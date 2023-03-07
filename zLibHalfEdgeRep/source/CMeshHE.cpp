#include "../pch.h"
#include "../include/CMeshHE.h"
#include <cassert>
#include <set>

namespace MeshHERepLib
{
    CMeshHE::CMeshHE(const unsigned long vertexCount, const std::vector<MeshIOLib::Triangle>& vecTriangles)
    {
        m_vertexCount = vertexCount;
        m_vecTriangles = vecTriangles;
    }

    CMeshHE::~CMeshHE()
    {

    }

    /*****
    * input : vertex 개수(중복 제거), triangle 리스트
    * output : halfEdge List 
    * discription : half edge 구성
    * reference : https://github.com/yig/halfedge
    *****/
    bool CMeshHE::Build()
    {
        // 예외처리 (0이면 error)
        assert(m_vertexCount);
        assert(m_vecTriangles.size());
        
        // edge list(중복 없음) 구성
        GenUnOrderedEdgeFromTriangles(m_edgeList, m_vecTriangles);
        assert(m_edgeList.size());

        auto vertCount = m_vertexCount;
        auto edgeCount = m_edgeList.size();
        auto faceCount = m_vecTriangles.size();

        // triangle을 순회하며 edge와 face id를 매핑한다.
        Edge2Index_map edge2fid_map;
        for (auto f = 0; f < faceCount; ++f)
        {
            edge2fid_map[std::make_pair(m_vecTriangles[f].i(), m_vecTriangles[f].j())] = f;
            edge2fid_map[std::make_pair(m_vecTriangles[f].j(), m_vecTriangles[f].k())] = f;
            edge2fid_map[std::make_pair(m_vecTriangles[f].k(), m_vecTriangles[f].i())] = f;
        }

        // data 초기화
        Clear();
        m_halfEdges.reserve(edgeCount * 2);
        m_vertex_halfedges.resize(vertCount, -1);
        m_face_halfedges.resize(faceCount, -1);
        m_edge_halfedges.resize(edgeCount, -1);

        // 먼저 edge를 구성해준다.
        for (int e = 0; e < edgeCount; ++e) 
        {
            const MeshIOLib::Edge& edge = m_edgeList[e];

            // half edge 초기화 : 서로의 index가 필요해서 먼저 추가함
            MeshIOLib::index_t index_h0 = -1;
            MeshHERepLib::HalfEdge& h0 = AddBlankHalfEdge(index_h0, m_halfEdges);
            MeshIOLib::index_t index_h1 = -1;
            MeshHERepLib::HalfEdge& h1 = AddBlankHalfEdge(index_h1, m_halfEdges);

            // face, edge, to_vertex 정보 추출
            auto iter = edge2fid_map.find(std::make_pair(edge._vertexID[0], edge._vertexID[1]));
            h0.face = iter->second;
        }



        return true;
    }

    bool CMeshHE::GenUnOrderedEdgeFromTriangles(std::vector<MeshIOLib::Edge>& edgeList, const std::vector<MeshIOLib::Triangle>& tris)
    {
        assert(&tris[0]);
        std::set<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>> edgeSet;
        for (int i = 0; i < tris.size(); ++i)
        {
            edgeSet.insert(std::make_pair(std::min(tris[i].i(), tris[i].j()), std::max(tris[i].i(), tris[i].j())));
            edgeSet.insert(std::make_pair(std::min(tris[i].j(), tris[i].k()), std::max(tris[i].j(), tris[i].k())));
            edgeSet.insert(std::make_pair(std::min(tris[i].i(), tris[i].k()), std::max(tris[i].i(), tris[i].k())));
        }

        edgeList.clear();
        edgeList.reserve(edgeSet.size());
        for (auto iter = edgeSet.begin(); iter != edgeSet.end(); ++iter)
            edgeList.push_back(MeshIOLib::Edge(iter->first, iter->second));

        return true;
    }

    void CMeshHE::Clear()
    {
        m_halfEdges.clear();
        m_vertex_halfedges.clear();
        m_face_halfedges.clear();
        m_edge_halfedges.clear();
        m_directedEdge2HalfEdgeMap.clear();
    }

    //MeshIOLib::index_t CMeshHE::AddBlankHalfEdge(MeshHERepLib::HalfEdge& blankHE, std::vector<MeshHERepLib::HalfEdge> heList)
    //{        
    //    heList.push_back(MeshHERepLib::HalfEdge());
    //    blankHE = heList.back(); 
    //    return heList.size() - 1;  
    //}

    MeshHERepLib::HalfEdge& CMeshHE::AddBlankHalfEdge(MeshIOLib::index_t& id, std::vector<MeshHERepLib::HalfEdge> heList)
    {
        heList.push_back(MeshHERepLib::HalfEdge());
        
        id = heList.size() - 1;
        return heList.back();
    }
}