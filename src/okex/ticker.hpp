#ifndef _OKEX_TICKER_HPP_
#define _OKEX_TICKER_HPP_
#include "coins.hpp"
namespace okex{
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
}
#endif
