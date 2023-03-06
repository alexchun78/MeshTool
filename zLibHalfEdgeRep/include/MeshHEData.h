#ifndef _MESHDATA_H
#define _MESHDATA_H
#include "../../zLibMeshIO/include/Vector.h"


namespace MeshHERepLib
{
    struct edgeHE_tag;
    typedef struct vertexHE_tag
    {
        Vec3 _position;
        edgeHE_tag* _ptrEdge;
    } VertexHE;

    typedef struct faceHE_tag
    {
        edgeHE_tag* edge;
    } FaceHE;

    typedef struct edgeHE_tag
    {
        vertexHE_tag* _ptrVertex;
        edgeHE_tag* _ptrPairEdge;
        edgeHE_tag* _ptrNextHE;
        edgeHE_tag* _ptrPrevHE;
        faceHE_tag* _ptrFace;

    } EdgeHE;
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
//
//typedef struct attributeHE_tag {
//
//    int _data;
//
//} AttributeHE;

#endif // _MESHDATA_H