cmake_minimum_required(VERSION "3.17")

project(http_log_monitoring)

set(CMAKE_CXX_STANDARD 17)
set(Boost_DIR /opt/homebrew/include)

include_directories( ${Boost_DIR} )

# search for *.cpp and *.h
file(GLOB http_log_SRC 
     "src/*.h"
     "src/*.cpp"
)

add_executable(http_log_monitoring ${http_log_SRC})

