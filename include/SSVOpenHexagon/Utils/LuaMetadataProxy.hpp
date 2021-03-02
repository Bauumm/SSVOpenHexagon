// Copyright (c) 2013-2020 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: https://opensource.org/licenses/AFL-3.0

#pragma once

#include "SSVOpenHexagon/Utils/ArgExtractor.hpp"
#include "SSVOpenHexagon/Utils/LuaMetadata.hpp"

#include <SSVUtils/Core/Log/Log.hpp>

#include <string>
#include <vector>
#include <utility>
#include <type_traits>

namespace hg::Utils
{

class LuaMetadataProxy
{
private:
    LuaMetadata& luaMetadata;
    std::string name;
    std::string (*erasedRet)(LuaMetadataProxy*);
    std::string (*erasedArgs)(LuaMetadataProxy*);
    std::string docs;
    std::vector<std::string> argNames;

    template <typename T>
    [[nodiscard]] constexpr static const char* typeToStr() noexcept
    {
        using Type = std::decay_t<T>;

        if constexpr(std::is_same_v<Type, void>)
        {
            return "void";
        }
        else if constexpr(std::is_same_v<Type, bool>)
        {
            return "bool";
        }
        else if constexpr(std::is_same_v<Type, int>)
        {
            return "int";
        }
        else if constexpr(std::is_same_v<Type, float>)
        {
            return "float";
        }
        else if constexpr(std::is_same_v<Type, double>)
        {
            return "double";
        }
        else if constexpr(std::is_same_v<Type, std::string>)
        {
            return "string";
        }
        else if constexpr(std::is_same_v<Type, unsigned int>)
        {
            return "unsigned int";
        }
        else if constexpr(std::is_same_v<Type, long>)
        {
            return "long";
        }
        else if constexpr(std::is_same_v<Type, unsigned long>)
        {
            return "unsigned long";
        }
        else if constexpr(std::is_same_v<Type, long long>)
        {
            return "long long";
        }
        else if constexpr(std::is_same_v<Type, unsigned long long>)
        {
            return "unsigned long long";
        }
        else if constexpr(std::is_same_v<Type, std::size_t>)
        {
            return "size_t";
        }
        else if constexpr(std::is_same_v<Type, std::tuple<float, float>>)
        {
            return "tuple<float, float>";
        }
        else
        {
            struct fail;
            return fail{};
        }
    }

    template <typename ArgT>
    static void addTypeToStr(std::vector<std::string>& types)
    {
        types.emplace_back(typeToStr<ArgT>());
    }

    template <typename F>
    [[nodiscard]] static std::string makeArgsString(LuaMetadataProxy* self)
    {
        using AE = Utils::ArgExtractor<decltype(&F::operator())>;

        std::vector<std::string> types;

        [&]<std::size_t... Is>(std::index_sequence<Is...>)
        {
            (addTypeToStr<typename AE::template NthArg<Is>>(types), ...);
        }
        (std::make_index_sequence<AE::numArgs>{});

        if(types.empty())
        {
            return "";
        }

        std::string res = types.at(0) + " " + self->argNames.at(0);

        if(types.size() == 1)
        {
            return res;
        }

        for(std::size_t i = 1; i < types.size(); ++i)
        {
            res += ", ";
            res += types.at(i);
            res += " ";
            res += self->argNames.at(i);
        }

        return res;
    }

    [[nodiscard]] std::string resolveArgNames(const std::string& docs)
    {
        std::size_t argNameSize = 0;
        for(const auto& argName : argNames)
        {
            argNameSize += argName.size() + 4;
        }

        std::string result;
        result.reserve(docs.size() + argNameSize);

        for(std::size_t i = 0; i < docs.size(); ++i)
        {
            if(docs[i] != '$')
            {
                result += docs[i];
                continue;
            }

            const std::size_t index = docs.at(i + 1) - '0';
            result += argNames.at(index);
            ++i;
        }

        return result;
    }

public:
    template <typename F>
    explicit LuaMetadataProxy(
        F&&, LuaMetadata& mLuaMetadata, const std::string& mName)
        : luaMetadata{mLuaMetadata}, name{mName},
          erasedRet{[](LuaMetadataProxy*) -> std::string {
              using AE =
                  Utils::ArgExtractor<decltype(&std::decay_t<F>::operator())>;

              return typeToStr<typename AE::Return>();
          }},
          erasedArgs{[](LuaMetadataProxy* self) {
              return makeArgsString<std::decay_t<F>>(self);
          }}
    {
    }

    ~LuaMetadataProxy()
    {
        try
        {
            luaMetadata.addFnEntry((*erasedRet)(this), name,
                (*erasedArgs)(this), resolveArgNames(docs));
        }
        catch(const std::exception& e)
        {
            ssvu::lo("LuaMetadataProxy")
                << "Failed to generate documentation for " << name << ": "
                << e.what() << '\n';
        }
        catch(...)
        {
            ssvu::lo("LuaMetadataProxy")
                << "Failed to generate documentation for " << name << '\n';
        }
    }

    LuaMetadataProxy& arg(const std::string& mArgName)
    {
        argNames.emplace_back(mArgName);
        return *this;
    }

    LuaMetadataProxy& doc(const std::string& mDocs)
    {
        docs = mDocs;
        return *this;
    }
};

} // namespace hg::Utils
