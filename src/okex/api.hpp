#ifndef _OKEX_API_HPP_
#define _OKEX_API_HPP_
#include <string_view>
#include "coins.hpp"
#include "ticker.hpp"
#include "Poco/Net/HTTPSClientSession.h"
namespace okex{
    class Api{
        public:
            Api();
           Ticker get_ticker(Coins from,Coins to);
        private:
           Poco::Net::HTTPSClientSession session;
    };
}
#endif
