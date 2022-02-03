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
    long revisionDate;
    std::string name;
    std::string id;
    std::string puuid;
    long summonerLevel;
};

class SpectatorV4 {
public:
    struct Champions {
        int pickTurn;
        long championId;
        long teamId;
    };
    struct Perk {
        std::vector<long> perkIds;
        long perkStyle;
        long perkSubStyle;
    };
    struct GameCustomizationObject {
        std::string category;
        std::string content;
    };
    struct GameParticipant {
        long championId;
        Perk perks;
        long profileIconId;
        bool bot;
        long teamId;
        std::string summonerName;
        std::string summonerId;
        long spell1Id;
        long spell2Id;
        std::vector<GameCustomizationObject> gameCustomizationObjects;
    };
public:
    SpectatorV4() = default;
    explicit SpectatorV4(Json::Document& Data);
    static const char* URLPath_;
    long gameId;
    std::string gameType;
    long gameStartTime;
    long mapId;
    long gameLength;
    std::string platformId;
    std::string gameMode;
    std::vector<Champions> bannedChampions;
    long gameQueueConfigId;
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