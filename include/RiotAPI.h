///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#pragma once
#include <string>
#include "Json.h"

class SummonerV4 {
public:
    SummonerV4() = default;
    explicit SummonerV4(Json::Document& Data);
    static const char* URLPath_;
    std::string accountId;
    int profileIconId;
    long revisionDate;
    std::string name;
    std::string id;
    std::string puuid;
    long summonerLevel;
};

class RiotAPI {
public:
    enum Region {
        BR, EUN, EUW, JP, KR, LA, LA2, NA, OC, TR, RU
    };
    RiotAPI() = delete;
    explicit RiotAPI(std::string Key, Region region);
    bool HasErrors();
public: //SummonerV4
    SummonerV4 SummonerByAccountID(const std::string& ID);
    SummonerV4 SummonerByName(const std::string& Name);
    SummonerV4 SummonerByPUUID(const std::string& ID);
    SummonerV4 SummonerByID(const std::string& ID);
    SummonerV4 SummonerByToken();
private:
    void Request(const std::string& Option);
    Json::Document JsonParser_;
    std::string APILink_{};
    std::string APIKey_{};
};