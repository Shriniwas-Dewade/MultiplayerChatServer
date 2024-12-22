cmake_minimum_required(VERSION 3.15)
project(MultiplayerChatServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost 1.71.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} include)

add_subdirectory(src/server)
add_subdirectory(src/client)
