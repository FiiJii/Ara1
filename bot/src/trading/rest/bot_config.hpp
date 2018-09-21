//
// Created by forest on 20/09/18.
//

#ifndef TRADING_BOT_CONFIG_HPP
#define TRADING_BOT_CONFIG_HPP


#include <string>

class Bot_Config {
public:
    long id;
    std::string url;
    bool bot_status;
    long time_interval;
    double max_lost;
};


#endif //TRADING_BOT_CONFIG_HPP
