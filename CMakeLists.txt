cmake_minimum_required(VERSION 3.9)
project(SimulatedAnnealing CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_ERROR_FLAGS "-Wall -Werror -Wextra")
set(CMAKE_CXX_NO_ERROR_FLAGS "-Wno-error=unused-function -Wno-error=unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_ERROR_FLAGS} ${CMAKE_CXX_NO_ERROR_FLAGS}")

set(SOURCE_FILES
        src/SimulatedAnnealing.h src/main.cpp src/Rectangles.h)

add_executable(SimulatedAnnealing ${SOURCE_FILES})