cmake_minimum_required(VERSION 3.14)
project(Question4Cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/exchange
    ${CMAKE_SOURCE_DIR}/strategy
    ${CMAKE_SOURCE_DIR}/util
    ${CMAKE_SOURCE_DIR}/binance_f_module
    ${CMAKE_SOURCE_DIR}/binance_s_module
    ${CMAKE_SOURCE_DIR}/gateio_f_module
    ${CMAKE_SOURCE_DIR}/gateio_s_module
)

file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/strategy/*.cpp
    ${CMAKE_SOURCE_DIR}/util/*.cpp
    ${CMAKE_SOURCE_DIR}/binance_f_module/*.cpp
    ${CMAKE_SOURCE_DIR}/binance_s_module/*.cpp
    ${CMAKE_SOURCE_DIR}/gateio_f_module/*.cpp
    ${CMAKE_SOURCE_DIR}/gateio_s_module/*.cpp
)

add_executable(question4 ${SOURCES})
