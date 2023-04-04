#ifndef _MESHIOLIB_EXPORT_H
#define _MESHIOLIB_EXPORT_H
#include "../include/CMeshIO.h"
#include <Windows.h>
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
        MESHIOLIB_API void LoadSTL(IN CMeshIO* object, IN const char* filename);
        MESHIOLIB_API void WriteOBJ(IN CMeshIO* object, IN const char* filename);
        MESHIOLIB_API void WriteOBJWithMeshData(IN CMeshIO* object, IN const char* filename, IN const std::vector<Vertex>& vecVerts, IN const std::vector<Triangle>& vecTris);
        MESHIOLIB_API std::vector<Triangle> GetTriangleList(IN CMeshIO* object);
        MESHIOLIB_API std::vector<Vertex> GetVertexList(IN CMeshIO* object);
        MESHIOLIB_API void TerminateMeshIO(IN CMeshIO* obj);
    }
};
#endif //_MESHIOLIB_EXPORT_H