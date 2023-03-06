#include "../pch.h"
#include <iostream>
#include <Windows.h>
#include "../include/CDLLManager.h"
#include "../include/CMeshIOManager.h"

int main()
{
    
    // dll 파일을 로드한다.
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CDLLManager* dllManager = new CDLLManager(path.c_str());
    auto handle = dllManager->GetDLLHandle();
    if(handle != NULL)
    {
        CMeshIOManager* ioManager = new CMeshIOManager(handle);
        bool bRtn = ioManager->LoadSTL("..\\smartslicerA.stl", handle);
        if (bRtn == false)
            return -1;
    }
    dllManager->TerminateDLL();
    /*
    std::string path = "../Common/dll/zLibMeshIO.dll";
    CMeshIOManager* ioManager = new CMeshIOManager(path.c_str());
    bool bRtn = ioManager->LoadSTL("..\\smartslicerA.stl");
    if (bRtn == false)
        return -1;
    */
    return 0;
}

