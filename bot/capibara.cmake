set(EJECUTABLE okex_linux) 
add_executable(okex_linux /media/sf_desarrollo/terceros/trading/bot/src/okex/api.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/main/main.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/coins.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/Edge_Data.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/Graph.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/rest/api.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/rest/result.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/rest/transaction.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/rest/transaction_detail.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/ticker.cpp
        src/trading/path_node.cpp src/trading/path_node.hpp)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "src/trading/" "include/" "/usr/include/" )
