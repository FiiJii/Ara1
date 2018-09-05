//
// Created by forest on 27/08/18.
//

#ifndef TRADING_TRANSACTION_DETAIL_HPP
#define TRADING_TRANSACTION_DETAIL_HPP

#include <string>
#include <api.hpp>


namespace  trading::rest {
    class Transaction_Detail {
        public:
            int id;
            std::string url;
            Coins from;
            Coins to;
            double fee;
            double price;
            double amount;
            int okex_oder;
    };
}


#endif //TRADING_TRANSACTION_DETAIL_HPP
