#ifndef _OKEX_COINS_HPP_
#define _OKEX_COINS_HPP_
#include <string>
#include <map>
namespace okex{
     enum Coins{
        btc,
        ltc,
        usd,
        eth  
    };

    std::string coin_name(Coins coin);
}
#endif
