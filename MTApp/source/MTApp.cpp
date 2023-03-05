#include "../pch.h"
#include <iostream>
#include <Windows.h>
#include "../include/CMeshIOManager.h"

int main()
{
    // dll ������ �ε��Ѵ�.
    std::string path = "../Common/dll/zLibMeshIO.dll";

    CMeshIOManager* ioManager = new CMeshIOManager(path.c_str());
    bool bRtn = ioManager->LoadSTL("..\\smartslicerA.stl");
    if (bRtn == false)
        return -1;

    ioManager->TerminateDLL();

    auto v = ioManager->GetVertexList();

    auto t = ioManager->GetTriangleList();
    
    return 0;
}

