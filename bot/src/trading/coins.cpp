#include "coins.hpp"
namespace trading{
    std::string coin_name(Coins coin){
        std::map<Coins,std::string> coin_names{{btc,"btc"},{ltc,"ltc"},{usd,"usdt"},{eth,"eth"}};

        return coin_names[coin];

    }
    Coins coin_value(std::string name){
        std::map<std::string,Coins> coin_names{{"btc",btc},{"ltc",ltc},{"usdt",usd},{"eth",eth}};
        return coin_names[name];
    }

    std::ostream &operator<<(std::ostream &os, Coins coin) {
        os<<coin_name(coin);
        return os;
    }

    Symbol symbol_value(std::string name)
    {
        Symbol symbol;
        auto separator_pos=name.find('_');
        symbol.from=coin_value(name.substr(separator_pos+1,name.length()-separator_pos));
        symbol.to=coin_value(name.substr(0,separator_pos));
        return symbol;

    }

    std::string symbol_name(Symbol symbol) {
        return coin_name(symbol.to)+"_"+coin_name(symbol.from);
    }

}
