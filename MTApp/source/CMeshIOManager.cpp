#include "../pch.h"
#include "../include/CMeshIOManager.h"

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

bool CMeshIOManager::LoadOBJ(const char* filepath)
{
    if (m_handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_LOADOBJ_MESHIO loadOBJFunc = (DLL_LOADOBJ_MESHIO)::GetProcAddress(m_handle, "LoadOBJ");
    loadOBJFunc(m_CMeshIO, filepath);

    DLL_GETVERTEX_MESHIO getVertexListFunc = (DLL_GETVERTEX_MESHIO)::GetProcAddress(m_handle, "GetVertexList");
    m_vecVertices = getVertexListFunc(m_CMeshIO);

    DLL_GETTRIANGLE_MESHIO getTriangleListFunc = (DLL_GETTRIANGLE_MESHIO)::GetProcAddress(m_handle, "GetTriangleList");
    m_vecTriangles = getTriangleListFunc(m_CMeshIO);

    return true;
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

bool CMeshIOManager::WriteOBJ(const char* filepath)
{
    if (m_handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_WRITEOBJ_MESHIO writeOBJFunc = (DLL_WRITEOBJ_MESHIO)::GetProcAddress(m_handle, "WriteOBJ");
    writeOBJFunc(m_CMeshIO, filepath);
    return true;
}

bool CMeshIOManager::WriteSTL(const char* filepath)
{
    if (m_handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_WRITESTL_MESHIO writeSTLFunc = (DLL_WRITESTL_MESHIO)::GetProcAddress(m_handle, "WriteSTL");
    writeSTLFunc(m_CMeshIO, filepath);
    return true;
}

bool CMeshIOManager::WriteOBJwithData(const char* filepath, const std::vector<MeshIOLib::Vertex>& vecVerts, const std::vector<MeshIOLib::Triangle>& vecTris)
{
    if (m_handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_WRITEOBJWITHDATA_MESHIO writeOBJFunc = (DLL_WRITEOBJWITHDATA_MESHIO)::GetProcAddress(m_handle, "WriteOBJWithMeshData");
    writeOBJFunc(m_CMeshIO, filepath, vecVerts, vecTris);
    return true;
}

bool CMeshIOManager::WriteSTLwithData(const char* filepath, const std::vector<MeshIOLib::Vertex>& vecVerts, const std::vector<MeshIOLib::Triangle>& vecTris)
{
    if (m_handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;


    DLL_WRITESTLWITHDATA_MESHIO writeSTLFunc = (DLL_WRITESTLWITHDATA_MESHIO)::GetProcAddress(m_handle, "WriteSTLWithMeshData");
    writeSTLFunc(m_CMeshIO, filepath, vecVerts, vecTris);
    return true;
}