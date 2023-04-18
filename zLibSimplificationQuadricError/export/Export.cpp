#include "Export.h"

namespace MeshSimplificationQELib
{
    CSimplificationQuadricError* CreateSimplificationQE(IN const std::vector<MeshIOLib::Vertex>& verts, IN const std::vector<MeshIOLib::Triangle>& tris)
    {
        return new CSimplificationQuadricError(verts, tris);
    }

    bool DoSimplification(IN CSimplificationQuadricError* object, IN const float reductionRate, IN const float agressive)
    {
        if (object == NULL)
            return false;

        return object->DoSimplification(reductionRate, agressive);
    }
    void GetSimplificationOutputData(IN CSimplificationQuadricError* object, OUT std::vector<MeshIOLib::Vertex>& verts, OUT std::vector<MeshIOLib::Triangle>& tris)
    {
        if (object == NULL)
            return;
        object->GetSimplificationOutputData(verts, tris);
    }

    void TerminateSimplificationQE(IN CSimplificationQuadricError* object)
    {
        if (object != nullptr)
        {
            delete object;
            object = nullptr;
        }
    }
};