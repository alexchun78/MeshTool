#include "../pch.h"
#include <iostream>
#include <Windows.h>
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CMeshIOData.h"

//typedef class_to_export* (*DLL_CREATE)();
//typedef int (*DLL_SUM)(class_to_export* obj, int a, int b);

typedef MeshIO::CMeshIO* (*DLL_CREATE_MESHIO)();
typedef void (*DLL_LOADSTL_MESHIO)(MeshIO::CMeshIO* object, const char* filename);
typedef std::vector<MeshIO::Vertex> (*DLL_GETVERTEX_MESHIO)(MeshIO::CMeshIO* object);


int main()
{
 //   std::cout << "Hello World!\n";
    // dll 파일을 로드한다.
    std::string path = "../Common/dll/zLibMeshIO.dll";
    HMODULE dll = LoadLibraryA(path.c_str());

    if (dll != NULL)
    {
        // 외부로 참조된 instance 함수를 취득
        DLL_CREATE_MESHIO pFunc = (DLL_CREATE_MESHIO)::GetProcAddress(dll, "CreateMeshIO");
        MeshIO::CMeshIO* obj = pFunc();

        DLL_LOADSTL_MESHIO loadSTLFunc = (DLL_LOADSTL_MESHIO)::GetProcAddress(dll, "LoadSTL");
        loadSTLFunc(obj, "..\\smartslicerA.stl");

        DLL_GETVERTEX_MESHIO getVertexListFunc = (DLL_GETVERTEX_MESHIO)::GetProcAddress(dll, "GetVertexList");
        std::vector<MeshIO::Vertex> verts = getVertexListFunc(obj);
        ::FreeLibrary(dll);
    }


    // dll 파일을 로드한다.
    //std::string path = "../Common/dll/zLibHalfEdgeRep.dll";
    //HMODULE dll = LoadLibraryA(path.c_str());

    //// 로드가 실패하면 종료.
    //if (dll != NULL) {
    //    // 외부로 참조된 instance 함수를 취득
    //    DLL_CREATE pFunc = (DLL_CREATE)::GetProcAddress(dll, "CreateClass");
    //    class_to_export* obj = pFunc();

    //    DLL_SUM pFunc2 = (DLL_SUM)::GetProcAddress(dll, "Sum");
    //    int result = pFunc2(obj, 10, 12);
    //    //int result2 = obj->sum(10, 12);
    //    std::cout << result << std::endl;
    //    //std::cout << result2 << std::endl;



    //    ::FreeLibrary(dll);
    //}
}
/*
extern "C" {
    MESHIOLIB_API CMeshIO* CreateClass();
    MESHIOLIB_API void LoadSTL(CMeshIO* object, const char* filename);
    MESHIOLIB_API std::vector<Triangle> GetTriangleList(CMeshIO* object);
    MESHIOLIB_API std::vector<Vertex> GetVertexList(CMeshIO* object);
    MESHIOLIB_API void Terminate(CMeshIO* obj);
}
*/

