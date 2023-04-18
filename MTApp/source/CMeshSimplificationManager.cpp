#include "../pch.h"
#include "../include/CMeshSimplificationManager.h"

//CMeshSimplificationManager::CMeshSimplificationManager(const HMODULE handle)
//{
//    if (handle != NULL)
//    {
//        // 외부로 참조된 instance 함수를 취득
//        DLL_CREATE_SIMPLIFICATION_QE pFunc = (DLL_CREATE_SIMPLIFICATION_QE)::GetProcAddress(handle, "CreateSimplificationQE");
//        m_CSimplificationQE = pFunc();
//        m_handle = handle;
//    }
//}

CMeshSimplificationManager::CMeshSimplificationManager(const HMODULE handle, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris)
{
    if (handle != NULL)
    {
        // 외부로 참조된 instance 함수를 취득
        DLL_CREATE_SIMPLIFICATION_QE pFunc = (DLL_CREATE_SIMPLIFICATION_QE)::GetProcAddress(handle, "CreateSimplificationQE");
        m_CSimplificationQE = pFunc(verts, tris);
        m_handle = handle;
    }
}

CMeshSimplificationManager::~CMeshSimplificationManager()
{
    if (m_CSimplificationQE == NULL)
        return;

    DLL_TERMINATE_SIMPLIFICATION_QE terminateFunc = (DLL_TERMINATE_SIMPLIFICATION_QE)::GetProcAddress(m_handle, "TerminateSimplificationQE");
    terminateFunc(m_CSimplificationQE);
}

bool CMeshSimplificationManager::DoSimplificationQE(IN const float reductionRate, IN const float agressive)
{
    if (m_CSimplificationQE == NULL)
        return false;

    DLL_RUN_SIMPLIFICATION_QE pFunc = (DLL_RUN_SIMPLIFICATION_QE)::GetProcAddress(m_handle, "DoSimplification");
    return pFunc(m_CSimplificationQE, reductionRate, agressive);
}

void CMeshSimplificationManager::GetSimplificationOutputData(OUT std::vector<MeshIOLib::Vertex>& verts, OUT std::vector<MeshIOLib::Triangle>& tris)
{
    if (m_CSimplificationQE == NULL)
        return;

    DLL_GETOUTPUT_SIMPLIFICATION_QE pFunc = (DLL_GETOUTPUT_SIMPLIFICATION_QE)::GetProcAddress(m_handle, "GetSimplificationOutputData");
    pFunc(m_CSimplificationQE, verts, tris);
}