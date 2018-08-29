

#include "api.hpp"
#include "../trading/ticker.hpp"
#include "coins.hpp"
#include <functional>
#include "Poco/JSON/Parser.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include <iostream>
#include <sstream>
#include <string>
#include "nlohmann/json.hpp"


using namespace Poco;
namespace okex{
    void Api::register_for_ticker(trading::Coins from,trading::Coins to){
        std::string channel="{'event':'addChannel','channel':'ok_sub_spot_"+coin_name(to)+"_"+coin_name(from)+"_ticker'}";
        auto sent_bytes=socket->sendFrame(channel.c_str(),channel.length(),Poco::Net::WebSocket::FRAME_TEXT);
    }
    void Api::listen(std::function<void(trading::Ticker)> callback){
            char buffer[1000];
            std::stringstream stringbuff;
            while(true){
                    int flags=0;
                    int received_bytes=socket->receiveFrame((void*)buffer,1000,flags);
                    for(int i=0;i<received_bytes;i++){
                        stringbuff<<buffer[i];
                    }
                    try{
                        nlohmann::json root_node;
                        stringbuff>>root_node;
                        for(auto node:root_node){
                            auto ticker_node=node;
                            trading::Ticker ticker=ticker_node;
                            callback(ticker);
                        }
                        stringbuff.str("");
                        stringbuff.clear();
                    }catch(std::exception& e){
                        std::cerr<<e.what()<<std::endl;
                        std::cout<<stringbuff.str()<<std::endl;
                        stringbuff.str("");
                        stringbuff.clear();

                    }

        }
    } 
    void Api::connect(){
        Poco::Net::HTTPSClientSession session("real.okex.com",10441);

        std::string endpoint="/";
        Net::HTTPRequest request(Net::HTTPRequest::HTTP_GET,endpoint,Net::HTTPRequest::HTTP_1_1);
        Net::HTTPResponse response;
        socket=std::make_unique<Poco::Net::WebSocket>(session,request,response);
    }
    Api::Api(){
        connect();

    }
}

