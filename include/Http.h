///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#pragma once
#include <string>

class HTTP {
public:
    static std::string Post(const std::string& IP, const std::string& Fields);
    static std::string Get(const std::string &IP, const std::string& Auth = "");
};
