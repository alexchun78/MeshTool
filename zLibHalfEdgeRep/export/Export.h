#ifndef _HALFEDGEREPLIB_EXPORT_H
#define _HALFEDGEREPLIB_EXPORT_H
#include "../include/CHalfEdgeRep.h"
#include <Windows.h>
#ifndef HALFEDGEREPLIB_API
#ifdef DLL_EXPORTS
#define HALFEDGEREPLIB_API _declspec(dllexport)
#else
#define HALFEDGEREPLIB_API _declspec(dllimport)
#endif
#endif

namespace MeshHERepLib
{
    extern "C" {
        HALFEDGEREPLIB_API CHalfEdgeRep* CreateHalfEdgeRep();
        HALFEDGEREPLIB_API MeshIOLib::index_t Build(IN CHalfEdgeRep* object, IN const unsigned long vertexListCount, IN const std::vector<MeshIOLib::Triangle>& vecTriangles);
        HALFEDGEREPLIB_API const HalfEdge& GetHalfEdge(IN CHalfEdgeRep* object, IN const MeshIOLib::index_t id);
        HALFEDGEREPLIB_API bool GetEdgeFromHalfEdge(IN CHalfEdgeRep* object, OUT std::pair<MeshIOLib::index_t, MeshIOLib::index_t>& edge, IN const MeshIOLib::index_t id);
        HALFEDGEREPLIB_API MeshIOLib::index_t GetHalfEdgeIndexFromEdge(IN CHalfEdgeRep* object, IN const MeshIOLib::index_t& v1, IN const MeshIOLib::index_t& v2);    
        HALFEDGEREPLIB_API bool FindVertexNeighborsFromVertex(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid);
        HALFEDGEREPLIB_API bool FindFaceNeighborsFromVertex(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid);
        HALFEDGEREPLIB_API bool FindFaceNeighborsFromFace(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t fid);
        HALFEDGEREPLIB_API MeshIOLib::index_t IsBoundaryVertex(IN CHalfEdgeRep* object, OUT const MeshIOLib::index_t vid);
        HALFEDGEREPLIB_API bool GetBoundaryVertices(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& boundaryVertices);
        HALFEDGEREPLIB_API bool GetBoundaryEdges(IN CHalfEdgeRep* object, OUT std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>>& boundaryEdges);
        HALFEDGEREPLIB_API void TerminateHalfEdgeRep(IN CHalfEdgeRep* object);
    }
};
#endif //_HALFEDGEREPLIB_EXPORT_H