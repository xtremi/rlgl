#pragma once
#include <glm/glm.hpp>


/*

                  x-----------x-----------x-----------x-----------------------------------x
                  |			  |			  |           |                                   |
                  |			  |			  |			  |                                   |
                  |			  |           |			  |                                   |
                  x-----------x---x---x---x-----------x                                   |
                  |			  |NW | N | NE|			  |                                   |
                  |			  x---x---x---x			  |                                   |
                  |			  | W | c | E |	 lvl 1	  |         lvl 2                     |
                  |			  x---x---x---x			  |                                   |
                  |			  |SW | S | SE|           |                                   |
                  x-----------x---x---x---x-----------x                                   |
                  |			  |			  |           |                                   |
                  |			  |			  |			  |                                   |
                  |			  |           |			  |                                   |
                  x-----------x-----------x-----------x-----------------------------------x
                                                      |
                                                      |
                                                      |
                                                      |



*/
namespace rlgl {

    enum class LODloc{ east, north_east, north, north_west, west, south_west, south, south_east, center };

	class LODcontroller {
	public:
		LODcontroller() = default;
		LODcontroller(
            const glm::vec3& pos, 
            float            centerQuadSize, 
            int              levels,
            float            centerLimitDistanceFactor) 
            : m_pos{ pos }, m_centerQuadSideLength{ centerQuadSize },
            m_levels{ levels }, m_centerLimitDistanceFactor{ centerLimitDistanceFactor }{}

        void setCenterPosition(const glm::vec3& pos) { m_pos = pos; }
        bool outOfCenterLimit(const glm::vec3& pos) const;


        int              levels() const { return m_levels; }
        glm::vec2        quadPosition(LODloc loc, int level) const;
        double           quadSideLength(int level) const;
        static glm::vec2 LODlocUnitDir(LODloc loc);

	private:
		float     m_centerQuadSideLength;
        int       m_levels;
        float     m_centerLimitDistanceFactor;
        glm::vec3 m_pos;
	};



 
}


/*
  KEEPING THIS FOR NOW.
  Calculations if each quad is twice as big as the previous (and not 3 times like the the current implementation)
  
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