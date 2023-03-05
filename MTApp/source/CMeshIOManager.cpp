#include "../pch.h"
#include "../include/CMeshIOManager.h"

typedef MeshIOLib::CMeshIO* (*DLL_CREATE_MESHIO)();
typedef void (*DLL_LOADSTL_MESHIO)(MeshIOLib::CMeshIO* object, const char* filename);
typedef std::vector<MeshIOLib::Vertex>(*DLL_GETVERTEX_MESHIO)(MeshIOLib::CMeshIO* object);
typedef std::vector<MeshIOLib::Triangle>(*DLL_GETTRIANGLE_MESHIO)(MeshIOLib::CMeshIO* object);

CMeshIOManager::CMeshIOManager()
{
    m_CMeshIO = NULL;
    m_dll = NULL;
}

CMeshIOManager::CMeshIOManager(const char* path)
{
    m_dll = LoadLibraryA(path);
    if (m_dll != NULL)
    {
        // 외부로 참조된 instance 함수를 취득
        DLL_CREATE_MESHIO pFunc = (DLL_CREATE_MESHIO)::GetProcAddress(m_dll, "CreateMeshIO");
        m_CMeshIO = pFunc();
    }
}

CMeshIOManager::~CMeshIOManager()
{

}

bool CMeshIOManager::LoadOBJ(const char* filename)
{
    return false;
}

bool CMeshIOManager::LoadSTL(const char* filepath)
{
    if (m_dll == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_LOADSTL_MESHIO loadSTLFunc = (DLL_LOADSTL_MESHIO)::GetProcAddress(m_dll, "LoadSTL");
    loadSTLFunc(m_CMeshIO, filepath);

    DLL_GETVERTEX_MESHIO getVertexListFunc = (DLL_GETVERTEX_MESHIO)::GetProcAddress(m_dll, "GetVertexList");
    m_vecVertices = getVertexListFunc(m_CMeshIO);

    DLL_GETTRIANGLE_MESHIO getTriangleListFunc = (DLL_GETTRIANGLE_MESHIO)::GetProcAddress(m_dll, "GetTriangleList");
    m_vecTriangles = getTriangleListFunc(m_CMeshIO);

    return true;
}

void CMeshIOManager::TerminateDLL()
{
    ::FreeLibrary(m_dll);
}