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

#include <QApplication>
#include <QWidget>
#include <QWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QThread>
#include <QThreadLambda.hpp>
#include "gui/GraphDrawer.hpp"
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QLayout>
void ticker_callback(trading::Ticker ticker) {
    nlohmann::json json = ticker;

}
void register_db(std::vector<trading::path_node> path,double initial_invest,double earnings){
    using namespace trading::rest;
    api api_rest("api-trading.arawato.co",80);
    auto err=api_rest.login("trading_bot","trading");
    if (err) std::cout<<"login error:"<<err->code<<";"<<err->message<<"\n";
    Transaction transaction;
    transaction.earnings=earnings;
    transaction.investment=initial_invest;
    transaction.details.reserve(path.size());
    std::cout<<"Path:Earnings("<<earnings<<") ";
    for(auto idx=0;idx<path.size()-1;idx++){
        Transaction_Detail detail={};
        auto& current_node=path[idx];
        auto& next_node=path[idx+1];
        transaction.details.push_back(
                {.id=0,
                 .url="",
                 .from=current_node.coin,
                 .to=next_node.coin,
                 .fee=current_node.fees,
                 .price=current_node.price,
                 .amount=current_node.amount
                });
        std::cout<<trading::coin_name(current_node.coin)<<"->"<<trading::coin_name(next_node.coin)<<",";
    }
    std::cout<<std::endl;

    auto r=api_rest.register_transaction(transaction);
   // if(r.failed())
     //   std::cout<<"***Error"<<r.get_error()->code<<r.get_error()->message<<"\n";
}

std::pair<std::vector<trading::path_node>,double> get_max_gain_path(trading::Graph& graph,
                                                        trading::Graph::NodeType node,
                                                        std::map<trading::Coins,int>& visited,
                                                        double capital){
        visited[node.value]++;
        if(visited[node.value]==2&&node.value==trading::Coins::usd){
                    trading::path_node path_node;
                    path_node.coin=node.value;
                    std::cout<<"Capital:"<<capital<<"\n";
                    return {{path_node},capital};
        }else if (visited[node.value]>2){
            return {{},0.0f};
        }else{
            std::vector<trading::path_node> max_path;
            float max_gain=0;
            trading::path_node max_path_node;
            for(auto& neighbor:node.neighbors) {
                auto fees=graph.get_edge(node.value,neighbor.get().value).fee;
                auto price=graph.get_edge(node.value,neighbor.get().value).price;
                auto new_capital=price==0?0:capital/price;
                trading::path_node path_node_temp;
                path_node_temp.amount=new_capital;
                path_node_temp.price=price;
                path_node_temp.coin=node.value;
                path_node_temp.fees=fees;
                auto [temp_path,gain]=get_max_gain_path(graph,neighbor,visited,new_capital);
                if(gain>max_gain) {
                    max_path = temp_path;
                    max_path_node=path_node_temp;
                    max_gain=gain;
                }
            }

            std::vector<trading::path_node> final_path;
            final_path.reserve(max_path.size()+1);
            final_path.push_back(max_path_node);
            final_path.insert(final_path.end(),max_path.begin(),max_path.end());
            return {final_path,max_gain};
        }

}
int main(int argc,char* argv[]) {
    okex::Api api;
    bool running=true;


    std::locale loc("en_US.utf8");
    std::locale::global(loc);

    try {

        api.register_for_ticker(trading::Coins::btc, trading::Coins::ltc);
        api.register_for_ticker(trading::Coins::usd, trading::Coins::btc);
        api.register_for_ticker(trading::Coins::usd, trading::Coins::ltc);

        auto end_time = std::chrono::high_resolution_clock::now();
        trading::Graph graph{{trading::Coins::ltc,
                              trading::Coins::btc,
                              trading::Coins::usd}
                             };
        std::mutex graph_mutex;


        std::thread qt_thread([&argc, &argv,&graph,&graph_mutex] {

        });


        QApplication qt_app(argc, argv);
        QWidget w;
        auto listen_thread=new QThreadLambda(&w,[&api,&graph,&graph_mutex](){
                        api.listen([&graph,&graph_mutex](trading::Ticker t) {
                                       graph_mutex.lock();
                                       graph.update_edge(t.from, t.to, trading::Edge_Data{.price=t.last});
                                       graph.update_edge(t.to, t.from, trading::Edge_Data{.price=1.0/t.last});
                                       graph_mutex.unlock();
                                        QThread::yieldCurrentThread();
                                   }
                        );

                });
        listen_thread->start();

        auto search_thread= new QThreadLambda(&w,[&running,&graph,&graph_mutex] {
                                                  using namespace trading;
                                                  while(running) {
                                                      graph_mutex.lock();
                                                      double initial_invest = 10000;
                                                      auto [success,root]=graph.get_node(Coins::usd);
                                                      std::map<trading::Coins,int> visited;
                                                      for(auto v:graph.get_vertices())
                                                          visited[v]=0;
                                                      if(!success) continue;
                                                      auto [max_path,gain]=get_max_gain_path(graph,root,visited,initial_invest);
                                                      if (max_path.size() > 0&&gain>initial_invest) {
                                                          register_db(max_path,initial_invest,gain-initial_invest);
                                                      }
                                                      graph_mutex.unlock();
                                                      QThread::yieldCurrentThread();
                                                  }
                                              }
        );
        search_thread->start();
        auto timer = new QTimer(&w);
        trading::ui::GraphDrawer graph_drawer(&w,graph,graph_mutex);
        graph_drawer.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        w.connect(timer,&QTimer::timeout,&graph_drawer,& trading::ui::GraphDrawer::update_graph);
        auto layout= new QVBoxLayout();

        layout->setSizeConstraint(QLayout::SetMaximumSize);
        layout->addWidget(&graph_drawer,1);
        w.setLayout(layout);
        timer->start(500);
        w.show();
        qt_app.exec();



    } catch (std::string& e) {
        std::cout << e << std::endl;
    }
    return 0;
}




