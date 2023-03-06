#ifndef PCH_H
#define PCH_H
#include "../Common/include/CMeshIO.h"
#include "../Common/include/CMeshIOData.h"

// add headers that you want to pre-compile here
typedef MeshIOLib::CMeshIO* (*DLL_CREATE_MESHIO)();
typedef void (*DLL_LOADSTL_MESHIO)(MeshIOLib::CMeshIO* object, const char* filename);
typedef std::vector<MeshIOLib::Vertex>(*DLL_GETVERTEX_MESHIO)(MeshIOLib::CMeshIO* object);
typedef std::vector<MeshIOLib::Triangle>(*DLL_GETTRIANGLE_MESHIO)(MeshIOLib::CMeshIO* object);


#endif //PCH_H
