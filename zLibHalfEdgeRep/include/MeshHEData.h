#ifndef _MESHHEREPLIB_MESHDATA_H
#define _MESHHEREPLIB_MESHDATA_H
#include "../../zLibMeshIO/include/Vector.h"
#include "../../Common/include/CMeshIOData.h"

namespace MeshHERepLib
{
    //typedef long index_t;

    typedef struct halfedge_t
    {
        // Index into the vertex array.
        MeshIOLib::index_t to_vertex;
        // Index into the face array.
        MeshIOLib::index_t face;
        // Index into the edges array.
        MeshIOLib::index_t edge;
        // Index into the halfedges array.
        MeshIOLib::index_t opposite_he;
        // Index into the halfedges array.
        MeshIOLib::index_t next_he;

        halfedge_t() :
            to_vertex(-1),
            face(-1),
            edge(-1),
            opposite_he(-1),
            next_he(-1)
        {}
    } HalfEdge;
}

// -- pointer (주소) 사용 방식
//typedef struct edgeHE_tag
//{
//    VertexHE* _ptrVertex;
//    EdgeHE* _ptrPairEdge;
//    FaceHE* _ptrFace;
//    EdgeHE* _ptrNext;
//
//} EdgeHE;
//
//typedef struct vertexHE_tag
//{
//    Vec3 _position;
//    EdgeHE* _ptrEdge;
//} VertexHE;
//
//
//typedef struct faceHE_tag
//{
//    EdgeHE* edge;
//} FaceHE;
#endif // _MESHHEREPLIB_MESHDATA_H