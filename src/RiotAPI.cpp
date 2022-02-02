///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#include "RiotAPI.h"
#include "Logger.h"
#include <iostream>
#include <utility>
#include "Http.h"

RiotAPI::RiotAPI(std::string Key, Region region) : APIKey_(std::move(Key)) {
    Log::Init();
    APILink_ = "https://";
    switch (region) {
        case BR:
            APILink_ += "br1";
            break;
        case EUN:
            APILink_ += "eun1";
            break;
        case EUW:
            APILink_ += "euw1";
            break;
        case JP:
            APILink_ += "jp1";
            break;
        case KR:
            APILink_ += "kr1";
            break;
        case LA:
            APILink_ += "la1";
            break;
        case LA2:
            APILink_ += "la2";
            break;
        case NA:
            APILink_ += "na1";
            break;
        case OC:
            APILink_ += "oc1";
            break;
        case TR:
            APILink_ += "tr1";
            break;
        case RU:
            APILink_ += "ru1";
            break;
        default:
            LOG(FATAL) << "Not a valid region!";
            return;
    }
    APILink_ += ".api.riotgames.com";
}

void RiotAPI::Request(const std::string& Option) {
    JsonParser_.SetObject();
    JsonParser_.Parse(HTTP::Get(Option + "?api_key=" + APIKey_).c_str());
}

bool RiotAPI::HasErrors() {
    if(JsonParser_.HasParseError() || !JsonParser_["status"].IsNull()) {
        LOG(ERROR) << JsonParser_["status"]["message"].GetString();
        return true;
    }
    return false;
}

SummonerV4 RiotAPI::SummonerByAccountID(const std::string &ID) {
    Request(APILink_ + SummonerV4::URLPath_ + "by-account/" + ID);
    if(!HasErrors())
        return SummonerV4(JsonParser_);
    return {};
}

SummonerV4 RiotAPI::SummonerByName(const std::string &Name) {
    Request(APILink_ + SummonerV4::URLPath_ + "by-name/" + Name);
    if(!HasErrors())
        return SummonerV4(JsonParser_);
    return {};
}

SummonerV4 RiotAPI::SummonerByPUUID(const std::string &ID) {
    Request(APILink_ + SummonerV4::URLPath_ + "by-puuid/" + ID);
    if(!HasErrors())
        return SummonerV4(JsonParser_);
    return {};
}

SummonerV4 RiotAPI::SummonerByID(const std::string &ID) {
    Request(APILink_ + SummonerV4::URLPath_ + ID);
    if(!HasErrors())
        return SummonerV4(JsonParser_);
    return {};
}

SummonerV4 RiotAPI::SummonerByToken() {
    Request(APILink_ + SummonerV4::URLPath_ + std::string("me"));
    if(!HasErrors())
        return SummonerV4(JsonParser_);
    return {};
}

SummonerV4::SummonerV4(Json::Document& Data) {
    accountId = Data["accountId"].GetString();
    profileIconId = Data["profileIconId"].GetInt();
    revisionDate = Data["revisionDate"].GetInt();
    name = Data["name"].GetString();
    id = Data["id"].GetString();
    puuid = Data["puuid"].GetString();
    summonerLevel = Data["summonerLevel"].GetInt();
}

const char* SummonerV4::URLPath_ = "/lol/summoner/v4/summoners/";