#include "Export.h"

namespace MeshIOLib
{
    // Export DLL
    CMeshIO* CreateMeshIO()
    {
        return new CMeshIO();
    }

    void LoadSTL(IN CMeshIO* object, IN const char* filename)
    {
        if (object == NULL)
            return;
        object->LoadSTL(filename);
    }

    void TerminateMeshIO(IN CMeshIO* object)
    {
        // ��ü ����
        if (object != nullptr)
        {
            delete object;
            object = nullptr;
        }
    }

    std::vector<Triangle> GetTriangleList(IN CMeshIO* object)
    {
        if (object == NULL)
            return std::vector<Triangle>();
        return object->GetTriangleList();
    }

    std::vector<Vertex> GetVertexList(IN CMeshIO* object)
    {
        if (object == NULL)
            return std::vector<Vertex>();
        return object->GetVertexList();
    }
};