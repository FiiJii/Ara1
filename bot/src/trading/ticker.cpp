#include "ticker.hpp"
#include "nlohmann/json.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include "utils.hpp"
namespace trading{
    void to_json(nlohmann::json& j, const Ticker& ticker) {
        j = nlohmann::json{
            {"from",coin_name(ticker.from)},
            {"to",coin_name(ticker.to)},
            {"buy", ticker.buy},
            {"high", ticker.high},
            {"last", ticker.last},
            {"low", ticker.low},
            {"sell", ticker.sell},
        };
    }

    void from_json(const nlohmann::json& j, Ticker& ticker) {    
            auto d=j["data"];
            std::string symbol=j["channel"];
            symbol.erase(0,12);
            symbol.erase(symbol.length()-7,7);
            auto separator_pos=symbol.find('_');
            ticker.from=coin_value(symbol.substr(separator_pos+1,symbol.length()-separator_pos));
            ticker.to=coin_value(symbol.substr(0,separator_pos));
            ticker.buy =s2d(d.at("buy").get<std::string>());
            ticker.high =s2d(d.at("high").get<std::string>());
            std::cout<<"***"<<d.at("last").get<std::string>()<<"***\n";
            std::cout<<"***"<<s2d(d.at("last").get<std::string>())<<"***\n";
            ticker.last =s2d(d.at("last").get<std::string>());
            ticker.low =s2d(d.at("low").get<std::string>());
            ticker.sell =s2d(d.at("sell").get<std::string>());
            ticker.vol =s2d(d.at("vol").get<std::string>());

    }

}
