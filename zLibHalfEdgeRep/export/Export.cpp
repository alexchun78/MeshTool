#include "Export.h"
namespace MeshHERepLib
{
    // Export DLL
    CHalfEdgeRep* CreateHalfEdgeRep()
    {
        return new CHalfEdgeRep();
    }

    MeshIOLib::index_t Build(IN CHalfEdgeRep* object, IN const unsigned long vertexListCount, IN const std::vector<MeshIOLib::Triangle>& vecTriangles)
    {
        if (object == NULL)
            return -2; // error code : -2

        //auto vertCnt = object->GetVertexCount();
        //auto& vecTris = object->GetTriangles();
        bool bRtn = object->Build(vertexListCount, vecTriangles);
        return (bRtn == true)? 1 : 0;
    }

    const HalfEdge& GetHalfEdge(IN CHalfEdgeRep* object, IN const MeshIOLib::index_t id)
    {
        if (object == NULL)
            return HalfEdge();

        return object->GetHalfEdge(id);
    }

    bool GetEdgeFromHalfEdge(IN CHalfEdgeRep* object, OUT std::pair<MeshIOLib::index_t, MeshIOLib::index_t>& edge, IN const MeshIOLib::index_t id)
    {
        if (object == NULL)
            return false;
        
        edge = object->GetEdgeFromHalfEdge(id);
        return true;
    }

    MeshIOLib::index_t GetHalfEdgeIndexFromEdge(IN CHalfEdgeRep* object, IN const MeshIOLib::index_t& v1, IN const MeshIOLib::index_t& v2)
    {
        if (object == NULL)
            return -2; // error code : -2

        return object->GetHalfEdgeIndexFromEdge(v1, v2);
    }

    bool FindVertexNeighborsFromVertex(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid)
    {
        if (object == NULL)
            return false;

        object->FindVertexNeighborsFromVertex(neighbors, vid);
        return true;
    }

    bool FindFaceNeighborsFromVertex(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t vid)
    {
        if (object == NULL)
            return false;

        object->FindFaceNeighborsFromVertex(neighbors, vid);
        return true;
    }

    bool FindFaceNeighborsFromFace(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& neighbors, IN MeshIOLib::index_t fid)
    {
        if (object == NULL)
            return false;

        object->FindFaceNeighborsFromFace(neighbors, fid);
        return true;
    }

    MeshIOLib::index_t IsBoundaryVertex(IN CHalfEdgeRep* object, OUT const MeshIOLib::index_t vid)
    {
        if (object == NULL)
            return -2; // error code : -2

        bool bRtn = object->IsBoundaryVertex(vid);
        return (bRtn == true) ? 1 : 0;
    }

    bool GetBoundaryVertices(IN CHalfEdgeRep* object, OUT std::vector<MeshIOLib::index_t>& boundaryVertices)    
    {
        if (object == NULL)
            return false; // error code : -2

        boundaryVertices.clear();

        boundaryVertices = object->GetBoundaryVertices();

        return true;
    }

    bool GetBoundaryEdges(IN CHalfEdgeRep* object, OUT std::vector<std::pair<MeshIOLib::index_t, MeshIOLib::index_t>>& boundaryEdges)
    {
        if (object == NULL)
            return false;

        boundaryEdges = object->GetBoundaryEdges();
        return true;
    }

    void TerminateHalfEdgeRep(IN CHalfEdgeRep* object)
    {
        if (object != nullptr)
        {
            delete object;
            object = nullptr;
        }
    }
}