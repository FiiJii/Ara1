set(EJECUTABLE okex_linux) 
add_executable(okex_linux /home/forest/desarrollo/trading/src/okex/coins.cpp 
/home/forest/desarrollo/trading/src/okex/api.cpp 
/home/forest/desarrollo/trading/src/main/main.cpp 
)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "include/" "/usr/include/" )
