set(EJECUTABLE okex_linux) 
add_executable(okex_linux src/okex/api.cpp 
src/main/main.cpp 
src/trading/coins.cpp 
src/trading/Edge_Data.cpp 
src/trading/Graph.cpp 
src/trading/GraphIterator.cpp 
src/trading/GraphNode.cpp 
src/trading/gui/GraphDrawer.cpp 
src/trading/IndexReference.cpp 
src/trading/path_node.cpp 
src/trading/QThreadLambda.cpp 
src/trading/rest/api.cpp 
src/trading/rest/bot_config.cpp 
src/trading/rest/result.cpp 
src/trading/rest/transaction.cpp 
src/trading/rest/transaction_detail.cpp 
src/trading/ticker.cpp 
src/utils/utils.cpp
        src/trading/rest/parity.cpp src/trading/rest/parity.hpp)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "src/trading/" "src/utils/" "include/" "/usr/include/" )
