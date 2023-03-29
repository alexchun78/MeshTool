#include "../pch.h"
#include <iostream>
#include <Windows.h>
#include "../include/CDLLManager.h"
#include "../include/CMeshIOManager.h"
#include "../include/CHEManager.h"

int main()
{
    // dll 파일을 로드한다.
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CDLLManager* dllManager = new CDLLManager(path.c_str());
    auto handle = dllManager->GetDLLHandle();
    if (handle == NULL)
        return -1;
    CMeshIOManager* ioManager = new CMeshIOManager(handle);
    bool bRtn = ioManager->LoadSTL("..\\smartslicerA.stl");
    if (bRtn == false)
        return -1;

    auto verts = ioManager->GetVertexList();
    auto tris = ioManager->GetTriangleList();

    path = "../Common/dll/zLibHalfEdgeRep.dll";
    CDLLManager* dllManager2 = new CDLLManager(path.c_str());
    auto handle2 = dllManager2->GetDLLHandle();
    if (handle2 == NULL)
        return -1;

    CHEManager* heManager = new CHEManager(handle2);
    auto nRtn = heManager->Build(verts.size(), tris);
    if (nRtn != 1)
        return -1;
    std::vector<MeshIOLib::index_t> neighbors;
    heManager->FindVertexNeighborsFromVertex(neighbors, 3);


    dllManager->TerminateDLL();
    dllManager2->TerminateDLL();

    return 0;
}
