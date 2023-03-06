#include "../pch.h"
#include "../include/CMeshIOManager.h"

CMeshIOManager::CMeshIOManager(const HMODULE handle)
{
    if (handle != NULL)
    {
        // 외부로 참조된 instance 함수를 취득
        DLL_CREATE_MESHIO pFunc = (DLL_CREATE_MESHIO)::GetProcAddress(handle, "CreateMeshIO");
        m_CMeshIO = pFunc();
    }
}
//
//CMeshIOManager::CMeshIOManager(const char* path)
//{
//    m_dll = LoadLibraryA(path);
//    if (m_dll != NULL)
//    {
//        // 외부로 참조된 instance 함수를 취득
//        DLL_CREATE_MESHIO pFunc = (DLL_CREATE_MESHIO)::GetProcAddress(m_dll, "CreateMeshIO");
//        m_CMeshIO = pFunc();
//    }
//}

CMeshIOManager::~CMeshIOManager()
{

}

bool CMeshIOManager::LoadOBJ(const char* filename, HMODULE handle)
{
    return false;
}

bool CMeshIOManager::LoadSTL(const char* filepath, HMODULE handle)
{
    if (handle == NULL)
        return false;

    if (m_CMeshIO == NULL)
        return false;

    DLL_LOADSTL_MESHIO loadSTLFunc = (DLL_LOADSTL_MESHIO)::GetProcAddress(handle, "LoadSTL");
    loadSTLFunc(m_CMeshIO, filepath);

    DLL_GETVERTEX_MESHIO getVertexListFunc = (DLL_GETVERTEX_MESHIO)::GetProcAddress(handle, "GetVertexList");
    m_vecVertices = getVertexListFunc(m_CMeshIO);

    DLL_GETTRIANGLE_MESHIO getTriangleListFunc = (DLL_GETTRIANGLE_MESHIO)::GetProcAddress(handle, "GetTriangleList");
    m_vecTriangles = getTriangleListFunc(m_CMeshIO);

    return true;
}


//bool CMeshIOManager::LoadSTL(const char* filepath)
//{
//    if (m_dll == NULL)
//        return false;
//
//    if (m_CMeshIO == NULL)
//        return false;
//
//    DLL_LOADSTL_MESHIO loadSTLFunc = (DLL_LOADSTL_MESHIO)::GetProcAddress(m_dll, "LoadSTL");
//    loadSTLFunc(m_CMeshIO, filepath);
//
//    //DLL_GETVERTEX_MESHIO getVertexListFunc = (DLL_GETVERTEX_MESHIO)::GetProcAddress(m_dll, "GetVertexList");
//    //m_vecVertices = getVertexListFunc(m_CMeshIO);
//
//    //DLL_GETTRIANGLE_MESHIO getTriangleListFunc = (DLL_GETTRIANGLE_MESHIO)::GetProcAddress(m_dll, "GetTriangleList");
//    //m_vecTriangles = getTriangleListFunc(m_CMeshIO);
//    BOOL test = FreeLibrary(m_dll);
//
//    return true;
//}
