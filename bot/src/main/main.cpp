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

        auto& current_node=path[idx];
        auto& next_node=path[idx+1];

        Transaction_Detail detail={};
        detail.id=0;
        detail.url="";
        detail.from=current_node.coin;
        detail.to=next_node.coin;
        detail.fee=current_node.fees;
        detail.price=current_node.price;
        detail.amount=current_node.amount;

        transaction.details.push_back(detail);
        std::cout<<trading::coin_name(current_node.coin)<<"->"<<trading::coin_name(next_node.coin)<<",";
    }
    std::cout<<std::endl;

    auto r=api_rest.register_transaction(transaction);
    if(r.failed())
        std::cout<<"***Error"<<r.get_error()->code<<r.get_error()->message<<"\n";
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
        }else if (visited[node.value]>1){
            return {{},0.0f};
        }else{
            std::vector<trading::path_node> max_path;
            float max_gain=0;
            trading::path_node max_path_node;
            for(auto& neighbor:node.neighbors) {
                auto fees_rate=0.0015f;
                auto price=graph.get_edge(node.value,neighbor.get().value).price;
                auto new_capital=price==0?0:capital/price;
                auto fees=new_capital*fees_rate;
                new_capital=new_capital-fees;

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
       std::cout<<"Starting "<<std::endl;
    okex::Api api;
    bool running=true;

     std::locale::global(std::locale::classic());

    try {

        std::cout<<"Inicializing API"<<std::endl;
        trading::rest::api api_rest("api-trading.arawato.co",80);
        std::cout<<"Login"<<std::endl;
        api_rest.login("trading_bot","trading");
        std::cout<<"Get Config"<<std::endl;
        auto result=api_rest.get_bot_config();
        std::cout<<"Building Graph"<<std::endl;
        if(result.failed()) throw  std::runtime_error(result.get_error().value().message);
        auto bot_config=result.get_value().value();
        for(auto coin_a:bot_config.coins) {
            for (auto coin_b:bot_config.coins){
                if(coin_a!=coin_b)
                    api.register_for_ticker(coin_a, coin_b);
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::cout<<"Building Graph\n";
        trading::Graph graph{bot_config.coins};

        std::mutex graph_mutex;


        std::thread qt_thread([&argc, &argv,&graph,&graph_mutex] {

        });


        QApplication qt_app(argc, argv);
        QWidget w;
        auto listen_thread=new QThreadLambda(&w,[&api,&graph,&graph_mutex](){
                        api.listen([&graph,&graph_mutex](trading::Ticker t) {
                                       graph_mutex.lock();
                                       trading::Edge_Data edge;
                                       trading::Edge_Data inverse_edge;
                                       edge.price=t.last;
                                       inverse_edge.price=1.0/t.last;
                                       graph.update_edge(t.from, t.to, edge);
                                       graph.update_edge(t.to, t.from, inverse_edge);
                                       graph_mutex.unlock();
                                        QThread::yieldCurrentThread();
                                   }
                        );

                });
       // listen_thread->start();

        auto search_thread= new QThreadLambda(&w,[&running,&graph,&graph_mutex] {
                                                  using namespace trading;
                                                  std::locale::global(std::locale::classic());
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
        //search_thread->start();
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
        std::cout<<"Inicializado\n";
        qt_app.exec();



    }catch (std::exception e) {
        std::cout << e.what() << std::endl;
    } catch (std::string& e) {
        std::cout << e << std::endl;
    }
    return 0;
}




