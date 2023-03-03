#ifndef _MESHDATA_H
#define _MESHDATA_H
#include "../../zLibMeshIO/include/Vector.h"

typedef struct vertexHE_tag
{
    Vec3 _position;
    EdgeHE* _ptrEdge;

} VertexHE;

typedef struct edgeHE_tag
{
    VertexHE* _ptrVertex;
    EdgeHE* _ptrPairEdge;
    FaceHE* _ptrFace;
    EdgeHE* _ptrNext;
} EdgeHE;

typedef struct faceHE_tag
{
    EdgeHE* edge;
} FaceHE;

typedef struct attributeHE_tag {

    int _data;

} AttributeHE;

#endif // _MESHDATA_H