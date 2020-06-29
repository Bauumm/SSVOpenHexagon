// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#pragma once

#include "SSVOpenHexagon/Components/CCustomWallHandle.hpp"
#include "SSVOpenHexagon/Components/CCustomWall.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>

namespace hg
{

class HexagonGame;

class CCustomWallManager
{
    std::vector<CCustomWall> _customWalls;
    std::vector<CCustomWallHandle> _freeHandles;
    std::vector<bool> _handleAvailable;
    CCustomWallHandle _nextFreeHandle{0};

    [[nodiscard]] bool isValidHandle(const CCustomWallHandle h) const noexcept;

public:
    [[nodiscard]] CCustomWallHandle create();

    void destroy(const CCustomWallHandle cwHandle);

    void setVertexPos(const CCustomWallHandle cwHandle, const int vertexIndex,
        const sf::Vector2f& pos);

    void setVertexColor(const CCustomWallHandle cwHandle, const int vertexIndex,
        const sf::Color& color);

    [[nodiscard]] sf::Vector2f getVertexPos(
        const CCustomWallHandle cwHandle, const int vertexIndex);

    [[nodiscard]] bool isOverlappingPlayer(const CCustomWallHandle cwHandle);

    void cleanup();
    void clear();
    void draw(HexagonGame& hexagonGame);

    template <typename F>
    [[nodiscard]] bool anyCustomWall(F&& f)
    {
        for(CCustomWallHandle h = 0; h < (int)_customWalls.size(); ++h)
        {
            if(!_handleAvailable[h])
            {
                if(f(_customWalls[h]))
                {
                    return true;
                }
            }
        }

        return false;
    }
};

} // namespace hg
