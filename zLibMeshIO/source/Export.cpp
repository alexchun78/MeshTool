#include "../include/Export.h"

namespace MeshIOLib
{
    // Export DLL
    CMeshIO* CreateMeshIO()
    {
        return new CMeshIO();
    }

    void LoadSTL(CMeshIO* object, const char* filename)
    {
        if (object == NULL)
            return;
        object->LoadSTL(filename);
    }

    void TerminateMeshIO(CMeshIO* object)
    {
        // ∞¥√º «ÿ¡¶
        if (object != nullptr)
        {
            delete object;
            object = nullptr;
        }
    }

    std::vector<Triangle> GetTriangleList(CMeshIO* object)
    {
        if (object == NULL)
            return std::vector<Triangle>();
        return object->GetTriangleList();
    }

    std::vector<Vertex> GetVertexList(CMeshIO* object)
    {
        if (object == NULL)
            return std::vector<Vertex>();
        return object->GetVertexList();
    }
};