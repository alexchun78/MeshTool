#include "../pch.h"
#include "../include/CSimplificationQuadricError.h"
#include <cassert>

CSimplificationQuadricError::CSimplificationQuadricError(const std::string dllPath, const std::vector<MeshIOLib::Vertex>& verts, const std::vector<MeshIOLib::Triangle>& tris)
{
    // (1) Load Data
    LoadMeshData(verts, tris);
    m_outVertices = verts;
    m_outTriangles = tris;

    // (2) dll manager ����
    m_dllManager = new CDLLManager(dllPath.c_str());
    auto handle = m_dllManager->GetDLLHandle();
    assert(handle != NULL);

    // (3) halfedge manager ����
    m_halfEdgeManager = new CHEManager(handle);

    //// [�߰�] normal ���� ������, noraml�� �����Ѵ�.
    //loopi(0, m_triangles.size())
    //{
    //    if (m_triangles[i]._normal != Vec3())
    //        continue;
    //    // normal vector�� ������ ���� ��츸 ���
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

    // [1] Target Count ���ϱ�
    int targetCount = ::round((float)m_outTriangles.size() * rate);
    assert(targetCount >= 4);

    // [2] ����ȭ
    Internal_Simplification(targetCount, agValue);


    // re-check : ����ȭ ������ ���
    if (m_outTriangles.size() >= m_triangles.size())
        return false;

    return true;
}

bool CSimplificationQuadricError::Internal_Simplification(const int targetCount, const double agressive)
{
    assert(m_halfEdgeManager != NULL);

    // [1] �ʱ�ȭ
    loopi(0, m_outTriangles.size())
        m_outTriangles[i]._deleted = 0; 

    // [2] �ʱ� Mesh Data ����
    // // [2-1] Half edge ����
    m_halfEdgeManager->Build(m_outVertices.size(), m_outTriangles);

    // // [2-2] SymetricMatrix ����
    ComposeSymetricMatrix(m_outVertices, m_outTriangles);

    // // [2-3] Calc error
    loopi(0, m_outTriangles.size())
    {
        // Calc Edge Error : ���� ����� �ؼ� face������ �����Ѵ�.
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

        // [2-1] �޽� ���� ������Ʈ : triangle list ����
        if (iter % 5 == 0)
            UpdateMesh(iter);

        // [2-2] ��Ƽ �÷��� �ʱ�ȭ
        loopi(0, m_outTriangles.size())
            m_outTriangles[i]._dirty = 0;

        // Threshold�Ʒ��� �ﰢ�� Edge���� ��� �����Ѵ�.
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

                MeshIOLib::Vertex& v0 = m_outVertices[vid0];
                MeshIOLib::Vertex& v1 = m_outVertices[vid1];

                // Border Checking
                if (m_halfEdgeManager->IsBoundaryVertex(vid0) != m_halfEdgeManager->IsBoundaryVertex(vid1))
                    continue;

                // Compute Vertex to Edge Collapse 
                Vec3 vertexErrorPos;
                CalQuadricError(vertexErrorPos, vid0, vid1);

                // Flipped triangle���� �˻� 
                // // vertex�� ������ �ﰢ�� ����Ʈ�� �����´�.
                std::vector<MeshIOLib::index_t> triIDs_vid0;
                bool bRtn = m_halfEdgeManager->FindFaceNeighborsFromVertex(triIDs_vid0, vid0);
                if (bRtn == false)
                    continue;
                
                vecDeleted0.resize(triIDs_vid0.size());
                if (IsFlipped(vecDeleted0, triIDs_vid0, vertexErrorPos, vid0, vid1, v0))
                    continue;

                // // vertex�� ������ �ﰢ�� ����Ʈ�� �����´�.
                std::vector<MeshIOLib::index_t> triIDs_vid1;
                bRtn = m_halfEdgeManager->FindFaceNeighborsFromVertex(triIDs_vid1, vid1);
                if (bRtn == false)
                    continue;

                vecDeleted1.resize(triIDs_vid1.size());
                if (IsFlipped(vecDeleted1, triIDs_vid1, vertexErrorPos, vid0, vid1, v1))
                    continue;

                // Flipped triangle�� �ƴϸ� edge ����
                v0._position = vertexErrorPos;
                v0._q = v1._q + v0._q;




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
    // [TODO] ���⼭ �̿� ���� ������Ʈ�� �ʿ����� ������?
    if(dst != backupCnt)
        m_halfEdgeManager->Build(m_outVertices.size(), m_outTriangles);
}

void CSimplificationQuadricError::ComposeSymetricMatrix(std::vector<MeshIOLib::Vertex>& verts, std::vector<MeshIOLib::Triangle>& tris)
{
    assert(verts.size() > 0);
    assert(tris.size() > 0);

    // �ʱ�ȭ
    loopi(0, verts.size())
    {
        verts[i]._q = MeshIOLib::SymetricMatrix();
    }

    // SymetricMatrix ����
    loopi(0, tris.size())
    {
        MeshIOLib::Triangle& triangle = tris[i];

        MeshIOLib::Vertex& p = verts[triangle.i()];
        MeshIOLib::Vertex& q = verts[triangle.j()];
        MeshIOLib::Vertex& r = verts[triangle.k()];

        Vec3 normal = Cross(q._position - p._position, r._position - p._position);
        Normalize(normal);
        triangle._normal = normal;
        // SymetricMatrix �����ϴ� ����
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
        // ũ�γ��� ��Ÿ
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

bool CSimplificationQuadricError::IsFlipped(std::vector<int>& vecDeleted, std::vector<MeshIOLib::index_t>& neighborTriangles, const Vec3& p, const MeshIOLib::index_t vid0, const MeshIOLib::index_t vid1, const MeshIOLib::Vertex& v0)
{
    if (m_halfEdgeManager == NULL)
    {
        // ����� ����� �� ����.
        return true;
    }

    if (neighborTriangles.size() == 0)
        return false;
    
    // �ﰢ������ ������ �������� Ȯ���Ѵ�.
    loopi(0, neighborTriangles.size())
    {
        MeshIOLib::Triangle& tri = m_outTriangles[neighborTriangles[i]];
        if (tri._deleted)
            continue;

        // ���� ���ؽ��� �ε����� ���Ѵ�.
        auto vid1_tmp = tri._vertexID[(v0._vid + 1) % 3];
        auto vid2_tmp = tri._vertexID[(v0._vid + 2) % 3];

        // �ش��ϴ� �ε����� ã�Ƽ� �ش� Edge�� ���ľ� �Ѵ�.
        if (vid1_tmp == vid1 || vid2_tmp == vid1)
        {
            vecDeleted[i] = 1;
            continue;
        }

        // ��ġ�� ������, vertex error vector���� ���ϴ� vector ����
        Vec3 d1 = m_vertices[vid1_tmp]._position - p;
        Vec3 d2 = m_vertices[vid2_tmp]._position - p;
        Normalize(d1);
        Normalize(d2);

        // ������ ��ġ��,
        if (fabs(Dot(d1, d2)) > 0.999f) 
            return true;

        Vec3 normal = Cross(d1, d2);
        Normalize(normal);
        vecDeleted[i] = 0;

        // ���� ������ ���Ϳ� ���� ����ó��
        if (Dot(normal, tri._normal) < 0.2)
            return true;
    }
    return false;
}

// Update triangle connections and edge error after a edge is collapsed
void CSimplificationQuadricError::update_triangles(int vid, std::vector<MeshIOLib::index_t>& neighborTriangles, std::vector<int>& vecDeleted, int& deletedTriCount)
{
    Vec3 p;
    loopk(0, neighborTriangles.size())
    {
        //Triangle& tri = m_outTriangles[r.tid];
        //if (tri._deleted)
        //    continue;
        //if (vecDeleted[k])
        //{
        //    tri.deleted = 1;
        //    deletedTriCount++;
        //    continue;
        //}

        //tri._vertexID[r.tvertex] = i0; // �̿� �ﰢ�� ������ vertex �ڽ��� ��ġ�� ã�� �Լ��� �ʿ��ϴ�. 
        //t.dirty = 1;
        //t.err[0] = calculate_error(t.v[0], t.v[1], p);
        //t.err[1] = calculate_error(t.v[1], t.v[2], p);
        //t.err[2] = calculate_error(t.v[2], t.v[0], p);
        //t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
        //refs.push_back(r);
    }
}