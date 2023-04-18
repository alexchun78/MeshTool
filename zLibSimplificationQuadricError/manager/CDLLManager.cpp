//#include "../pch.h"
//#include "CDLLManager.h"
//
//namespace MeshSimplificationQELib
//{
//    CDLLManager::CDLLManager(const char* path)
//    {
//        CreateDLL(path);
//    }
//
//    CDLLManager::~CDLLManager()
//    {
//        TerminateDLL();
//    }
//
//    void CDLLManager::CreateDLL(const char* path)
//    {
//        m_dll = LoadLibraryA(path);
//    }
//    void CDLLManager::TerminateDLL()
//    {
//        ::FreeLibrary(m_dll);
//    }
//};