// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <Windows.h>
#include "../Common/include/CMeshIO.h"
#include "../Common/include/CHalfEdgeRep.h"
#include "../Common/include/CMeshIOData.h"

// add headers that you want to pre-compile here
// // MeshIO
typedef MeshIOLib::CMeshIO* (*DLL_CREATE_MESHIO)();
typedef void (*DLL_LOADSTL_MESHIO)(MeshIOLib::CMeshIO* object, const char* filename);
typedef void(*DLL_WRITEOBJWITHMESHDATA_MESHIO)(IN MeshIOLib::CMeshIO* object, IN const char* filename, IN const std::vector<MeshIOLib::Vertex>& vecVerts, IN const std::vector<MeshIOLib::Triangle>& vecTris);
typedef std::vector<MeshIOLib::Vertex>(*DLL_GETVERTEX_MESHIO)(MeshIOLib::CMeshIO* object);
typedef std::vector<MeshIOLib::Triangle>(*DLL_GETTRIANGLE_MESHIO)(MeshIOLib::CMeshIO* object);
typedef void(*DLL_TERMINATE_MESHIO)(MeshIOLib::CMeshIO* object);

// // HalfEdge
typedef MeshHERepLib::CHalfEdgeRep* (*DLL_CREATE_HEREP)();
typedef MeshIOLib::index_t(*DLL_BUILD_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, IN const unsigned long vertexListCount, IN const std::vector<MeshIOLib::Triangle>& vecTriangles);
typedef const MeshHERepLib::HalfEdge& (*DLL_GETHALFEDGE_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, IN const MeshIOLib::index_t id);
typedef bool (*DLL_GETEDGEFROMHALFEDGE_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT std::pair<MeshIOLib::index_t, MeshIOLib::index_t>& edge, IN const MeshIOLib::index_t id);
typedef MeshIOLib::index_t(*DLL_GETHALFEDGEINDEXFROMEDGE_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, IN const MeshIOLib::index_t& v1, IN const MeshIOLib::index_t& v2);
typedef bool (*DLL_FINDVERTEXNEIBORSFROMVERTEX_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid);
typedef bool (*DLL_FINDFACENEIGHBORSFROMVERTEX_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid);
typedef bool (*DLL_FINDFACENEIGHBORSFROMFACE_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t fid);
typedef MeshIOLib::index_t(*DLL_ISBOUNDARYVERTEX_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT const MeshIOLib::index_t vid);
typedef bool (*DLL_GETBOUNDARYVERTICES_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& boundaryVertices);
typedef bool (*DLL_GETBOUNDARYEDGES_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object, OUT std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>>& boundaryEdges);
typedef void (*DLL_TERMINATE_HEREP)(IN MeshHERepLib::CHalfEdgeRep* object);

#endif //PCH_H
