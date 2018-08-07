#ifndef _OKEX_COINS_HPP_
#define _OKEX_COINS_HPP_
#include <string>
#include <map>
namespace trading{
     enum Coins{
        btc,
        ltc,
        usd,
        eth  
    };

    std::string coin_name(Coins coin);
    Coins coin_value(std::string name);
}
#endif
