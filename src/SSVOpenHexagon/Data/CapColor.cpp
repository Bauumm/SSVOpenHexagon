// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVOpenHexagon/Data/CapColor.hpp"

#include "SSVOpenHexagon/SSVUtilsJson/SSVUtilsJson.hpp"

#include <string>

namespace hg
{

[[nodiscard]] CapColor parseCapColor(const ssvuj::Obj& obj) noexcept
{
    if(ssvuj::isObjType<std::string>(obj))
    {
        const std::string str = ssvuj::getExtr<std::string>(obj);

        if(str == "main")
        {
            return CapColorMode::Main{};
        }

        if(str == "main_darkened")
        {
            return CapColorMode::MainDarkened{};
        }
    }

    if(ssvuj::isObj(obj))
    {
        const bool legacy = ssvuj::getExtr<bool>(obj, "legacy", true);
        if(legacy)
        {
            const int index = ssvuj::getExtr<int>(obj, "index", 0);
            return CapColorMode::ByIndex{index};
        }
        else
        {
            return ColorData{false, ssvuj::getExtr<bool>(obj, "dynamic", false),
                ssvuj::getExtr<bool>(obj, "dynamic_offset", false),
                ssvuj::getExtr<float>(obj, "dynamic_darkness", 1.f),
                ssvuj::getExtr<float>(obj, "hue_shift", 0.f),
                ssvuj::getExtr<float>(obj, "offset", 0.f),
                ssvuj::getExtr<sf::Color>(obj, "value", sf::Color::White),
                ssvuj::getExtr<sf::Color>(obj, "pulse", sf::Color::White)};
        }
    }

    // Fallback case:
    return CapColorMode::ByIndex{0};
}

} // namespace hg
