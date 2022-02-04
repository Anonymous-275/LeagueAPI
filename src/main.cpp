///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#include "RiotAPI.h"
#include "Logger.h"

int main() {
    RiotAPI API("RGAPI-af2ded68-2272-460e-a0cb-678dda32ee33", RiotAPI::EUW);
    auto Summoner = API.SummonerByName("Anonymous275");
    LOG(INFO) << Summoner.name << " level " << Summoner.summonerLevel;
    LOG(INFO) << API.GetChampNamesByIDs({99})[0];
    //auto Match = API.GetSummonerActiveGame(Summoner);
    /*for(const auto& entry : Match.participants) {
        LOG(INFO) << entry.summonerName << " : champID " << entry.championId << " : team " << entry.teamId;
    }*/
    return 0;
}

