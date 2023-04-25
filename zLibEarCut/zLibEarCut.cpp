#include "../zLibMeshIO/include/Vector.h"
#include <vector>
#include <queue>

typedef struct idx_tag {
    unsigned int id;
    bool isLive = true;
} IDX;

#define loopi(a,b) for(int i = a; i < b; ++i)

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


            // 1.2.4. 유효하면, 삼각형 리스트에 저장한다.


            // 1.2.5. 해당 인덱스를 index list에서 지운다. ( -> islive = false) 


            // 1.2.6. 반복문 탈출한다. 
        }


    }





    return 0;
}