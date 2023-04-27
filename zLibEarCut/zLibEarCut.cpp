#include "../zLibMeshIO/include/Vector.h"
#include "header/CEarCut.h"
#include <iostream>

int main(int argc, char** argv)
{
    // input data 초기화 : PVT4//sample01.txt
    //std::vector<Vec2> inputData
    //{
    //    Vec2(50.0, 130.0),
    //    Vec2(20.0, 90.0),
    //    Vec2(40.0, 65.0),
    //    Vec2(30.0, 35.0),
    //    Vec2(15.0, 55.0),
    //    Vec2(5.0, 30.0),
    //    Vec2(35.0, 15.0),
    //    Vec2(55.0, 20.0),
    //    Vec2(70.0, 95.0),            
    //};
    // input data 초기화 : PVT4//sample02.txt
    std::vector<Vec2> inputData
    {
        vec2(15	,135),
        vec2(20	,115),
        vec2(10	,90) ,
        vec2(15	,70) ,
        vec2(5	,45) ,
        vec2(15	,25) ,
        vec2(25	,35) ,
        vec2(45	,15) ,
        vec2(65	,25) ,
        vec2(80	,10) ,
        vec2(95	,65) ,
        vec2(80	,80) ,
        vec2(120,95) ,
        vec2(80	,110),
        vec2(85	,125),
        vec2(75	,125),
        vec2(70	,140),
        vec2(55	,125),
    };

    CEarCut earCut(inputData);
    std::vector<Triangle> tris;
    bool bRtn = earCut.DoEarcutSubDivisionWithoutHole(tris);
    if (bRtn == false)
        return -1;
    
    for (int i = 0; i < tris.size(); ++i)
        std::cout << tris[i]._id[0] << "," << tris[i]._id[1] << "," << tris[i]._id[2] << std::endl;

    return 0;
}
