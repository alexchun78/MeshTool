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

    void LoadOBJ(IN CMeshIO* object, IN const char* filename)
    {
        if (object == NULL)
            return;
        object->LoadOBJ(filename);
    }

    void WriteOBJ(IN CMeshIO* object, IN const char* filename)
    {
        if (object == NULL)
            return;
        object->WriteOBJ(filename);
    }

    void WriteOBJWithMeshData(IN CMeshIO* object, IN const char* filename, IN const std::vector<Vertex>& vecVerts, IN const std::vector<Triangle>& vecTris)
    {
        if (object == NULL)
            return;
        object->WriteOBJ(filename, vecVerts, vecTris);
    }

    void WriteSTL(IN CMeshIO* object, IN const char* filename)
    {
        if (object == NULL)
            return;
        object->WriteSTL(filename);
    }

    void WriteSTLWithMeshData(IN CMeshIO* object, IN const char* filename, IN const std::vector<Vertex>& vecVerts, IN const std::vector<Triangle>& vecTris)
    {
        if (object == NULL)
            return;
        object->WriteSTL(filename, vecVerts, vecTris);
    }

    void TerminateMeshIO(IN CMeshIO* object)
    {
        // ∞¥√º «ÿ¡¶
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