//
// Created by forest on 27/08/18.
//

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <iostream>
#include "api.hpp"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Base64Decoder.h"
#include "nlohmann/json.hpp"
#include <chrono>
#include "utils.hpp"
namespace trading::rest {
    api::api(std::string host, int port) {
        session = std::make_unique<Poco::Net::HTTPClientSession>(host, port);
    }

    std::optional<Error> api::login(std::string user, std::string password) {
        using namespace Poco::Net;
        using namespace nlohmann;
        std::string endpoint(api_root + "/auth/token/");
        json body = {
                {"username", user},
                {"password", password}
        };
        HTTPRequest request(HTTPRequest::HTTP_POST, endpoint, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setContentLength(body.dump().length());

        session->sendRequest(request) << body;
        HTTPResponse response;
        std::string response_string;
        session->receiveResponse(response) >> response_string;
        if (response.getStatus() != HTTPResponse::HTTPStatus::HTTP_OK) {
            return std::optional<trading::rest::Error>(Error{response.getStatus(), response_string});
        }
        json response_json = json::parse(response_string);
        token = response_json["access"];
        refresh_token = response_json["refresh"];
        return std::optional<trading::rest::Error>();
    }

    std::optional<Error> api::refresh() {
        using namespace Poco::Net;
        using namespace nlohmann;
        std::string endpoint(api_root + "/auth/token_refresh/");
        json body = {
                {"refresh", refresh_token}
        };
        HTTPRequest request(HTTPRequest::HTTP_POST, endpoint, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setContentLength(body.dump().length());
        session->sendRequest(request) << body;
        HTTPResponse response;
        std::string response_string;
        session->receiveResponse(response) >> response_string;
        if (response.getStatus() != HTTPResponse::HTTPStatus::HTTP_OK) {
            return std::optional<trading::rest::Error>(Error{response.getStatus(), response_string});
        }
        json response_json = json::parse(response_string);
        token = response_json["access"];
        refresh_token = response_json["refresh"];
        return std::optional<trading::rest::Error>();
    }


    long  extract_expiration(std::string token){
        auto first_dot=token.find(".");
        auto second_dot=token.find(".",first_dot+1);
        std::istringstream stream{token.substr(first_dot+1,second_dot-first_dot-1)};
        Poco::Base64Decoder decoder{stream};
        nlohmann::json decoded_token;
        decoder>>decoded_token;
        return decoded_token["exp"];
    }
    bool api::is_session_valid() {
        auto current_time = std::chrono::high_resolution_clock::now();
        return  current_time.time_since_epoch().count()>expiration;
    }

    result<Transaction_Detail> api::register_detail(const Transaction& transaction, Transaction_Detail& detail) {
        using namespace Poco::Net;
        std::string endpoint=api_root+"/trading/transaction_details/";
        HTTPRequest request(HTTPRequest::HTTP_POST, endpoint, HTTPMessage::HTTP_1_1);
        char amount_buffer[40];
        char fee_buffer[40];
        sprintf(amount_buffer,"%.10F",detail.amount);
        sprintf(fee_buffer,"%.10F",detail.fee);

        auto parity_result=get_parity(coin_name(detail.to)+"_"+coin_name(detail.from));
        if(parity_result.failed()){
            throw std::runtime_error("can't get the parity data");
        }
        auto _parity=parity_result.get_value().value();
        nlohmann::json json={ {"transaction",transaction.url},
                              {"parity",_parity.url},
                              {"amount", amount_buffer},
                              {"action", action_name(detail.action)},
                              {"commission", fee_buffer},
                              {"okex_order", 0}
                            };
        auto body=json.dump();
        request.setContentType("application/json");
        request.setContentLength(body.length());
        set_auth_headers(request);
        session->sendRequest(request)<<body;
        HTTPResponse response;
        std::string response_body{std::istreambuf_iterator<char>(session->receiveResponse(response)),{}};
        if(response.getStatus()!=HTTPResponse::HTTPStatus::HTTP_CREATED)
            return rest::result<Transaction_Detail>{nullptr,Error{response.getStatus(),response_body}} ;
        auto response_json=nlohmann::json::parse(response_body);
        detail.id=response_json["id"];
        detail.url=response_json["url"];
        return result{detail};
    }

    result<Transaction> api::register_transaction(Transaction& transaction) {
        using namespace Poco::Net;

        std::string endpoint=api_root+"/trading/transactions/";
        HTTPRequest request(HTTPRequest::HTTP_POST, endpoint, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        char investment_buffer[40];
        char earnings_buffer[40];
        sprintf(investment_buffer,"%.10F",transaction.investment);
        sprintf(earnings_buffer,"%.10F",transaction.earnings);
        nlohmann::json json={
                {"investment",investment_buffer},
                {"earnings",earnings_buffer}
        };
        auto body = json.dump();
        request.setContentLength(body.length());
        set_auth_headers(request);
        session->sendRequest(request)<<body;
        HTTPResponse response;
        std::string response_body{std::istreambuf_iterator<char>(session->receiveResponse(response)),{}};
        if(response.getStatus()!=HTTPResponse::HTTPStatus::HTTP_CREATED) {
            auto error = Error{response.getStatus(), body+response_body};
            return result<Transaction>{nullptr,error};
        }
        auto response_json=nlohmann::json::parse(response_body);
        transaction.id=response_json["id"];
        transaction.url=response_json["url"];
        for(auto& detail:transaction.details) {
            auto r=register_detail(transaction, detail);
            if(r.failed()){
                std::cout<<r.get_error()->code<<";"<<r.get_error()->message<<"\n";
            }
        }
        return result{transaction};
    }

    void api::set_auth_headers(Poco::Net::HTTPRequest& request) {

        request.set("Authorization","Bearer "+token);
    }

    result<Bot_Config> api::get_bot_config() {
        using namespace Poco::Net;
        std::string endpoint=api_root+"/config/bot/";
        HTTPRequest request(HTTPRequest::HTTP_GET, endpoint, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setContentLength(0);
        set_auth_headers(request);
        session->sendRequest(request);
        HTTPResponse response;
        std::string response_body{std::istreambuf_iterator<char>(session->receiveResponse(response)),{}};
        if(response.getStatus()!=HTTPResponse::HTTPStatus::HTTP_OK) {
            auto error = Error{response.getStatus(), response_body};
            return result<Bot_Config>{nullptr,error};
        }
        auto response_json=nlohmann::json::parse(response_body);
        if(!(response_json.count("results")>0)||response_json["counts"]>0)
            return {nullptr,Error{1,"data is not array"} };
        auto config_node=response_json["results"][0];
        std::cout<<config_node<<std::endl;
        Bot_Config bot_config;
        bot_config.id =config_node["id"];
        bot_config.url=config_node["url"];
        bot_config.max_lost=s2d(config_node["max_lost"]);
        bot_config.bot_status=config_node["bot_status"];
        bot_config.time_interval= config_node["time_interval"];
        for(auto coin_node:config_node["currencies"]) {
            bot_config.coins.push_back(coin_node["symbol"]);
        }


        return result{bot_config};
    }

    result<parity> api::get_parity(std::string symbol) {
        using namespace Poco::Net;
        std::string endpoint=api_root+"/config/currency/?symbol="+symbol;
        HTTPRequest request(HTTPRequest::HTTP_GET, endpoint, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setContentLength(0);
        set_auth_headers(request);
        session->sendRequest(request);
        HTTPResponse response;
        std::string response_body{std::istreambuf_iterator<char>(session->receiveResponse(response)),{}};
        auto response_json=nlohmann::json::parse(response_body);
        std::cout<<endpoint<<std::endl;
        std::cout<<response.getStatus()<<response_body<<std::endl;
        if(response.getStatus()!=HTTPResponse::HTTPStatus::HTTP_OK) {
            auto error = Error{response.getStatus(), response_body};
            return result<parity>{nullptr,error};
        }


        if(!(response_json.count("results")>0)||response_json["counts"]>0)
            return {nullptr,Error{1,"data is not array"} };
        auto parity_node=response_json["results"][0];
        std::cout<<parity_node<<std::endl;
        parity _parity;
        _parity.id =parity_node["id"];
        _parity.url=parity_node["url"];
        _parity.name=parity_node["name"];
        _parity.symbol=parity_node["symbol"];

        return {_parity};
    }

}
