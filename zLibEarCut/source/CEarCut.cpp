#include "../header/CEarCut.h"
#include <vector>
#include <queue>
#include <limits>
#define loopi(a,b) for(int i = a; i < b; ++i)
#define loopj(a,b) for(int j = a; j < b; ++j)
#define NO 0
#define YES 1
#define COLLINEAR 2

//#define SAMEFLOAT(a,b) (fabs(a - b) < std::numeric_limits<float>::epsilon())
bool AreSame(float a, float b)
{
    return fabs(a - b) < std::numeric_limits<float>::epsilon();
}

CEarCut::CEarCut()
{

}

CEarCut::CEarCut(const std::vector<vec2>& polyline)
{
    m_polyline = polyline;
}

CEarCut::CEarCut(const std::vector<vec2>& polyline, const int startId)
{
    std::vector<vec2> temp(polyline.size());
    std::copy(polyline.begin() + startId, polyline.end(), temp.begin());
    std::copy(polyline.begin(), polyline.begin() + startId, temp.begin()+temp.size()-startId);
    //for (int i = startId; i < polyline.size(); ++i)
    //{

    //}
    m_polyline = temp;
}

CEarCut::~CEarCut()
{

}

bool CEarCut::DoEarcutSubDivisionWithoutHole(std::vector<Triangle>& tris)
{
    if (m_polyline.size() < 3)
        return false;
    // 1. vertex index list를 구성한다.
    std::vector<IDX> idxList(m_polyline.size());    

    // 2. vertex index list가 3개 남을 때까지 반복한다.
    int count = idxList.size();
    int preID = 0, midID = 0, postID = 0;
    std::queue<int> queueIDX;
    // 1. 버텍스가 3개 남을 때까지 반복한다.  
    while (count > 3)
    {
        // 1.1 대기열을 구성한다.
        std::vector<vec2> temp;
        temp.reserve(m_polyline.size());
        queueIDX = std::queue<int>(); // 초기화
        for (int i = 0; i < idxList.size(); ++i)
        {
            if (idxList[i]._isLive)
            {
                queueIDX.push(i);
                temp.push_back(m_polyline[i]);
            }                
        }

        // 1.2. 대기열이 끝날 때까지 반복한다.
        while (!queueIDX.empty())
        {
            // 1.2.1. 처음 idx를 가져온다.
            midID = queueIDX.front();
            queueIDX.pop();

            // 1.2.2. 주변 idx를 가져온다.
            bool bRtn = GetPrevAndPostID(preID, postID, midID, idxList);
            if (bRtn == false)
                return false;

            // 1.2.3. ear를 구성할 수 있는 유효한 인덱스 들인지 확인한다.
            // // 같은 선 상에 있는 지 검사
            bool isSameLine = IsPointOnSameLine(m_polyline[preID], m_polyline[midID], m_polyline[postID]);
            if (isSameLine == true)
                continue;

            // // convex인지 reflex인지 검사
            bool isConvex = IsConvex(m_polyline[preID], m_polyline[midID], m_polyline[postID]);
            if (isConvex == false)
                continue;
            
            // // 다른 점이 삼각형 내부에 있는 지 여부 확인
            bool isPointsInside = IsPointsInsideTriagle(m_polyline[preID], m_polyline[midID], m_polyline[postID], temp);
            if (isPointsInside == true)
                continue;

            // 1.2.4. 유효하면, 삼각형 리스트에 저장한다.
            tris.push_back(Triangle(preID, midID, postID));

            // 1.2.5. 해당 인덱스를 index list에서 지운다. ( -> islive = false)
            idxList[midID]._isLive = false;
            
            // count 감소시킨다.
            count--;

            // 1.2.6. 마지막 삼각형을 저장한다.
            if (count == 3)
            {
                std::vector<int> lastTri(3);
                int idx = 0;
                loopi(0, idxList.size())
                {
                    if (idxList[i]._isLive)
                    {
                        lastTri[idx++] = i;
                    }
                }
                tris.push_back(Triangle(lastTri[0], lastTri[1], lastTri[2]));
            }

            // 1.2.7. 반복문 탈출한다. 
            break;
        }

    }


    return true;
}

bool CEarCut::GetPrevAndPostID(int& prev, int& post, const int id, const std::vector<IDX>& ids)
{
    // prev 구하기
    prev = post = -1;
    for(int i = id-1; i >=0; --i)
    {
        if (ids[i]._isLive == true)
        {
            prev = i;
            break;
        }
    }
    if (prev == -1)
    {
        for (int i = ids.size()-1; i >id; --i)
        {
            if (ids[i]._isLive == true)
            {
                prev = i;
                break;
            }
        }
    }
    if (prev == -1)
        return false;

    // post 구하기
    for (int i = id+1; i < ids.size(); ++i)
    {
        if (ids[i]._isLive == true)
        {
            post = i;
            break;
        }
    }
    if (post == -1)
    {
        for (int i = 0; i < id; ++i)
        {
            if (ids[i]._isLive == true)
            {
                post = i;
                break;
            }
        }
    }
    if (post == -1)
        return false;

    return true;
}

bool CEarCut::IsPointsInsideTriagle(const vec2& p1, const vec2& p2, const vec2& p3, const std::vector<vec2>& polygon)
{
    std::vector<vec2> tri{ p1,p2,p3 };

    // 바운딩 박스를 구한다.
    float minX = ::fmin(::fmin(p1.x, p2.x), p3.x) - 10;

    loopi(0, polygon.size())
    {
        auto& p = polygon[i];
        if (p == p1 || p == p2 || p == p3)
            continue;

        vec2 sRay = vec2(minX, p.y);
        vec2 eRay = p;

        int intersections = 0;
        loopj(0, 3)
        {
            vec2& a = tri[j];
            vec2& b = tri[(j+1)%3];

            // Test if current side intersects with ray.
            int nResult = AreIntersecting(a.x, a.y, b.x, b.y, sRay.x, sRay.y, eRay.x, eRay.y);
            // If yes, intersections++;
            if (nResult == YES || nResult == COLLINEAR)
                intersections++;
        }

        if ((intersections & 1) == 1)
        {
            // Inside of polygon
            return true;
        }
        else 
        {
            // Outside of polygon
            return false;
        }
    }
    
    //loopi(0, polygon.size())
    //{
    //    bool bRtn = InsidePolygon(tri, polygon[i]);
    //    if (bRtn == true)
    //        return true;
    //}
    return false;
}

bool CEarCut::IsConvex(const vec2& p1, const vec2& p2, const vec2& p3)
{
// 1.2.3.1. 두 선분을 구한다.
// 1.2.3.2. 두 선분의 중점을 구한다.
// 1.2.3.3. 두 선분의 중점을 잇는 선분을 구한다.
// 1.2.3.4. 잇는 선분의 중점을 구한다. 
// 1.2.3.5. 잇는 선분의 중점이 POLYLINE에 속한 지 확인한다.
// 1.2.3.6. 속하면, convex이고, 속하지 않으면 reflex이다.  
    vec2 center1 = GetCenterOfEdge(p1,p2);
    vec2 center2 = GetCenterOfEdge(p2,p3);
    vec2 center3 = GetCenterOfEdge(center1, center2);
    return InsidePolygon(m_polyline, center3);
}

bool CEarCut::InsidePolygon(const std::vector<vec2>& polygon, const vec2& pt)
{
    // reference : https://hgko1207.github.io/2021/04/13/cpp-coding-3/

    int i, j = 0;
    bool inside = false;
    int nVert = polygon.size();
    for (i = 0, j = nVert - 1; i < nVert; j = i++)
    {
        if (((polygon[i].y > pt.y) != (polygon[j].y > pt.y)) &&
            (pt.x < (polygon[j].x - polygon[i].x) * (pt.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }

    return inside;
}

vec2 CEarCut::GetCenterOfEdge(const vec2& p1, const vec2& p2)
{
    float x = ::fmin(p1.x, p2.x);
    float y = ::fmin(p1.y, p2.y);

    x += ::fabs((p1.x - p2.x)/2.0f);
    y += ::fabs((p1.y - p2.y)/2.0f);
    return vec2(x, y);
}

bool CEarCut::IsPointOnSameLine(const vec2& p1, const vec2& p2, const vec2& p3)
{
    if ((AreSame(p1.x, p2.x) && AreSame(p2.x, p3.x)) 
        || (AreSame(p1.y, p2.y) && AreSame(p2.y, p3.y)))
    {
        return true;
    }
    return false;
}

// ref : https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
int CEarCut::AreIntersecting(
    float v1x1, float v1y1, float v1x2, float v1y2,
    float v2x1, float v2y1, float v2x2, float v2y2) 
{
    float d1, d2;
    float a1, a2, b1, b2, c1, c2;

    // Convert vector 1 to a line (line 1) of infinite length.
    // We want the line in linear equation standard form: A*x + B*y + C = 0
    // See: http://en.wikipedia.org/wiki/Linear_equation
    a1 = v1y2 - v1y1;
    b1 = v1x1 - v1x2;
    c1 = (v1x2 * v1y1) - (v1x1 * v1y2);

    // Every point (x,y), that solves the equation above, is on the line,
    // every point that does not solve it, is not. The equation will have a
    // positive result if it is on one side of the line and a negative one 
    // if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
    // 2 into the equation above.
    d1 = (a1 * v2x1) + (b1 * v2y1) + c1;
    d2 = (a1 * v2x2) + (b1 * v2y2) + c1;

    // If d1 and d2 both have the same sign, they are both on the same side
    // of our line 1 and in that case no intersection is possible. Careful, 
    // 0 is a special case, that's why we don't test ">=" and "<=", 
    // but "<" and ">".
    if (d1 > 0 && d2 > 0) return NO;
    if (d1 < 0 && d2 < 0) return NO;

    // The fact that vector 2 intersected the infinite line 1 above doesn't 
    // mean it also intersects the vector 1. Vector 1 is only a subset of that
    // infinite line 1, so it may have intersected that line before the vector
    // started or after it ended. To know for sure, we have to repeat the
    // the same test the other way round. We start by calculating the 
    // infinite line 2 in linear equation standard form.
    a2 = v2y2 - v2y1;
    b2 = v2x1 - v2x2;
    c2 = (v2x2 * v2y1) - (v2x1 * v2y2);

    // Calculate d1 and d2 again, this time using points of vector 1.
    d1 = (a2 * v1x1) + (b2 * v1y1) + c2;
    d2 = (a2 * v1x2) + (b2 * v1y2) + c2;

    // Again, if both have the same sign (and neither one is 0),
    // no intersection is possible.
    if (d1 > 0 && d2 > 0) return NO;
    if (d1 < 0 && d2 < 0) return NO;

    // If we get here, only two possibilities are left. Either the two
    // vectors intersect in exactly one point or they are collinear, which
    // means they intersect in any number of points from zero to infinite.
    if ((a1 * b2) - (a2 * b1) == 0.0f) return COLLINEAR;

    // If they are not collinear, they must intersect in exactly one point.
    return YES;
}