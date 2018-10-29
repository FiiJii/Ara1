//
// Created by forest on 20/09/18.
//

#ifndef TRADING_BOT_CONFIG_HPP
#define TRADING_BOT_CONFIG_HPP


#include <string>
#include <vector>
#include "coins.hpp"
class Bot_Config {
public:
    long id;
    std::string url;
    bool bot_status;
    long time_interval;
    double max_lost;
    std::vector<std::string> coins;
};


#endif //TRADING_BOT_CONFIG_HPP
