#include "rlglLodController.h"
using namespace rlgl;


/*
 |  |  |    |        |                |                                |
 |  |  |    |        |                |                                |
 x--|--x----x--------x----------------x--------------------------------x
    |
    |0.5  1      2           4                       8
    |  
    |     1    2.5         5.5                      11.5


 11.5 = 2^0 + 2^1 + 2^2 + (2^8 / 2) + 0.5  = 1+2+4+4 + 0.5   
 8.5  = 2^0 + 2^1 + (2^2 / 2) + 0.5 = 1+2+2 + 0.5  
 4.5  = 2^0 + (2^1 / 2) = 2 + 0.5   


 sumOfPowers = 2^(N+1) - 1 = 2^0 + 2^1 + 2^2 .... + 2^N

 */

glm::vec2 LODcontroller::quadPosition(LODloc loc, int level) {

    float quadSideL = quadSideLength(level);
    float length = m_centerQuadSideLength * (glm::pow(2, level + 1) - 1.0 + 0.5);    //length to end of level
    length -= quadSideL/2.f;

    return LODlocUnitDir(loc) * length;
}

double LODcontroller::quadSideLength(int level) {
    return m_centerQuadSideLength * glm::pow(2, level);
}


glm::vec2 LODcontroller::LODlocUnitDir(LODloc loc) {
    switch (loc)
    {
    case rlgl::LODloc::center:
        return glm::vec2(0.f);        break;
    case rlgl::LODloc::east:
        return glm::vec2(1.f, 0.f);   break;
    case rlgl::LODloc::north_east:
        return glm::vec2(1.f, 1.f);   break;
    case rlgl::LODloc::north:
        return glm::vec2(0.f, 1.f);   break;
    case rlgl::LODloc::north_west:
        return glm::vec2(-1.f, 1.f);  break;
    case rlgl::LODloc::west:
        return glm::vec2(-1.f, 0.f);  break;
    case rlgl::LODloc::south_west:
        return glm::vec2(-1.f, -1.f); break;
    case rlgl::LODloc::south:
        return glm::vec2(0.f, -1.f);  break;
    case rlgl::LODloc::south_east:
        return glm::vec2(1.f, -1.f);  break;
    default:
        return glm::vec2(0.f);
    }
}