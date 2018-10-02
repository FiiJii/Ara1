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

}
