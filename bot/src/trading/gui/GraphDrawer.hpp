//
// Created by forest on 25/09/18.
//

#ifndef TRADING_GRAPHDRAWER_HPP
#define TRADING_GRAPHDRAWER_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <Graph.hpp>
#include <memory>
#include <QTimer>
#include <atomic>
#include <mutex>

namespace trading::ui {
    class GraphDrawer : public QWidget {
    public:
        GraphDrawer(QWidget *parent,Graph& graph,std::mutex& graph_mutex);
    public slots:
        void update_graph();

    private:
        QGraphicsScene* scene;
        QGraphicsView* view;
        QTimer* timer;
        Graph& graph;
        std::mutex& graph_mutex;
    };

}
#endif //TRADING_GRAPHDRAWER_HPP
