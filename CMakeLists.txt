cmake_minimum_required(VERSION 3.30)
project(COMP345_Warzone)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/*/*.cpp
        ${PROJECT_SOURCE_DIR}/*/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
