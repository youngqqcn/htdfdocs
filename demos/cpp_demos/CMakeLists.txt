cmake_minimum_required(VERSION 3.12)

project(cpp_demos)

set(CMAKE_BUILD_TYPE Debug)

find_library(secp256k1 libsecp256k1.a)

add_library(httplib htdf/http/httplib.cc)

add_library(htdf htdf/htdf.cpp
htdf/crypto/sha256.cpp 
htdf/crypto/ripemd160.cpp
htdf/bech32/bech32.cpp
htdf/crypto/string.cpp
htdf/crypto/strencodings.cpp)
target_link_libraries(htdf httplib libsecp256k1.a  pthread)

add_executable(test htdf/test.cpp  )
target_link_libraries(test htdf libsecp256k1.a )

# add_executable(htdf_address_generate htdf_address_generate.cpp )
# target_link_libraries(htdf_address_generate htdf libsecp256k1.a )

add_executable(htdf_transfer htdf_transfer.cpp )
target_link_libraries(htdf_transfer htdf libsecp256k1.a )

