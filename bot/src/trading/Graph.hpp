//
// Created by forest on 06/08/18.
//

#ifndef TRADING_GRAPH_H
#define TRADING_GRAPH_H
#include <vector>
#include <map>
#include "coins.hpp"
#include "Edge_Data.hpp"

namespace trading {
    class Graph {
        public:
            explicit Graph(std::vector<Coins> coins);
            void update_edge(Coins from,Coins to,Edge_Data data);
            Edge_Data get_edge(Coins from, Coins to);
            bool has_edge(Coins from, Coins to);
        private:
            std::vector<Coins> vertices;
            std::map<Coins,std::map<Coins,Edge_Data>> edges;
    };
}


#endif //TRADING_GRAPH_H
