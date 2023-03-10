#include "../include/Export.h"
namespace MeshHERepLib
{
    // Export DLL
    CHalfEdgeRep* CreateHalfEdgeRep(const unsigned long vertexCount, const std::vector<MeshIOLib::Triangle>& vecTriangles)
    {
        return new CHalfEdgeRep(vertexCount, vecTriangles);
    }

    MeshIOLib::index_t Build(CHalfEdgeRep* object)
    {
        if (object == NULL)
            return -2; // error code : -2

        auto vertCnt = object->GetVertexCount();
        auto& vecTris = object->GetTriangles();
        bool bRtn = object->Build(vertCnt, vecTris);
        return (bRtn == true)? 1 : 0;
    }

    const HalfEdge& GetHalfEdge(CHalfEdgeRep* object, const MeshIOLib::index_t id)
    {
        if (object == NULL)
            return HalfEdge();

        return object->GetHalfEdge(id);
    }

    GetEdgeFromHalfEdge(CHalfEdgeRep* object, OUT std::pair<MeshIOLib::index_t, MeshIOLib::index_t>& outEdge, const MeshIOLib::index_t id)
    {
        if (object == NULL)
            return std::make_pair(-2, -2); // error code : -2

        return object->GetEdgeFromHalfEdge(id);
    }

    MeshIOLib::index_t GetHalfEdgeIndexFromEdge(CHalfEdgeRep* object, const MeshIOLib::index_t& v1, const MeshIOLib::index_t& v2)
    {
        if (object == NULL)
            return -2; // error code : -2

        return object->GetHalfEdgeIndexFromEdge(v1, v2);
    }

    bool FindVertexNeighborsFromVertex(CHalfEdgeRep* object, std::vector<MeshIOLib::index_t>& neighbors, MeshIOLib::index_t vid)
    {
        if (object == NULL)
            return false;

        object->FindVertexNeighborsFromVertex(neighbors, vid);
        return true;
    }

    bool FindFaceNeighborsFromVertex(CHalfEdgeRep* object, std::vector<MeshIOLib::index_t>& neighbors, MeshIOLib::index_t vid)
    {
        if (object == NULL)
            return false;

        object->FindFaceNeighborsFromVertex(neighbors, vid);
        return true;
    }

    bool FindFaceNeighborsFromFace(CHalfEdgeRep* object, std::vector<MeshIOLib::index_t>& neighbors, MeshIOLib::index_t fid)
    {
        if (object == NULL)
            return false;

        object->FindFaceNeighborsFromFace(neighbors, fid);
        return true;
    }

    MeshIOLib::index_t IsBoundaryVertex(CHalfEdgeRep* object, const MeshIOLib::index_t vid)
    {
        if (object == NULL)
            return -2; // error code : -2

        bool bRtn = object->IsBoundaryVertex(vid);
        return (bRtn == true) ? 1 : 0;
    }

    std::vector<MeshIOLib::index_t> GetBoundaryVertices(CHalfEdgeRep* object)
    {
        if (object == NULL)
            return std::vector<MeshIOLib::index_t>();

        return object->GetBoundaryVertices();
    }

    std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>> GetBoundaryEdges(CHalfEdgeRep* object)
    {
        if (object == NULL)
            return std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>>();

        return object->GetBoundaryEdges();
    }

    void TerminateHalfEdgeRep(CHalfEdgeRep* object)
    {
        if (object != nullptr)
        {
            delete object;
            object = nullptr;
        }
    }
}