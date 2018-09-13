//
// Created by forest on 27/08/18.
//

#ifndef TRADING_TRANSACTION_HPP
#define TRADING_TRANSACTION_HPP

#include <vector>
#include "transaction_detail.hpp"
namespace trading::rest {
    class Transaction_Detail;
    class Transaction {
        public:
            int id;
            std::string url;
            std::vector<Transaction_Detail> details;
            double earnings;
            double investment;
    };
}


#endif //TRADING_TRANSACTION_HPP
