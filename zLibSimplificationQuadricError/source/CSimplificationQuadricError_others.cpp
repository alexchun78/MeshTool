#include "../pch.h"
#include "../include/CSimplificationQuadricError.h"
#include <cassert>

bool CSimplificationQuadricError::Internal_Simplification_FQM(const int targetCount, const double agressive)
{
	// [1] 초기화
	loopi(0, m_outTriangles.size())
		m_outTriangles[i]._deleted = 0;

	// [2] 초기 Mesh Data 구성
	// // [2-1] boundary 및 이웃 삼각형 정보 구성
	Internal_SetInitData_FQM(m_outVertices, m_outTriangles, m_refs);
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
			tri._error[j] = CalQuadricError_FQM(p, tri._vertexID[j], tri._vertexID[(j + 1) % 3]);
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

		// [3-1] 메쉬 구조 업데이트 : triangle list 조정
		if (iter % 5 == 0)
		{
			UpdateMesh(iter);
		}
		// [2] 더티 플래그 초기화 
		// clear dirty flag
		loopi(0, m_outTriangles.size())
			m_outTriangles[i]._dirty = 0;

		// Threshold아래의 삼각형 Edge들은 모두 삭제한다.
		double threshold = 0.000000001 * pow(double(iter + 3), agressive);
		loopi(0, m_outTriangles.size())
		{
			MeshIOLib::Triangle& tri = m_outTriangles[i];
			if (tri._error[3] >= threshold)
				continue;
			if (tri._deleted)
				continue;
			if (tri._dirty)
				continue;
			loopj(0, 3)
			{
				if (tri._error[j] >= threshold)
					continue;

				size_t vid0 = tri._vertexID[j];
				size_t vid1 = tri._vertexID[(j + 1) % 3];

				MeshIOLib::Vertex& v0 = m_outVertices[vid0];
				MeshIOLib::Vertex& v1 = m_outVertices[vid1];

				// Border Checking
				if (v0._border != v1._border)
					continue;

				// Compute Vertex to Edge Collapse 
				Vec3 vertexErrorPos;
				CalQuadricError_FQM(vertexErrorPos, vid0, vid1);

				vecDeleted0.resize(v0._tcount); // normals temporarily
				vecDeleted1.resize(v1._tcount); // normals temporarily

				// dont remove if flipped
				if (IsFlipped_FQM(vecDeleted0, vertexErrorPos, vid0, vid1, v0)) 
					continue;
				if (IsFlipped_FQM(vecDeleted1, vertexErrorPos, vid1, vid0, v1)) 
					continue;

				// not flipped, so remove edge
				v0._position = vertexErrorPos;
				v0._q = v1._q + v0._q;
				int tstart = m_refs.size();

				UpdateTriangles_FQM(deletedCount, vid0, v0, vecDeleted0);
				UpdateTriangles_FQM(deletedCount, vid0, v1, vecDeleted1);

				int tcount = m_refs.size() - tstart;
				if (tcount <= v0._tcount)
				{
					// save ram
					if (tcount)
						memcpy(&m_refs[v0._tstart], &m_refs[tstart], tcount * sizeof(MeshIOLib::Ref));
				}
				else
					// append
					v0._tstart = tstart;

				v0._tcount = tcount;
				break;
			}
		}
		if (deletedCount <= 0)
			break;
		deletedCount = 0;
	}
	CompactMesh_FQM();
	return true;
}

void CSimplificationQuadricError::CompactMesh_FQM()
{
	int dst = 0;
	loopi(0, m_outVertices.size())
	{
		m_outVertices[i]._tcount = 0;
	}
	loopi(0, m_outTriangles.size())
		if (!m_outTriangles[i]._deleted)
		{
			MeshIOLib::Triangle& t = m_outTriangles[i];
			m_outTriangles[dst++] = t;
			loopj(0, 3)
				m_outVertices[t._vertexID[j]]._tcount = 1;
		}

	m_outTriangles.resize(dst);
	dst = 0;
	loopi(0, m_outVertices.size())
		if (m_outVertices[i]._tcount)
		{
			m_outVertices[i]._tstart = dst;
			m_outVertices[dst]._position = m_outVertices[i]._position;
			dst++;
		}

	loopi(0, m_outTriangles.size())
	{
		MeshIOLib::Triangle& t = m_outTriangles[i];
		loopj(0, 3)
			t._vertexID[j] = m_outVertices[t._vertexID[j]]._tstart;
	}
	m_outVertices.resize(dst);
}

void CSimplificationQuadricError::UpdateTriangles_FQM(int& deletedTriCount, const int vid, const MeshIOLib::Vertex& v, const std::vector<int>& vecDeleted)
{
	Vec3 p;
	loopk(0, v._tcount)
	{
		MeshIOLib::Ref& r = m_refs[v._tstart + k];
		MeshIOLib::Triangle& t = m_outTriangles[r._tid]; // 이웃 삼각형 리스트
		if (t._deleted)continue;
		if (vecDeleted[k])
		{
			t._deleted = 1;
			deletedTriCount++;
			continue;
		}
		t._vertexID[r._tvertex] = vid; // 이웃 삼각형 내에서 자신의 인덱스를 수정
		t._dirty = 1;
		t._error[0] = CalQuadricError_FQM(p, t._vertexID[0], t._vertexID[1]);//(Vec3& p, const size_t v1, const size_t v2)
		t._error[1] = CalQuadricError_FQM(p, t._vertexID[1], t._vertexID[2]);
		t._error[2] = CalQuadricError_FQM(p, t._vertexID[2], t._vertexID[0]);
		t._error[3] = min(t._error[0], min(t._error[1], t._error[2]));
		m_refs.push_back(r);
	}
}

bool CSimplificationQuadricError::IsFlipped_FQM(std::vector<int>& vecDeleted, const Vec3& p, const MeshIOLib::index_t vid0, const MeshIOLib::index_t vid1, const MeshIOLib::Vertex& v0)
{
	loopk(0, v0._tcount)
	{
		// 한 버텍스에 접해져 있는 삼각형 리스트 
		MeshIOLib::Triangle& t = m_outTriangles[m_refs[v0._tstart + k]._tid];
		if (t._deleted)
			continue;

		// 다음 버텍스들의 인덱스를 구한다.
		int s = m_refs[v0._tstart + k]._tvertex;
		int vid1_tmp = t._vertexID[(s + 1) % 3];
		int vid2_tmp = t._vertexID[(s + 2) % 3];

		// 인덱스가 겹치면,
		if (vid1_tmp == vid1 || vid2_tmp == vid1) // delete ?
		{
			vecDeleted[k] = 1;
			continue;
		}
		// 20번째  vertex 값이 다르다
		// 겹치지 않으면, 새로 만든 점과의 각각의 거리를 구하고,
		Vec3 d1 = m_outVertices[vid1_tmp]._position - p;
		Vec3 d2 = m_outVertices[vid2_tmp]._position - p;
		Normalize(d1);
		Normalize(d2);

		// 방향이 겹치면,
		if (fabs(Dot(d1, d2)) > 0.999f)
			return true;

		Vec3 normal = Cross(d1, d2);
		Normalize(normal);
		vecDeleted[k] = 0;

		// 수직 관계의 벡터에 대한 예외처리
		if (Dot(normal, t._normal) < 0.2)
			return true;
	}
	return false;
}

double CSimplificationQuadricError::CalQuadricError_FQM(Vec3& p, const size_t v1, const size_t v2)
{
	MeshIOLib::SymetricMatrix q = m_outVertices[v1]._q + m_outVertices[v2]._q;
	bool bBorder = m_outVertices[v1]._border & m_outVertices[v2]._border;
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

void CSimplificationQuadricError::UpdateMesh_FQM(const int iter)
{
	if (iter > 0)
	{
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
	}

	Internal_UpdateRefData_FQM(m_outVertices, m_outTriangles, m_refs);
}

void CSimplificationQuadricError::Internal_SetInitData_FQM(std::vector<MeshIOLib::Vertex>& vertices, std::vector<MeshIOLib::Triangle>& tris, std::vector<MeshIOLib::Ref>& refs)
{
    std::vector<int> vcount, vids;

    loopi(0, vertices.size())
        vertices[i]._border = 0;

	// boundary vertex 구하기
	loopi(0, vertices.size())
	{
		MeshIOLib::Vertex& v = vertices[i];
		vcount.clear();
		vids.clear();
		loopj(0, v._tcount)
		{
			int triangleID = refs[v._tstart + j]._tid;
			MeshIOLib::Triangle& t = tris[triangleID];
			loopk(0, 3)
			{
				int ofs = 0;
				int id = t._vertexID[k];
				while (ofs < vcount.size())
				{
					if (vids[ofs] == id)
						break;
					ofs++;
				}
				if (ofs == vcount.size())
				{
					vcount.push_back(1);
					vids.push_back(id);
				}
				else
					vcount[ofs]++;
			}
		}
		loopj(0, vcount.size()) if (vcount[j] == 1)
			vertices[vids[j]]._border = 1;
	}
}

void CSimplificationQuadricError::Internal_UpdateRefData_FQM(std::vector<MeshIOLib::Vertex>& vertices, std::vector<MeshIOLib::Triangle>& tris, std::vector<MeshIOLib::Ref>& refs)
{
	// Init Reference ID list
	loopi(0, vertices.size())
	{
		vertices[i]._tstart = 0;
		vertices[i]._tcount = 0;
	}

	loopi(0, tris.size())
	{
		MeshIOLib::Triangle& t = tris[i];
		loopj(0, 3) vertices[t._vertexID[j]]._tcount++;
	}

	int tstart = 0;
	loopi(0, vertices.size())
	{
		MeshIOLib::Vertex& v = vertices[i];
		v._tstart = tstart;
		tstart += v._tcount;
		v._tcount = 0;
	}

	// Write References
	// refs는 한 버텍스에 접해져 있는 삼각형 리스트 구할 때 필요한 정보이다. 
	refs.resize(tris.size() * 3);
	loopi(0, tris.size())
	{
		MeshIOLib::Triangle& t = tris[i];
		loopj(0, 3)
		{
			MeshIOLib::Vertex& v = vertices[t._vertexID[j]];
			refs[v._tstart + v._tcount]._tid = i;
			refs[v._tstart + v._tcount]._tvertex = j;
			v._tcount++;
		}
	}
}