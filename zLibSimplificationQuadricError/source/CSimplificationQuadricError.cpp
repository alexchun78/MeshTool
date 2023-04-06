#include "../pch.h"
#include "../include/CSimplificationQuadricError.h"
#include <cassert>


CSimplificationQuadricError::CSimplificationQuadricError(const std::string dllPath, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris)
{
    // (1) Load Data
    LoadMeshData(verts, tris);
    m_outVertices = verts;
    m_outTriangles = tris;

    // (2) dll manager 생성
    m_dllManager = new CDLLManager(dllPath.c_str());
    auto handle = m_dllManager->GetDLLHandle();
    assert(handle != NULL);

    // (3) halfedge manager 생성
    m_halfEdgeManager = new CHEManager(handle);

    //// [추가] normal 값이 없으면, noraml을 생산한다.
    //loopi(0, m_triangles.size())
    //{
    //    if (m_triangles[i]._normal != Vec3())
    //        continue;
    //    // normal vector를 구하지 않은 경우만 계산
    //}

}

CSimplificationQuadricError::~CSimplificationQuadricError()
{
    if (m_dllManager != NULL)
    {
        m_dllManager->TerminateDLL();
        m_dllManager = NULL;
    }

    if (m_halfEdgeManager != NULL)
    {
        m_halfEdgeManager = NULL;
    }
}

bool CSimplificationQuadricError::LoadMeshData(const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris)
{
    assert(verts.size() > 3);
    assert(tris.size() > 3);

    m_vertices = verts;
    m_triangles = tris;

    return true;
}

// reductionRate : target triangle count, 0.5
// agressive : threshold increase -> more simplification, 7
bool CSimplificationQuadricError::DoSimplification(const float reductionRate = 0.5f, const float agressive = 7.0f)
{
    assert(m_outVertices.size() > 3);
    assert(m_outTriangles.size() > 3);

    float rate = reductionRate;
    float agValue = agressive;

    assert(rate > 0.0f);
    if (rate > 1.0f)
        rate = 1.0f;

    // [1] Target Count 정하기
    int targetCount = ::round((float)m_outTriangles.size() * rate);
    assert(targetCount >= 4);

    // [2] 간략화
    Internal_Simplification(targetCount, agValue);


    // re-check : 간략화 실패한 경우
    if (m_outTriangles.size() >= m_triangles.size())
        return false;

    return true;
}

bool CSimplificationQuadricError::Internal_Simplification(const int targetCount, const double agressive)
{
    assert(m_halfEdgeManager != NULL);

    // [1] 초기화
    loopi(0, m_outTriangles.size())
        m_outTriangles[i]._deleted = 0; 

    // [2] 초기 Mesh Data 구성
    // // [2-1] Half edge 구성
    m_halfEdgeManager->Build(m_outVertices.size(), m_outTriangles);

    // // [2-2] SymetricMatrix 구성
    ComposeSymetricMatrix(m_outVertices, m_outTriangles);

    // // [2-3] Calc error
    loopi(0, m_outTriangles.size())
    {
        // Calc Edge Error : 에러 계산을 해서 face단위로 저장한다.
        MeshIOLib::Triangle& tri = m_outTriangles[i];
        Vec3 p;
        loopj(0, 3)
        {
            tri._error[j] = CalQuadricError(p, tri._vertexID[j], tri._vertexID[(j + 1) % 3]);
        }
        tri._error[3] = min(tri._error[0], min(tri._error[1], tri._error[2]));
    }
    
    // [3] Main Algorithm
    std::vector<int> vecDeleted0, vecDeleted1;
    int deletedCount = 0;
    int triCount = m_outTriangles.size();

    for (int iter = 0; iter < 100; ++iter)
    {
        if (triCount - deletedCount <= targetCount)
            break;

        // [2-1] 메쉬 구조 업데이트 : triangle list 조정
        if (iter % 5 == 0)
            UpdateMesh(iter);
        //CompactMesh2(iter);

        // [2-2] 더티 플래그 초기화
        loopi(0, m_outTriangles.size())
            m_outTriangles[i]._dirty = 0;

        // Threshold아래의 삼각형 Edge들은 모두 삭제한다.
        // 
        double threshold = 0.000000001 * pow(double(iter + 3), agressive);

        //printf("iteration %d - triangles %d threshold %g\n", iteration, triangle_count - deleted_triangles, threshold);
        if (iter == 1)
            auto d = 0;
        loopi(0, m_outTriangles.size())
        {
            auto& testTri = m_outTriangles[18];
            MeshIOLib::Triangle& tri = m_outTriangles[i];
            if (tri._error[3] >= threshold)
                continue;
            if (tri._deleted)
                continue;
            if (tri._dirty)
                continue;

            loopj(0,3)
            {
                if (tri._error[j] >= threshold)
                    continue;

                size_t vid0 = tri._vertexID[j];
                size_t vid1 = tri._vertexID[(j+1)%3];

                MeshIOLib::Vertex& v0 = m_outVertices[vid0];
                MeshIOLib::Vertex& v1 = m_outVertices[vid1];

                // Border Checking
                if (m_halfEdgeManager->IsBoundaryVertex(vid0) != m_halfEdgeManager->IsBoundaryVertex(vid1))
                    continue;

                // Compute Vertex to Edge Collapse 
                Vec3 vertexErrorPos;
                CalQuadricError(vertexErrorPos, vid0, vid1);

                // Flipped triangle인지 검사 
                // // vertex와 인접한 삼각형 리스트를 가져온다.
                std::vector<MeshIOLib::index_t> triIDs_vid0;
                bool bRtn = m_halfEdgeManager->FindFaceNeighborsFromVertex(triIDs_vid0, vid0);
                if (bRtn == false)
                    continue;

                // 이웃 face들을 순서대로 정렬
                std::qsort(triIDs_vid0.data(), triIDs_vid0.size(), sizeof(MeshIOLib::index_t),
                    [](const void* x, const void* y) {
                        const int arg1 = *static_cast<const int*>(x);
                        const int arg2 = *static_cast<const int*>(y);
                        if (arg1 < arg2)
                            return -1;
                        if (arg1 > arg2)
                            return 1;
                        return 0;
                    });

                // // vertex와 인접한 삼각형 리스트를 가져온다.
                std::vector<MeshIOLib::index_t> triIDs_vid1;
                bRtn = m_halfEdgeManager->FindFaceNeighborsFromVertex(triIDs_vid1, vid1);
                if (bRtn == false)
                    continue;

                // 이웃 face들을 순서대로 정렬
                std::qsort(triIDs_vid1.data(), triIDs_vid1.size(), sizeof(MeshIOLib::index_t),
                    [](const void* x, const void* y) {
                        const int arg1 = *static_cast<const int*>(x);
                        const int arg2 = *static_cast<const int*>(y);
                        if (arg1 < arg2)
                            return -1;
                        if (arg1 > arg2)
                            return 1;
                        return 0;
                    });

                vecDeleted0.resize(triIDs_vid0.size());
                if (IsFlipped(vecDeleted0, triIDs_vid0, vertexErrorPos, vid0, vid1, v0))
                    continue;

                vecDeleted1.resize(triIDs_vid1.size());
                if (IsFlipped(vecDeleted1, triIDs_vid1, vertexErrorPos, vid1, vid0, v1))
                    continue;

                // Flipped triangle이 아니면 edge 제거
                v0._position = vertexErrorPos;
                v0._q = v1._q + v0._q;

                // face 정보 업데이트
                std::vector<size_t> vidList0(triIDs_vid0.size(), 4);
                std::vector<size_t> vidList1(triIDs_vid1.size(), 4);
                FindVidListFromFaceList(vidList0, triIDs_vid0, vid0);
                FindVidListFromFaceList(vidList1, triIDs_vid1, vid1);

                UpdateTriangles(deletedCount, vid0, triIDs_vid0, vidList0, vecDeleted0);
                UpdateTriangles(deletedCount, vid0, triIDs_vid1, vidList1, vecDeleted1);
                
                auto& debug_verts = m_outVertices;
                auto& debug_tris = m_outTriangles;
                break;
            }
            if (triCount - deletedCount <= targetCount)
            {
                auto nDebugCount = iter;
                break;
            }       
        }
    }

    CompactMesh();
    return true;
}

void CSimplificationQuadricError::UpdateMesh(const int iter)
{
    if (iter == 0)
        return;

    int dst = 0;
    int backupCnt = m_outTriangles.size();
    loopi(0, m_outTriangles.size())
    {
        if (!m_outTriangles[i]._deleted)
        {
            m_outTriangles[dst++] = m_outTriangles[i];
        }
    }
    m_outTriangles.resize(dst);
    // [NOTE] 
    // [TODO] 여기서 이웃 정보 업데이트가 필요하지 않을까?
    if(dst != backupCnt)
        m_halfEdgeManager->Build(m_outVertices.size(), m_outTriangles);
}

void CSimplificationQuadricError::ComposeSymetricMatrix(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris)
{
    assert(verts.size() > 0);
    assert(tris.size() > 0);

    // 초기화
    loopi(0, verts.size())
    {
        verts[i]._q = MeshIOLib::SymetricMatrix();
    }

    // SymetricMatrix 구성
    loopi(0, tris.size())
    {
        MeshIOLib::Triangle& triangle = tris[i];

        MeshIOLib::Vertex& p = verts[triangle._vertexID[0]];
        MeshIOLib::Vertex& q = verts[triangle._vertexID[1]];
        MeshIOLib::Vertex& r = verts[triangle._vertexID[2]];

        Vec3 normal = Cross(q._position - p._position, r._position - p._position);
        Normalize(normal);
        triangle._normal = normal;
        // SymetricMatrix 구성하는 공식
        auto sym = MeshIOLib::SymetricMatrix(normal.x, normal.y, normal.z, -(double)Dot(normal, verts[triangle._vertexID[0]]._position));
        verts[triangle._vertexID[0]]._q = verts[triangle._vertexID[0]]._q + sym;
        verts[triangle._vertexID[1]]._q = verts[triangle._vertexID[1]]._q + sym;
        verts[triangle._vertexID[2]]._q = verts[triangle._vertexID[2]]._q + sym;
    }
}

double CSimplificationQuadricError::CalQuadricError(Vec3& p, const size_t v1, const size_t v2)
{
    MeshIOLib::SymetricMatrix q = m_outVertices[v1]._q + m_outVertices[v2]._q;
    bool bBorder = m_halfEdgeManager->IsBoundaryVertex(v1) & m_halfEdgeManager->IsBoundaryVertex(v2);
    double error = 0;
    // Quadric SymetricMatrix
    // [ 0 1 2 3 ] 
    // [ 1 4 5 6 ]
    // [ 2 5 7 8 ]
    // [ 3 6 8 9 ]
    double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);
    if (det != 0 && !bBorder)
    {
        // 크로네터 델타
        p.x = -1 / det * (q.det(1, 2, 3, 4, 5, 6, 5, 7, 8)); // vx = A41/det(q_delta)
        p.y = 1 / det * (q.det(0, 2, 3, 1, 5, 6, 2, 7, 8)); // vy = A42/det(q_delta)
        p.z = -1 / det * (q.det(0, 1, 3, 1, 4, 6, 2, 5, 8)); // vz = A43/det(q_delta)

        error = CalVertexError(q, p);
    }
    else
    {
        Vec3 p1 = m_outVertices[v1]._position;
        Vec3 p2 = m_outVertices[v2]._position;
        Vec3 p3 = (p1 + p2) * 0.5;

        double error1 = CalVertexError(q, p1);
        double error2 = CalVertexError(q, p2);
        double error3 = CalVertexError(q, p3);
        error = min(error1, min(error2, error3));
        if (error == error1)
            p = p1;
        if (error == error2)
            p = p2;
        if (error == error3)
            p = p3;
    }
    return error;
}

double CSimplificationQuadricError::CalVertexError(MeshIOLib::SymetricMatrix q, Vec3 v)
{
    double x = v.x;
    double y = v.y;
    double z = v.z;

    // (vT)X(v) + (2yT)(v)+ z : Quadric error formulation
    return  q[0] * x * x + q[4] * y * y + q[7] * z * z + 2 * q[1] * x * y + 2 * q[5] * y * z + 2 * q[2] * x * z + 2 * q[3] * x
        + 2 * q[6] * y + 2 * q[8] * z + q[9];
}

bool CSimplificationQuadricError::IsFlipped(std::vector<int>& vecDeleted, std::vector<MeshIOLib::index_t>& neighborTriangles, const Vec3& p, 
    const MeshIOLib::index_t vid0, const MeshIOLib::index_t vid1, const MeshIOLib::Vertex& v0)
{
    if (m_halfEdgeManager == NULL)
    {
        // 제대로 계산할 수 없다.
        return true;
    }

    if (neighborTriangles.size() == 0)
        return false;

    // 삼각형들이 뒤집힌 상태인지 확인한다.
    loopi(0, neighborTriangles.size())
    {
        MeshIOLib::Triangle& tri = m_outTriangles[neighborTriangles[i]];
        if (tri._deleted)
            continue;

        size_t tmpVID = 0;
        loopj(0,3)
        {
            if (tri._vertexID[j] == vid0)
            {
                tmpVID = j;
                break;
            }                
        }
        
        // 다음 버텍스의 인덱스를 구한다.
        auto vid1_tmp = tri._vertexID[(tmpVID + 1) % 3];
        auto vid2_tmp = tri._vertexID[(tmpVID + 2) % 3];

        // 해당하는 인덱스는 찾아서 해당 Edge를 합쳐야 한다.
        if (vid1_tmp == vid1 || vid2_tmp == vid1)
        {
            vecDeleted[i] = 1;
            continue;
        }
        // 20번째  vertex 값이 다르다
        // 겹치지 않으면, vertex error vector에서 향하는 vector 구함
        Vec3 d1 = m_outVertices[vid1_tmp]._position - p;
        Vec3 d2 = m_outVertices[vid2_tmp]._position - p;
        Normalize(d1);
        Normalize(d2);

        // 방향이 겹치면,
        if (fabs(Dot(d1, d2)) > 0.999f) 
            return true;

        Vec3 normal = Cross(d1, d2);
        Normalize(normal);
        vecDeleted[i] = 0;

        // 수직 관계의 벡터에 대한 예외처리
        if (Dot(normal, tri._normal) < 0.2)
            return true;
    }
    return false;
}

// Update triangle connections and edge error after a edge is collapsed
void CSimplificationQuadricError::UpdateTriangles(int& deletedTriCount, const int vid, const std::vector<MeshIOLib::index_t>& neighborTriangles, 
    const std::vector<size_t> vidList, const std::vector<int>& vecDeleted)
{
    Vec3 p;
    loopi(0, neighborTriangles.size())
    {
        MeshIOLib::Triangle& tri = m_outTriangles[neighborTriangles[i]];
        
        if (tri._deleted)
            continue;
        
        if (vecDeleted[i])
        {
            tri._deleted = 1;
            deletedTriCount++;
            continue;
        }

        // 이웃 삼각형에서 자신의 index로 수정
        tri._vertexID[vidList[i]] = vid;
        tri._dirty = 1;
        tri._error[0] = CalQuadricError(p, tri._vertexID[0], tri._vertexID[1]);
        tri._error[1] = CalQuadricError(p, tri._vertexID[1], tri._vertexID[2]);
        tri._error[2] = CalQuadricError(p, tri._vertexID[2], tri._vertexID[0]);
        tri._error[3] = min(tri._error[0], min(tri._error[1], tri._error[2]));
    }
}

void CSimplificationQuadricError::FindVidListFromFaceList(std::vector<size_t>& vidList, 
    const std::vector<MeshIOLib::index_t>& neighborTriangles, const MeshIOLib::index_t vertexIndex)
{
    vidList.clear();
    vidList.resize(neighborTriangles.size());
    loopi(0, neighborTriangles.size())
    {
        auto tid = neighborTriangles[i];
        loopj(0, 3)
        {
            if (m_outTriangles[tid]._vertexID[j] == vertexIndex)
            {
                vidList[i] = j;
                break;
            }
        }        
    }
}

void CSimplificationQuadricError::CompactMesh()
{
    size_t dst = 0;
    loopi(0, m_outTriangles.size())
    {
        MeshIOLib::Triangle& tri = m_outTriangles[i];
        if (!tri._deleted)
        {
            m_outTriangles[dst] = tri;
            loopj(0, 3)
            {
                m_outVertices[tri._vertexID[j]]._bReMesh = true;
                m_outVertices[tri._vertexID[j]]._triangleID = dst;
                m_outVertices[tri._vertexID[j]]._vid = j;
            }
            dst++;
        }
    }
    m_outTriangles.resize(dst);

    dst = 0;
    loopi(0, m_outVertices.size())
    {
        auto& vert = m_outVertices[i];
        if (vert._bReMesh)
        {
            m_outVertices[dst++] = vert;
        }
    }
    m_outVertices.resize(dst);

    //m_halfEdgeManager->Build(m_outVertices.size(), m_outTriangles);
}

void CSimplificationQuadricError::CompactMesh2(const int iter)
{
    if (iter == 0)
        return;

    size_t dst = 0;
    size_t backupCnt = m_outTriangles.size();

    loopi(0, m_outTriangles.size())
    {
        MeshIOLib::Triangle& tri = m_outTriangles[i];
        if (!tri._deleted)
        {
            m_outTriangles[dst] = tri;
            loopj(0, 3)
            {
                m_outVertices[tri._vertexID[j]]._bReMesh = true;
                m_outVertices[tri._vertexID[j]]._triangleID = dst;
                m_outVertices[tri._vertexID[j]]._vid = j;
            }
            dst++;
        }
    }
    m_outTriangles.resize(dst);

    dst = 0;
    loopi(0, m_outVertices.size())
    {
        auto& vert = m_outVertices[i];
        if (vert._bReMesh)
        {
            m_outVertices[dst++] = vert;
        }
    }
    m_outVertices.resize(dst);
    // [NOTE] 
    // [TODO] 여기서 이웃 정보 업데이트가 필요하지 않을까?
    if (dst != backupCnt)
        m_halfEdgeManager->Build(m_outVertices.size(), m_outTriangles);
}