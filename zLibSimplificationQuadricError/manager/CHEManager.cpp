//#include "../pch.h"
//#include "CHEManager.h"
//
//#define NRTRN_CHKNULL(val) if(val == NULL){return;}
//#define RTRN_CHKNULL(val) if(val == NULL){return false;}
//
//namespace MeshSimplificationQELib
//{
//    CHEManager::CHEManager(const HMODULE handle)
//    {
//        if (handle != NULL)
//        {
//            // 외부로 참조된 instance 함수를 취득
//            DLL_CREATE_HEREP pFunc = (DLL_CREATE_HEREP)::GetProcAddress(handle, "CreateHalfEdgeRep");
//            m_CHalfEdgeRep = pFunc();
//            m_handle = handle;
//        }
//    }
//
//    CHEManager::~CHEManager()
//    {
//        NRTRN_CHKNULL(m_CHalfEdgeRep);
//
//        DLL_TERMINATE_HEREP terminateFunc = (DLL_TERMINATE_HEREP)::GetProcAddress(m_handle, "TerminateHalfEdgeRep");
//        terminateFunc(m_CHalfEdgeRep);
//    }
//
//    MeshIOLib::index_t CHEManager::Build(IN const unsigned long vertexListCount, IN const std::vector<MeshIOLib::Triangle>& vecTriangles)
//    {
//        DLL_BUILD_HEREP pFunc = (DLL_BUILD_HEREP)::GetProcAddress(m_handle, "Build");
//        return pFunc(m_CHalfEdgeRep, vertexListCount, vecTriangles);
//    }
//
//    const MeshHERepLib::HalfEdge& CHEManager::GetHalfEdge(IN const MeshIOLib::index_t id)
//    {
//        DLL_GETHALFEDGE_HEREP pFunc = (DLL_GETHALFEDGE_HEREP)::GetProcAddress(m_handle, "GetHalfEdge");
//        return pFunc(m_CHalfEdgeRep, id);
//    }
//
//    bool CHEManager::GetEdgeFromHalfEdge(std::pair<MeshIOLib::index_t, MeshIOLib::index_t>& edge, const MeshIOLib::index_t id)
//    {
//        DLL_GETEDGEFROMHALFEDGE_HEREP pFunc = (DLL_GETEDGEFROMHALFEDGE_HEREP)::GetProcAddress(m_handle, "GetEdgeFromHalfEdge");
//        return pFunc(m_CHalfEdgeRep, edge, id);
//    }
//
//    MeshIOLib::index_t CHEManager::GetHalfEdgeIndexFromEdge(IN const MeshIOLib::index_t& v1, IN const MeshIOLib::index_t& v2)
//    {
//        DLL_GETHALFEDGEINDEXFROMEDGE_HEREP pFunc = (DLL_GETHALFEDGEINDEXFROMEDGE_HEREP)::GetProcAddress(m_handle, "GetHalfEdgeIndexFromEdge");
//        return pFunc(m_CHalfEdgeRep, v1, v2);
//    }
//
//    bool CHEManager::FindVertexNeighborsFromVertex(OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid)
//    {
//        DLL_FINDVERTEXNEIBORSFROMVERTEX_HEREP pFunc = (DLL_FINDVERTEXNEIBORSFROMVERTEX_HEREP)::GetProcAddress(m_handle, "FindVertexNeighborsFromVertex");
//        return pFunc(m_CHalfEdgeRep, neighbors, vid);
//    }
//
//    bool CHEManager::FindFaceNeighborsFromVertex(OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid)
//    {
//        DLL_FINDFACENEIGHBORSFROMVERTEX_HEREP pFunc = (DLL_FINDFACENEIGHBORSFROMVERTEX_HEREP)::GetProcAddress(m_handle, "FindFaceNeighborsFromVertex");
//        return pFunc(m_CHalfEdgeRep, neighbors, vid);
//    }
//
//    bool CHEManager::FindFaceNeighborsFromFace(OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t fid)
//    {
//        DLL_FINDFACENEIGHBORSFROMFACE_HEREP pFunc = (DLL_FINDFACENEIGHBORSFROMFACE_HEREP)::GetProcAddress(m_handle, "FindFaceNeighborsFromFace");
//        return pFunc(m_CHalfEdgeRep, neighbors, fid);
//    }
//
//    MeshIOLib::index_t CHEManager::IsBoundaryVertex(OUT const MeshIOLib::index_t vid)
//    {
//        DLL_ISBOUNDARYVERTEX_HEREP pFunc = (DLL_ISBOUNDARYVERTEX_HEREP)::GetProcAddress(m_handle, "IsBoundaryVertex");
//        return pFunc(m_CHalfEdgeRep, vid);
//    }
//
//    bool CHEManager::GetBoundaryVertices(OUT std::vector<MeshIOLib::index_t>& boundaryVertices)
//    {
//        DLL_GETBOUNDARYVERTICES_HEREP pFunc = (DLL_GETBOUNDARYVERTICES_HEREP)::GetProcAddress(m_handle, "GetBoundaryVertices");
//        return pFunc(m_CHalfEdgeRep, boundaryVertices);
//    }
//
//    bool CHEManager::GetBoundaryEdges(OUT std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>>& boundaryEdges)
//    {
//        DLL_GETBOUNDARYEDGES_HEREP pFunc = (DLL_GETBOUNDARYEDGES_HEREP)::GetProcAddress(m_handle, "GetBoundaryEdges");
//        return pFunc(m_CHalfEdgeRep, boundaryEdges);
//    }
//};
