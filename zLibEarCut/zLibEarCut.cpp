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
#if 1
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
#else
    // 손 모양의 폴리곤 
    // -> 10번째 점을 시작 지점으로 할 경우에 점이 5개 남았는데, 모든 점을 검사해도 IsConvex를 통과하지 못해서 무한 루프 발생함
    // -> [TODO] 남아 있는 점
    std::vector<Vec2> inputData
    { 
        vec2(496.200,208.600), vec2(557.100,319.900), vec2(606.100,415.800), vec2(644.900,500.900), vec2(627.100,534.400),
  vec2(593.200,512.700), vec2(568.900,469.200), vec2(539.800,424.000), vec2(500.500,433.900), vec2(486.000,501.800), vec2(481.700,569.700),
  vec2(493.500,663.800), vec2(495.700,733.500), vec2(496.800,783.300),vec2(489.800,840.300), vec2(477.900,855.700), vec2(461.800,850.200),
  vec2(457.500,822.200), vec2(456.900,770.600), vec2(456.400,736.200),
  vec2(448.300,678.300), vec2(437.500,612.200), vec2(418.100,595.000), vec2(405.200,642.100), vec2(396.100,714.500), vec2(381.500,773.300),
  vec2(369.100,862.000), vec2(351.400,885.500), vec2(329.800,866.500),vec2(334.700,817.600), vec2(348.700,744.300), vec2(354.600,688.200),
  vec2(372.400,586.000), vec2(360.500,586.000), vec2(339.000,641.200), vec2(314.200,701.800),
  vec2(279.800,811.300), vec2(272.200,815.800), vec2(256.100,808.600),vec2(249.600,786.900),
  vec2(256.100,754.300), vec2(287.300,656.600), vec2(297.000,622.200), vec2(322.800,537.100), vec2(312.600,530.800), vec2(288.400,557.900),
  vec2(251.800,626.700), vec2(231.300,657.500), vec2(215.100,656.600),vec2(213.000,621.300), vec2(235.100,572.400), vec2(250.700,535.300) ,
  vec2(289.400,461.100), vec2(299.100,420.400), vec2(306.100,363.300), vec2(313.100,319.000), vec2(323.900,264.700), vec2(336.300,235.700),
  vec2(356.200,217.600), vec2(496.200,208.600) 
    };
#endif

#if 1
    CEarCut earCut(inputData);
    std::vector<Triangle> tris;
    bool bRtn = earCut.DoEarcutSubDivisionWithoutHole(tris);
    if (bRtn == false)
        return -1;

    for (int i = 0; i < tris.size(); ++i)
        std::cout << tris[i]._id[0] << "," << tris[i]._id[1] << "," << tris[i]._id[2] << std::endl;

#else
    // test
   // for (int i = 0; i < inputData.size(); ++i)
    {
        int i = 10;
        CEarCut earCut(inputData, i);
        std::vector<Triangle> tris;
        bool bRtn = earCut.DoEarcutSubDivisionWithoutHole(tris);
        if (bRtn == false)
            return -1;
        for (int i = 0; i < tris.size(); ++i)
            std::cout << tris[i]._id[0] << "," << tris[i]._id[1] << "," << tris[i]._id[2] << std::endl;
    }
#endif
    return 0;
}
