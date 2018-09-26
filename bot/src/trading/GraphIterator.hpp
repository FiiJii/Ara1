//
// Created by forest on 26/09/18.
//

#ifndef TRADING_GRAPHITERATOR_HPP
#define TRADING_GRAPHITERATOR_HPP

#include "Graph.hpp"
namespace trading {
    class GraphIterator {

    public:
        GraphIterator(Graph &graph,Coins root):graph{graph},root{root}{};

    private:
        Graph& graph;
        Coins root;
    };
}

#endif //TRADING_GRAPHITERATOR_HPP
