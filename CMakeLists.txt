cmake_minimum_required(VERSION 3.10)
project(DNS_Resolver)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

file(GLOB SOURCES "src/*.cpp")
add_executable(resolver ${SOURCES})
