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
#include "transaction.hpp"
#include "bot_config.hpp"

namespace trading::rest {
    class api {
    public:
        api(std::string host, int port);

        std::optional<Error> login(std::string user, std::string password);
        std::optional<Error> refresh();
        bool is_session_valid();
        result<Transaction> register_transaction(Transaction& transaction);
        result<Transaction_Detail> register_detail(const Transaction& transaction,Transaction_Detail& detail);
<<<<<<< HEAD
        result<Transaction_Detail> get_bot_config();

=======
        result<Bot_Config> get_bot_config();
>>>>>>> b6437e49cc5c63016d217686507109530bc7b619
    private:
        std::unique_ptr<Poco::Net::HTTPClientSession> session;
        std::string api_root="/api";
        std::string token;
        std::string refresh_token;
        long expiration;
        void set_auth_headers(Poco::Net::HTTPRequest& request);
    };
}
#endif //TRADING_API_HPP
