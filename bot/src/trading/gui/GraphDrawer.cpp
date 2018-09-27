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
#include <QGraphicsLineItem>

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
    void draw_coin(glm::vec3 position,
                   QGraphicsScene* scene,
                   Coins coin){

        auto item = new QGraphicsEllipseItem();
        auto fill_color = QColor::fromRgb(100, 100, 240, 255);
        auto line_color = QColor::fromRgb(60, 60, 60, 255);
        item->setRect(QRect{(int) position.x, (int) position.y, 100, 100});
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(fill_color);
        item->setPen(QPen(line_color));
        item->setBrush(brush);
        auto label = new QGraphicsSimpleTextItem(item);
        label->setBrush(QBrush(QColor(240, 230, 230)));
        label->setFont(QFont("Helvetica", 14, QFont::Bold));
        label->setText(QString::fromStdString(coin_name(coin)));
        label->setPos(item->rect().x() + 50 - label->boundingRect().width() * 0.5,
                      item->rect().y() + 50 - label->boundingRect().height() * 0.5);
        scene->addItem(item);
    }
    void draw_edge(glm::vec3 from,glm::vec3 to,QGraphicsScene* scene, Edge_Data const& data){
        using namespace glm;
        auto line_color = QColor::fromRgb(60, 60, 60, 255);
        auto line = new QGraphicsLineItem(QLineF(from.x,from.y,to.x,to.y));
        line->setPen(QPen(line_color));
        auto label = new QGraphicsSimpleTextItem();
        label->setPen(QPen(line_color));
        label->setFont(QFont("Helvetica", 10, QFont::Bold));
        char buffer[100];
        sprintf(buffer,"%f",data.price);
        label->setText(QString::fromStdString(std::string(buffer)));
        auto position=from+glm::normalize(to-from)*20.0f;
        label->setPos( position.x,position.y);
        scene->addItem(line);
        scene->addItem(label);
        std::cout<<from.x<<","<<from.y<<","<<from.z<<std::endl;
        std::cout<<to.x<<","<<to.y<<","<<to.z<<std::endl;
        std::cout<<position.x<<","<<position.y<<","<<position.z<<std::endl;
        std::cout<<buffer<<std::endl;
    }

    void draw_node(Graph& graph,
                 Graph::NodeType& node,
                 glm::vec3 position,
                 QGraphicsScene* scene,
                 std::map<Coins,VertexProperties> & vertex_properties,
                 std::map<std::pair<Coins,Coins>,VertexProperties>& edge_properties){
        using namespace glm;
        if(vertex_properties.find(node.value)==std::end(vertex_properties) ) vertex_properties[node.value]={false};
        if(vertex_properties[node.value].drawn)
            return;
        draw_coin(position,scene,node.value);
        vertex_properties[node.value].drawn = true;
        auto rotation = angleAxis(radians((float) 180.0 / (float) node.neighbors.size()), vec3(0, 0, 1));
        auto offset = vec3(0, 150, 0);
        float i=0;
        for (auto neighbor:node.neighbors) {
            auto neighbor_position=position + ((i*rotation)  * offset);
            draw_node(graph,neighbor, neighbor_position , scene, vertex_properties, edge_properties);
            draw_edge(position,neighbor_position,scene,graph.get_edge(node.value,neighbor.get().value));
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
            draw_node(graph, root.get(),origin,scene,vertex_properties,edge_properties);
        this->update();
        graph_mutex.unlock();
    }
}