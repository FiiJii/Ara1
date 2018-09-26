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
bool trading::Graph::has_edge(trading::Coins from, trading::Coins to) const{

    return edges.find(from)!=edges.end()&&edges.at(from).find(to)!=edges.at(from).end();
}
trading::Edge_Data trading::Graph::get_edge(trading::Coins from, trading::Coins to) const{

    return edges.at(from).at(to);
}

std::vector<trading::Coins> trading::Graph::get_vertices() const{
    return std::vector<trading::Coins>{vertices};

}

std::vector<std::tuple<trading::Coins, trading::Coins,trading::Edge_Data>> trading::Graph::get_edges() const{
    std::vector<std::tuple<trading::Coins, trading::Coins,trading::Edge_Data>> edge_list{};
    for(auto kp:edges){
        for(auto kp2:kp.second){
            edge_list.push_back({kp.first,kp2.first,kp2.second});
        }
    }
    return edge_list;
}

std::vector<trading::Coins> trading::Graph::get_neighbors(trading::Coins from) const {
    std::vector<trading::Coins> neighbors;
    neighbors.reserve(edges.at(from).size());
    for(auto kp:edges.at(from)){
        neighbors.push_back(kp.first);
    }
    return neighbors;
}

