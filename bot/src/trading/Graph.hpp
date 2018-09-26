//
// Created by forest on 06/08/18.
//

#ifndef TRADING_GRAPH_H
#define TRADING_GRAPH_H
#include <vector>
#include <map>
#include "coins.hpp"
#include "Edge_Data.hpp"
#include <tuple>
namespace trading {
    class Graph {
        public:
            explicit Graph(std::vector<Coins> coins);
            void update_edge(Coins from,Coins to,Edge_Data data);
            Edge_Data get_edge(Coins from, Coins to) const;
            std::vector<Coins> get_neighbors(Coins from) const;
            bool has_edge(Coins from, Coins to) const;
            std::vector<Coins>  get_vertices() const;
            std::vector<std::tuple<Coins,Coins,Edge_Data>>  get_edges() const;

        private:
            std::vector<Coins> vertices;
            std::map<Coins,std::map<Coins,Edge_Data>> edges;
    };
}


#endif //TRADING_GRAPH_H
