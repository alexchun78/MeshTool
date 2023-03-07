#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "include/CDLLManager.h"
#include "include/CMeshIOManager.h"
#include "include/CMeshHE.h"

int main()
{
    // dll 파일을 로드한다.
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CDLLManager* dllManager = new CDLLManager(path.c_str());
    auto handle = dllManager->GetDLLHandle();
    if (handle == NULL)
        return -1;

    CMeshIOManager* ioManager = new CMeshIOManager(handle);
    bool bRtn = ioManager->LoadSTL("..\\smartslicerA.stl", handle);
    if (bRtn == false)
        return -1;

    auto verts = ioManager->GetVertexList();
    auto tris = ioManager->GetTriangleList();

    MeshHERepLib::CMeshHE* heManager = new MeshHERepLib::CMeshHE(verts.size(), tris);
    heManager->Build();


    dllManager->TerminateDLL();

    return 0;
}

