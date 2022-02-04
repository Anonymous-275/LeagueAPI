///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#include "RiotAPI.h"
#include "Logger.h"
#include <iostream>
#include <utility>
#include "Http.h"
#include <psapi.h>
#include <tlhelp32.h>
#include <filesystem>

namespace fs = std::filesystem;

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
    JsonParser_.Parse(HTTP::Get(Option + "?api_key=" + APIKey_).c_str());
}

void RiotAPI::RequestNoKey(const std::string& Option) {
    JsonParser_.Parse(HTTP::Get(Option).c_str());
}

bool RiotAPI::HasErrors() {
    if((!JsonParser_.IsObject() && !JsonParser_.IsArray()) || JsonParser_.HasParseError())return true;
    if(JsonParser_.IsObject() && JsonParser_.HasMember("status")) {
        LOG(ERROR) << "API Response -> " << JsonParser_["status"]["message"].GetString();
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

SpectatorV4 RiotAPI::GetSummonerActiveGame(const SummonerV4& Summoner) {
    Request(APILink_ + SpectatorV4::URLPath_ + "active-games/by-summoner/" + Summoner.id);
    if(!HasErrors())
        return SpectatorV4(JsonParser_);
    return {};
}

std::vector<std::string> RiotAPI::GetChampNamesByIDs(const std::vector<int64_t>& IDs) {
    RequestNoKey("https://ddragon.leagueoflegends.com/api/versions.json");
    if(HasErrors())return {};
    std::string Ver = JsonParser_.GetArray()[0].GetString();
    RequestNoKey("https://ddragon.leagueoflegends.com/cdn/"+Ver+"/data/en_US/champion.json");
    if(HasErrors())return {};
    std::vector<std::string> Names;
    const auto& Champions = JsonParser_["data"].GetObj();
    for (const auto& entry : Champions) {
        for(int64_t ID : IDs) {
            if (entry.value["key"].GetString() == std::to_string(ID)) {
                Names.emplace_back(entry.name.GetString());
            }
        }
    }
    return Names;
}

uint32_t GetPID() {
    SetLastError(0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(Process32First(Snapshot, &pe32)) {
        do{
            if(std::string("LeagueClient.exe") == pe32.szExeFile)break;
        }while(Process32Next(Snapshot, &pe32));
    }
    if(Snapshot != INVALID_HANDLE_VALUE) {
        CloseHandle(Snapshot);
    }
    if(GetLastError() != 0)return 0;
    return pe32.th32ProcessID;
}

std::string RiotAPI::GetLocalSummonerName() {
    uint32_t PID = GetPID();
    if(PID == 0)return "";
    auto Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetPID());
    std::string Path(MAX_PATH, 0);
    GetModuleFileNameExA(Handle, nullptr, &Path[0], MAX_PATH);
    CloseHandle(Handle);
    Path = fs::path(Path).parent_path().string() + "\\lockfile";
    if(!fs::exists(Path))return "";
    std::ifstream Key(Path);
    if(!Key.is_open())return "";
    auto Size = fs::file_size(Path);
    std::string Buffer(Size, 0);
    Key.read(&Buffer[0], std::streamsize(Size));
    Key.close();
    Buffer = Buffer.substr(Buffer.find(':')+1);
    Buffer = Buffer.substr(Buffer.find(':')+1);
    std::string Port = Buffer.substr(0, Buffer.find(':'));
    Buffer = Buffer.substr(Buffer.find(':')+1);
    std::string Pass = Buffer.substr(0, Buffer.find(':'));
    JsonParser_.Parse(HTTP::Get("https://127.0.0.1:"+Port+"/lol-summoner/v1/current-summoner", Pass).c_str());
    if(HasErrors() || !JsonParser_.HasMember("displayName"))return "";
    return JsonParser_["displayName"].GetString();
}


SummonerV4::SummonerV4(Json::Document& Data) {
    accountId = Data["accountId"].GetString();
    profileIconId = Data["profileIconId"].GetInt();
    revisionDate = Data["revisionDate"].GetInt64();
    name = Data["name"].GetString();
    id = Data["id"].GetString();
    puuid = Data["puuid"].GetString();
    summonerLevel = Data["summonerLevel"].GetInt64();
}

const char* SummonerV4::URLPath_ = "/lol/summoner/v4/summoners/";

SpectatorV4::SpectatorV4(rapidjson::Document& Data) {
    gameId = Data["gameId"].GetInt64();
    gameType = Data["gameType"].GetString();
    gameStartTime = Data["gameStartTime"].GetInt64();
    mapId = Data["mapId"].GetInt64();
    gameLength = Data["gameLength"].GetInt64();
    platformId = Data["platformId"].GetString();
    gameMode = Data["gameMode"].GetString();

    const auto& BannedArray = Data["bannedChampions"].GetArray();
    for(const auto& entry : BannedArray) {
        const auto& Obj = entry.GetObj();
        bannedChampions.emplace_back(Champions{Obj["pickTurn"].GetInt(), Obj["championId"].GetInt64(), Obj["teamId"].GetInt64()});
    }
    if(Data.HasMember("gameQueueConfigId")) {
        gameQueueConfigId = Data["gameQueueConfigId"].GetInt64();
    }
    ObserverEncryptionKey = Data["observers"]["encryptionKey"].GetString();

    const auto& SummonerArray = Data["participants"].GetArray();
    for(const auto& entry : SummonerArray) {
        const auto& Obj = entry.GetObj();
        Perk perks;
        const auto& perk = Obj["perks"].GetObj();
        std::vector<int64_t> perkIds;
        for(const auto& Id : perk["perkIds"].GetArray()) {
            perkIds.emplace_back(Id.GetInt64());
        }
        perks = Perk{perkIds, perk["perkStyle"].GetInt64(), perk["perkSubStyle"].GetInt64()};

        std::vector<GameCustomizationObject> Objects;
        for(const auto& CustomObjects : Obj["gameCustomizationObjects"].GetArray()) {
            const auto& CObj = entry.GetObj();
            Objects.emplace_back(GameCustomizationObject{CObj["category"].GetString(), CObj["content"].GetString()});
        }

        participants.emplace_back(GameParticipant{
            Obj["championId"].GetInt64(),
            perks,
            Obj["profileIconId"].GetInt64(),
            Obj["bot"].GetBool(),
            Obj["teamId"].GetInt64(),
            Obj["summonerName"].GetString(),
            Obj["summonerId"].GetString(),
            Obj["spell1Id"].GetInt64(),
            Obj["spell2Id"].GetInt64(),
            Objects
        });
    }
}


const char* SpectatorV4::URLPath_ = "/lol/spectator/v4/";
