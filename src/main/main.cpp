#include <iostream>
#include <string>
#include "api.hpp"
#include "coins.hpp"
#include <chrono>
int main(){
    okex::Api api;

    try{
        auto start_time=std::chrono::high_resolution_clock::now();
        auto ticker=api.get_ticker(okex::Coins::ltc,okex::Coins::btc);
        ticker=api.get_ticker(okex::Coins::ltc,okex::Coins::btc);
        ticker=api.get_ticker(okex::Coins::ltc,okex::Coins::usd);
        ticker=api.get_ticker(okex::Coins::btc,okex::Coins::usd);
        ticker=api.get_ticker(okex::Coins::eth,okex::Coins::usd);
        ticker=api.get_ticker(okex::Coins::eth,okex::Coins::usd);
        auto end_time=std::chrono::high_resolution_clock::now();
        std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count()<<std::endl;
    }catch(std::string e){
        std::cout<<e<<std::endl;
    }
    return 0;
}
