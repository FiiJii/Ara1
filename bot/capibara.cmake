set(EJECUTABLE okex_linux) 
add_executable(okex_linux /media/sf_desarrollo/terceros/trading/bot/src/okex/api.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/main/graph_builder.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/main/main.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/coins.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/Graph.cpp 
/media/sf_desarrollo/terceros/trading/bot/src/trading/ticker.cpp
        src/trading/Edge_Data.cpp src/trading/Edge_Data.hpp)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "src/trading/" "include/" "/usr/include/" )
