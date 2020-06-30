// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#pragma once

#include "SSVOpenHexagon/Components/CCustomWallHandle.hpp"

#include <SSVUtils/Core/Common/Frametime.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <array>

namespace hg
{

class HexagonGame;

class CCustomWall
{
public:
    using Handle = int;

private:
    std::array<sf::Vector2f, 4> vertexPositions;
    std::array<sf::Color, 4> vertexColors;

public:
    CCustomWall();

    void update(HexagonGame& mHexagonGame, ssvu::FT mFT);
    void draw(HexagonGame& mHexagonGame);

    [[nodiscard]] bool isOverlapping(const sf::Vector2f& mPoint) const noexcept;

    void setVertexPos(const int vertexIndex, const sf::Vector2f& pos) noexcept;
    void setVertexColor(const int vertexIndex, const sf::Color& color) noexcept;

    [[nodiscard]] sf::Vector2f getVertexPos(
        const int vertexIndex) const noexcept;
};

} // namespace hg
