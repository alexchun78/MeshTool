#ifndef CEARCUT_H
#define CEARCUT_H 

#include "../../zLibMeshIO/include/Vector.h"
#include <vector>

typedef struct idx_tag {
    unsigned int _id;
    bool _isLive = true;
} IDX;

typedef struct triangle_tag {

    unsigned int _id[3];

    triangle_tag(int a, int b, int c)
    {
        _id[0] = a;
        _id[1] = b;
        _id[2] = c;
    }

} Triangle;

class CEarCut
{
public:
    CEarCut();
    CEarCut(const std::vector<vec2>& polyline);
    CEarCut(const std::vector<vec2>& polyline, const int startId);
    ~CEarCut();

    bool DoEarcutSubDivisionWithoutHole(std::vector<Triangle>& tris);


private:
    bool InsidePolygon(const std::vector<vec2>& polygon, const vec2& pt);
    vec2 GetCenterOfEdge(const vec2& p1, const vec2& p2);
    bool GetPrevAndPostID(int& prev, int& post, const int id, const std::vector<IDX>& ids);
    int AreIntersecting(
        float v1x1, float v1y1, float v1x2, float v1y2,
        float v2x1, float v2y1, float v2x2, float v2y2);
    
    bool IsPointOnSameLine(const vec2& p1, const vec2& p2, const vec2& p3);
    bool IsConvex(const vec2& p1, const vec2& p2, const vec2& p3);
    bool IsPointsInsideTriagle(const vec2& p1, const vec2& p2, const vec2& p3, const std::vector<vec2>& polygon);

private:
    std::vector<vec2> m_polyline;

};

#endif // CEARCUT_H





