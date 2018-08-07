//
// Created by forest on 06/08/18.
//

#include "Graph.hpp"

trading::Graph::Graph(std::vector<trading::Coins> coins) {
    vertices.reserve(coins.size());
    for(auto c:coins) {
        vertices.push_back(c);
        edges[c]=std::map<Coins,Edge_Data>();

    }
}

void trading::Graph::update_edge(trading::Coins from, trading::Coins to,Edge_Data data) {
    edges[from][to]=data;
}
bool trading::Graph::has_edge(trading::Coins from, trading::Coins to) {

    return edges[from].find(to)!=edges[from].end();
}
trading::Edge_Data trading::Graph::get_edge(trading::Coins from, trading::Coins to) {

    return edges[from][to];
}

