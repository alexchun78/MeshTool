// zLibSimplificationQuadricError.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../pch.h"
#include "../include/CDLLManager.h"
#include "../../Common/include/CMeshIOManager.h"
#include <iostream>
#define INTRTRN_CHKNULL(val) if(val == NULL){return -1;}
int main()
{
    // dll 파일을 로드한다.
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CDLLManager* dll = new CDLLManager(path.c_str());
    auto handle = dll->GetDLLHandle();
    INTRTRN_CHKNULL(handle);

    path = "../Common/dll/zLibHalfEdgeRep.dll";
    CDLLManager* dllHE = new CDLLManager(path.c_str());
    auto handleHE = dllHE->GetDLLHandle();
    NULL_RTRN_INT(handleHE);

    // file import
    CMeshIOManager* ioManager = new CMeshIOManager(handle);
    {
        bool bRtn = ioManager->LoadSTL("..\\splitedBox.stl", handle);
        if (bRtn == false)
            return -1;
    }
    auto verts = ioManager->GetVertexList();
    auto tris = ioManager->GetTriangleList();
    
    // do half edge process
    


    return 0;
}
