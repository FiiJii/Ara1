//
// Created by forest on 29/08/18.
//

#ifndef TRADING_PATH_NODE_HPP
#define TRADING_PATH_NODE_HPP

#include "coins.hpp"

namespace trading {
    class path_node {
        public:
            Coins coin;
            double amount;
            double price;
            double fees;
    };
}


#endif //TRADING_PATH_NODE_HPP
