cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 20)
# set (CMAKE_CXX_FLAGS -O3)

project(Test)

add_executable(threads threads.cpp)
add_executable(coroutines coroutines.cpp)
add_executable(clones clones.cpp)
add_executable(rose rose.cpp)
add_executable(traits traits.cpp)
add_executable(ctorthrow ctor_throw.cpp)
add_executable(mkunique my_make_unique.cpp)
add_executable(valueptr value_ptr.cpp)
add_executable(funcptr funcptr.cpp)