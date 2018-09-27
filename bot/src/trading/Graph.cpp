//
// Created by forest on 06/08/18.
//

#include "Graph.hpp"
#include <algorithm>
#include <rest/result.hpp>

namespace trading {
    trading::Graph::Graph(std::vector<trading::Coins> coins) {
        nodes.reserve(coins.size());
        for (auto c:coins) {
            GraphNode<Coins, VectorReference> node;
            node.value = c;
            nodes.push_back(node);
            edges[c] = std::map<Coins, Edge_Data>();
        }
    }

    void trading::Graph::update_edge(trading::Coins from, trading::Coins to, Edge_Data data) {
        edges[from][to] = data;
        auto iter=std::find_if(nodes.begin(),nodes.end(),[&from](auto a){return a.value==from;});
        if(iter!=nodes.end()) {
            auto &neighbors = (*iter).neighbors;
            auto iter_neighbor=std::find_if(neighbors.begin(), neighbors.end(), [to](auto a) { return a.get().value == to; });
            if(iter_neighbor==neighbors.end()){
                neighbors.push_back(get_node(to).second);
            }
        }
    }

    bool trading::Graph::has_edge(trading::Coins from, trading::Coins to) const {

        return edges.find(from) != edges.end() && edges.at(from).find(to) != edges.at(from).end();
    }

    trading::Edge_Data trading::Graph::get_edge(trading::Coins from, trading::Coins to) const {

        return edges.at(from).at(to);
    }

    std::vector<trading::Coins> trading::Graph::get_vertices() const {
        std::vector<Coins> v;
        v.reserve(nodes.size());
        for (auto n:nodes) {
            v.push_back(n.value);
        }
        return v;

    }

    std::vector<std::tuple<trading::Coins, trading::Coins, trading::Edge_Data>> trading::Graph::get_edges() const {
        std::vector<std::tuple<trading::Coins, trading::Coins, trading::Edge_Data>> edge_list{};
        for (auto kp:edges) {
            for (auto kp2:kp.second) {
                edge_list.push_back({kp.first, kp2.first, kp2.second});
            }
        }
        return edge_list;
    }

    std::vector<trading::Coins> trading::Graph::get_neighbors(trading::Coins from) const {
        std::vector<trading::Coins> neighbors;
        neighbors.reserve(edges.at(from).size());
        for (auto kp:edges.at(from)) {
            neighbors.push_back(kp.first);
        }
        return neighbors;
    }

    std::pair<bool,VectorReference<Graph::NodeType>> trading::Graph::get_node(trading::Coins coin) {
        auto iter=std::find_if(std::begin(nodes),std::end(nodes),[coin](auto a){return a.value==coin;});
        if(iter!=std::end(nodes)){
            return  std::make_pair(true,VectorReference<GraphNode<Coins, VectorReference>>{nodes,std::distance(std::begin(nodes),iter)});
        }
        return std::make_pair(false,VectorReference<GraphNode<Coins, VectorReference>>{nodes});
    }

}