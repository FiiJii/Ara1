
# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
set(CMAKE_BUILD_TYPE Debug)
cmake_minimum_required (VERSION 2.8.11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set (CMAKE_CXX_STANDARD 17)
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)
#ADD_DEFINITIONS(
#        -std=c++11 
#            )
project (Trading)
include(capibara.cmake)

FIND_PACKAGE(Poco COMPONENTS Net NetSSL JSON REQUIRED)
INCLUDE_DIRECTORIES( ${Poco_INCLUDE_DIRS})
INCLUDE_DIRECTORIES("/usr/include")
INCLUDE_DIRECTORIES( ./include )
message(WARNING ${Poco_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES( ${EJECUTABLE} ${Poco_LIBRARIES} )

FIND_PACKAGE( Boost 1.40 COMPONENTS system filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boostsu_INCLUDE_DIR} )
TARGET_LINK_LIBRARIES( ${EJECUTABLE} ${Boost_LIBRARIES} )


