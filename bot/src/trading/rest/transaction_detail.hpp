//
// Created by forest on 27/08/18.
//

#ifndef TRADING_TRANSACTION_DETAIL_HPP
#define TRADING_TRANSACTION_DETAIL_HPP

#include <string>
#include <api.hpp>
#include "transaction.hpp"

namespace  trading::rest {
    class transaction_detail {
        public:
            int id;
            Transaction transaction;
            Coins from;
            Coins to;
            double fee;
            double price;
            double amount;
            int okex_oder;
    };
}


#endif //TRADING_TRANSACTION_DETAIL_HPP
