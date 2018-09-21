//
// Created by forest on 27/08/18.
//

#ifndef TRADING_RESULT_HPP
#define TRADING_RESULT_HPP

#include <string>
#include <optional>

namespace trading::rest{

    class Error{
    public:
        int code;
        std::string message;
    };
    template <typename  T>
    class result{
    public:
         result(T _value): value{_value}{

        }

        result(nullptr_t aNullptr,Error _error):error{_error} {
        }

        const std::optional<Error> get_error() {
            return  error;
        }
        const std::optional<T> get_value() {
            return value;
        }

        bool success(){
            return get_value().has_value();
        }
        bool failed(){
            return !success();
        }

    private:
        std::optional<T> value;
        std::optional<Error> error;

    };
}


#endif //TRADING_RESULT_HPP
