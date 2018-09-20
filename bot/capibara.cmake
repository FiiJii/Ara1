set(EJECUTABLE okex_linux) 
add_executable(okex_linux /home/forest/desarrollo/trading_backend/bot/src/okex/api.cpp 
/home/forest/desarrollo/trading_backend/bot/src/main/main.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/Edge_Data.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/path_node.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/ticker.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/rest/transaction_detail.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/rest/transaction.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/rest/result.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/rest/api.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/Graph.cpp 
/home/forest/desarrollo/trading_backend/bot/src/trading/coins.cpp
        src/trading/rest/bot_config.hpp)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "src/trading/" "include/" "/usr/include/" )
