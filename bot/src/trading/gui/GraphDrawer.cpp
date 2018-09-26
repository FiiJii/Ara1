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

    void GraphDrawer::update_graph() {
        using namespace glm;
        auto origin=vec3(0);
        auto up_offset=vec3(0,200,0);
        qDebug()<<"updating";
        graph_mutex.lock();
        qDebug()<<"updating 2";
        auto vertices=graph.get_vertices();
        auto position=origin+up_offset;
        auto rotation=glm::angleAxis(radians((float)360.0/(float)vertices.size()),vec3(0,0,1));
        scene->clear();

        for(auto v:vertices){

                auto item= new QGraphicsEllipseItem();
                auto fill_color=QColor::fromRgb(240,100,100,255);
                auto line_color=QColor::fromRgb(60,60,60,255);


                if(v==Coins::usd) {
                    item->setRect(QRect{(int) origin.x, (int) origin.y, 100, 100});
                }else {
                    item->setRect(QRect{(int)position.x,(int)position.y,100,100});
                    position=rotation*position;
                    fill_color=QColor::fromRgb(100,100,240,255);
                }
                QPen pen(line_color);
                QBrush brush;
                brush.setStyle(Qt::SolidPattern);
                brush.setColor(fill_color);
                item->setPen(pen);
                item->setBrush(brush);
                auto label= new QGraphicsSimpleTextItem(item);
                label->setBrush(QBrush(QColor(240,230,230)));
                label->setFont(QFont("Helvetica", 14 ,QFont::Bold));
                label->setText(QString::fromStdString(coin_name(v)));
                label->setPos(item->rect().x()+50-label->boundingRect().width()*0.5,item->rect().y()+50-label->boundingRect().height()*0.5);

                scene->addItem(item);

        }
        this->update();
        graph_mutex.unlock();
    }
}