set(EJECUTABLE okex_linux) 
add_executable(okex_linux src/okex/api.cpp 
src/main/main.cpp 
src/trading/coins.cpp 
src/trading/Edge_Data.cpp 
src/trading/Graph.cpp 
src/trading/path_node.cpp 
src/trading/rest/api.cpp 
src/trading/rest/result.cpp 
src/trading/rest/transaction.cpp 
src/trading/rest/transaction_detail.cpp 
src/trading/ticker.cpp 
)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "src/trading/" "include/" "/usr/include/" )
