cmake_minimum_required (VERSION 3.0)

project (ShamirSecretSharing)
set (CMAKE_CXX_STANDARD 11)

add_executable(Shamir main.cpp)

include_directories( . )

add_library(sha256 sha256.cpp sha256.h)
add_library(get_insecure_randomness get_insecure_randomness.cpp get_insecure_randomness.h)
add_library(multiblock multiblock.cpp multiblock.h sha256)
add_library(rijndael rijndael.cpp rijndael.h)
add_library(oneblockshamir oneblockshamir.cpp oneblockshamir.h rijndael get_insecure_randomness)
add_library(shamirmulti shamirmulti.cpp shamirmulti.h oneblockshamir multiblock rijndael get_insecure_randomness)
add_library(wordlist wordlist.cpp wordlist.h)

target_link_libraries(Shamir sha256 get_insecure_randomness multiblock oneblockshamir rijndael shamirmulti wordlist)
