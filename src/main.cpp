///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#include "RiotAPI.h"
#include "Logger.h"

int main() {
    RiotAPI API("RGAPI-096f7db7-f83b-4913-8f87-d8a2f8f8c513", RiotAPI::EUW);
    auto Summoner = API.SummonerByName("Anonymous275");
    LOG(INFO) << Summoner.name << " level " << Summoner.summonerLevel;

    LOG(INFO) << API.GetLocalSummonerName();

    return 0;
}

