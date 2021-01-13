cmake_minimum_required(VERSION 3.12)

project(cpp_demos)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenSSL REQUIRED)
find_library(secp256k1 libsecp256k1.a)

add_executable(main main.cc test.cc htdf/htdf.cc rand.cpp htdf/crypto/sha256.cpp htdf/crypto/ripemd160.cpp)
target_link_libraries(main libsecp256k1.a OpenSSL::SSL)

