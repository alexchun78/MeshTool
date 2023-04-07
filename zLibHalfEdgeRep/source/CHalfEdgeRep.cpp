#include "../pch.h"
#include "../include/CHalfEdgeRep.h"
#include <cassert>
#include <set>
namespace MeshHERepLib
{
    CHalfEdgeRep::CHalfEdgeRep()
    {
    }

    CHalfEdgeRep::~CHalfEdgeRep()
    {
    }

    /*****
    * input : vertex ����(�ߺ� ����), triangle ����Ʈ
    * output : halfEdge List
    * discription : half edge ����
    * reference : https://github.com/yig/halfedge
    * reference : https://jerryyin.info/geometry-processing-algorithms/half-edge/
    *****/
    bool CHalfEdgeRep::Build(const unsigned long vertexListCount, const std::vector<MeshIOLib::Triangle>& vecTriangles)
    {
        // ����ó�� (0�̸� error)
        assert(vertexListCount);
        assert(vecTriangles.size());

        // edge list(�ߺ� ����) ����
        Internal_GenUnOrderedEdgeFromTriangles(m_edgeList, vecTriangles);
        assert(m_edgeList.size());

        auto vertCount = vertexListCount;
        auto edgeCount = m_edgeList.size();
        auto faceCount = vecTriangles.size();

        // triangle�� ��ȸ�ϸ� edge�� face id�� �����Ѵ�.
        Edge2Index_map edge2fid_map;
        
        loopi(0,faceCount)
        {
            edge2fid_map[std::make_pair(vecTriangles[i].i(), vecTriangles[i].j())] = i;
            edge2fid_map[std::make_pair(vecTriangles[i].j(), vecTriangles[i].k())] = i;
            edge2fid_map[std::make_pair(vecTriangles[i].k(), vecTriangles[i].i())] = i;
        }

        // data �ʱ�ȭ
        Internal_Clear();
        m_halfEdges.reserve(edgeCount * 2);
        m_vertex_halfedges.resize(vertCount, -1);
        m_face_halfedges.resize(faceCount, -1);
        m_edge_halfedges.resize(edgeCount, -1);

        // ���� edge�� �������ش�.
        loopi(0, edgeCount)
        {
            const MeshIOLib::Edge& edge = m_edgeList[i];

            // half edge �ʱ�ȭ : ������ index�� �ʿ��ؼ� ���� �߰���
            MeshIOLib::index_t index_h0 = -1;
            MeshHERepLib::HalfEdge& h0 = Internal_AddBlankHalfEdge(index_h0, m_halfEdges);
            MeshIOLib::index_t index_h1 = -1;
            MeshHERepLib::HalfEdge& h1 = Internal_AddBlankHalfEdge(index_h1, m_halfEdges);

            // face, edge, to_vertex ���� �Է�
            h0.face = Internal_FindEdge2FaceIndex(edge2fid_map, edge._vertexID[0], edge._vertexID[1]);
            h0.edge = i;
            h0.to_vertex = edge._vertexID[1];

            h1.face = Internal_FindEdge2FaceIndex(edge2fid_map, edge._vertexID[1], edge._vertexID[0]);
            h1.edge = i;
            h1.to_vertex = edge._vertexID[0];

            // opposit half ���� �Է�
            h0.opposite_he = index_h1;
            h1.opposite_he = index_h0;

            // edge ���� map ����
            assert(m_edge2HalfEdgeMap.find(std::make_pair(edge._vertexID[0], edge._vertexID[1])) == m_edge2HalfEdgeMap.end());
            assert(m_edge2HalfEdgeMap.find(std::make_pair(edge._vertexID[1], edge._vertexID[0])) == m_edge2HalfEdgeMap.end());
            m_edge2HalfEdgeMap[std::make_pair(edge._vertexID[0], edge._vertexID[1])] = index_h0;
            m_edge2HalfEdgeMap[std::make_pair(edge._vertexID[1], edge._vertexID[0])] = index_h1;

            // ������ ���� ���� ����
            /*
            ���� ���������� ���� ���������� ���ؽ��� ������ ���������� ���ٸ�,
            �ݴ��� ���������� �����Ѵ�.
            ����, ���� ���ؽ��� �ٿ���� ���ؽ����, ������ ���������� �ٿ���� ���������̴�.
            */
            // vertex2halfEdge
            if (m_vertex_halfedges[h0.to_vertex] == -1 || h1.face == -1)
                m_vertex_halfedges[h0.to_vertex] = h0.opposite_he;
            if (m_vertex_halfedges[h1.to_vertex] == -1 || h0.face == -1)
                m_vertex_halfedges[h1.to_vertex] = h1.opposite_he;

            // face2halfEdge
            if (h0.face != -1 && m_face_halfedges[h0.face] == -1)
                m_face_halfedges[h0.face] = index_h0;
            if (h1.face != -1 && m_face_halfedges[h1.face] == -1)
                m_face_halfedges[h1.face] = index_h1;

            // edge2halfEdge
            assert(m_edge_halfedges[i] == -1);
            m_edge_halfedges[i] = index_h0;
        }

        // boundary ���� �� next halfEdge ���� �߰�
        std::vector<MeshIOLib::index_t> boundaryHalfEdgeIndices;
        loopi(0, m_halfEdges.size())
        {
            // �ٿ���� halfedge ����
            auto& he = m_halfEdges[i];
            if (he.face == -1)
            {
                boundaryHalfEdgeIndices.push_back(i);
                continue;
            }
            // next halfedge ����
            he.next_he = Internal_AddNextHalfEdge(he,vecTriangles,m_edge2HalfEdgeMap);
        }

        // butterfly vertex : Ư�� vertex���� ������ halfEdge ������ �ټ��� ���
        // [NOTE] half-edge ���������� butterfly vertex ��� ���� -> ���� halfEdge�� ���Ƿ� 1���� ����
        Internal_HandlingButterFlyVertices(m_halfEdges, boundaryHalfEdgeIndices);

        return true;
    }

    bool CHalfEdgeRep::Internal_GenUnOrderedEdgeFromTriangles(std::vector<MeshIOLib::Edge>& edgeList, const std::vector<MeshIOLib::Triangle>& tris)
    {
        assert(&tris[0]);
        std::set<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>> edgeSet;
        loopi(0, tris.size())
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

    void CHalfEdgeRep::Internal_Clear()
    {
        m_halfEdges.clear();
        m_vertex_halfedges.clear();
        m_face_halfedges.clear();
        m_edge_halfedges.clear();
        m_edge2HalfEdgeMap.clear();
    }

    MeshHERepLib::HalfEdge& CHalfEdgeRep::Internal_AddBlankHalfEdge(MeshIOLib::index_t& id, std::vector<MeshHERepLib::HalfEdge>& heList)
    {
        heList.push_back(MeshHERepLib::HalfEdge());

        id = heList.size() - 1;
        return heList.back();
    }

    MeshIOLib::index_t CHalfEdgeRep::Internal_FindEdge2FaceIndex(const Edge2Index_map& map, const MeshIOLib::index_t v0, const MeshIOLib::index_t v1)
    {
        assert(map.size());

        auto iter = map.find(std::make_pair(v0, v1));
        if (iter == map.end())
        {
            assert(map.find(std::make_pair(v1, v0)) != map.end());
            return -1;
        }
        return iter->second;
    }

    MeshIOLib::index_t CHalfEdgeRep::Internal_AddNextHalfEdge(const HalfEdge& he, const std::vector<MeshIOLib::Triangle>& tris, Edge2Index_map& halfedgeMap)
    {
        assert(tris.size() != 0);
        assert(halfedgeMap.size() != 0);
        assert(he.face != -1);

        const auto& tri = tris[he.face];
        MeshIOLib::index_t v1 = he.to_vertex;
        MeshIOLib::index_t v2 = -1;
        if (v1 == tri._vertexID[0])
            v2 = tri._vertexID[1];
        else if (v1 == tri._vertexID[1])
            v2 = tri._vertexID[2];
        else if (v1 == tri._vertexID[2])
            v2 = tri._vertexID[0];

        // debug
        //if (v2 == -1)
        //    return -1;
        assert(v2 != -1);

        return halfedgeMap[std::make_pair(v1, v2)];
    }

    void CHalfEdgeRep::Internal_HandlingButterFlyVertices(std::vector<MeshHERepLib::HalfEdge>& halfEdges, const std::vector<MeshIOLib::index_t>& boundaryHalfEdgeIndices)
    {
        // butterfly vertex : Ư�� vertex���� ������ halfEdge ������ �ټ��� ���
        // butterfly vertex ����
        // vertex, halfedge map ����
        std::map<MeshIOLib::index_t, std::set<MeshIOLib::index_t>> vertex2outgoing_boundaryHalfEdges;
        for (auto iter = boundaryHalfEdgeIndices.begin(); iter != boundaryHalfEdgeIndices.end(); ++iter)
        {
            const MeshIOLib::index_t originVert = halfEdges[halfEdges[(*iter)].opposite_he].to_vertex;
            vertex2outgoing_boundaryHalfEdges[originVert].insert((*iter));
            if (vertex2outgoing_boundaryHalfEdges[originVert].size() > 1)
                std::cerr << "butterfly vertex occurred." << std::endl;
        }
        // [NOTE] half-edge ���������� butterfly vertex ��� ���� -> ���� halfEdge�� ���Ƿ� 1���� ����
        for (auto iter = boundaryHalfEdgeIndices.begin(); iter != boundaryHalfEdgeIndices.end(); ++iter)
        {
            HalfEdge& he = halfEdges[(*iter)];
            std::set<MeshIOLib::index_t> outgoingBoundaryHalfEdges = vertex2outgoing_boundaryHalfEdges[he.to_vertex];
            if (outgoingBoundaryHalfEdges.size() != 0)
            {
                he.next_he = (*outgoingBoundaryHalfEdges.begin());
                outgoingBoundaryHalfEdges.erase(outgoingBoundaryHalfEdges.begin());
            }
        }
#ifndef NDEBUG
        for (auto iter = vertex2outgoing_boundaryHalfEdges.begin(); iter != vertex2outgoing_boundaryHalfEdges.end(); ++iter)
        {
            assert(iter->second.empty());
        }
#endif     
    }

    const HalfEdge& CHalfEdgeRep::GetHalfEdge(const MeshIOLib::index_t id) const
    {
        assert(id < m_halfEdges.size());
        return m_halfEdges.at(id);
    }

    std::pair<MeshIOLib::index_t, MeshIOLib::index_t> CHalfEdgeRep::GetEdgeFromHalfEdge(const MeshIOLib::index_t id) const
    {
        assert(id < m_halfEdges.size());
        auto& he = m_halfEdges[id];
        return std::make_pair(m_halfEdges[he.opposite_he].to_vertex, he.to_vertex);
    }

    MeshIOLib::index_t CHalfEdgeRep::GetHalfEdgeIndexFromEdge(const MeshIOLib::index_t& v1, const MeshIOLib::index_t& v2) const
    {
        assert(m_edge2HalfEdgeMap.size() != 0);
        auto result = m_edge2HalfEdgeMap.find(std::make_pair(v1, v2));
        if (result == m_edge2HalfEdgeMap.end())
            return -1;
        return result->second;
    }

    void CHalfEdgeRep::FindVertexNeighborsFromVertex(std::vector<MeshIOLib::index_t>& neighbors, const MeshIOLib::index_t vid) const
    {
        assert(vid < m_vertex_halfedges.size());

        neighbors.clear();
        auto start_index = m_vertex_halfedges[vid];
        auto hei = start_index;
        do {
            // TO DO
            auto& he = m_halfEdges[hei];
            neighbors.push_back(he.to_vertex);
            hei = m_halfEdges[he.opposite_he].next_he; // prev halfedge

        } while (hei != start_index);        
    }

    void CHalfEdgeRep::FindFaceNeighborsFromVertex(std::vector<MeshIOLib::index_t>& neighbors, MeshIOLib::index_t vid) const
    {
        assert(vid < m_vertex_halfedges.size());

        neighbors.clear();
        // face�� halfedge index�� �����´�.
        auto start_index = m_vertex_halfedges[vid];
        auto hei = start_index;
        do {

            // TO DO
            auto& he = m_halfEdges[hei];
            neighbors.push_back(he.face);
            hei = m_halfEdges[he.opposite_he].next_he; // prev halfedge�� twin 
            // counter clockwise : prev halfedge�� twin
            // clockwise : twin�� next halfedge
        } while (hei != start_index);
    }

    void CHalfEdgeRep::FindFaceNeighborsFromFace(std::vector<MeshIOLib::index_t>& neighbors, MeshIOLib::index_t fid) const
    {
        assert(fid < m_face_halfedges.size());

        neighbors.clear();
        // face�� halfedge index�� �����´�.
        auto start_index = m_face_halfedges[fid];
        auto hei = start_index;
        do {
            auto& he = m_halfEdges[hei];
            neighbors.push_back(he.face);

            hei = he.next_he;
        } while (hei != start_index);
    }

    bool CHalfEdgeRep::IsBoundaryVertex(const MeshIOLib::index_t vid) const
    {
        assert(vid < m_vertex_halfedges.size());
        return m_halfEdges[m_vertex_halfedges[vid]].face == -1;
    }

    std::vector<MeshIOLib::index_t> CHalfEdgeRep::GetBoundaryVertices() const
    {
        std::vector<MeshIOLib::index_t> boundaryVertices;

        for (auto iter = m_halfEdges.begin(); iter != m_halfEdges.end(); ++iter)
        {
            if (iter->face == -1)
            {
                boundaryVertices.push_back(iter->to_vertex);
                boundaryVertices.push_back(m_halfEdges[iter->opposite_he].to_vertex);
            }
        }
        return boundaryVertices;
    }

    std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>> CHalfEdgeRep::GetBoundaryEdges() const
    {
        std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>> boundaryEdges;

        loopi(0, m_halfEdges.size())
        {
            if (m_halfEdges[i].face == -1)
            {
                boundaryEdges.push_back(GetEdgeFromHalfEdge(i));
            }
        }
        return boundaryEdges;
    }
}