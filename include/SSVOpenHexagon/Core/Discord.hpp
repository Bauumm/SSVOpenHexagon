// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#pragma once

#include "SSVOpenHexagon/Global/Common.hpp"

#include "discord/discord.h"

#include <string_view>

namespace hg::Discord
{

class discord_manager
{
private:
    discord::Core* _core;
    bool _initialized;

public:
    discord_manager();
    ~discord_manager();

    discord_manager(const discord_manager&) = delete;
    discord_manager& operator=(const discord_manager&) = delete;

    discord_manager(discord_manager&&) = delete;
    discord_manager& operator=(discord_manager&&) = delete;

    bool run_callbacks();

    bool set_rich_presence_in_menu();
    bool set_rich_presence_in_game(std::string_view level_name, float time);
};

} // namespace hg::Discord
