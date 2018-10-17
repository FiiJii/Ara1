//
// Created by forest on 27/08/18.
//

#ifndef TRADING_TRANSACTION_DETAIL_HPP
#define TRADING_TRANSACTION_DETAIL_HPP

#include <string>
#include <api.hpp>


namespace  trading::rest {
    enum class Action{
        Buy,
        Sell
    };
    std::string action_name(Action action);


    class Transaction_Detail {
        public:
            int id;
            std::string url;
            Coins from;
            Coins to;
            double fee;
            double price;
            Action action;
            double amount;
            int okex_oder;
    };
}


#endif //TRADING_TRANSACTION_DETAIL_HPP
