#include <iostream>
#include <string>
#include "api.hpp"
#include "coins.hpp"
#include <chrono>
int main(){
    okex::Api api;

    try{
        auto start_time=std::chrono::high_resolution_clock::now();
        api.listen_for_ticker(okex::Coins::ltc,okex::Coins::btc);
        auto end_time=std::chrono::high_resolution_clock::now();
        std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count()<<std::endl;
    }catch(std::string e){
        std::cout<<e<<std::endl;
    }
    return 0;
}
