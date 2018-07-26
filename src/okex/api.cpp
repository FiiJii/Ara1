#include "api.hpp"


#include "ticker.hpp"
#include "coins.hpp"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"

using namespace Poco;
namespace okex{
    Ticker Api::get_ticker(Coins from,Coins to){
        std::string endpoint="/api/v1/ticker.do?symbol="+coin_name(from)+"_"+coin_name(to);
        Net::HTTPRequest request(Net::HTTPRequest::HTTP_GET,endpoint);
        session.sendRequest(request);
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
        };
        return ticker;

    } 
    Api::Api():
     session("www.okex.com"){

    }
}

