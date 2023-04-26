#include "../zLibMeshIO/include/Vector.h"
#include <vector>
#include <queue>

typedef struct idx_tag {
    unsigned int id;
    bool isLive = true;
} IDX;

#define loopi(a,b) for(int i = a; i < b; ++i)

float CalcAngleBetweenTwoStraightLine(vec2& a, vec2& b, vec2& c);

int main(int argc, char** argv)
{
    // input data 초기화 : PVT4//sample01.txt
    std::vector<Vec2> inputData
    {
        Vec2(50.0, 130.0),
        Vec2(20.0, 90.0),
        Vec2(40.0, 65.0),
        Vec2(30.0, 35.0),
        Vec2(15.0, 55.0),
        Vec2(5.0, 30.0),
        Vec2(35.0, 15.0),
        Vec2(55.0, 20.0),
        Vec2(70.0, 95.0),            
    };

    std::vector<IDX> idxList(inputData.size());
    
    // 1. vertex index list를 구성한다.
    loopi(0,idxList.size())
    {
        //idxList[i].id = i;
        idxList[i].isLive = true;
    }
    
    // y축 기준으로 최대 최소 점을 찾는다.
    // 최대점 기준으로 현재 point가 어디에 있는 지 확인한다.
    // 

    // 2. vertex index list가 3개 남을 때까지 반복한다.
    int count = idxList.size();
    int preID=0, midID=0, postID=0;
    std::queue<int> queueIDX;
    // 1. 버텍스가 3개 남을 때까지 반복한다.  
    while (count > 3)
    {
        // 1.1 대기열을 구성한다.
        queueIDX = std::queue<int>(); // 초기화
        for (int i = 0; i < idxList.size(); ++i)
        {
            if (idxList[i].isLive)
                queueIDX.push(i);
        }

        // 1.2. 대기열이 끝날 때까지 반복한다.
        while (!queueIDX.empty())
        {
            // 1.2.1. 처음 idx를 가져온다.
            midID = queueIDX.front();
            queueIDX.pop();

            // 1.2.2. 주변 idx를 가져온다.
            preID = queueIDX.back();
            postID = queueIDX.front();

            // 1.2.3. ear를 구성할 수 있는 유효한 인덱스 들인지 확인한다.
            // // 각도 검사
            // 벡터 연산으로 각도를 구해보자
            vec2 a = inputData[preID] - inputData[midID];
            //Normalize(a);
            vec2 b = inputData[postID] - inputData[midID];
            //Normalize(b);
            float angle = CalcAngleBetweenTwoStraightLine(inputData[preID], inputData[midID], inputData[postID]);
           //float angle= CalcAngleBetweenTwoVectors(a, b);
            int abb = 0;
            //(b-a)
            // // 다른 점이 삼각형 내부에 있는 지 여부 확인


            // 1.2.4. 유효하면, 삼각형 리스트에 저장한다.


            // 1.2.5. 해당 인덱스를 index list에서 지운다. ( -> islive = false)
            // count 감소시킨다.


            // 1.2.6. 반복문 탈출한다. 
        }
    }

    return 0;
}

//https://www.nagwa.com/en/explainers/407162748438/
float CalcAngleBetweenTwoStraightLine(vec2& a, vec2& b,vec2& c)
{
    // y = mx+b
    // y-y0 = m(x-x0)
    // m = (y1-y0) / (x1-x0) --> clockwise
    // m = - ((y1-y0) / (x1-x0)) --> countclockwise
    // tanA = abs((m1-m2) / (1+m1*m2))
    auto m1 = -1 * (b.y - a.y) / (b.x - a.x);
    auto m2 = -1 * (c.y - b.y) / (c.x - b.x);

    auto tanA = ::abs((m1 - m2) / (1 + m1 * m2));

    auto angle = ::atan(tanA);
    angle = angle * 180 / 3.14159;

    Vec2 t1 = a - b;
    t1 = Normalized(t1);
    Vec2 t2 = b-c;
    t2 = Normalized(t2);
    float dotAB = Dot(t1, t2);
    auto rad = ::acos(dotAB);
    auto angle2 = rad * 180 / 3.14159;

    //float m = (b.y - a.y) / (b.x - a.x);
    //float angle = ::atan(m);

    //float dotAB = Dot(a, b);
    //float aSq = ::sqrt(a.x * a.x + a.y * a.y);
    //float bSq = ::sqrt(b.x * b.x + b.y * b.y);

    //float cos = ::abs(dotAB) / aSq * bSq;

    return angle;
}


// https://hgko1207.github.io/2021/04/13/cpp-coding-3/
struct Point
{
    int x;
    int y;
};

bool InsidePolygon(int nvert, Point polygon[], int pointx, int pointy)
{
    int i, j = 0;
    bool inside = false;

    for (i = 0, j = nvert - 1; i < nvert; j = i++)
    {
        if (((polygon[i].y > pointy) != (polygon[j].y > pointy)) &&
            (pointx < (polygon[j].x - polygon[i].x) * (pointy - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }

    return inside;
}