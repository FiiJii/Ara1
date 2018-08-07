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
        api.register_for_ticker(trading::Coins::ltc, trading::Coins::btc);
        api.register_for_ticker(trading::Coins::eth, trading::Coins::btc);

        auto end_time = std::chrono::high_resolution_clock::now();
        trading::Graph graph{{trading::Coins::ltc,
                              trading::Coins::btc,
                              trading::Coins::usd}
                             };
        std::mutex graph_mutex;
        std::thread listen_thread([&graph,&api,&graph_mutex] {
            api.listen([&graph,&graph_mutex](trading::Ticker t) {
                           graph_mutex.lock();
                               graph.update_edge(t.from, t.to, trading::Edge_Data{price:t.buy});
                               graph.update_edge(t.to, t.from, trading::Edge_Data{price:t.sell});
                           graph_mutex.unlock();
                       }
            );
        }
        );
        std::thread search_thread([&graph,&graph_mutex] {

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




