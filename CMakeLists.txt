cmake_minimum_required(VERSION 3.10)

project(DNS_Resolver)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

include_directories(${CMAKE_SOURCE_DIR}/include)


file(GLOB PACKET_SOURCES src/packet/*.cpp)

set(SOURCES
    ${PACKET_SOURCES}
    src/bit_utils.cpp
    src/Receiver.cpp
    src/Resolver.cpp
    main.cpp
)

add_executable(dns_resolver ${SOURCES})