#ifndef CMESHHE_H
#define CMESHHE_H
#include "../include/MeshHEData.h"
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CMeshIOData.h"
#include "MeshHEData.h"
#include <vector>

namespace MeshHERepLib
{
    class CMeshHE 
    {
    public:
        CMeshHE(const std::vector<MeshIOLib::Vertex>& vecVertices, const std::vector<MeshIOLib::Triangle>& vecTriangles);
        ~CMeshHE();
    
        bool Build();

    private:
        std::vector<MeshIOLib::Vertex> m_vecVertices;
        std::vector<MeshIOLib::Triangle> m_vecTriangles;
        std::vector<MeshHERepLib::EdgeHE> m_halfEdgeList;
    };
}
#endif // CMESHHE_H

