#include <algorithm>
#include "api.hpp"
#include <chrono>
#include "coins.hpp"
#include "Graph.hpp"
#include <iostream>
#include "nlohmann/json.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

void ticker_callback(trading::Ticker ticker) {
    nlohmann::json json = ticker;

}

int main() {
    okex::Api api;

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
        std::thread search_thread([&graph,&graph_mutex] {
                std::vector<std::vector<trading::Coins>> paths;
                paths.push_back({trading::Coins::usd,trading::Coins ::btc,trading::Coins::ltc,trading::Coins::usd});
                paths.push_back({trading::Coins::usd,trading::Coins ::ltc,trading::Coins::btc,trading::Coins::usd});
                while(true) {
                    graph_mutex.lock();
                    double initial_invest = 10000;
                    double max_path_gain = 0;
                    std::vector<trading::Coins> max_path;
                    for (auto path:paths) {
                        double current_capital = initial_invest;
                        for (int idx = 0; idx < path.size() - 1; idx++) {
                            auto price=graph.get_edge(path[idx], path[idx + 1]).price;
                            current_capital = price==0?0:current_capital /price ;
                        }
                        double path_gain = current_capital - initial_invest;
                        if (path_gain > max_path_gain) {
                            max_path_gain = path_gain;
                            max_path = path;
                        }
                    }
                    if (max_path.size() > 0) {
                        for(auto c:max_path)
                            std::cout<<trading::coin_name(c)<<":";

                        std::cout<<"gain="<<max_path_gain<<std::endl;
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




