

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
#include <chrono>
#include "nlohmann/json.hpp"
#include "symbols.hpp"
#include "Poco/Zip/Decompress.h"
#include "Poco/InflatingStream.h"

using namespace Poco;
namespace okex{
    void Api::register_for_ticker(trading::Coins from,trading::Coins to){
        std::string channel="{'event':'addChannel','channel':'ok_sub_spot_"+coin_name(to)+"_"+coin_name(from)+"_ticker'}";
        auto sent_bytes=socket->sendFrame(channel.c_str(),channel.length(),Poco::Net::WebSocket::FRAME_TEXT);
    }
    void Api::ping(){
        std::string channel="{'event':'ping'}";
        auto sent_bytes=socket->sendFrame(channel.c_str(),channel.length(),Poco::Net::WebSocket::FRAME_TEXT);
    }
    void Api::listen(std::function<void(trading::Ticker)> callback){
            char buffer[1000];
            std::stringstream deflated_stream;

            auto last_time=std::chrono::system_clock::now();

            while(true){
                    int flags=0;
                    auto current_time=std::chrono::system_clock::now();
                    auto seconds=std::chrono::duration_cast<std::chrono::seconds>(current_time-last_time);
                    if(seconds.count()>30) {
                        last_time=current_time;
                        ping();
                    }
                    int received_bytes=socket->receiveFrame((void*)buffer,1000,flags);
                    Poco::InflatingOutputStream inflating_stream(deflated_stream,15);
                    static char dummy_head[2] = {
                            0x8 + 0x7 * 0x10,
                            (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
                    };
                inflating_stream.write(dummy_head,2);
                inflating_stream.write(buffer,received_bytes);
                    //for(int i=0;i<received_bytes;i++){
                    //    inflating_stream<<buffer[i];
                    //}
                    inflating_stream.close();
                    try{
                        nlohmann::json root_node;

                        deflated_stream>>root_node;


                        for(auto node:root_node){
                            auto ticker_node=node;
                            trading::Ticker ticker=ticker_node;
                            std::cout<<ticker.from<<"_"<<ticker.to<<"("<< ticker.last<<")\n";
                            callback(ticker);
                        }
                        deflated_stream.str("");
                        deflated_stream.clear();
                    }catch(std::exception& e){
                        std::cerr<<e.what()<<std::endl;
                        deflated_stream.str("");
                        deflated_stream.clear();

                    }

        }
    } 
    void Api::connect(){
        Poco::Net::HTTPSClientSession session("real.okex.com",10441);

        std::string endpoint="/";
        Net::HTTPRequest request(Net::HTTPRequest::HTTP_GET,endpoint,Net::HTTPRequest::HTTP_1_1);
        Net::HTTPResponse response;
        socket=std::make_unique<Poco::Net::WebSocket>(session,request,response);
        std::cout<<"hand shake:"<<response.getStatus()<<std::endl;
    }
    Api::Api(){
        for(auto s:okex::symbols) {
            this->symbols[s]=true;
        }
        connect();

    }

    trading::Symbol Api::get_supported_symbol_for(trading::Symbol symbol) {

        if(this->symbols.count(trading::symbol_name(symbol))>0)
            return symbol;
        trading::Symbol inverted_symbol={symbol.to,symbol.from};
        if(this->symbols.count(trading::symbol_name(inverted_symbol))>0)
            return inverted_symbol;
        //return symbol;
    }
}

