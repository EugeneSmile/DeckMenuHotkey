#ifndef STRINGHELPER_H_GUARD
#define STRINGHELPER_H_GUARD

#include <unordered_map>
#include <string>
#include <optional>

std::unordered_map<std::string, std::string> getPairs(const std::string &line);
std::optional<std::pair<std::string, std::string>> getPair(const std::string &pair);
bool convertToBool(const std::string &str);

#endif