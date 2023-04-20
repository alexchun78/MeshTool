// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define loopi(start,end) for(size_t i = start; i < end ; ++i)
#define loopj(start,end) for(size_t j = start; j < end ; ++j)
#define loopk(start,end) for(size_t k = start; k < end ; ++k)

// add headers that you want to pre-compile here
//#include "../Common/include/CMeshIO.h"
//#include "../Common/include/CMeshIOData.h"

// add headers that you want to pre-compile here
//typedef MeshIOLib::CMeshIO* (*DLL_CREATE_MESHIO)();
//typedef void (*DLL_LOADSTL_MESHIO)(MeshIOLib::CMeshIO* object, const char* filename);
//typedef std::vector<MeshIOLib::Vertex>(*DLL_GETVERTEX_MESHIO)(MeshIOLib::CMeshIO* object);
//typedef std::vector<MeshIOLib::Triangle>(*DLL_GETTRIANGLE_MESHIO)(MeshIOLib::CMeshIO* object);

#endif //PCH_H
