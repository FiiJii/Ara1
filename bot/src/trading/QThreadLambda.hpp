//
// Created by forest on 25/09/18.
//

#ifndef TRADING_QTHREADLAMBDA_HPP
#define TRADING_QTHREADLAMBDA_HPP

#include <QThread>
#include <functional>

class QThreadLambda:public QThread {
public:
    QThreadLambda(QObject* parent,std::function<void(void)> function):QThread(parent), callback{function}{};
    virtual void run(){
        callback();
    }
private:
    std::function<void(void)> callback;
};


#endif //TRADING_QTHREADLAMBDA_HPP
