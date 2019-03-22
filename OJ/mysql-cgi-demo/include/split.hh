#pragma once

#include <vector>
#include <string>


// 按 delimiter 分割
std::vector<std::string> split(std::string const& s, std::string const& delimiter);

// 按 " " 分割
std::vector<std::string> split(std::string const& s);
