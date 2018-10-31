#ifndef _OKEX_API_HPP_
#define _OKEX_API_HPP_
#include <string_view>
#include "coins.hpp"
#include "../trading/ticker.hpp"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/WebSocket.h"
#include <functional>
#include <memory>
#include <map>
#include "coins.hpp"
namespace okex{
    class Api{
        public:
           Api();
           //Get the ticker for the symbol from_to
           //@param [in] from the coin used to pay
           //@param [in] to the buyed coin
           void register_for_ticker(trading::Coins from,trading::Coins to);
           void ping();
           void listen(std::function<void(trading::Ticker)> callback);
           trading::Symbol  get_supported_symbol_for(trading::Symbol symbol);
        private:
           std::unique_ptr<Poco::Net::WebSocket> socket;
           std::map<std::string,bool> symbols;
           void connect();

    };
}
#endif
