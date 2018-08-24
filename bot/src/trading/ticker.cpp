#include "ticker.hpp"
#include "nlohmann/json.hpp"
#include <string>
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
            ticker.buy =std::stod(d.at("buy").get<std::string>());
            ticker.high =std::stod(d.at("high").get<std::string>());
            ticker.last =std::stod(d.at("last").get<std::string>());
            ticker.low =std::stod(d.at("low").get<std::string>());
            ticker.sell =std::stod(d.at("sell").get<std::string>());
            ticker.vol =std::stod(d.at("vol").get<std::string>());

    }

}
