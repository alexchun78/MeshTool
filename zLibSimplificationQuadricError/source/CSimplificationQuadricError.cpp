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
    std::vector<int> deleted0, deleted1;
    int deletedCount = 0;
    int triCount = m_outTriangles.size();

    for (int iter = 0; iter < 100; ++iter)
    {
        if (triCount - deletedCount <= targetCount)
            break;

        // [2-1] 메쉬 구조 업데이트 : triangle list 조정
        if (iter % 5 == 0)
            UpdateMesh(iter);

        // [2-2] 더티 플래그 초기화
        loopi(0, m_outTriangles.size())
            m_outTriangles[i]._dirty = 0;

        // Threshold아래의 삼각형 Edge들은 모두 삭제한다.
        // 
        double threshold = 0.000000001 * pow(double(iter + 3), agressive);

        //printf("iteration %d - triangles %d threshold %g\n", iteration, triangle_count - deleted_triangles, threshold);

        loopi(0, m_outTriangles.size())
        {
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

                MeshIOLib::Vertex& v0 = m_outVertices[tri._vertexID[j]];
                MeshIOLib::Vertex& v1 = m_outVertices[tri._vertexID[(j+1)%3]];

                // Border Checking
                if (m_halfEdgeManager->IsBoundaryVertex(vid0) != m_halfEdgeManager->IsBoundaryVertex(vid1))
                    continue;

                // Compute Vertex to Edge Collapse 


            }
        }



    }

    return true;
}

void CSimplificationQuadricError::UpdateMesh(const int iter)
{
    if (iter == 0)
        return;

    int dst = 0;
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
        MeshIOLib::Vertex& p = verts[triangle.i()];
        MeshIOLib::Vertex& q = verts[triangle.j()];
        MeshIOLib::Vertex& r = verts[triangle.k()];

        Vec3 normal = Cross(q._position - p._position, r._position - p._position);
        Normalize(normal);
        triangle._normal = normal;
        // SymetricMatrix 구성하는 공식
        p._q = p._q + MeshIOLib::SymetricMatrix(normal.x, normal.y, normal.z, -(double)Dot(normal, p._position));
        q._q = p._q;
        r._q = p._q;
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
    if (det != 0 && bBorder)
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
        else if (error == error2)
            p = p2;
        else if (error == error3)
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

