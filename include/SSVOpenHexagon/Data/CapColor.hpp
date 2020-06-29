// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#pragma once

#include "SSVOpenHexagon/Global/Common.hpp"
#include "SSVOpenHexagon/Data/ColorData.hpp"
#include "SSVOpenHexagon/SSVUtilsJson/SSVUtilsJson.hpp"

#include <variant>

namespace hg
{

namespace CapColorMode
{

// clang-format off
struct Main         { };
struct MainDarkened { };
struct ByIndex      { int index; };
// clang-format on

} // namespace CapColorMode

using CapColor = std::variant<  //
    CapColorMode::Main,         //
    CapColorMode::MainDarkened, //
    CapColorMode::ByIndex,      //
    ColorData                   //
    >;

[[nodiscard]] CapColor parseCapColor(const ssvuj::Obj& obj) noexcept;

} // namespace hg
