#ifndef _OKEX_API_HPP_
#define _OKEX_API_HPP_
#include <string_view>
#include "coins.hpp"
#include "ticker.hpp"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/WebSocket.h"
namespace okex{
    class Api{
        public:
           Api();
           //Get the ticker for the symbol from_to
           //@param [in] from the coin used to pay
           //@param [in] to the buyed coin
           Ticker get_ticker(Coins from,Coins to);
        private:
           Poco::Net::HTTPSClientSession session;

    };
}
#endif
