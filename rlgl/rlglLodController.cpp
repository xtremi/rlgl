#include "rlglLodController.h"
using namespace rlgl;


/*

 x--|--x----x--------x----------------x--------------------------------x
 |  |  |    |        |                |                                |
 |  |  |    |        |                |                                |
 x--|--x----x--------x----------------x--------------------------------x
    |0.5  1      3           9                       27
    |
    |---->1
    |----------->3         
    |----------------------->9
    |----------------------------------------------->27

    NOTE:   Distance from center of center quad to center of quad N is
            equal to width of quad N

1  = 3^0
3  = 3^1
9  = 3^3
27 = 3^4

 */
glm::vec2 LODcontroller::quadPosition(LODloc loc, int level) const {

    float length = m_centerQuadSideLength * glm::pow(3, level);    //length to end of level
    return LODlocUnitDir(loc) * length;
}

double LODcontroller::quadSideLength(int level) const {
    return m_centerQuadSideLength * glm::pow(3, level);
}


bool LODcontroller::outOfCenterLimit(const glm::vec3& pos) const {
    return glm::abs(glm::distance(glm::vec2(pos.x, pos.y), glm::vec2(m_pos.x, m_pos.y))) >
        m_centerLimitDistanceFactor * m_centerQuadSideLength;
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

/*If quads a doubling in size instead of 3x*/
/*glm::vec2 LODcontroller::quadPosition(LODloc loc, int level) const {

    float quadSideL = quadSideLength(level);
    float length = m_centerQuadSideLength * (glm::pow(2, level + 1) - 1.0 + 0.5);    //length to end of level
    length -= quadSideL / 2.f;

    return LODlocUnitDir(loc) * length;
}

double LODcontroller::quadSideLength(int level) const {
    return m_centerQuadSideLength * glm::pow(2, level);
}
*/