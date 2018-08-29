//
// Created by forest on 27/08/18.
//

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <iostream>
#include "api.hpp"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "nlohmann/json.hpp"
trading::rest::api::api(std::string host, int port) {
    session=std::make_unique<Poco::Net::HTTPClientSession>(host,port);
}

std::optional<trading::rest::result> trading::rest::api::login(std::string user, std::string password) {
    using namespace Poco::Net;
    using namespace nlohmann;
    std::string endpoint(api_root+"/auth/token/");
    json body={
            {"username",user},
            {"password",password}
    };
    HTTPRequest request(HTTPRequest::HTTP_POST,endpoint,HTTPMessage::HTTP_1_1);
    request.setContentType("application/json");
    request.setContentLength(body.dump().length());

    session->sendRequest(request)<<body;
    HTTPResponse response;
    std::string response_string;
    session->receiveResponse(response)>>response_string;
    if(response.getStatus()!=HTTPResponse::HTTPStatus::HTTP_OK) {
        return std::optional<trading::rest::result>(result{response.getStatus(),response_string});
    }
    json response_json=json::parse(response_string);
    token=response_json["access"];
    refresh_token=response_json["refresh"];
    return std::optional<trading::rest::result>();
}

std::optional<trading::rest::result> trading::rest::api::refresh() {
    return std::optional<trading::rest::result>();
}

bool trading::rest::api::is_session_valid() {
    return false;
}

std::optional<trading::rest::result> trading::rest::api::register_transaction() {
    return std::optional<trading::rest::result>();
}
