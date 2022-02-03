///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#pragma once
#include <vector>
#include <string>
#include "Json.h"

class SummonerV4 {
public:
    SummonerV4() = default;
    explicit SummonerV4(Json::Document& Data);
    static const char* URLPath_;
    std::string accountId;
    int profileIconId;
    int64_t revisionDate;
    std::string name;
    std::string id;
    std::string puuid;
    int64_t summonerLevel;
};

class SpectatorV4 {
public:
    struct Champions {
        int pickTurn;
        int64_t championId;
        int64_t teamId;
    };
    struct Perk {
        std::vector<int64_t> perkIds;
        int64_t perkStyle;
        int64_t perkSubStyle;
    };
    struct GameCustomizationObject {
        std::string category;
        std::string content;
    };
    struct GameParticipant {
        int64_t championId;
        Perk perks;
        int64_t profileIconId;
        bool bot;
        int64_t teamId;
        std::string summonerName;
        std::string summonerId;
        int64_t spell1Id;
        int64_t spell2Id;
        std::vector<GameCustomizationObject> gameCustomizationObjects;
    };
public:
    SpectatorV4() = default;
    explicit SpectatorV4(Json::Document& Data);
    static const char* URLPath_;
    int64_t gameId;
    std::string gameType;
    int64_t gameStartTime;
    int64_t mapId;
    int64_t gameLength;
    std::string platformId;
    std::string gameMode;
    std::vector<Champions> bannedChampions;
    int64_t gameQueueConfigId;
    std::string ObserverEncryptionKey;
    std::vector<GameParticipant> participants;
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
public: //SpectatorV4
    SpectatorV4 GetSummonerActiveGame(const SummonerV4& Summoner);
private:
    void Request(const std::string& Option);
    Json::Document JsonParser_;
    std::string APILink_{};
    std::string APIKey_{};
};