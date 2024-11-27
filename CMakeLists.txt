cmake_minimum_required(VERSION 3.13)
project(HTTP_INTERFACE_PROGRAM)

set(DEFAULT_URL "http://localhost")
set(DEFAULT_PORT "8080")

set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0167 NEW)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(ZLIB REQUIRED)
find_package(GTest REQUIRED)
find_package(CURL REQUIRED) 

add_executable(http_interface_program main.cpp archive.cpp http_archive.cpp)
add_executable(http_interface_program_tests tests/test_set.cpp)

target_compile_definitions(http_interface_program PRIVATE DEFAULT_PORT="${DEFAULT_PORT}")
target_compile_definitions(http_interface_program_tests PRIVATE DEFAULT_URL="${DEFAULT_URL}" DEFAULT_PORT="${DEFAULT_PORT}")

target_link_libraries(http_interface_program ${Boost_LIBRARIES} ZLIB::ZLIB)
target_include_directories(http_interface_program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})

target_include_directories(http_interface_program_tests PRIVATE ${CURL_INCLUDE_DIR})
target_link_libraries(http_interface_program_tests PRIVATE GTest::GTest GTest::Main ${CURL_LIBRARIES})

enable_testing()

add_test(NAME test COMMAND http_interface_program_tests)