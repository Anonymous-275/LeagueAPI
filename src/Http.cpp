///
/// Created by Anonymous275 on 2/2/22
/// Copyright (c) 2021-present Anonymous275 read the LICENSE file for more info.
///

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <cpp-httplib/httplib.h>
#include "Logger.h"
#include "Http.h"
#include <mutex>


std::string HTTP::Get(const std::string &IP, const std::string& Auth) {
    static std::mutex Lock;
    std::scoped_lock Guard(Lock);

    auto pos = IP.find('/',10);

    httplib::Client cli(IP.substr(0, pos));
    cli.set_connection_timeout(std::chrono::seconds(5));

    if(!Auth.empty()) {
        cli.set_basic_auth("riot", Auth.c_str());
        cli.enable_server_certificate_verification(false);
    }

    auto res = cli.Get(IP.substr(pos).c_str());

    std::string Ret;
    if(res.error() == httplib::Error::Success){
        Ret = res->body;
        if(res->status != 200) {
            LOG(ERROR) << res->status << " -> " << res->reason;
        }
    } else {
        LOG(ERROR) << "HTTP Get failed on " << httplib::to_string(res.error());
    }

    return Ret;
}

std::string HTTP::Post(const std::string& IP, const std::string& Fields) {
    static std::mutex Lock;
    std::scoped_lock Guard(Lock);

    auto pos = IP.find('/',10);

    httplib::Client cli(IP.substr(0, pos));
    cli.set_connection_timeout(std::chrono::seconds(5));
    std::string Ret;

    if(!Fields.empty()) {
        httplib::Result res = cli.Post(IP.substr(pos).c_str(), Fields, "application/json");

        if(res.error() == httplib::Error::Success) {
            if(res->status != 200) {
                LOG(ERROR) << res->reason;
            }
            Ret = res->body;
        } else{
            LOG(ERROR) << "HTTP Post failed on " << httplib::to_string(res.error());
        }
    } else {
        httplib::Result res = cli.Post(IP.substr(pos).c_str());
        if(res.error() == httplib::Error::Success) {
            if (res->status != 200) {
                LOG(ERROR) << res->reason;
            }
            Ret = res->body;
        } else {
            LOG(ERROR) << "HTTP Post failed on " << httplib::to_string(res.error());
        }
    }
    return Ret;
}

