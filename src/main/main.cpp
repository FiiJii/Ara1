#include <iostream>
#include <string>
#include "api.hpp"
#include "coins.hpp"
#include <chrono>
#include "nlohmann/json.hpp"
void ticker_callback(okex::Ticker ticker){
    nlohmann::json json=ticker;
    std::cout<<"received ticker:"<<json<<std::endl;
}
int main(){
    okex::Api api;

    try{
        auto start_time=std::chrono::high_resolution_clock::now();
        api.register_for_ticker(okex::Coins::ltc,okex::Coins::btc);
        api.register_for_ticker(okex::Coins::eth,okex::Coins::btc);
        api.listen(ticker_callback);        
        auto end_time=std::chrono::high_resolution_clock::now();

        std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count()<<std::endl;
    }catch(std::string e){
        std::cout<<e<<std::endl;
    }
    return 0;
}


