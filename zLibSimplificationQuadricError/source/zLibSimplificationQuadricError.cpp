// zLibSimplificationQuadricError.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../pch.h"
#include "../manager/CDLLManager.h"
#include "../manager/CHEManager.h"
#include "../CMeshIOManager.h"
#include "../include/CSimplificationQuadricError.h"
#include <iostream>

#define INTRTRN_CHKNULL(val) if(val == NULL){return -1;}
int main()
{
    // dll ������ �ε��Ѵ�.
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CDLLManager* dll = new CDLLManager(path.c_str());
    auto handle = dll->GetDLLHandle();
    INTRTRN_CHKNULL(handle);

    path = "../Common/dll/zLibHalfEdgeRep.dll";
    CDLLManager* dllHE = new CDLLManager(path.c_str());
    auto handleHE = dllHE->GetDLLHandle();
    INTRTRN_CHKNULL(handleHE);

    // file import
    CMeshIOManager* ioManager = new CMeshIOManager(handle);
    {
        bool bRtn = ioManager->LoadSTL("..\\splitedBox.stl");
        if (bRtn == false)
            return -1;
    }
    auto verts = ioManager->GetVertexList();
    auto tris = ioManager->GetTriangleList();
    
    // do half edge process
    CHEManager* heManager = new CHEManager(handleHE);
    auto nRtn = heManager->Build(verts.size(), tris);
    if (nRtn != 1)
        return -1;
    std::vector<MeshIOLib::index_t> neighbors;
    heManager->FindVertexNeighborsFromVertex(neighbors, 3);

    // do simplification process
    CSimplificationQuadricError* pSimply = new CSimplificationQuadricError(verts, tris);
    pSimply->DoSimplification(0.5, 7);
    // (1) ���� ����Ʈ ->�ϴ� ������Ʈ�� ����
    // (2) HalfEdge ȭ
    // (3)

    // terminate
    dll->TerminateDLL();
    dllHE->TerminateDLL();

    return 0;
}
