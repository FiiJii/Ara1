# CMakeLists files in this project can
# refer coin the root source directory of the project as ${HELLO_SOURCE_DIR} and
# coin the root binary directory of the project as ${HELLO_BINARY_DIR}.
set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required (VERSION 2.8.11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set (CMAKE_CXX_STANDARD 17)

#ADD_DEFINITIONS(
#        -std=c++11 
#            )

project (Trading)
include(capibara.cmake)


FIND_PACKAGE(Qt5 COMPONENTS Core Widgets REQUIRED)
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${EJECUTABLE} ${Qt5Widgets_LIBRARIES})
set_property(TARGET ${EJECUTABLE} PROPERTY POSITION_INDEPENDENT_CODE ON)





FIND_PACKAGE(Poco COMPONENTS Net NetSSL JSON REQUIRED)
INCLUDE_DIRECTORIES( ${Poco_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("/usr/include")
INCLUDE_DIRECTORIES( ./include )
link_directories(./libs)
message(WARNING ${Poco_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES( ${EJECUTABLE} ${Poco_LIBRARIES} )