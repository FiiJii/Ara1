#ifndef _OKEX_TICKER_HPP_
#define _OKEX_TICKER_HPP_
#include "coins.hpp"
#include "nlohmann/json.hpp"
namespace trading{
    class Ticker{
        public:
            Coins from;
            Coins to;
            double buy;
		    double high;
    		double last;
		    double low;
		    double sell;
		    double vol;
    };
    
    void to_json(nlohmann::json& j, const Ticker& ticker) ;
    void from_json(const nlohmann::json& j, Ticker& ticker) ;
}



#endif
