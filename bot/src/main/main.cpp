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

void ticker_callback(trading::Ticker ticker) {
    nlohmann::json json = ticker;

}
void register_db(std::vector<trading::Coins> path){
    Poco::Net::HTTPClientSession session("localhost",8000);
    std::string url_transaction("/api/trading/transactions/");
    std::string url_transaction_details("/api/trading/transactions_details/");
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, url_transaction, Poco::Net::HTTPMessage::HTTP_1_1);
    Poco::Net::HTTPResponse response;
    session.sendRequest(request);
    auto& response_body=session.receiveResponse(response);
    if(response.getStatus()==Poco::Net::HTTPResponse::HTTP_CREATED){
        nlohmann::json response_json;
        response_body>>response_json;
        for(int i=0;i<path.size();i++){
            Poco::Net::HTTPRequest request_detail(Poco::Net::HTTPRequest::HTTP_POST, url_transaction, Poco::Net::HTTPMessage::HTTP_1_1);
            Poco::Net::HTTPResponse response_detail;
            auto tries=3;
            do{
                session.sendRequest(request_detail);
                auto& detail_body=session.receiveResponse(response_detail);
            }while(tries-->0&&response_detail.getStatus()!=Poco::Net::HTTPResponse::HTTP_CREATED);

        }
    }
}
int main() {
    okex::Api api;
    trading::rest::api api_rest("localhost",8000);
    auto res=api_rest.login("trading_bot","trading");
    if(res)
        std::cout<<"resultado:"<<res->code<<";"<<res->message<<"\n";
    return 0;
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




