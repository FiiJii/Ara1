set(EJECUTABLE okex_linux) 
add_executable(okex_linux /media/sf_desarrollo/terceros/trading/src/okex/api.cpp 
/media/sf_desarrollo/terceros/trading/src/okex/coins.cpp 
/media/sf_desarrollo/terceros/trading/src/main/main.cpp 
)
target_include_directories(okex_linux PUBLIC "src/okex/" "src/main/" "include/" "/usr/include/" )
