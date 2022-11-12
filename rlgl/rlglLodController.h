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
		LODcontroller(float centerQuadSize, int _levels) 
            : m_centerQuadSideLength{centerQuadSize}, m_levels{_levels}{}

        void setCurrentPosition(const glm::vec2& pos) {
            m_currentCenterPos = pos;
        }

        glm::vec2 quadPosition(LODloc loc, int level);
        double quadSideLength(int level);

        static glm::vec2 LODlocUnitDir(LODloc loc);

        int levels() { return m_levels; }

	private:
		glm::vec2 m_currentCenterPos;
		double    m_centerQuadSideLength;
        int       m_levels;

	};



 
}