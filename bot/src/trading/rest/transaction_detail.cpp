//
// Created by forest on 27/08/18.
//

#include "transaction_detail.hpp"

std::string trading::rest::action_name(trading::rest::Action action) {
    switch (action){
        case Action::Buy: return "buy";
        case Action::Sell: return "sell";
        default:
            return "none";
    }
}
