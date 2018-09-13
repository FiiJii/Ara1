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
        nlohmann::json json={ {"transaction",transaction.url},
                              {"parity",coin_name(detail.to)+"_"+coin_name(detail.from)},
                              {"amount", amount_buffer},
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
            return result<Transaction_Detail>{Error{response.getStatus(),response_body}} ;
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
            auto error = Error{response.getStatus(), response_body};
            std::cout << error.code << ";;" << error.message << "\n";
            auto _result=result<Transaction>{error};
            std::cout << "result"<<_result.get_error()->code << ";;" << _result.get_error()->message << "\n";
            return result<Transaction>{error};
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

}
