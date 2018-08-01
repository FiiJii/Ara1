#include "api.hpp"
#include "ticker.hpp"
#include "coins.hpp"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include <iostream>
#include <sstream>


using namespace Poco;
namespace okex{
    void Api::listen_for_ticker(Coins from,Coins to){    
        std::string endpoint="/";
        Net::HTTPRequest request(Net::HTTPRequest::HTTP_GET,endpoint,Net::HTTPRequest::HTTP_1_1);
        Net::HTTPResponse response;
            auto socket=Poco::Net::WebSocket(session,request,response);
            std::stringstream stringbuff;
             
            std::string channel="{'event':'addChannel','channel':'ok_sub_spot_ltc_btc_ticker'}";
            auto sent_bytes=socket.sendFrame(channel.c_str(),channel.length(),Poco::Net::WebSocket::FRAME_TEXT);
            Poco::Buffer<char> buffer(1);

            while(true){
                    int flags=0;
                    int received_bytes=socket.receiveFrame(buffer,flags);
                    for(auto b:buffer){
                        stringbuff<<b;
                        std::cout<<b;
                    }
                    JSON::Parser parser;
                    std::cout<<stringbuff.str()<<std::endl;

                    try{
                        auto rootVar=parser.parse(stringbuff);
                        auto root=rootVar.extract<JSON::Object::Ptr>();
                        if(!root->has("ticker")){
                            std::cout<<"failed querying:"<<endpoint<<std::endl;
                            root->stringify(std::cout);
                        }
                        auto ticker_node=root->getObject("ticker");
                        Ticker ticker{
                            .from=from,
                            .to=to,
                            .buy =ticker_node->optValue<double>("buy",0),
                            .high=ticker_node->optValue<double>("high",0),
                            .last=ticker_node->optValue<double>("last",0),
                            .low =ticker_node->optValue<double>("low",0),
                            .sell=ticker_node->optValue<double>("sell",0),
                            .vol =ticker_node->optValue<double>("vol",0),
                        };
                        std::cout<<"buy:"<<ticker.buy<<std::endl;
                        buffer.clear();
                    }catch(std::exception& e){
                        std::cerr<<e.what()<<std::endl;
                        buffer.clear();
                        stringbuff.str("");
                        stringbuff.clear();
                    }

        }


        /*session.sendRequest(request);
        Net::HTTPResponse response;
        std::istream& response_body=session.receiveResponse(response);
        JSON::Parser parser;
        auto rootVar=parser.parse(response_body);
        auto root = rootVar.extract<JSON::Object::Ptr>();
        if(!root->has("ticker")){
            std::cout<<"failed querying:"<<endpoint<<std::endl;
            root->stringify(std::cout);
            throw std::string("Do not have a ticker");
        }
        auto ticker_node=root->getObject("ticker");
        Ticker ticker{
            .from=from,
            .to=to,
            .buy =ticker_node->optValue<double>("buy",0),
            .high=ticker_node->optValue<double>("high",0),
            .last=ticker_node->optValue<double>("last",0),
            .low =ticker_node->optValue<double>("low",0),
            .sell=ticker_node->optValue<double>("sell",0),
            .vol =ticker_node->optValue<double>("vol",0),
        };*/

    } 
    Api::Api():
     session("real.okex.com",10441){

    }
}

