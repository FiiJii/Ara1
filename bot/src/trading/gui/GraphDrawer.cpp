//
// Created by forest on 25/09/18.
//

#include <iostream>
#include "GraphDrawer.hpp"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <QDebug>
#include <QVBoxLayout>
#include <QGraphicsItem>
#include <QBrush>
#include <QGraphicsTextItem>
#include <QFont>
namespace trading::ui {
    GraphDrawer::GraphDrawer(QWidget *parent,Graph& graph,std::mutex& graph_mutex):QWidget(parent),graph{graph},graph_mutex{graph_mutex}{

        this->scene=new QGraphicsScene();

        scene->addText("Probando");
        this->view=new QGraphicsView(scene,this);
        view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        auto layout= new QVBoxLayout();

        layout->setSizeConstraint(QLayout::SetMaximumSize);
        layout->addWidget(view,1);
        setLayout(layout);

    }
    struct VertexProperties{
        bool drawn;
    };
    void draw_node(Graph::NodeType& node,
                 glm::vec3 position,
                 QGraphicsScene* scene,
                 std::map<Coins,VertexProperties> & vertex_properties,
                 std::map<std::pair<Coins,Coins>,VertexProperties>& edge_properties){
        using namespace glm;
        if(vertex_properties.find(node.value)==std::end(vertex_properties) ) vertex_properties[node.value]={false};
        if(vertex_properties[node.value].drawn)
            return;

        auto item = new QGraphicsEllipseItem();
        auto fill_color = QColor::fromRgb(240, 100, 100, 255);
        auto line_color = QColor::fromRgb(60, 60, 60, 255);
        item->setRect(QRect{(int) position.x, (int) position.y, 100, 100});
        fill_color = QColor::fromRgb(100, 100, 240, 255);
        QPen pen(line_color);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(fill_color);
        item->setPen(pen);
        item->setBrush(brush);
        auto label = new QGraphicsSimpleTextItem(item);
        label->setBrush(QBrush(QColor(240, 230, 230)));
        label->setFont(QFont("Helvetica", 14, QFont::Bold));
        label->setText(QString::fromStdString(coin_name(node.value)));
        label->setPos(item->rect().x() + 50 - label->boundingRect().width() * 0.5,
                      item->rect().y() + 50 - label->boundingRect().height() * 0.5);
        scene->addItem(item);
        vertex_properties[node.value].drawn = true;
        auto rotation = angleAxis(radians((float) 180.0 / (float) node.neighbors.size()), vec3(0, 0, 1));
        auto offset = vec3(0, 150, 0);
        float i=0;
        for (auto neighbor:node.neighbors) {
            draw_node(neighbor, position + ((i*rotation)  * offset), scene, vertex_properties, edge_properties);
            i+=1;
        }

    }
    void GraphDrawer::update_graph() {
        using namespace glm;
        auto origin=vec3(0);
        auto up_offset=vec3(0,200,0);
        qDebug()<<"updating";
        graph_mutex.lock();
        qDebug()<<"updating 2";
        auto vertices=graph.get_vertices();
        auto position=origin+up_offset;

        scene->clear();

        std::map<std::pair<Coins,Coins>,VertexProperties> edge_properties;
        std::map<Coins,VertexProperties> vertex_properties;
        auto [success,root]=graph.get_node(Coins::usd);
        if(success)
            draw_node(root.get(),origin,scene,vertex_properties,edge_properties);
        this->update();
        graph_mutex.unlock();
    }
}