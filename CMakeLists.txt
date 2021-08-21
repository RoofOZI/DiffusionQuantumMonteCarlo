cmake_minimum_required(VERSION 3.14)
project(DiffusionMC CXX)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -D_GLIBCXX_DEBUG)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
file(GLOB source src/*.cxx)

add_library(simudmc SHARED ${source})
add_executable(main.exe main.cxx)

target_include_directories(simudmc PUBLIC include)
target_include_directories(main.exe PUBLIC include)
target_link_libraries(main.exe simudmc)
