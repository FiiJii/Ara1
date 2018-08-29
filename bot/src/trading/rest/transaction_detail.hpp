//
// Created by forest on 27/08/18.
//

#ifndef TRADING_TRANSACTION_DETAIL_HPP
#define TRADING_TRANSACTION_DETAIL_HPP

#include <string>
#include "transaction.hpp"

namespace  trading::rest {
    class transaction_detail {
        public:
            int id;
            Transaction transaction;
            std::string parity;
            double fee;
            double price;
            double amount;
    };
}


#endif //TRADING_TRANSACTION_DETAIL_HPP
