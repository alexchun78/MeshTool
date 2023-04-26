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
    // input data �ʱ�ȭ : PVT4//sample01.txt
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
    
    // 1. vertex index list�� �����Ѵ�.
    loopi(0,idxList.size())
    {
        //idxList[i].id = i;
        idxList[i].isLive = true;
    }
    
    // y�� �������� �ִ� �ּ� ���� ã�´�.
    // �ִ��� �������� ���� point�� ��� �ִ� �� Ȯ���Ѵ�.
    // 

    // 2. vertex index list�� 3�� ���� ������ �ݺ��Ѵ�.
    int count = idxList.size();
    int preID=0, midID=0, postID=0;
    std::queue<int> queueIDX;
    // 1. ���ؽ��� 3�� ���� ������ �ݺ��Ѵ�.  
    while (count > 3)
    {
        // 1.1 ��⿭�� �����Ѵ�.
        queueIDX = std::queue<int>(); // �ʱ�ȭ
        for (int i = 0; i < idxList.size(); ++i)
        {
            if (idxList[i].isLive)
                queueIDX.push(i);
        }

        // 1.2. ��⿭�� ���� ������ �ݺ��Ѵ�.
        while (!queueIDX.empty())
        {
            // 1.2.1. ó�� idx�� �����´�.
            midID = queueIDX.front();
            queueIDX.pop();

            // 1.2.2. �ֺ� idx�� �����´�.
            preID = queueIDX.back();
            postID = queueIDX.front();

            // 1.2.3. ear�� ������ �� �ִ� ��ȿ�� �ε��� ������ Ȯ���Ѵ�.
            // // ���� �˻�
            // ���� �������� ������ ���غ���
            vec2 a = inputData[preID] - inputData[midID];
            //Normalize(a);
            vec2 b = inputData[postID] - inputData[midID];
            //Normalize(b);
            float angle = CalcAngleBetweenTwoStraightLine(inputData[preID], inputData[midID], inputData[postID]);
           //float angle= CalcAngleBetweenTwoVectors(a, b);
            int abb = 0;
            //(b-a)
            // // �ٸ� ���� �ﰢ�� ���ο� �ִ� �� ���� Ȯ��


            // 1.2.4. ��ȿ�ϸ�, �ﰢ�� ����Ʈ�� �����Ѵ�.


            // 1.2.5. �ش� �ε����� index list���� �����. ( -> islive = false)
            // count ���ҽ�Ų��.


            // 1.2.6. �ݺ��� Ż���Ѵ�. 
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