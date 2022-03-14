cmake_minimum_required(VERSION "3.17")

project(http_log_monitoring)

set(CMAKE_CXX_STANDARD 17)
set(ThirdPartyInc_DIR /opt/homebrew/include) #boost and google test
set(ThirdPartyLib_DIR /opt/homebrew/lib)

include_directories( ${ThirdPartyInc_DIR} )
link_directories( ${ThirdPartyLib_DIR} )

# search for *.cpp and *.h
file(GLOB http_log_SRC
     "src/*.h"
     "src/*.cpp")

add_executable(http_log_monitoring ${http_log_SRC} src/Buffer.h src/Parser.h src/Alert.h)

#TESTS
add_executable(Google_Tests_run test/ParserTest.cpp test/BufferTest.cpp test/AlertTest.cpp)
target_link_libraries(Google_Tests_run)
target_link_libraries(Google_Tests_run gtest gtest_main)









