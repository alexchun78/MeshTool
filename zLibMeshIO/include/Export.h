#ifndef _MESHIOLIB_EXPORT_H
#define _MESHIOLIB_EXPORT_H
#include "../include/CMeshIO.h"

#ifndef MESHIOLIB_API
#ifdef DLL_EXPORTS
#define MESHIOLIB_API _declspec(dllexport)
#else
#define MESHIOLIB_API _declspec(dllimport)
#endif
#endif

namespace MeshIOLib
{
    extern "C" {
        MESHIOLIB_API CMeshIO* CreateMeshIO();
        MESHIOLIB_API void LoadSTL(CMeshIO* object, const char* filename);
        MESHIOLIB_API std::vector<Triangle> GetTriangleList(CMeshIO* object);
        MESHIOLIB_API std::vector<Vertex> GetVertexList(CMeshIO* object);
        MESHIOLIB_API void TerminateMeshIO(CMeshIO* obj);
    }
};
#endif //_MESHIOLIB_EXPORT_H