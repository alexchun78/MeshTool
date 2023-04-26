#include "../pch.h"
#include <iostream>
#include <Windows.h>
#include "../include/CDLLManager.h"
#include "../include/CMeshIOManager.h"
#include "../include/CHEManager.h"
#include "../include/CMeshSimplificationManager.h"
#include <time.h>

int main()
{
    // [1] Mesh IO DLL TEST
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CDLLManager* dllIO = new CDLLManager(path.c_str());
    auto handle = dllIO->GetDLLHandle();
    if (handle == NULL)
        return -1;

    const char* inputData = "..\\DATA\\input\\SurgicalGuide_1.stl";
    const char* outputData = "..\\DATA\\output\\SurgicalGuide_1_75.stl";
    float reductionRate = 0.75f;
    CMeshIOManager* ioManager = new CMeshIOManager(handle);
    bool bRtn = ioManager->LoadSTL(inputData);
    //bool bRtn = ioManager->LoadSTL("..\\DATA\\input\\2_bunnyO_new.stl");
    //bool bRtn = ioManager->LoadSTL("..\\DATA\\input\\prosthesis.stl");
    //bool bRtn = ioManager->LoadSTL("..\\DATA\\input\\SurgicalGuide_1.stl");
    //bool bRtn = ioManager->LoadSTL("..\\DATA\\input\\cast3br.stl");
    if (bRtn == false)
        return -1;

    auto verts = ioManager->GetVertexList();
    auto tris = ioManager->GetTriangleList();

    // [2] Half Edge DLL TEST
    path = "../Common/dll/zLibHalfEdgeRep.dll";
    CDLLManager* dllHalfEdge = new CDLLManager(path.c_str());
    auto handle2 = dllHalfEdge->GetDLLHandle();
    if (handle2 == NULL)
        return -1;

    CHEManager* heManager = new CHEManager(handle2);
    auto nRtn = heManager->Build(verts.size(), tris);
    if (nRtn != 1)
        return -1;
    std::vector<MeshIOLib::index_t> neighbors;
    heManager->FindVertexNeighborsFromVertex(neighbors, 3);

// 시간 측정
clock_t start, end;
double result;
 
// 시간 측정
start = clock();
    // [3] Mesh Simplificiation DLL TEST
    path = "../Common/dll/zLibSimplificationQuadricError.dll";
    CDLLManager* dllSimplificationQE = new CDLLManager(path.c_str());
    auto handle3 = dllSimplificationQE->GetDLLHandle();
    if (handle3 == NULL)
        return -1;

    CMeshSimplificationManager* simplificationQEManager = new CMeshSimplificationManager(handle3, verts, tris);
    bRtn = simplificationQEManager->DoSimplificationQE(reductionRate, 7); // 0.25,0.5,0.75
    if (bRtn == false)
        return -1;
    std::vector<MeshIOLib::Vertex> simVerts;
    std::vector<MeshIOLib::Triangle> simTris;
    simplificationQEManager->GetSimplificationOutputData(simVerts, simTris);

    // 시간 측정
    end = clock();
    result = (double)(end - start);
    auto resultSec = (result) / CLOCKS_PER_SEC;
    std::cout << "elasped time : " << result << std::endl;

    // [4] 결과 파일 출력
    ioManager->WriteSTLwithData(outputData, simVerts, simTris);

    dllIO->TerminateDLL();
    dllHalfEdge->TerminateDLL();
    dllSimplificationQE->TerminateDLL();

    return 0;
}

