#include "coins.hpp"
namespace okex{
    std::string coin_name(Coins coin){
        std::map<Coins,std::string> coin_names{{btc,"btc"},{ltc,"ltc"},{usd,"usdt"},{eth,"eth"}};

        return coin_names[coin];
    }
}
