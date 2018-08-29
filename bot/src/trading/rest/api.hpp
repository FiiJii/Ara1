//
// Created by forest on 27/08/18.
//

#ifndef TRADING_API_HPP
#define TRADING_API_HPP


#include <string>
#include <optional>
#include <bits/unique_ptr.h>
#include "result.hpp"
#include "Poco/Net/HTTPSClientSession.h"
namespace trading::rest {
    class api {
    public:
        api(std::string host, int port);

        std::optional<result> login(std::string user, std::string password);
        std::optional<result> refresh();
        bool is_session_valid();
        std::optional<result> register_transaction();
    private:
        std::unique_ptr<Poco::Net::HTTPClientSession> session;
        std::string api_root="/api";
        std::string token;
        std::string refresh_token;
    };
}
#endif //TRADING_API_HPP
