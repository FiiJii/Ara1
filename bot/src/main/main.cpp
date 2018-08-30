#include <algorithm>
#include "api.hpp"
#include <chrono>
#include "coins.hpp"
#include "Graph.hpp"
#include <iostream>
#include "nlohmann/json.hpp"
#include <memory>
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <rest/api.hpp>
#include <path_node.hpp>
#include <rest/transaction.hpp>

void ticker_callback(trading::Ticker ticker) {
    nlohmann::json json = ticker;

}
void register_db(std::vector<trading::path_node> path){
    using namespace trading::rest;
    api api_rest("localhost",8000);
    auto err=api_rest.login("trading_bot","trading");
    if (err) std::cout<<"login error:"<<err->code<<";"<<err->message<<"\n";
    Transaction transaction;
    transaction.details.reserve(path.size());
    for(auto idx=0;idx<path.size()-1;idx++){
        Transaction_Detail detail={};
        auto& current_node=path[idx];
        auto& next_node=path[idx+1];
        transaction.details.push_back(
                {.id=0,
                 .url="",
                 .from=current_node.coin,
                 .to=next_node.coin,
                 .fee=0,
                 .price=next_node.price,
                 .amount=next_node.price
                });
    }

    auto r=api_rest.register_transaction(transaction);
    if(r.failed())
        std::cout<<"***Error"<<r.get_error()->code<<r.get_error()->message<<"\n";
}
int main() {
    okex::Api api;
    bool running=true;




    try {
        auto start_time = std::chrono::high_resolution_clock::now();
        api.register_for_ticker(trading::Coins::btc, trading::Coins::ltc);
        api.register_for_ticker(trading::Coins::usd, trading::Coins::btc);
        api.register_for_ticker(trading::Coins::usd, trading::Coins::ltc);

        auto end_time = std::chrono::high_resolution_clock::now();
        trading::Graph graph{{trading::Coins::ltc,
                              trading::Coins::btc,
                              trading::Coins::usd}
                             };
        std::mutex graph_mutex;
        std::thread listen_thread([&graph,&api,&graph_mutex] {
            api.listen([&graph,&graph_mutex](trading::Ticker t) {
                           graph_mutex.lock();
                                std::cout<<"adding:"<<trading::coin_name(t.from)<<"->"<<trading::coin_name(t.to)<<"="<<t.last<<std::endl;
                                std::cout<<"adding:"<<trading::coin_name(t.to)<<"->"<<trading::coin_name(t.from)<<"="<<1.0/t.last<<std::endl;
                               graph.update_edge(t.from, t.to, trading::Edge_Data{price:t.last});
                               graph.update_edge(t.to, t.from, trading::Edge_Data{price:1.0/t.last});
                           graph_mutex.unlock();
                       }
            );

        }
        );
        std::thread search_thread([&running,&graph,&graph_mutex] {
                using namespace trading;
                std::vector<std::vector<path_node>> paths;
                paths.push_back({{Coins::usd},{Coins ::btc},{Coins::ltc},{Coins::usd}});
                paths.push_back({{Coins::usd},{Coins ::ltc},{Coins::btc},{Coins::usd}});
                while(running) {
                    graph_mutex.lock();
                    double initial_invest = 10000;
                    double max_path_gain = 0;
                    std::vector<path_node> max_path;
                    for (auto path:paths) {
                        double current_capital = initial_invest;
                        for (int idx = 0; idx < path.size() - 1; idx++) {
                            auto price=graph.get_edge(path[idx].coin, path[idx + 1].coin).price;
                            current_capital = price==0?0:current_capital /price;
                            path[idx+1].amount=current_capital;
                            path[idx+1].price=price;
                        }
                        double path_gain = current_capital - initial_invest;
                        if (path_gain > max_path_gain) {
                            max_path_gain = path_gain;
                            max_path = path;
                        }
                    }

                    if (max_path.size() > 0) {
                        register_db(max_path);
                    }
                    graph_mutex.unlock();
                }
            }
        );
        listen_thread.join();
        search_thread.join();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << std::endl;
    } catch (std::string& e) {
        std::cout << e << std::endl;
    }
    return 0;
}




