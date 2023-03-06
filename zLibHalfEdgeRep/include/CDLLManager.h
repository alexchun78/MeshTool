#pragma once
#include <Windows.h>

class CDLLManager
{
public : 
    CDLLManager(const char* path);
    ~CDLLManager();

    void TerminateDLL();
    inline HMODULE GetDLLHandle() {
        return m_dll;
    }

protected:
    void CreateDLL(const char* path);

private :
    HMODULE m_dll;

};

