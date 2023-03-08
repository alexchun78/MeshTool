#include "../pch.h"
#include "../include/CMeshIO.h"
#include <cstring> // memcpy
#include <thread>
#include <algorithm>
#include <future>
#include <cassert>
#include <set>

namespace MeshIOLib
{
    void parallel_sort(std::vector<Vertex>::iterator begin, std::vector<Vertex>::iterator end, int threads);

    CMeshIO::CMeshIO() 
    {
    }

    CMeshIO::~CMeshIO()
    {

    }

    void CMeshIO::LoadOBJ(const char* filename)
    {
        // [TODO] (2023-03-02) LaodSTL과 같이 vertex의 중복 검사 및 제거해주는 구문이 필요함.
        // 
        // 0. 예외 처리
        if (filename == NULL)
            return;

        if ((char)filename[0] == 0)
            return;

        // 1. 데이터 초기화
        m_vecVertices.clear();
        m_vecTriangles.clear();

        // 2. 파일 열기
        FILE* fn;
        if ((::fopen_s(&fn, filename, "rb")) != 0) 
        {
            std::cout << "file %s is not found " << std::endl;
        }
        if (fn == NULL)
            return;

        char line[1000];
        // 3. 파일을 다 읽을 때까지 반복
        size_t triCount = 0;
        while(fgets(line, 1000, fn) != NULL)
        {
            // 3.1. 버텍스이면 버텍스 양식에 맞추어 입력한다.
            // vt(texture)와 vn(normal)도 필요시 추가
            if (line[0] == 'v' && line[1] == ' ')
            {
                float x, y, z;
                if (::sscanf_s(line, "v %f %f %f", &x, &y, &z) == 3)
                {
                    m_vecVertices.push_back(Vertex(x, y, z));
                    continue;
                }
            }
             
            // 3.2. 페이스이면 페이스 양식에 맞추어 입력한다. (1 베이스)
            if (line[0] == 'f')
            {
                int ids[9];
                Triangle triangle;
                bool isDone = false;
                // f v1/vt1/vn1 v2/vt2/vn2 v2/vt2/vn2
                // (vertex/texture/normal)
                if ((::sscanf_s(line, "f %d %d %d", ids+0, ids+1, ids+2) == 3) || 
                    (::sscanf_s(line, "f %d// %d// %d//", &ids[0], &ids[1], &ids[2]) == 3) ||
                    (sscanf_s(line, "f %d//%d %d//%d %d//%d", &ids[0], &ids[3], &ids[1], &ids[4], &ids[2], &ids[5]) == 6) ||               
                    (sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",  &ids[0], &ids[6], &ids[3], &ids[1], &ids[7], &ids[4], &ids[2], &ids[8], &ids[5]) == 6))
                {
                    isDone = true;
                }
                if (isDone)
                {
                    // normal과 texture는 필요할 때 추가
                    triangle._vertexID[0] = ids[0] - 1;
                    triangle._vertexID[1] = ids[1] - 1;
                    triangle._vertexID[2] = ids[2] - 1;                    
                    m_vecTriangles.push_back(triangle);

                    // triangle 정보 업데이트
                    m_vecVertices[triangle._vertexID[0]]._triangleID = triCount;
                    m_vecVertices[triangle._vertexID[1]]._triangleID = triCount;
                    m_vecVertices[triangle._vertexID[2]]._triangleID = triCount++;
                }
            }
        }
           
        // 4. 파일을 닫는다.
        if(fn != NULL)
            ::fclose(fn);

    }

    void CMeshIO::LoadSTL(const char* filename)
    {
        // mesh from verts
        // 0. 예외 처리
        if (filename == NULL)
            return;

        if ((char)filename[0] == 0)
            return;

        // 1. 데이터 초기화
        m_vecVertices.clear();
        m_vecTriangles.clear();

        // 2. vertex data 추출
        std::vector<Vertex> verts_temp = LoadSTL_Vertices(filename);
        m_vecVertices.reserve(verts_temp.size());
        m_vecTriangles.resize(verts_temp.size()/3);

        // 3. vertex와 face에 정보 입력
        triangle_tag tri;
        size_t vid = 0;
        for (size_t i = 0; i < verts_temp.size(); ++i)
        {
            auto fID = i / 3;
            verts_temp[i]._triangleID = fID;
            verts_temp[i]._vid = vid;
            m_vecTriangles[fID]._vertexID[vid++] = i;            
            if (vid == 3)
                vid = 0;           
        }

        // 4. vertex 순으로 정렬 (병렬 처리)(to deduplicate)
        auto threads = std::thread::hardware_concurrency();
        if (threads == 0)
        {
            threads = 8;
        }

        parallel_sort(verts_temp.begin(), verts_temp.end(), threads);
        
        // 5. 중복된 버텍스를 제거하고, triangle index리스트를 새롭게 구성한다.  
        std::vector<Vertex> verts_result;
        std::vector<Vertex> temps;
        Vertex prevVertex = verts_temp[0];          
        size_t totalvertexCount = verts_temp.size();
        for (size_t i = 0; i < totalvertexCount; ++i)
        {
            // 이전 값과 비교해서 같으면 저장한다.
            if (prevVertex == verts_temp[i])
            {
                temps.push_back(verts_temp[i]);
                if(i != totalvertexCount-1)
                    continue;
            }

            // 이전 데이터를 정리한다.
            // // tri ids의 공간을 할당한다.
            size_t tempSize = temps.size();
            prevVertex._ptrTriIDs.reserve(tempSize);
            prevVertex._ptrVids.reserve(tempSize);
            for (size_t j = 0; j < tempSize; ++j)
            {
                prevVertex._ptrTriIDs.push_back(temps[j]._triangleID);
                prevVertex._ptrVids.push_back(temps[j]._vid);
            }

            // 최초 데이터만 최종 결과로 저장한다.
            size_t tempID = m_vecVertices.size();
            for (size_t t = 0; t < tempSize; ++t)
            {
                size_t fID = prevVertex._ptrTriIDs[t];
                size_t vID = prevVertex._ptrVids[t];
                m_vecTriangles[fID]._vertexID[vID] = tempID;
            }
            // 불필요한 데이터 삭제 : 만약 필요하다면 추후 되돌리기
            prevVertex._ptrTriIDs.clear();
            prevVertex._ptrVids.clear();
            m_vecVertices.push_back(prevVertex);

            // 임시 저장된 데이터들을 삭제한다.
            temps.clear();

            // 새롭게 저장한다.
            prevVertex = verts_temp[i];
            temps.push_back(prevVertex);
        }
    }

    void CMeshIO::WriteOBJ(const char* filename)
    {

    }

    void CMeshIO::WriteSTL(const char* filename)
    {

    }

    std::vector<Vertex> CMeshIO::LoadSTL_Vertices(const char* filename)
    {
        // 1. file open
        std::ifstream file;
        file.open(filename, std::ios::in);
        if (!file)
        {
            return std::vector<Vertex>();
        }

        std::string line;
        // 2. 첫 줄 읽어온다.
        std::getline(file, line);
        // 2.1. ascii 코드 방식의 형식과 동일한지 검사
        if (line.rfind("solid ", 0) == 0)
        {
            // 2.1.1. 다음 줄 읽어온다.
            std::getline(file, line);
            line = trim(line);
            // 2.1.2. 패싯(facet)을 찾는다.
            if (line.rfind("facet", 0) == 0)
            {
                // 2.1.2.1. 패시이 있으면, 파일 종료 및 하위 함수 호출
                file.close();
                return Load_ASCII(filename);
            }
        }
        // 2.2. ascii 방식이 아니면, 바이너리 방식이므로 파일 종료 및 하위 함수 호출
        file.close();
        
        return Load_Binary(filename);
    }

    std::vector<Vertex> CMeshIO::Load_ASCII(const char* filename)
    {
        std::vector<Vertex> verts_temp;

        // 1. file open
        std::ifstream file;
        file.open(filename, std::ios::in);
        if (!file)
        {
            return std::vector<Vertex>();
        }

        // 2. 끝날때까지 반복
        std::string line;
        while (!file.eof()) 
        {
            // 2.1. 한 줄씩 읽어온다.
            std::getline(file, line);
            // 2.2. 앞 뒤 공백 제거
            line = trim(line);
            // 2.3. vertex 글자를 찾고,
            if (line.rfind("vertex", 0) == 0)
            {
                // 2.3.1. 찾으면, vector값을 저장한다.
                verts_temp.push_back(get_vector(line));
            }
        }
        file.close();

        return verts_temp;
    }

    std::vector<Vertex> CMeshIO::Load_Binary(const char* filename)
    {
        // 1. file open
        std::ifstream file;
        file.open(filename,  std::ios::in | std::ios::binary);
        if (!file)
        {
            return std::vector<Vertex>();
        }
        /* 형식
            UINT8[80] – 헤더 - 80바이트
            UINT32 – 삼각형 수 - 4바이트
            foreach 삼각형 - 50바이트:
                REAL32[3] – 일반 벡터 - 12바이트
                REAL32[3] – 정점 1 - 12바이트
                REAL32[3] – 정점 2 - 12바이트
                REAL32[3] – 정점 3 - 12바이트
                UINT16 – 속성 바이트 수 - 2바이트
            끝
        */
        
        // 2. 헤더(80)을 건너 뛴다.
        file.seekg(80);
        
        // 3. 삼각형 수를 읽어온다.(4)
        int num_faces;
        file.read(reinterpret_cast<char*>(&num_faces), 4);
        // 3.1. 전체 버텍스 수 정하기 (중복허용)
        const unsigned int num_vertices = num_faces * 3;
        // 3.2. 삼각형 전체 바이트 수 (남은 전체 문서 길이)
        size_t length = num_faces * 50;
        if(length < 0) // 예외처리 : 사이즈가 너무 커서 unsigned int 범위를 벗어나는 경우 
            return std::vector<Vertex>();

        // 4. 파일 전체 읽어오기
        char* ret = new char[length];
        file.read(ret, length);

        // 5. 전체 버텍스 저장시키기
        std::vector<Vertex> verts_temp(num_vertices);
        for (int i = 0; i < num_faces; ++i) 
        {
            for (int j = 0; j < 3; ++j)
            {
                const int id = i * 3 + j;
                std::memcpy(&verts_temp[id]._position, &ret[(50 * i) + (j * 12) + 12], 12);
            }
        }
        file.close();

        return verts_temp;
    }

    // parallel_sort
    void parallel_sort(std::vector<Vertex>::iterator begin, std::vector<Vertex>::iterator end, int threads)
    {
        if (threads < 2 || end - begin < 2)
        {
            std::sort(begin, end);
        }
        else
        {
            const auto mid = begin + (end - begin) / 2;
            if (threads == 2)
            {
                auto future = std::async(parallel_sort, begin, mid, threads / 2);
                std::sort(mid, end);
                future.wait();
            }
            else
            {
                auto a = std::async(std::launch::async, parallel_sort, begin, mid, threads / 2);
                auto b = std::async(std::launch::async, parallel_sort, mid, end, threads / 2);
                a.wait();
                b.wait();
            }
            std::inplace_merge(begin, mid, end);
        }
    }
};