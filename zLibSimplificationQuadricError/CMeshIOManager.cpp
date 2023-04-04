#include "pch.h"
#include "CMeshIOManager.h"

CMeshIOManager::CMeshIOManager(const HMODULE handle)
{
    if (handle != NULL)
    {
        // 외부로 참조된 instance 함수를 취득
        DLL_CREATE_MESHIO pFunc = (DLL_CREATE_MESHIO)::GetProcAddress(handle, "CreateMeshIO");
        m_CMeshIO = pFunc();
        m_handle = handle;
    }
}

CMeshIOManager::~CMeshIOManager()
{
    if (m_CMeshIO == NULL)
        return;

    DLL_TERMINATE_MESHIO terminateFunc = (DLL_TERMINATE_MESHIO)::GetProcAddress(m_handle, "TerminateMeshIO");
    terminateFunc(m_CMeshIO);
}

bool CMeshIOManager::LoadOBJ(const char* filename)
{
    return false;
}

bool CMeshIOManager::LoadSTL(const char* filepath)
{
    if (m_handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_LOADSTL_MESHIO loadSTLFunc = (DLL_LOADSTL_MESHIO)::GetProcAddress(m_handle, "LoadSTL");
    loadSTLFunc(m_CMeshIO, filepath);

    DLL_GETVERTEX_MESHIO getVertexListFunc = (DLL_GETVERTEX_MESHIO)::GetProcAddress(m_handle, "GetVertexList");
    m_vecVertices = getVertexListFunc(m_CMeshIO);

    DLL_GETTRIANGLE_MESHIO getTriangleListFunc = (DLL_GETTRIANGLE_MESHIO)::GetProcAddress(m_handle, "GetTriangleList");
    m_vecTriangles = getTriangleListFunc(m_CMeshIO);

    return true;
}

void CMeshIOManager::WriteOBJWithMeshData(const char* filename, const std::vector<MeshIOLib::Vertex>& vecVerts, const std::vector<MeshIOLib::Triangle>& vecTris)
{
    if (m_handle == NULL)
        return;

    if (m_CMeshIO == NULL)
        return;

    DLL_WRITEOBJWITHMESHDATA_MESHIO WriteOBJFunc = (DLL_WRITEOBJWITHMESHDATA_MESHIO)::GetProcAddress(m_handle, "WriteOBJWithMeshData");
    WriteOBJFunc(m_CMeshIO, filename, vecVerts, vecTris);
    return;
}