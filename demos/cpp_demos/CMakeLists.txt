cmake_minimum_required(VERSION 3.12)

project(cpp_demos)

set(CMAKE_BUILD_TYPE Debug)

find_library(secp256k1 libsecp256k1.a)

add_executable(test
    htdf/test.cc 
    htdf/htdf.cc 
    htdf/crypto/sha256.cpp 
    htdf/crypto/ripemd160.cpp
    htdf/bech32/bech32.cpp
    htdf/crypto/string.cpp
    htdf/crypto/strencodings.cpp
    )

target_link_libraries(test libsecp256k1.a )

