#ifndef _MESHISIMPLIFICATION_EXPORT_H
#define _MESHISIMPLIFICATION_EXPORT_H
#include "../include/CSimplificationQuadricError.h"
#include "../../Common/include/CMeshIO.h"
#include <Windows.h>
#ifndef MESHSIMPLIFICATION_API
#ifdef DLL_EXPORTS
#define MESHSIMPLIFICATION_API _declspec(dllexport)
#else
#define MESHSIMPLIFICATION_API _declspec(dllimport)
#endif
#endif

namespace MeshSimplificationQELib
{
    extern "C" {
        MESHSIMPLIFICATION_API CSimplificationQuadricError* CreateSimplificationQE(IN const std::vector<MeshIOLib::Vertex>& verts, IN const std::vector<MeshIOLib::Triangle>& tris);
        //MESHSIMPLIFICATION_API CSimplificationQuadricError* CreateSimplificationQE();
        MESHSIMPLIFICATION_API bool DoSimplification(IN CSimplificationQuadricError* object, IN const float reductionRate, IN const float agressive);
        MESHSIMPLIFICATION_API void GetSimplificationOutputData(IN CSimplificationQuadricError* object, OUT std::vector<MeshIOLib::Vertex>& verts, OUT std::vector<MeshIOLib::Triangle>& tris);
        MESHSIMPLIFICATION_API void TerminateSimplificationQE(IN CSimplificationQuadricError* object);
    }
}
#endif //_MESHISIMPLIFICATION_EXPORT_H