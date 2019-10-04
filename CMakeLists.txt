cmake_minimum_required(VERSION 3.13)
project(templates)

set(CMAKE_CXX_STANDARD 17)

add_executable(fibSimple fibSimple.cpp)
add_executable(fibStruct fibStruct.cpp)
add_executable(fibTricky fibTricky.cpp)
add_executable(funcSignature funcSignature.cpp)
add_executable(myTuple myTuple.cpp)