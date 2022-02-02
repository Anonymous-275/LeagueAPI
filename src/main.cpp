///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#include "RiotAPI.h"
#include "Logger.h"

int main() {
    RiotAPI API("RGAPI-18b2f4ff-7501-46fa-9d13-3f1adba229ae", RiotAPI::EUW);
    auto Summoner = API.SummonerByName("Sam390");
    LOG(INFO) << Summoner.name << " level " << Summoner.summonerLevel;
    return 0;
}

