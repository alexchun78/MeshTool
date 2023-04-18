//#include "../pch.h"
//#include "../manager/CDLLManager.h"
//#include "../manager/CHEManager.h"
//#include "../CMeshIOManager.h"
//#include "../include/CSimplificationQuadricError.h"
//#include <iostream>
//#define TIMECHK 
//#define INTRTRN_CHKNULL(val) if(val == NULL){return -1;}
//int main()
//{
//#ifdef TIMECHK
//    // 시간 측정
//    clock_t start, end;
//    double result;
//#endif
//#ifdef TIMECHK
//    // 시간 측정
//    start = clock();
//#endif
//    // dll 파일을 로드한다.
//    std::string path = "../Common/dll/zLibMeshIO.dll";
//    CDLLManager* dll = new CDLLManager(path.c_str());
//    auto handle = dll->GetDLLHandle();
//    INTRTRN_CHKNULL(handle);
//
//    path = "../Common/dll/zLibHalfEdgeRep.dll";
//
//    // file import
//    CMeshIOManager* ioManager = new CMeshIOManager(handle);
//    {
//        //bool bRtn = ioManager->LoadSTL("..\\2_bunnyO_new.stl");
//        bool bRtn = ioManager->LoadSTL("..\\cast3br.stl");
//        if (bRtn == false)
//            return -1;
//    }
//    auto verts = ioManager->GetVertexList();
//    auto tris = ioManager->GetTriangleList();
//    
//#ifdef TIMECHK
//    // 시간 측정
//    end = clock();
//    result = (double)(end - start);
//    auto resultSec = (result) / CLOCKS_PER_SEC;
//    std::cout << "Elesped Time : " << resultSec << std::endl;
//#endif
//
//    // do simplification process
//    CSimplificationQuadricError* pSimply = new CSimplificationQuadricError(path, verts, tris);
//    pSimply->DoSimplification(0.5, 7);
//
//    std::vector<MeshIOLib::Vertex> _outputVerts;
//    std::vector<MeshIOLib::Triangle> _outputTris;
//    pSimply->GetSimplificationOuputData(_outputVerts, _outputTris);
//
//    // 파일 오픈
//    FILE* file;
//    if ((::fopen_s(&file, "C:\\_dev\\MeshTool\\cast3br_simple.obj", "w")) != 0)
//    {
//        std::cout << "write_obj: can't write data file " << "C:\\_dev\\MeshTool\\cast3br_simple.obj" << "." << std::endl;
//    }
//    if (file == NULL)
//        return -1;
//
//    // 순회하면서 작성
//    for(int i = 0; i < _outputVerts.size(); i++)
//    {
//        fprintf(file, "v %g %g %g\n", _outputVerts[i]._position.x, _outputVerts[i]._position.y, _outputVerts[i]._position.z);
//    }
//    for (int i = 0; i < _outputTris.size(); i++)
//    {
//        fprintf(file, "f %d %d %d\n", _outputTris[i]._vertexID[0] + 1, _outputTris[i]._vertexID[1] + 1, _outputTris[i]._vertexID[2] + 1);
//    }
//    // terminate
//    dll->TerminateDLL();
//
//    return 0;
//}
