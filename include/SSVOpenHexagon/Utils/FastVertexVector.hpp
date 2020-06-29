// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <SSVUtils/Core/Assert/Assert.hpp>
#include <SSVUtils/Core/Common/LikelyUnlikely.hpp>

#include <cstddef>
#include <cstring>
#include <memory>

namespace hg::Utils
{

// TODO: move
template <sf::PrimitiveType TPrimitive>
struct FastVertexVector : public sf::Drawable
{
private:
    std::unique_ptr<sf::Vertex[]> _data{};
    std::size_t _size{};
    std::size_t _capacity{};

public:
    [[gnu::always_inline]] void reserve_more(const std::size_t n)
    {
        reserve(_size + n);
    }

    void reserve(const std::size_t n)
    {
        if(SSVU_LIKELY(_capacity >= n))
        {
            return;
        }

        auto new_data = std::make_unique<sf::Vertex[]>(n);
        std::memcpy(new_data.get(), _data.get(), sizeof(sf::Vertex) * _size);
        _data = std::move(new_data);

        _capacity = n;
    }

    [[gnu::always_inline]] void unsafe_emplace_other(
        const FastVertexVector& rhs) noexcept
    {
        SSVU_ASSERT(_size + rhs._size <= _capacity);
        std::memcpy(_data.get() + _size, rhs._data.get(),
            sizeof(sf::Vertex) * rhs._size);
        _size += rhs._size;
    }

    [[gnu::always_inline]] void clear() noexcept
    {
        _size = 0;
    }

    [[gnu::always_inline, nodiscard]] std::size_t size() const noexcept
    {
        return _size;
    }

    template <typename... Ts>
    [[gnu::always_inline]] void unsafe_emplace_back(Ts&&... xs)
    {
        SSVU_ASSERT(_size <= _capacity);
        new(&_data[_size++]) sf::Vertex{std::forward<Ts>(xs)...};
    }

    template <typename... Ts>
    [[gnu::always_inline]] void batch_unsafe_emplace_back(
        const sf::Color& color, Ts&&... positions)
    {
        SSVU_ASSERT(_size + sizeof...(positions) <= _capacity);
        ((new(&_data[_size++]) sf::Vertex{positions, color}), ...);
    }

    void draw(sf::RenderTarget& mRenderTarget,
        sf::RenderStates mRenderStates) const override
    {
        mRenderTarget.draw(_data.get(), _size, TPrimitive, mRenderStates);
    }

    [[gnu::always_inline, nodiscard]] sf::Vertex& operator[](
        const std::size_t i) noexcept
    {
        SSVU_ASSERT(i < _size);
        return _data[i];
    }

    [[gnu::always_inline, nodiscard]] const sf::Vertex& operator[](
        const std::size_t i) const noexcept
    {
        SSVU_ASSERT(i < _size);
        return _data[i];
    }
};

} // namespace hg::Utils
