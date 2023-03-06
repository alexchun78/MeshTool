#ifndef CMESHHE_H
#define CMESHHE_H
#include "../include/MeshHEData.h"
#include "../../Common/include/CMeshIO.h"
#include "../../Common/include/CMeshIOData.h"

class CMeshHE
{
public:
    CMeshHE(const std::vector<MeshIOLib::Vertex>& vecVertices, const std::vector<MeshIOLib::Triangle>& vecTriangles);
    ~CMeshHE();
    
    

};

#endif // CMESHHE_H

