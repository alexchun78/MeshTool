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


            // 1.2.4. ��ȿ�ϸ�, �ﰢ�� ����Ʈ�� �����Ѵ�.


            // 1.2.5. �ش� �ε����� index list���� �����. ( -> islive = false) 


            // 1.2.6. �ݺ��� Ż���Ѵ�. 
        }


    }





    return 0;
}