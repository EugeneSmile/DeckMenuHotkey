#include "StringHelper.h"

#include <sstream>
#include <algorithm>

std::unordered_map<std::string, std::string> getPairs(const std::string &line)
{
    std::stringstream ss{line};
    std::string substring;
    std::unordered_map<std::string, std::string> return_map;
    while (std::getline(ss, substring, ' '))
    {
        auto pair = getPair(substring);
        if (pair.has_value())
            return_map.emplace(pair.value());
    }
    return return_map;
}

std::optional<std::pair<std::string, std::string>> getPair(const std::string &pair)
{
    std::pair<std::string, std::string> return_pair;
    auto it = pair.find('=');
    if (it != std::string::npos)
    {
        return_pair.first = pair.substr(0, it);
        return_pair.second = pair.substr(it + 1);
        return return_pair;
    }
    else
    {
        return {};
    }
}
