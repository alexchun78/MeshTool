#include "../pch.h"
#include "../include/CMeshHE.h"

namespace MeshHERepLib
{
    CMeshHE::CMeshHE(const std::vector<MeshIOLib::Vertex>& vecVertices, const std::vector<MeshIOLib::Triangle>& vecTriangles)
    {
        m_vecVertices = vecVertices;
        m_vecTriangles = vecTriangles;
    }

    CMeshHE::~CMeshHE()
    {

    }

    bool CMeshHE::Build()
    {
        if (m_vecVertices.size() == 0 || m_vecTriangles.size() == 0)
            return false;

        // 먼저 edge를 구성해준다.
        for (int i = 0; i < m_vecTriangles.size(); ++i) {
            MeshHERepLib::EdgeHE edgeHE;
            auto& tri = m_vecTriangles[i];
            for (int t = 0; t < 3; ++t)
            {
               // edgeHE._ptrVertex->_position = m_vecVertices[(int)tri._vertexID[t]]._position;
               // edge
            }

            
        }



        return true;
    }

}